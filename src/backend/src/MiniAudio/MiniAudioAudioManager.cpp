#include <SalBackend/Audio/MiniAudio/MiniaudioAudioManager.hpp>
#include <SalBackend/Audio/MiniAudio/MiniaudioAudio.hpp>
#include <miniaudio.h>

namespace sal::backend {
    struct MiniaudioAudioManager::Impl {
        ma_engine Engine{};
        bool Initialized = false;
    };

    MiniaudioAudioManager::MiniaudioAudioManager()
        : interface::IAudioManager(AudioAPI::MiniAudio),
          _impl(std::make_unique<Impl>()) {}

    MiniaudioAudioManager::~MiniaudioAudioManager() { Terminate(); }

    bool MiniaudioAudioManager::Initialize() {
        // load miniaudio
        if (_impl->Initialized) {
            return true;
        }

        if (ma_engine_init(nullptr, &_impl->Engine) != MA_SUCCESS) {
            return false;
        }

        _impl->Initialized = true;
        return true;
    }

    bool MiniaudioAudioManager::IsInitialized() const {
        return _impl->Initialized;
    }

    void MiniaudioAudioManager::Terminate() {
        if (!_impl || !_impl->Initialized) {
            return;
        }

        // terminate all resources
        for (UniquePtr<interface::IAudio> &audio : _resources) {
            audio->Unload();
        }

        ma_engine_uninit(&_impl->Engine);
        _impl->Initialized = false;
    }

    interface::IAudio *MiniaudioAudioManager::CreateAudio() {
        if (!_impl || !_impl->Initialized) {
            return nullptr;
        }

        UniquePtr<interface::IAudio> audio =
            std::make_unique<MiniaudioAudio>(*this);

        interface::IAudio *ptr = audio.get();
        _resources.push_back(std::move(audio));

        return ptr;
    }

    void *MiniaudioAudioManager::GetHandle() const { return &_impl->Engine; }
}
