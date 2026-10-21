#pragma once

#include <SalCore/Precompiled.hpp>
#include <SalCore/Math/Color.hpp>
#include <iostream>

namespace sal {
    class Console {
    public:
        // Colors
        inline static constexpr Color TimestampColor{0.5f, 0.5f, 0.5f};
        inline static constexpr Color MessageColor{1.0f, 1.0f, 1.0f};

        inline static constexpr Color InfoColor{0.0f, 1.0f, 1.0f};
        inline static constexpr Color WarningColor{1.0f, 0.8f, 0.0f};
        inline static constexpr Color ErrorColor{1.0f, 0.0f, 0.0f};
        inline static constexpr Color SuccessColor{0.0f, 1.0f, 0.0f};
        inline static constexpr Color ReadMessageColor{0.7f, 0.3f, 0.7f};

        // Methods
        template <typename... Args>
        static void Print(const Args &...args) {
            PrintTo("INFO", InfoColor, std::cout, args...);
        }

        template <typename... Args>
        static void Warn(const Args &...args) {
            PrintTo("WARN", WarningColor, std::cerr, args...);
        }

        template <typename... Args>
        static void Error(const Args &...args) {
            PrintTo("ERROR", ErrorColor, std::cerr, args...);
        }

        template <typename... Args>
        static void Success(const Args &...args) {
            PrintTo("SUCCESS", SuccessColor, std::cout, args...);
        }

        template <typename... Args>
        static void PrintCustom(const Color &color, const String &tag,
                                const Args &...args) {
            PrintTo(tag, color, std::cout, args...);
        }

        template <typename... Args>
        static void PrintClear(const Color &color, const Args &...args) {
            StringStream message;
            bool first = true;

            auto append = [&](const auto &value) {
                if (!first) {
                    message << ' ';
                }

                WriteArgument(message, value);
                first = false;
            };

            (append(args), ...);

            std::cout << ToAnsiColor(color) << message.str() << "\033[0m";
        }

        static void Clear() { std::cout << "\033[2J\033[H" << std::flush; }

        static String Read(const String &message) {
            std::cout << ToAnsiColor(TimestampColor) << '[' << GetTimestamp()
                      << ']' << "\033[0m" << ToAnsiColor(ReadMessageColor)
                      << message << ": "
                      << "\033[0m" << std::flush;

            String input;
            std::getline(std::cin, input);

            return input;
        }

    private:
        static String ToAnsiColor(const Color &color) {
            const int r = static_cast<int>(Clamp01(color.R) * 255.0f);
            const int G = static_cast<int>(Clamp01(color.G) * 255.0f);
            const int B = static_cast<int>(Clamp01(color.B) * 255.0f);

            return "\033[38;2;" + std::to_string(r) + ";" + std::to_string(G) +
                   ";" + std::to_string(B) + "m";
        }

        static String GetTimestamp() {
            const std::time_t currentTime = std::time(nullptr);
            const std::tm *localTime = std::localtime(&currentTime);

            if (localTime == nullptr) {
                return "00:00:00";
            }

            StringStream stream;
            stream << std::put_time(localTime, "%H:%M:%S");

            return stream.str();
        }

        template <typename T>
        static void WriteArgument(StringStream &stream, const T &value) {
            using Type = std::remove_cvref_t<T>;

            if constexpr (std::is_same_v<Type, bool>) {
                stream << std::boolalpha << value;
            } else if constexpr (std::is_enum_v<Type>) {
                stream << ToString(value);
            } else if constexpr (requires { value.ToString(); }) {
                stream << value.ToString();
            } else if constexpr (requires { stream << value; }) {
                stream << value;
            } else {
                stream << "[Unsupported type]";
            }
        }

        template <typename... Args>
        static void PrintTo(const char *tag, const Color &tagColor,
                            std::ostream &output, const Args &...args) {
            StringStream message;
            bool first = true;

            auto append = [&](const auto &value) {
                if (!first) {
                    message << ' ';
                }

                WriteArgument(message, value);
                first = false;
            };

            (append(args), ...);

            output << ToAnsiColor(TimestampColor) << '[' << GetTimestamp()
                   << ']' << "\033[0m"

                   << ToAnsiColor(tagColor) << '[' << tag << ']' << "\033[0m"

                   << ": " << ToAnsiColor(MessageColor) << message.str() << ';'
                   << "\033[0m" << '\n';
        }
    };
}
