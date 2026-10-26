#pragma once

#include <SalInterface/Audio/IAudio.hpp>

namespace sal::backend {
    class MiniaudioAudio final : public interface::IAudio {
        struct Impl;

    public:
        MiniaudioAudio(interface::IAudioManager &manager);
        ~MiniaudioAudio() override;

        bool Load(const Path &file) override;
        bool Load(const Vector<Byte> &data) override;
        bool IsLoaded() const override { return _loaded; }
        void Unload() override;

        void Play() override;
        void Pause() override;
        void Stop() override;

        float GetVolume() const override { return _volume; }
        float GetPitch() const override { return _pitch; }
        double GetTimestamp() const override;
        Vector3 GetPosition() const override { return _position; }
        bool IsLooped() const override { return _looped; }
        AudioState GetState() const override { return _state; }
        double GetDuration() const override { return _duration; }

        void SetVolume(float volume) override;
        void SetPitch(float pitch) override;
        void Seek(double timestamp) override;
        void SetPosition(const Vector3 &position) override;
        void SetLoop(bool enabled) override;

    private:
        UniquePtr<Impl> _impl;

        bool _loaded = false;
        AudioState _state = AudioState::Unloaded;
        double _duration = 0.0;
        Vector3 _position = Vector3::Zero;
        bool _looped = false;
        float _pitch = 1.0f;
        float _volume = 1.0f;
    };
}
