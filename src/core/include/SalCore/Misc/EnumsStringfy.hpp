#pragma once

#include <SalCore/Misc/Enums.hpp>
#include <SalCore/Type/Convert.hpp>

namespace sal {
    // Window
    Sal_Stringify(
        WindowAPI, switch (t) {
            case WindowAPI::GLFW:
                return "GLFW";
            default:
                return "Unknown";
        });

    Sal_Stringify(
        WindowState, switch (t) {
            case WindowState::Windowed:
                return "Windowed";
            case WindowState::Fullscreen:
                return "Fullscreen";
            case WindowState::BorderlessFullscreen:
                return "BorderlessFullscreen";
        });

    // Renderer
    Sal_Stringify(
        RendererAPI, switch (t) {
            case RendererAPI::OpenGL:
                return "OpenGL";
            default:
                return "Unknown";
        });

    Sal_Stringify(
        ShaderType, switch (t) {
            case ShaderType::VertexShader:
                return "VertexShader";
            case ShaderType::FragmentShader:
                return "FragmentShader";
            default:
                return "Unknown";
        });

    Sal_Stringify(
        TextureImageFormat, switch (t) {
            case TextureImageFormat::RGB:
                return "RGB";
            case TextureImageFormat::RGBA:
                return "RGBA";
            default:
                return "Unknown";
        });

    Sal_Stringify(
        TextureType, switch (t) {
            case TextureType::Texture2D:
                return "Texture2D";
            default:
                return "Unknown";
        });

    Sal_Stringify(
        TextureFilter, switch (t) {
            case TextureFilter::Linear:
                return "Linear";
            case TextureFilter::Nearest:
                return "Nearest";
            default:
                return "Unknown";
        });

    Sal_Stringify(
        TextureFilterChannel, switch (t) {
            case TextureFilterChannel::MagFilter:
                return "MagFilter";
            case TextureFilterChannel::MinFilter:
                return "MinFilter";
            default:
                return "Unknown";
        });

    Sal_Stringify(
        TextureWrap, switch (t) {
            case TextureWrap::Repeat:
                return "Repeat";
            case TextureWrap::MirroredRepeat:
                return "MirroredRepeat";
            case TextureWrap::ClampToEdge:
                return "ClampToEdge";
            case TextureWrap::ClampToBorder:
                return "ClampToBorder";
            case TextureWrap::MirrorClampToEdge:
                return "MirrorClampToEdge";
            default:
                return "Unknown";
        });

    Sal_Stringify(
        TextureWrapChannel, switch (t) {
            case TextureWrapChannel::S:
                return "S";
            case TextureWrapChannel::T:
                return "T";
            default:
                return "Unknown";
        });

    // Input
    Sal_Stringify(
        Key, switch (t) {
            case Key::Unknown:
                return "Unknown";

            case Key::A:
                return "A";
            case Key::B:
                return "B";
            case Key::C:
                return "C";
            case Key::D:
                return "D";
            case Key::E:
                return "E";
            case Key::F:
                return "F";
            case Key::G:
                return "G";
            case Key::H:
                return "H";
            case Key::I:
                return "I";
            case Key::J:
                return "J";
            case Key::K:
                return "K";
            case Key::L:
                return "L";
            case Key::M:
                return "M";
            case Key::N:
                return "N";
            case Key::O:
                return "O";
            case Key::P:
                return "P";
            case Key::Q:
                return "Q";
            case Key::R:
                return "R";
            case Key::S:
                return "S";
            case Key::T:
                return "T";
            case Key::U:
                return "U";
            case Key::V:
                return "V";
            case Key::W:
                return "W";
            case Key::X:
                return "X";
            case Key::Y:
                return "Y";
            case Key::Z:
                return "Z";

            // Numbers
            case Key::Num0:
                return "0";
            case Key::Num1:
                return "1";
            case Key::Num2:
                return "2";
            case Key::Num3:
                return "3";
            case Key::Num4:
                return "4";
            case Key::Num5:
                return "5";
            case Key::Num6:
                return "6";
            case Key::Num7:
                return "7";
            case Key::Num8:
                return "8";
            case Key::Num9:
                return "9";

            // Function keys
            case Key::F1:
                return "F1";
            case Key::F2:
                return "F2";
            case Key::F3:
                return "F3";
            case Key::F4:
                return "F4";
            case Key::F5:
                return "F5";
            case Key::F6:
                return "F6";
            case Key::F7:
                return "F7";
            case Key::F8:
                return "F8";
            case Key::F9:
                return "F9";
            case Key::F10:
                return "F10";
            case Key::F11:
                return "F11";
            case Key::F12:
                return "F12";
            case Key::F13:
                return "F13";
            case Key::F14:
                return "F14";
            case Key::F15:
                return "F15";
            case Key::F16:
                return "F16";
            case Key::F17:
                return "F17";
            case Key::F18:
                return "F18";
            case Key::F19:
                return "F19";
            case Key::F20:
                return "F20";
            case Key::F21:
                return "F21";
            case Key::F22:
                return "F22";
            case Key::F23:
                return "F23";
            case Key::F24:
                return "F24";

            // Modifiers
            case Key::LeftShift:
                return "Left Shift";
            case Key::RightShift:
                return "Right Shift";
            case Key::LeftControl:
                return "Left Control";
            case Key::RightControl:
                return "Right Control";
            case Key::LeftAlt:
                return "Left Alt";
            case Key::RightAlt:
                return "Right Alt";
            case Key::LeftSuper:
                return "Left Super";
            case Key::RightSuper:
                return "Right Super";

            // Navigation
            case Key::Up:
                return "Up";
            case Key::Down:
                return "Down";
            case Key::Left:
                return "Left";
            case Key::Right:
                return "Right";
            case Key::Home:
                return "Home";
            case Key::End:
                return "End";
            case Key::PageUp:
                return "Page Up";
            case Key::PageDown:
                return "Page Down";
            case Key::Insert:
                return "Insert";
            case Key::Delete:
                return "Delete";

            // Common keys
            case Key::Space:
                return "Space";
            case Key::Enter:
                return "Enter";
            case Key::Escape:
                return "Escape";
            case Key::Tab:
                return "Tab";
            case Key::Backspace:
                return "Backspace";
            case Key::CapsLock:
                return "Caps Lock";
            case Key::NumLock:
                return "Num Lock";
            case Key::ScrollLock:
                return "Scroll Lock";
            case Key::PrintScreen:
                return "Print Screen";
            case Key::Pause:
                return "Pause";
            case Key::Menu:
                return "Menu";

            // Punctuation
            case Key::Apostrophe:
                return "Apostrophe";
            case Key::Comma:
                return "Comma";
            case Key::Minus:
                return "Minus";
            case Key::Period:
                return "Period";
            case Key::Slash:
                return "Slash";
            case Key::Semicolon:
                return "Semicolon";
            case Key::Equal:
                return "Equal";
            case Key::LeftBracket:
                return "Left Bracket";
            case Key::RightBracket:
                return "Right Bracket";
            case Key::Backslash:
                return "Backslash";
            case Key::GraveAccent:
                return "Grave Accent";

            // Keypad
            case Key::Keypad0:
                return "Keypad 0";
            case Key::Keypad1:
                return "Keypad 1";
            case Key::Keypad2:
                return "Keypad 2";
            case Key::Keypad3:
                return "Keypad 3";
            case Key::Keypad4:
                return "Keypad 4";
            case Key::Keypad5:
                return "Keypad 5";
            case Key::Keypad6:
                return "Keypad 6";
            case Key::Keypad7:
                return "Keypad 7";
            case Key::Keypad8:
                return "Keypad 8";
            case Key::Keypad9:
                return "Keypad 9";
            case Key::KeypadDecimal:
                return "Keypad Decimal";
            case Key::KeypadDivide:
                return "Keypad Divide";
            case Key::KeypadMultiply:
                return "Keypad Multiply";
            case Key::KeypadSubtract:
                return "Keypad Subtract";
            case Key::KeypadAdd:
                return "Keypad Add";
            case Key::KeypadEnter:
                return "Keypad Enter";
            case Key::KeypadEqual:
                return "Keypad Equal";
            default:
                return "Unknown";
        });

    Sal_Stringify(
        MouseButton, switch (t) {
            case MouseButton::Unknown:
                return "Unknown";
            case MouseButton::Left:
                return "Left";
            case MouseButton::Middle:
                return "Middle";
            case MouseButton::Right:
                return "Right";
            case MouseButton::Button4:
                return "Button 4";
            case MouseButton::Button5:
                return "Button 5";
            case MouseButton::Button6:
                return "Button 6";
            case MouseButton::Button7:
                return "Button 7";
            case MouseButton::Button8:
                return "Button 8";
            default:
                return "Unknown";
        });

    Sal_Stringify(
        MouseState, switch (t) {
            case MouseState::Normal:
                return "Normal";
            case MouseState::Hidden:
                return "Hidden";
            case MouseState::Disabled:
                return "Disabled";
        });

    Sal_Stringify(
        InputType, switch (t) {
            case InputType::Unknown:
                return "Unknown";
            case InputType::Keyboard:
                return "Keyboard";
            case InputType::Mouse:
                return "Mouse";
            case InputType::MouseScroll:
                return "MouseScroll";
            case InputType::MouseMovement:
                return "MouseMovement";
        });

    Sal_Stringify(
        InputState, switch (t) {
            case InputState::Begin:
                return "Begin";
            case InputState::Change:
                return "Change";
            case InputState::End:
                return "End";
        });

    // Audio
    Sal_Stringify(
        AudioAPI, switch (t) {
            case sal::AudioAPI::MiniAudio:
                return "MiniAudio";
            default:
                return "Unknown";
        });

    Sal_Stringify(
        AudioState, switch (t) {
            case sal::AudioState::Playing:
                return "Playing";
            case sal::AudioState::Paused:
                return "Paused";
            case sal::AudioState::Stopped:
                return "Stopped";
            case sal::AudioState::Unloaded:
                return "Unloaded";
        });

    // RunService

    Sal_Stringify(
        RunPriority, switch (t) {
            case sal::RunPriority::First:
                return "First";
            case sal::RunPriority::Input:
                return "Input";
            case sal::RunPriority::Update:
                return "Update";
            case sal::RunPriority::Render:
                return "Render";
            case sal::RunPriority::Last:
                return "Last";
        });
}
