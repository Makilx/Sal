#include <SalBackend/Rendering/OpenGL/OpenGLMesh.hpp>
#include <SalBackend/Rendering/OpenGL/OpenGLShaders.hpp>
#include <SalBackend/Rendering/OpenGL/OpenGLTexture.hpp>
#include <SalBackend/Rendering/OpenGL/OpenGLRenderer.hpp>

#include <glad/glad.h>

namespace sal::backend {
    void BuildInstanceTextureStack(
        const Dictionary<interface::ITexture *, uint32_t> &textureLayers,
        uint32_t layerCount, unsigned int _instancedTextureStack) {
        if (layerCount == 0) {
            return;
        }

        constexpr GLsizei width = 1024;
        constexpr GLsizei height = 1024;

        glBindTexture(GL_TEXTURE_2D_ARRAY, _instancedTextureStack);

        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexImage3D(
            GL_TEXTURE_2D_ARRAY,
            0,
            GL_RGBA8,
            width,
            height,
            static_cast<GLsizei>(layerCount),
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            nullptr);

        Vector<uint8_t> whitePixels(width * height * 4, 255);

        glTexSubImage3D(
            GL_TEXTURE_2D_ARRAY,
            0,
            0, 0, 0,
            width, height, 1,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            whitePixels.data());

        for (const auto &[texture, layer] : textureLayers) {
            if (!texture) {
                continue;
            }

            auto *openGLTexture = dynamic_cast<OpenGLTexture *>(texture);

            if (!openGLTexture) {
                continue;
            }

            glCopyImageSubData(
                openGLTexture->GetHandle(),
                GL_TEXTURE_2D,
                0, 0, 0, 0,
                _instancedTextureStack,
                GL_TEXTURE_2D_ARRAY,
                0, 0, 0, static_cast<GLint>(layer),
                width, height, 1);
        }

        glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    }
}

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

        // create stack
        glGenTextures(1, &_instancedTextureStack);

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

        glDeleteTextures(1, &_instancedTextureStack);
        _instancedTextureStack = 0;

        // unload resources
        for (UniquePtr<interface::IRenderResource> &resource : _resources) {
            if (!resource->IsCreated()) {
                continue;
            }
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

        Console::Success("Terminated renderer successfully");
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
            // assert
            if (!command.Mesh) {
                Console::Warn("Command mesh is not present");
                continue;
            }

            if (!command.Shader) {
                Console::Warn("Command shader is not present");
                continue;
            }

            if (command.Instance.empty()) {
                Console::Warn("No instances to draw");
                continue;
            }

            // bind vao
            OpenGLMesh *mesh = static_cast<OpenGLMesh *>(command.Mesh);
            OpenGLShaderProgram *shaders = static_cast<OpenGLShaderProgram *>(command.Shader);

            glBindVertexArray(mesh->GetHandle());
            glUseProgram(shaders->GetHandle());

            // draw instanced / normal
            bool isInstanced = command.Instance.size() >= 3;

            shaders->Set("Sal_LightDirection", Vector3{-0.4f, -1.0f, -0.3f});
            shaders->Set("Sal_LightColor", Vector3{1.0f, 1.0f, 1.0f});
            shaders->Set("Sal_AmbientColor", Vector3{0.2f, 0.2f, 0.2f});

            shaders->Set("Sal_Instanced", isInstanced ? 1 : 0);
            if (isInstanced) {
                // Collect textures.
                Dictionary<interface::ITexture *, uint32_t> textureLayers;

                uint32_t nextLayer = 1;

                command.InstanceStackData.clear();
                command.InstanceStackData.reserve(command.Instance.size());

                for (const interface::RenderInstance &instance : command.Instance) {
                    interface::InstanceTextureStackData stackData;

                    if (instance.Albedo) {
                        auto [it, inserted] = textureLayers.try_emplace(
                            instance.Albedo,
                            nextLayer);

                        if (inserted) {
                            nextLayer++;
                        }

                        stackData.Albedo = it->second;
                    }

                    command.InstanceStackData.push_back(stackData);
                }

                // Upload instance data.
                mesh->SetInstanceTransforms(command.Instance);
                mesh->SetInstanceStackData(command.InstanceStackData);

                // Build texture array.
                BuildInstanceTextureStack(textureLayers, nextLayer, _instancedTextureStack);

                // Bind texture array.
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D_ARRAY, _instancedTextureStack);

                shaders->Set("Sal_AlbedoStack", 0);

                // Draw instanced.
                glDrawElementsInstanced(
                    GL_TRIANGLES,
                    static_cast<GLsizei>(mesh->GetIndicesCount()),
                    GL_UNSIGNED_INT,
                    nullptr,
                    static_cast<GLsizei>(command.Instance.size()));

                _renderStatus.InstancedDrawCalls++;

            } else {
                // draw
                for (interface::RenderInstance &instance : command.Instance) {
                    // uniforms
                    shaders->Set("Sal_Tint", instance.Material.Tint);
                    shaders->Set("Sal_Metallic", instance.Material.Metallic);
                    shaders->Set("Sal_Roughness", instance.Material.Roughness);

                    // textures
                    shaders->Set("Sal_Albedo", 0);
                    shaders->Set("Sal_HasAlbedo", (instance.Albedo != nullptr) ? 1 : 0);
                    if (instance.Albedo) {
                        OpenGLTexture *texture = static_cast<OpenGLTexture *>(instance.Albedo);
                        glActiveTexture(GL_TEXTURE0);
                        glBindTexture(GL_TEXTURE_2D, texture->GetHandle());
                    }

                    // draw
                    glDrawElements(GL_TRIANGLES,
                                   static_cast<GLsizei>(mesh->GetIndicesCount()),
                                   GL_UNSIGNED_INT, nullptr);

                    _renderStatus.DrawCalls++;
                }
            }

            // unbind all
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
            Console::Warn("Out of frame drawing call");
            return;
        }

        if (!command.Mesh) {
            Console::Warn("Command mesh is not present");
            return;
        }

        if (!command.Shader) {
            Console::Warn("Command shader is not present");
            return;
        }

        if (command.Instance.empty()) {
            Console::Warn("No instances to draw");
            return;
        }

        _frameCommands.push_back(command);
    }
}
