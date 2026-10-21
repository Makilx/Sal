#include <SalCore/IO/File.hpp>

#include <filesystem>
#include <fstream>

using namespace sal;

bool File::WriteAllText(const Path &filePath, const String &source) {
    std::ofstream file(filePath, std::ios::out | std::ios::trunc);

    if (!file.is_open()) {
        return false;
    }

    file << source;

    return file.good();
}

bool File::WriteAllBytes(const Path &filePath, const Vector<Byte> &bytes) {
    std::ofstream file(filePath,
                       std::ios::out | std::ios::binary | std::ios::trunc);

    if (!file.is_open()) {
        return false;
    }

    file.write(reinterpret_cast<const char *>(bytes.data()),
               static_cast<std::streamsize>(bytes.size()));

    return file.good();
}

bool File::ReadAllText(const Path &filePath, String *out) {
    if (out == nullptr || !std::filesystem::is_regular_file(filePath)) {
        return false;
    }

    std::ifstream file(filePath, std::ios::in);

    if (!file.is_open()) {
        return false;
    }

    *out = String(std::istreambuf_iterator<char>(file),
                  std::istreambuf_iterator<char>());

    return file.good() || file.eof();
}

bool File::ReadAllBytes(const Path &filePath, Vector<Byte> *out) {
    if (out == nullptr || !std::filesystem::is_regular_file(filePath)) {
        return false;
    }

    std::ifstream file(filePath, std::ios::in | std::ios::binary);

    if (!file.is_open()) {
        return false;
    }

    file.seekg(0, std::ios::end);
    const auto size = file.tellg();
    file.seekg(0, std::ios::beg);

    if (size < 0) {
        return false;
    }

    out->resize(static_cast<SizeT>(size));

    if (size > 0) {
        file.read(reinterpret_cast<char *>(out->data()),
                  static_cast<std::streamsize>(size));
    }

    return file.good() || file.eof();
}

bool File::Exists(const Path &filePath) {
    return std::filesystem::exists(filePath);
}

bool File::Delete(const Path &filePath) {
    if (!Exists(filePath)) {
        return false;
    }

    return std::filesystem::remove(filePath);
}

bool File::Rename(const Path &filePath, const String &newName) {
    if (!Exists(filePath)) {
        return false;
    }

    Path target = filePath.parent_path() / newName;

    std::filesystem::rename(filePath, target);

    return true;
}

bool File::Move(const Path &filePath, const Path &targetPath) {
    if (!Exists(filePath)) {
        return false;
    }

    std::filesystem::rename(filePath, targetPath);

    return true;
}

bool File::Copy(const Path &filePath, const Path &targetPath) {
    if (!Exists(filePath)) {
        return false;
    }

    std::filesystem::copy(filePath, targetPath);

    return true;
}

bool Directory::Exists(const Path &directoryPath) {
    return std::filesystem::is_directory(directoryPath);
}

bool Directory::Delete(const Path &directoryPath) {
    if (!Exists(directoryPath)) {
        return false;
    }

    return std::filesystem::remove_all(directoryPath) > 0;
}

bool Directory::Rename(const Path &directoryPath, const String &newName) {
    if (!Exists(directoryPath)) {
        return false;
    }

    const Path targetPath = directoryPath.parent_path() / newName;

    std::filesystem::rename(directoryPath, targetPath);

    return true;
}

bool Directory::Move(const Path &directoryPath, const Path &targetPath) {
    if (!Exists(directoryPath)) {
        return false;
    }

    std::filesystem::rename(directoryPath, targetPath);

    return true;
}

bool Directory::Copy(const Path &directoryPath, const Path &targetPath) {
    if (!Exists(directoryPath)) {
        return false;
    }

    std::filesystem::copy(directoryPath, targetPath,
                          std::filesystem::copy_options::recursive);

    return true;
}

bool Directory::ClearChildren(const Path &directoryPath) {
    if (!Exists(directoryPath)) {
        return false;
    }

    for (const auto &entry :
         std::filesystem::directory_iterator(directoryPath)) {
        std::filesystem::remove_all(entry.path());
    }

    return true;
}

Vector<DirectoryItem> Directory::ListItems(const Path &directoryPath) {
    Vector<DirectoryItem> items;

    if (!Exists(directoryPath)) {
        return items;
    }

    for (const auto &entry :
         std::filesystem::directory_iterator(directoryPath)) {
        items.push_back(
            {entry.is_directory() ? ItemType::Directory : ItemType::File,
             entry.path()});
    }

    return items;
}