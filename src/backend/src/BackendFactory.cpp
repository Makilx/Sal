#include <SalBackend/BackendFactory.hpp>
#include <SalBackend/Rendering/OpenGL/OpenGLRenderer.hpp>
#include <SalBackend/Window/GLFWWindow.hpp>
#include <SalBackend/Audio/MiniAudio/MiniaudioAudioManager.hpp>

namespace sal::backend {
    UniquePtr<interface::IRenderer> BackendFactory::CreateRenderer() {
        switch (_renderAPI) {
        case RendererAPI::OpenGL:
            return std::make_unique<backend::OpenGLRenderer>();
        }
    }

    UniquePtr<interface::IWindow> BackendFactory::CreateWindow() {
        switch (_windowAPI) {
        case WindowAPI::GLFW:
            return std::make_unique<backend::GLFWWindow>();
        }
    }

    UniquePtr<interface::IAudioManager> BackendFactory::CreateAudioManager() {
        switch (_audioAPI) {
        case AudioAPI::MiniAudio:
            return std::make_unique<backend::MiniaudioAudioManager>();
        }
    }
}
