#pragma once

#include <string>
#include <fstream>
#include <vector>

class FileSystem {

private:
    static FileSystem* instance;

    FileSystem();

public:
    FileSystem(const FileSystem&) = delete;
    FileSystem& operator=(const FileSystem&) = delete;

    static FileSystem& get_instance();

    void write_file(const std::string& filename,
                    const std::string& data);
    void free_file(const std::string& filename);

    std::string read_file(const std::string& filename);

    bool exists(const std::string& filename);

    std::vector<std::string> list_files(const std::string& directory = ".");
};