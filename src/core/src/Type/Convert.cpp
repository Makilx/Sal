#include <SalCore/Precompiled.hpp>
#include <SalCore/Type/Convert.hpp>

namespace sal {
    String ToString(float t) { return std::to_string(t); }

    String ToString(int t) { return std::to_string(t); }

    String ToString(double t) { return std::to_string(t); }

    String ToString(bool t) { return t ? "true" : "false"; }

    String ToLower(const String &string) {
        String result = string;

        std::transform(
            result.begin(), result.end(), result.begin(),
            [](unsigned char c) { return static_cast<char>(std::tolower(c)); });

        return result;
    }

    bool StrToBool(const String &string, bool *out) {
        if (out == nullptr) {
            return false;
        }

        String lowered = ToLower(string);

        if (lowered == "true" || lowered == "yes" || lowered == "1") {
            *out = true;
            return true;
        } else if (lowered == "false" || lowered == "no" || lowered == "0") {
            *out = false;
            return true;
        }

        return false;
    }

    template <typename T>
    bool StringToNumber(const String &string, T *out) {
        if (out == nullptr) {
            return false;
        }

        const char *begin = string.data();
        const char *end = begin + string.size();

        auto [ptr, ec] = std::from_chars(begin, end, *out);

        return ec == std::errc{} && ptr == end;
    }

    bool ToInt(const String &string, int *out) {
        return StringToNumber(string, out);
    }

    bool ToFloat(const String &string, float *out) {
        return StringToNumber(string, out);
    }

    bool ToDouble(const String &string, double *out) {
        return StringToNumber(string, out);
    }
}
