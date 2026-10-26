#include <SalBackend/Rendering/OpenGL/OpenGLRenderer.hpp>
#include <SalBackend/Rendering/OpenGL/OpenGLMesh.hpp>
#include <SalBackend/Rendering/OpenGL/OpenGLShaders.hpp>
#include <SalBackend/Rendering/OpenGL/OpenGLTexture.hpp>
#include <glad/glad.h>

namespace sal::backend {
    bool OpenGLRenderer::Initialize(interface::IWindow *window) {
        if (_loaded) {
            return true;
        }

        // check window
        if (!window->IsInitialized()) {
            Console::Error("Renderer => Failed to initialize because window is "
                           "not active");
            return false;
        }

        // load glad
        if (!gladLoadGL()) {
            Console::Error("Renderer => Failed to load glad (OpenGL)");
            return false;
        }

        // reset stats
        _inFrame = false;
        _frameCommands.clear();
        _renderStatus = {};
        _resources.clear();

        // set window
        _currentWindow = window;

        // gl stats
        { // calculate viewport
            IVector2 begin = this->_viewport.Scale
                                 ? (_currentWindow->GetSize().ToFloat() *
                                    this->_viewport.Begin)
                                       .ToInteger()
                                 : this->_viewport.Begin.ToInteger();

            IVector2 size = this->_viewport.Scale
                                ? (_currentWindow->GetSize().ToFloat() *
                                   this->_viewport.Size)
                                      .ToInteger()
                                : this->_viewport.Size.ToInteger();

            glViewport(begin.X, begin.Y, size.X, size.Y);
        }
        glEnable(GL_DEPTH_TEST);

        // create connections
        _resizedConnection =
            window->Resized.Connect([this](WindowResizedEvent &e) {
                // calculate viewport
                IVector2 begin =
                    this->_viewport.Scale
                        ? (e.Size.ToFloat() * this->_viewport.Begin).ToInteger()
                        : this->_viewport.Begin.ToInteger();

                IVector2 size =
                    this->_viewport.Scale
                        ? (e.Size.ToFloat() * this->_viewport.Size).ToInteger()
                        : this->_viewport.Size.ToInteger();

                glViewport(begin.X, begin.Y, size.X, size.Y);
            });

        _loaded = true;
        return true;
    }

    void OpenGLRenderer::Terminate() {
        if (!_loaded) {
            return;
        }

        // unload resources
        for (UniquePtr<interface::IRenderResource> &resource : _resources) {
            resource->Destroy();
        }

        _resources.clear();
        _frameCommands.clear();
        _inFrame = false;
        _renderStatus = {};

        // clear connections
        _resizedConnection.Disconnect();
        _resizedConnection = {};

        _currentWindow = nullptr;
        _loaded = false;
    }

    void OpenGLRenderer::BeginFrame() {
        if (_inFrame) {
            return;
        }

        _inFrame = true;
        _renderStatus = {};

        // gl begin
        glClearColor(_backgroundColor.R, _backgroundColor.G, _backgroundColor.B,
                     _backgroundColor.A);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLRenderer::EndFrame() {
        if (!_inFrame) {
            return;
        }

        _inFrame = false;

        // rendering
        for (interface::RenderCommand &command : _frameCommands) {
            // command check
            if ((!command.Material || !command.Material->Shaders ||
                 !command.Material->Shaders->IsCreated()) ||
                (!command.Mesh || !command.Mesh->IsCreated()) ||
                command.Transforms.empty()) {
                continue;
            }

            // shaders
            OpenGLShaderProgram *shaders =
                static_cast<OpenGLShaderProgram *>(command.Material->Shaders);

            glUseProgram(shaders->GetHandle());
            shaders->Set("Sal_TintColor", command.Material->Tint);

            // textures
            bool hasTexture = command.Material->Texture &&
                              command.Material->Texture->IsCreated();
            shaders->Set("Sal_HasTexture", hasTexture);

            if (hasTexture) {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, static_cast<OpenGLTexture *>(
                                                 command.Material->Texture)
                                                 ->GetHandle());
                shaders->Set("Sal_Texture_0", 0);
            }

            // instanced / normal drawing
            OpenGLMesh *mesh = static_cast<OpenGLMesh *>(command.Mesh);
            glBindVertexArray(mesh->GetHandle());

            if (command.Instanced) {
                if (command.Transforms.empty()) {
                    glBindVertexArray(0);
                    continue;
                }

                // Upload per-instance model matrices
                mesh->SetInstanceTransforms(command.Transforms);

                // Camera uniforms
                shaders->Set("Sal_View", _renderView.View);
                shaders->Set("Sal_Projection", _renderView.Projection);

                glDrawElementsInstanced(
                    GL_TRIANGLES, static_cast<GLsizei>(mesh->GetIndicesCount()),
                    GL_UNSIGNED_INT, nullptr,
                    static_cast<GLsizei>(command.Transforms.size()));

                _renderStatus.DrawCalls++;
                _renderStatus.InstancedDrawCalls++;
            } else {
                // A non-instanced command uses its first transform
                shaders->Set("Sal_Model", command.Transforms.front());
                shaders->Set("Sal_View", _renderView.View);
                shaders->Set("Sal_Projection", _renderView.Projection);

                glDrawElements(GL_TRIANGLES,
                               static_cast<GLsizei>(mesh->GetIndicesCount()),
                               GL_UNSIGNED_INT, nullptr);

                _renderStatus.DrawCalls++;
            }
            glBindVertexArray(0);
        }

        // finish
        _frameCommands.clear();
    }

    interface::IMesh *OpenGLRenderer::CreateMesh() {
        if (!_loaded) {
            return nullptr;
        }
        UniquePtr<OpenGLMesh> mesh = std::make_unique<OpenGLMesh>();
        OpenGLMesh *ptr = mesh.get();

        _resources.push_back(std::move(mesh));
        return ptr;
    }

    interface::IShader *OpenGLRenderer::CreateShader() {
        if (!_loaded) {
            return nullptr;
        }
        UniquePtr<OpenGLShader> mesh = std::make_unique<OpenGLShader>();
        OpenGLShader *ptr = mesh.get();

        _resources.push_back(std::move(mesh));
        return ptr;
    }

    interface::ITexture *OpenGLRenderer::CreateTexture() {
        if (!_loaded) {
            return nullptr;
        }
        UniquePtr<OpenGLTexture> mesh = std::make_unique<OpenGLTexture>();
        OpenGLTexture *ptr = mesh.get();

        _resources.push_back(std::move(mesh));
        return ptr;
    }

    interface::IShaderProgram *OpenGLRenderer::CreateProgram() {
        if (!_loaded) {
            return nullptr;
        }
        UniquePtr<OpenGLShaderProgram> mesh =
            std::make_unique<OpenGLShaderProgram>();
        OpenGLShaderProgram *ptr = mesh.get();

        _resources.push_back(std::move(mesh));
        return ptr;
    }

    void OpenGLRenderer::Draw(const interface::RenderCommand &command) {
        if (!_loaded || !_inFrame) {
            return;
        }

        _frameCommands.push_back(command);
    }
}
