#include "PhysFS/PhysFS.hpp"

#include <cstring>


std::string_view
PhysFS::getBaseDir()
{
    return PHYSFS_getBaseDir();
}

std::string_view
PhysFS::getPrefDir(std::string_view org_name, std::string_view app_name)
{
    return PHYSFS_getPrefDir(org_name.data(), app_name.data());
}

std::string_view
PhysFS::getWriteDir()
{
    return PHYSFS_getWriteDir();
}

PhysFS::Version
PhysFS::getLinkedVersion() noexcept
{
    return Version();
}

PhysFS::IOResult
PhysFS::init(char const* argv0) noexcept
{
    return PHYSFS_init(argv0) != 0
             ? IOResult::PHYSFS_OK
             : IOResult::PHYSFS_ERROR;
}

PhysFS::IOResult
PhysFS::deinit() noexcept
{
    return PHYSFS_deinit() != 0
             ? IOResult::PHYSFS_OK
             : IOResult::PHYSFS_ERROR;
}

PhysFS::ArchiveInfoList
PhysFS::supportedArchiveTypes()
{
    ArchiveInfoList types;
    const auto      physfs_types = PHYSFS_supportedArchiveTypes();
    for (const ArchiveInfo** info = physfs_types; *info != nullptr; ++info)
    {
        types.push_back(**info);
    }

    return types;
}

std::string_view
PhysFS::getDirSeparator()
{
    return PHYSFS_getDirSeparator();
}

void PhysFS::permitSymbolicLinks(bool allow) noexcept
{
    PHYSFS_permitSymbolicLinks(allow);
    return;
}

PhysFS::StringList
PhysFS::getCdRomDirs()
{
    StringList available_drives;
    auto       physfs_drives = PHYSFS_getCdRomDirs();
    for (char** drive = physfs_drives; *drive != nullptr; ++drive)
    {
        available_drives.push_back(*drive);
    }

    PHYSFS_freeList(physfs_drives);
    return available_drives;
}

void PhysFS::getCdRomDirs(StringCallback callback, void* extra) noexcept
{
    PHYSFS_getCdRomDirsCallback(callback, extra);
}

PhysFS::IOResult
PhysFS::setWriteDir(std::string_view new_dir) noexcept
{
    return PHYSFS_setWriteDir(new_dir.data())
             ? IOResult::PHYSFS_OK
             : IOResult::PHYSFS_ERROR;
}

void PhysFS::unmount(std::string_view old_dir) noexcept
{
    PHYSFS_unmount(old_dir.data());
}

PhysFS::StringList
PhysFS::getSearchPath()
{
    StringList physfs_paths;
    auto       search_paths = PHYSFS_getSearchPath();
    for (char** path = search_paths; *path != nullptr; ++path)
    {
        physfs_paths.push_back(*path);
    }

    PHYSFS_freeList(search_paths);
    return physfs_paths;
}

void PhysFS::getSearchPath(PhysFS::StringCallback callback, void* data) noexcept
{
    PHYSFS_getSearchPathCallback(callback, data);
}

void PhysFS::setSaneConfig(std::string_view org_name,
                           std::string_view game_name,
                           std::string_view archive_ext,
                           bool             include_cdroms,
                           bool             archives_first) noexcept
{
    PHYSFS_setSaneConfig(org_name.data(), game_name.data(), archive_ext.data(), include_cdroms, archives_first);
}

int PhysFS::mkdir(std::string_view dir_name) noexcept
{
    return PHYSFS_mkdir(dir_name.data());
}

int PhysFS::deleteFile(std::string_view filename) noexcept
{
    return PHYSFS_delete(filename.data());
}

std::string_view
PhysFS::getRealDir(std::string_view filename)
{
    return PHYSFS_getRealDir(filename.data());
}

PhysFS::StringList
PhysFS::enumerateFiles(std::string_view directory) noexcept
{
    StringList dirs;

    auto physfs_dirs = PHYSFS_enumerateFiles(directory.data());
    for (char** path = physfs_dirs; *path != nullptr; ++path)
    {
        dirs.push_back(*path);
    }

    PHYSFS_freeList(physfs_dirs);
    return dirs;
}

void PhysFS::enumerateFiles(std::string_view directory, EnumFilesCallback callback, void* data) noexcept
{
    PHYSFS_enumerate(directory.data(), callback, data);
}

bool PhysFS::exists(std::string_view filename) noexcept
{
    return PHYSFS_exists(filename.data());
}

PhysFS::MetaData
PhysFS::getMetaData(std::string_view filename) noexcept
{
    MetaData meta;
    PHYSFS_stat(filename.data(), &meta);
    return meta;
}

bool PhysFS::isDirectory(std::string_view filename) noexcept
{
    return getMetaData(filename).filetype == FileType::PHYSFS_FILETYPE_DIRECTORY;
}

bool PhysFS::isSymbolicLink(std::string_view filename) noexcept
{
    return getMetaData(filename).filetype == FileType::PHYSFS_FILETYPE_SYMLINK;
}

PhysFS::sint64
PhysFS::getLastModTime(std::string_view filename) noexcept
{
    return getMetaData(filename).modtime;
}

bool PhysFS::isInititalised() noexcept
{
    return PHYSFS_isInit();
}

bool PhysFS::symbolicLinksPermitted() noexcept
{
    return PHYSFS_symbolicLinksPermitted();
}

PhysFS::IOResult
PhysFS::setAllocator(const Allocator* allocator) noexcept
{
    return PHYSFS_setAllocator(allocator) != 0
             ? IOResult::PHYSFS_OK
             : IOResult::PHYSFS_ERROR;
}

PhysFS::IOResult
PhysFS::mount(std::string_view dir, std::string_view mount_point, bool append_to_path) noexcept
{
    return PHYSFS_mount(dir.data(), mount_point.data(), append_to_path) != 0
             ? IOResult::PHYSFS_OK
             : IOResult::PHYSFS_ERROR;
}

PhysFS::FileIO_ErrorCode
PhysFS::getLastErrorCode() noexcept
{
    return PHYSFS_getLastErrorCode();
}

std::string_view
PhysFS::getMountPoint(std::string_view dir)
{
    return PHYSFS_getMountPoint(dir.data());
}

PHYSFS_File*
PhysFS::open(std::string_view file_name, PhysFS::IOMode mode)
{
    PHYSFS_File* handle = nullptr;
    switch (mode)
    {
    case IOMode::READ:
        handle = PHYSFS_openRead(file_name.data());
        break;


    case IOMode::APPEND:
        handle = PHYSFS_openAppend(file_name.data());
        break;

    case IOMode::WRITE:
        handle = PHYSFS_openWrite(file_name.data());
        break;
    }

    return handle;
}

bool PhysFS::close(PHYSFS_File* file) noexcept
{
    if (!file)
    {
        return true;
    }

    if (PHYSFS_close(file) == 1)
    {
        return true;
    }

    return false;
}

PhysFS::sint64
PhysFS::writeBytes(PHYSFS_File* handle, const void* buffer, PhysFS::uint64 length)
{
    if (handle)
    {
        return PHYSFS_writeBytes(handle, buffer, length);
    }

    return 0;
}

PhysFS::sint64
PhysFS::readBytes(PHYSFS_File* handle, void* buffer, PhysFS::uint64 length)
{
    return PHYSFS_readBytes(handle, buffer, length);
}

PhysFS::sint64
PhysFS::length(PHYSFS_File* handle)
{
    return PHYSFS_fileLength(handle);
}

int PhysFS::seek(PHYSFS_file* handle, PhysFS::uint64 pos)
{
    return PHYSFS_seek(handle, pos);
}


PhysFS::sint16
PhysFS::Util::swapSLE16(sint16 value) noexcept
{
    return PHYSFS_swapSLE16(value);
}

PhysFS::uint16
PhysFS::Util::swapULE16(uint16 value) noexcept
{
    return PHYSFS_swapULE16(value);
}

PhysFS::sint32
PhysFS::Util::swapSLE32(sint32 value) noexcept
{
    return PHYSFS_swapSLE32(value);
}

PhysFS::uint32
PhysFS::Util::swapULE32(uint32 value) noexcept
{
    return PHYSFS_swapULE32(value);
}

PhysFS::sint64
PhysFS::Util::swapSLE64(sint64 value) noexcept
{
    return PHYSFS_swapSLE64(value);
}

PhysFS::uint64
PhysFS::Util::swapULE64(uint64 value) noexcept
{
    return PHYSFS_swapULE64(value);
}

PhysFS::sint16
PhysFS::Util::swapSBE16(sint16 value) noexcept
{
    return PHYSFS_swapSBE16(value);
}

PhysFS::uint16
PhysFS::Util::swapUBE16(uint16 value) noexcept
{
    return PHYSFS_swapUBE16(value);
}

PhysFS::sint32
PhysFS::Util::swapSBE32(sint32 value) noexcept
{
    return PHYSFS_swapSBE32(value);
}

PhysFS::uint32
PhysFS::Util::swapUBE32(uint32 value) noexcept
{
    return PHYSFS_swapUBE32(value);
}

PhysFS::sint64
PhysFS::Util::swapSBE64(sint64 value) noexcept
{
    return PHYSFS_swapSBE64(value);
}

PhysFS::uint64
PhysFS::Util::swapUBE64(uint64 value) noexcept
{
    return PHYSFS_swapUBE64(value);
}

std::string
PhysFS::Util::utf8FromUcs4(const uint32* src)
{
    std::string       value;
    const std::size_t length = strlen(reinterpret_cast<const char*>(src));
    char*             buffer = new char[length]; // will be smaller than len
    PHYSFS_utf8FromUcs4(src, buffer, length);
    value.append(buffer);
    return value;
}

std::string
PhysFS::Util::utf8ToUcs4(const char* src)
{
    std::string       value;
    const std::size_t length = strlen(src) * 4;
    char*             buffer = new char[length]; // will be smaller than len
    PHYSFS_utf8ToUcs4(src, reinterpret_cast<uint32*>(buffer), length);
    value.append(buffer);
    return value;
}

std::string
PhysFS::Util::utf8FromUcs2(const uint16* src)
{
    std::string       value;
    const std::size_t length = strlen(reinterpret_cast<const char*>(src));
    char*             buffer = new char[length]; // will be smaller than len
    PHYSFS_utf8FromUcs2(src, buffer, length);
    value.append(buffer);
    return value;
}

std::string
PhysFS::Util::utf8ToUcs2(const char* src)
{
    std::string value;
    std::size_t length = strlen(src) * 2;
    char*       buffer = new char[length]; // will be smaller than len
    PHYSFS_utf8ToUcs2(src, reinterpret_cast<uint16*>(buffer), length);
    value.append(buffer);
    return value;
}

std::string
PhysFS::Util::utf8FromLatin1(const char* src)
{
    std::string       value;
    const std::size_t length = strlen(reinterpret_cast<const char*>(src)) * 2;
    char*             buffer = new char[length]; // will be smaller than len
    PHYSFS_utf8FromLatin1(src, buffer, length);
    value.append(buffer);
    return value;
}
