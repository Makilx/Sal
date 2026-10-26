#pragma once

// Enums used on the interface

namespace sal {
    // Window
    enum class WindowAPI {
        GLFW = 0,
    };

    enum class WindowState {
        Windowed = 0,
        Fullscreen,
        BorderlessFullscreen,
    };

    // Renderer
    enum class RendererAPI {
        OpenGL = 0,
    };

    enum class ShaderType {
        VertexShader = 0,
        FragmentShader,
    };

    enum class TextureImageFormat {
        RGB = 0,
        RGBA,
    };

    enum class TextureType {
        Texture2D = 0,
    };

    enum class TextureFilter {
        Linear = 0,
        Nearest,
    };

    enum class TextureFilterChannel {
        MagFilter = 0,
        MinFilter,
    };

    enum class TextureWrap {
        Repeat = 0,
        MirroredRepeat,
        ClampToEdge,
        ClampToBorder,
        MirrorClampToEdge,
    };

    enum class TextureWrapChannel {
        S = 0,
        T,
    };

    // Input
    enum class Key {
        Unknown = 0,

        // Letters
        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P,
        Q,
        R,
        S,
        T,
        U,
        V,
        W,
        X,
        Y,
        Z,

        // Numbers
        Num0,
        Num1,
        Num2,
        Num3,
        Num4,
        Num5,
        Num6,
        Num7,
        Num8,
        Num9,

        // Function keys
        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,
        F13,
        F14,
        F15,
        F16,
        F17,
        F18,
        F19,
        F20,
        F21,
        F22,
        F23,
        F24,

        // Modifiers
        LeftShift,
        RightShift,
        LeftControl,
        RightControl,
        LeftAlt,
        RightAlt,
        LeftSuper,
        RightSuper,

        // Navigation
        Up,
        Down,
        Left,
        Right,
        Home,
        End,
        PageUp,
        PageDown,
        Insert,
        Delete,

        // Common keys
        Space,
        Enter,
        Escape,
        Tab,
        Backspace,
        CapsLock,
        NumLock,
        ScrollLock,
        PrintScreen,
        Pause,
        Menu,

        // Punctuation
        Apostrophe,
        Comma,
        Minus,
        Period,
        Slash,
        Semicolon,
        Equal,
        LeftBracket,
        RightBracket,
        Backslash,
        GraveAccent,

        // Keypad
        Keypad0,
        Keypad1,
        Keypad2,
        Keypad3,
        Keypad4,
        Keypad5,
        Keypad6,
        Keypad7,
        Keypad8,
        Keypad9,
        KeypadDecimal,
        KeypadDivide,
        KeypadMultiply,
        KeypadSubtract,
        KeypadAdd,
        KeypadEnter,
        KeypadEqual,
    };

    enum class MouseButton {
        Unknown = 0,
        Left,
        Middle,
        Right,
        Button4,
        Button5,
        Button6,
        Button7,
        Button8,
    };

    enum class MouseState {
        Normal = 0,
        Hidden,
        Disabled,
    };

    enum class InputType {
        Unknown = 0,
        Keyboard,
        Mouse,
        MouseScroll,
        MouseMovement,
    };

    enum class InputState {
        Begin = 0,
        Change,
        End,
    };

    // Audio
    enum class AudioAPI {
        MiniAudio = 0,
    };

    enum class AudioState {
        Playing = 0,
        Paused,
        Stopped,
        Unloaded,
    };

    // Render Service
    enum class RunPriority : SizeT {
        First = 0,
        Input = 1000,
        Update = 2000,
        Render = 3000,
        Last = 5000
    };
}

#include "EnumsStringfy.hpp" // IWYU pragma: keep
