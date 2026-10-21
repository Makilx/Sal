#include <SalBackend/Rendering/OpenGL/OpenGLTexture.hpp>
#include <glad/glad.h>
#include <stb_image.h>

namespace {
    GLenum ConvertTextureType(sal::TextureType type) {
        switch (type) {
        case sal::TextureType::Texture2D:
            return GL_TEXTURE_2D;
        }
    }

    void ConvertImageFormat(sal::TextureImageFormat format, GLenum &internal,
                            GLenum &data) {
        switch (format) {
        case sal::TextureImageFormat::RGB:
            internal = GL_RGB8;
            data = GL_RGB;
        case sal::TextureImageFormat::RGBA:
            internal = GL_RGBA8;
            data = GL_RGBA;
        }
    }

    GLenum ConvertTextureFilter(sal::TextureFilter filter) {
        switch (filter) {
        case sal::TextureFilter::Linear:
            return GL_LINEAR;
        case sal::TextureFilter::Nearest:
            return GL_NEAREST;
        }
    }

    GLenum ConvertTextureWrap(sal::TextureWrap wrap) {
        switch (wrap) {
        case sal::TextureWrap::Repeat:
            return GL_REPEAT;
            break;

        case sal::TextureWrap::MirroredRepeat:
            return GL_MIRRORED_REPEAT;
            break;

        case sal::TextureWrap::ClampToEdge:
            return GL_CLAMP_TO_EDGE;
            break;

        case sal::TextureWrap::ClampToBorder:
            return GL_CLAMP_TO_BORDER;
            break;

        case sal::TextureWrap::MirrorClampToEdge:
            return GL_MIRROR_CLAMP_TO_EDGE;
        }
    }
}

namespace sal::backend {
    bool OpenGLTexture::Create(const Path &file, TextureType type) {
        // use stbimage to load the texture
        int channels;
        IVector2 size;
        unsigned char *data =
            stbi_load(file.c_str(), &size.X, &size.Y, &channels, 0);

        if (!data) {
            Console::Error("Texture => Failed to load image file at path:",
                           file, ". Reason: ", stbi_failure_reason());
            return false;
        }

        // determine matching component structure
        TextureImageFormat format;
        if (channels == 4) {
            format = TextureImageFormat::RGBA;
        } else if (channels == 3) {
            format = TextureImageFormat::RGB;
        } else {
            Console::Error("Texture => Unsupported channel count (", channels,
                           ") in file:", file);
            stbi_image_free(data);
            return false;
        }

        // load image
        SizeT dataSize = static_cast<SizeT>(size.X) *
                         static_cast<SizeT>(size.Y) *
                         static_cast<SizeT>(channels);

        // copy the pixel data into a byte vector.
        Vector<Byte> pixels(dataSize);
        std::memcpy(pixels.data(), data, dataSize);

        // upload the image.
        bool success = Create(format, size, pixels, type);
        stbi_image_free(data);
        return success;
    }

    bool OpenGLTexture::Create(TextureImageFormat format, const IVector2 &size,
                               const Vector<Byte> &data, TextureType type) {
        if (_created) {
            Destroy();
        }

        if (size.X <= 0 || size.Y <= 0 || data.empty()) {
            return false;
        }

        _type = type;
        GLenum textureType = ConvertTextureType(_type);
        glCreateTextures(textureType, 1, &_texture);

        GLenum internalFormat, dataFormat;
        ConvertImageFormat(format, internalFormat, dataFormat);

        glTextureStorage2D(_texture, 1, internalFormat, size.X, size.Y);

        GLint previousAlignment;
        glGetIntegerv(GL_UNPACK_ALIGNMENT, &previousAlignment);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glBindTexture(textureType, _texture);
        const char *rawData = reinterpret_cast<const char *>(data.data());

        glTextureSubImage2D(_texture, 0, 0, 0, size.X, size.Y, dataFormat,
                            GL_UNSIGNED_BYTE, rawData);

        glPixelStorei(GL_UNPACK_ALIGNMENT, previousAlignment);

        // apply formats
        _created = true;
        _size = size;
        SetWrap(TextureWrapChannel::S, _wrapS);
        SetWrap(TextureWrapChannel::T, _wrapT);
        SetFilter(TextureFilterChannel::MagFilter, _magFilter);
        SetFilter(TextureFilterChannel::MinFilter, _minFilter);
        return true;
    }

    void OpenGLTexture::Destroy() {
        if (!_created) {
            return;
        }

        glDeleteTextures(1, &_texture);

        _created = false;
        _size = IVector2::Zero;
    }

    void OpenGLTexture::SetWrap(TextureWrapChannel channel, TextureWrap wrap) {
        if (channel == TextureWrapChannel::S) {
            _wrapS = wrap;
        } else {
            _wrapT = wrap;
        }

        if (!_created) {
            return;
        }

        GLenum targetChannel = channel == TextureWrapChannel::S
                                   ? GL_TEXTURE_WRAP_S
                                   : GL_TEXTURE_WRAP_T;
        GLenum targetWrap = ConvertTextureWrap(wrap);

        glTextureParameteri(_texture, targetChannel, targetWrap);
    }

    void OpenGLTexture::SetFilter(TextureFilterChannel channel,
                                  TextureFilter filter) {
        if (channel == TextureFilterChannel::MagFilter) {
            _magFilter = filter;
        } else {
            _minFilter = filter;
        }

        if (!_created) {
            return;
        }

        GLenum targetChannel = channel == TextureFilterChannel::MagFilter
                                   ? GL_TEXTURE_MAG_FILTER
                                   : GL_TEXTURE_MIN_FILTER;
        GLenum targetFilter = ConvertTextureFilter(filter);
        glTextureParameteri(_texture, targetChannel, targetFilter);
    }
}
