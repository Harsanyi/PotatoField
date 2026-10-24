#include "Assets.h"
#include <filesystem>
#include <stdexcept>
#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#endif

namespace
{
    std::filesystem::path root;
}

void Assets::initialize(const char* executable)
{
    namespace fs = std::filesystem;
    fs::path application;
#if defined(_WIN32)
    wchar_t path[32768];
    const auto count = GetModuleFileNameW(nullptr, path, 32768);
    if (count == 0 || count == 32768) throw std::runtime_error("Cannot resolve executable path");
    application = fs::path(std::wstring(path, count));
#elif defined(__linux__)
    application = fs::read_symlink("/proc/self/exe");
#elif defined(__APPLE__)
    uint32_t count = 0;
    _NSGetExecutablePath(nullptr, &count);
    std::string path(count, '\0');
    if (_NSGetExecutablePath(path.data(), &count) != 0) throw std::runtime_error("Cannot resolve executable path");
    application = fs::weakly_canonical(path.c_str());
#else
    application = fs::absolute(executable);
#endif
    root = application.parent_path() / "assets";
    if (!fs::is_directory(root)) throw std::runtime_error("Missing assets directory beside the executable: " + root.string());
}

std::string Assets::file(const std::string& relativePath)
{
    if (root.empty()) throw std::runtime_error("Initialize asset paths before loading resources");
    return (root / relativePath).string();
}
