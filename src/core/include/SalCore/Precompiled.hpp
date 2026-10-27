#pragma once

// Precompiled headers for
// the engine
// IWYU pragma: keep

#include <iostream>      // IWYU pragma: keep
#include <string>        // IWYU pragma: keep
#include <sstream>       // IWYU pragma: keep
#include <string_view>   // IWYU pragma: keep
#include <cmath>         // IWYU pragma: keep
#include <vector>        // IWYU pragma: keep
#include <unordered_map> // IWYU pragma: keep
#include <functional>    // IWYU pragma: keep
#include <algorithm>     // IWYU pragma: keep
#include <charconv>      // IWYU pragma: keep
#include <filesystem>    // IWYU pragma: keep
#include <ctime>         // IWYU pragma: keep
#include <iomanip>       // IWYU pragma: keep
#include <type_traits>   // IWYU pragma: keep
#include <memory>        // IWYU pragma: keep
#include <array>         // IWYU pragma: keep
#include <concepts>      // IWYU pragma: keep
#include <cstring>       // IWYU pragma: keep
#include <thread>        // IWYU pragma: keep
#include <chrono>        // IWYU pragma: keep
#include <variant>       // IWYU pragma: keep

// Typedef
namespace sal {
    using String = std::string;
    using StringView = std::string_view;
    using StringStream = std::stringstream;
    using SizeT = std::size_t;
    using Path = std::filesystem::path;
    using Byte = std::byte;

    template <typename T>
    using UniquePtr = std::unique_ptr<T>;

    template <typename T>
    using Vector = std::vector<T>;

    template <typename K, typename T>
    using Dictionary = std::unordered_map<K, T>;

    template <typename T>
    using Function = std::function<T>;

    template <typename T, SizeT N>
    using Array = std::array<T, N>;
}
