#pragma once

#include <SalCore/Core.hpp>
#include <SalInterface/Audio/IAudio.hpp>

namespace sal::interface {
    class IAudioManager {
    public:
        IAudioManager(AudioAPI api) : _audioAPI(api) {}
        virtual ~IAudioManager() = default;

        AudioAPI GetAudioAPI() const { return _audioAPI; }

    public:
        virtual bool Initialize() = 0;
        virtual bool IsInitialized() const = 0;
        virtual void Terminate() = 0;

        virtual UniquePtr<IAudio> CreateAudio() = 0;

    private:
        AudioAPI _audioAPI;
    };
}
