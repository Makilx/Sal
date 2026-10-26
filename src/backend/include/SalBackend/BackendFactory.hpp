#pragma once

#include <SalCore/Core.hpp>
#include <SalInterface/Window/IWindow.hpp>
#include <SalInterface/Renderer/IRenderer.hpp>
#include <SalInterface/Audio/IAudioManager.hpp>

namespace sal::backend {
    class BackendFactory final {
    public:
        BackendFactory(RendererAPI renderAPI, WindowAPI windowAPI,
                       AudioAPI audioAPI)
            : _renderAPI(renderAPI), _windowAPI(windowAPI),
              _audioAPI(audioAPI) {}

        UniquePtr<interface::IRenderer> CreateRenderer();
        UniquePtr<interface::IWindow> CreateWindow();
        UniquePtr<interface::IAudioManager> CreateAudioManager();

    private:
        RendererAPI _renderAPI;
        WindowAPI _windowAPI;
        AudioAPI _audioAPI;
    };
}
