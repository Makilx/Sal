#pragma once

#include <SalInterface/Audio/IAudioManager.hpp>

namespace sal::backend {
    class MiniaudioAudioManager final : public interface::IAudioManager {
    public:
        MiniaudioAudioManager();
        ~MiniaudioAudioManager();

        bool Initialize() override;
        bool IsInitialized() const override;
        void Terminate() override;

        interface::IAudio *CreateAudio() override;

    public:
        void *GetHandle() const;

    private:
        struct Impl;
        UniquePtr<Impl> _impl;
        Vector<UniquePtr<interface::IAudio>> _resources;
    };
}
