#include <SalBackend/Audio/MiniAudio/MiniaudioAudio.hpp>
#include <SalBackend/Audio/MiniAudio/MiniaudioAudioManager.hpp>
#include <miniaudio.h>

namespace sal::backend {
    struct MiniaudioAudio::Impl {
        ma_sound Sound{};
        ma_decoder Decoder{};
        bool SoundInitialized = false;
        bool DecoderInitialized = false;

        Vector<Byte> Data;
    };

    MiniaudioAudio::MiniaudioAudio(interface::IAudioManager &manager)
        : interface::IAudio(AudioAPI::MiniAudio, manager),
          _impl(std::make_unique<Impl>()) {}

    MiniaudioAudio::~MiniaudioAudio() { Unload(); }

    bool MiniaudioAudio::Load(const Path &file) {
        Unload();

        MiniaudioAudioManager &manager =
            static_cast<MiniaudioAudioManager &>(GetAudioManager());

        if (!manager.IsInitialized()) {
            return false;
        }

        auto *engine = static_cast<ma_engine *>(manager.GetHandle());

        const String path = file.string();

        if (ma_sound_init_from_file(engine, path.c_str(), MA_SOUND_FLAG_STREAM,
                                    nullptr, nullptr,
                                    &_impl->Sound) != MA_SUCCESS) {
            return false;
        }

        _impl->SoundInitialized = true;
        _loaded = true;

        ma_sound_set_volume(&_impl->Sound, _volume);
        ma_sound_set_pitch(&_impl->Sound, _pitch);
        ma_sound_set_looping(&_impl->Sound, _looped);
        ma_sound_set_position(&_impl->Sound, _position.X, _position.Y,
                              _position.Z);

        float duration = 0.0f;
        if (ma_sound_get_length_in_seconds(&_impl->Sound, &duration) ==
            MA_SUCCESS) {
            _duration = duration;
        }

        _state = AudioState::Stopped;
        return true;
    }

    bool MiniaudioAudio::Load(const Vector<Byte> &data) {
        Unload();

        MiniaudioAudioManager &manager =
            static_cast<MiniaudioAudioManager &>(GetAudioManager());

        if (!manager.IsInitialized() || data.empty()) {
            return false;
        }

        ma_engine *engine = static_cast<ma_engine *>(manager.GetHandle());

        _impl->Data = data;

        if (ma_decoder_init_memory(_impl->Data.data(), _impl->Data.size(),
                                   nullptr, &_impl->Decoder) != MA_SUCCESS) {
            _impl->Data.clear();
            return false;
        }

        _impl->DecoderInitialized = true;

        if (ma_sound_init_from_data_source(engine, &_impl->Decoder, 0, nullptr,
                                           &_impl->Sound) != MA_SUCCESS) {
            ma_decoder_uninit(&_impl->Decoder);
            _impl->DecoderInitialized = false;
            _impl->Data.clear();
            return false;
        }

        _impl->SoundInitialized = true;
        _loaded = true;

        ma_sound_set_volume(&_impl->Sound, _volume);
        ma_sound_set_pitch(&_impl->Sound, _pitch);
        ma_sound_set_looping(&_impl->Sound, _looped);
        ma_sound_set_position(&_impl->Sound, _position.X, _position.Y,
                              _position.Z);

        float duration = 0.0f;
        if (ma_sound_get_length_in_seconds(&_impl->Sound, &duration) ==
            MA_SUCCESS) {
            _duration = duration;
        }

        _state = AudioState::Stopped;
        return true;
    }

    void MiniaudioAudio::Unload() {
        if (_impl->SoundInitialized) {
            ma_sound_uninit(&_impl->Sound);
            _impl->SoundInitialized = false;
        }

        if (_impl->DecoderInitialized) {
            ma_decoder_uninit(&_impl->Decoder);
            _impl->DecoderInitialized = false;
        }

        _impl->Data.clear();
        _loaded = false;
        _duration = 0.0;
        _state = AudioState::Unloaded;
    }

    void MiniaudioAudio::Play() {
        if (!_loaded) {
            return;
        }

        if (ma_sound_start(&_impl->Sound) == MA_SUCCESS) {
            _state = AudioState::Playing;
        }
    }

    void MiniaudioAudio::Pause() {
        if (!_loaded || _state != AudioState::Playing) {
            return;
        }

        // Stop processing without resetting the playback cursor.
        if (ma_node_set_state(&_impl->Sound, ma_node_state_stopped) ==
            MA_SUCCESS) {
            _state = AudioState::Paused;
        }
    }

    void MiniaudioAudio::Stop() {
        if (!_loaded) {
            return;
        }

        ma_sound_stop(&_impl->Sound);
        ma_sound_seek_to_pcm_frame(&_impl->Sound, 0);
        _state = AudioState::Stopped;
    }

    double MiniaudioAudio::GetTimestamp() const {
        if (!_loaded) {
            return 0.0;
        }

        float timestamp = 0.0f;

        if (ma_sound_get_cursor_in_seconds(&_impl->Sound, &timestamp) !=
            MA_SUCCESS) {
            return 0.0;
        }

        return timestamp;
    }

    void MiniaudioAudio::SetVolume(float volume) {
        _volume = std::max(0.0f, volume);

        if (_loaded) {
            ma_sound_set_volume(&_impl->Sound, _volume);
        }
    }

    void MiniaudioAudio::SetPitch(float pitch) {
        _pitch = std::max(0.01f, pitch);

        if (_loaded) {
            ma_sound_set_pitch(&_impl->Sound, _pitch);
        }
    }

    void MiniaudioAudio::Seek(double timestamp) {
        if (!_loaded) {
            return;
        }

        timestamp = std::clamp(timestamp, 0.0, _duration);

        ma_sound_seek_to_second(&_impl->Sound, static_cast<float>(timestamp));
    }

    void MiniaudioAudio::SetPosition(const Vector3 &position) {
        _position = position;

        if (_loaded) {
            ma_sound_set_position(&_impl->Sound, position.X, position.Y,
                                  position.Z);
        }
    }

    void MiniaudioAudio::SetLoop(bool enabled) {
        _looped = enabled;

        if (_loaded) {
            ma_sound_set_looping(&_impl->Sound, enabled);
        }
    }
}
