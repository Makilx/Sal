#pragma once

#include <SalCore/Precompiled.hpp>

namespace sal {
    class File {
    public:
        static bool WriteAllText(const Path &filePath, const String &source);
        static bool WriteAllBytes(const Path &filePath,
                                  const Vector<Byte> &bytes);

        static bool ReadAllText(const Path &filePath, String *out);
        static bool ReadAllBytes(const Path &filePath, Vector<Byte> *out);

        static bool Exists(const Path &filePath);
        static bool Delete(const Path &filePath);
        static bool Rename(const Path &filePath, const String &newName);
        static bool Move(const Path &filePath, const Path &targetPath);
        static bool Copy(const Path &filePath, const Path &targetPath);
    };

    enum class ItemType { File, Directory };

    struct DirectoryItem {
        ItemType type;
        Path path;
    };

    class Directory {
    public:
        static bool Exists(const Path &directoryPath);
        static bool Delete(const Path &directoryPath);
        static bool Rename(const Path &directoryPath, const String &newName);
        static bool Move(const Path &directoryPath, const Path &targetPath);
        static bool Copy(const Path &directoryPath, const Path &targetPath);
        static bool ClearChildren(const Path &directoryPath);
        static Vector<DirectoryItem> ListItems(const Path &directoryPath);
    };
}