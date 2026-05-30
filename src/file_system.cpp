#include "file_system.h"

#include <sstream>
#include <fstream>

#ifdef _WIN32
#include <io.h>
#else
#include <dirent.h>
#include <sys/stat.h>
#endif

FileSystem *FileSystem::instance = nullptr;

FileSystem::FileSystem() {}

FileSystem &FileSystem::get_instance()
{
    if (instance == nullptr)
    {
        instance = new FileSystem();
    }
    return *instance;
}

void FileSystem::write_file(const std::string &filename,
                            const std::string &value)
{
    std::ofstream file("DB/" + filename + ".txt", std::ios::app);
    if (file.is_open())
    {
        file << value;
    }
}

void FileSystem::free_file(const std::string &filename)
{
    std::ofstream file("DB/" + filename + ".txt");
    if (file.is_open())
    {
        file << "";
    }
}

std::string FileSystem::read_file(const std::string &filename)
{
    std::ifstream file("DB/" + filename + ".txt");
    if (!file.is_open())
    {
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

bool FileSystem::exists(const std::string &filename)
{
    std::ifstream file("DB/" + filename + ".txt");
    return file.is_open();
}

std::vector<std::string> FileSystem::list_files(const std::string &directory)
{
    std::vector<std::string> result;
    std::string dir = directory;
    if (dir.empty())
    {
        dir = ".";
    }

#ifdef _WIN32
    // Windows implementation using _findfirst/_findnext
    std::string pattern = dir;
    if (pattern.back() != '\\' && pattern.back() != '/')
    {
        pattern += "\\*";
    }
    else
    {
        pattern += "*";
    }

    struct _finddata_t data;
    intptr_t handle = _findfirst(pattern.c_str(), &data);
    if (handle == -1)
    {
        return result;
    }

    do
    {
        if ((data.attrib & _A_SUBDIR) == 0)
        {
            result.push_back(data.name);
        }
    } while (_findnext(handle, &data) == 0);

    _findclose(handle);
#else
    // Unix/Linux/macOS implementation using opendir/readdir
    DIR *dirp = opendir(dir.c_str());
    if (dirp == nullptr)
    {
        return result;
    }

    struct dirent *entry;
    while ((entry = readdir(dirp)) != nullptr)
    {
        // Skip directories, only include files
        if (entry->d_type != DT_DIR)
        {
            result.push_back(entry->d_name);
        }
    }

    closedir(dirp);
#endif

    return result;
}
