#pragma once

#include <SalCore/Core.hpp>
#include <SalCore/Misc/Enums.hpp>

namespace sal::interface {
    class IAudioManager;

    class IAudio {
    public:
        IAudio(AudioAPI api, IAudioManager &manager)
            : _audioAPI(api), _manager(manager) {}
        virtual ~IAudio() = default;

        AudioAPI GetAudioAPI() const { return _audioAPI; }
        IAudioManager &GetAudioManager() const { return _manager; }

    public:
        virtual bool Load(const Path &file) = 0;
        virtual bool Load(const Vector<Byte> &data) = 0;
        virtual bool IsLoaded() const = 0;
        virtual void Unload() = 0;

        virtual void Play() = 0;
        virtual void Pause() = 0;
        virtual void Stop() = 0;

        virtual float GetVolume() const = 0;
        virtual float GetPitch() const = 0;
        virtual double GetTimestamp() const = 0;
        virtual Vector3 GetPosition() const = 0;
        virtual bool IsLooped() const = 0;
        virtual AudioState GetState() const = 0;
        virtual double GetDuration() const = 0;

        virtual void SetVolume(float volume) = 0;
        virtual void SetPitch(float pitch) = 0;
        virtual void Seek(double timestamp) = 0;
        virtual void SetPosition(const Vector3 &position) = 0;
        virtual void SetLoop(bool enabled) = 0;

    private:
        AudioAPI _audioAPI;
        IAudioManager &_manager;
    };
}
