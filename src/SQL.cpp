#include "SQL.h"
#include "file_system.h"
#include <iostream>
#include <sstream>

static std::string strip_extension(std::string name) {
    auto pos = name.find_last_of('.');
    if (pos != std::string::npos) {
        name.erase(pos);
    }
    return name;
}

SQL::SQL(std::string name, std::string password): db_name{name}, db_password{password} {}

int SQL::create_table(std::vector<std::string> arr, std::string name) {
    for (const auto& element : this->tables) {
        if (element.first == name) {
            std::cout << "table with this name exist" << std::endl;
            return 1;
        }
    }
    tables.insert({name, Table(arr)});
    FileSystem& fs = FileSystem::get_instance();

    for (int i = 0; i < static_cast<int>(arr.size()); ++i) {
        fs.write_file(name, arr[i]);
        if (i < static_cast<int>(arr.size()) - 1) {
            fs.write_file(name, "-");
        }
    }
    fs.write_file(name, "\r\n");
    std::cout << "table " << name << " created" << std::endl;
    return 0;
}

static std::string trim_cr(std::string s) {
    if (!s.empty() && s.back() == '\r') {
        s.pop_back();
    }
    return s;
}

static std::vector<std::string> split_line(const std::string& line, char delimiter) {
    std::vector<std::string> result;
    std::string part;
    std::istringstream stream(line);
    while (std::getline(stream, part, delimiter)) {
        result.push_back(part);
    }
    return result;
}

void SQL::find_files(const std::string& directory) {
    FileSystem& fs = FileSystem::get_instance();
    std::vector<std::string> file_names = fs.list_files(directory);
    

    for (const auto& file_name : file_names) {
        if (file_name.size() < 5) {
            continue;
        }
        if (file_name.substr(file_name.size() - 4) != ".txt") {
            continue;
        }

        std::string table_name = strip_extension(file_name);
        if (table_name.empty() || this->tables.find(table_name) != this->tables.end()) {
            continue;
        }

        std::string path = directory + "/" + file_name;
        std::string content = fs.read_file(table_name);
        std::cout << "loading: " << path << std::endl;
        std::istringstream input(content);
        std::string line;
        std::vector<std::vector<std::string>> rows;
        size_t max_columns = 0;

        while (std::getline(input, line)) {
            line = trim_cr(line);
            if (line.empty()) {
                continue;
            }
            std::vector<std::string> values = split_line(line, '-');
            max_columns = std::max(max_columns, values.size());
            rows.push_back(std::move(values));
        }

        if (rows.empty()) {
            continue;
        }

        std::vector<std::string> header = rows[0];
        if (header.size() < max_columns) {
            for (size_t i = header.size(); i < max_columns; ++i) {
                header.push_back("value" + std::to_string(i + 1));
            }
        }

        Table table(header);
        for (size_t i = 1; i < rows.size(); ++i) {
            auto row = rows[i];
            if (row.size() < max_columns) {
                row.resize(max_columns);
            }
            table.insert(row);
        }

        tables.insert({table_name, std::move(table)});
        std::cout << "auto-created database table: " << table_name << std::endl;
    }
}

void SQL::select_all() {
    if (tables.empty()) {
        std::cout << "No databases found." << std::endl;
        return;
    }

    for (const auto& element : this->tables) {
        std::cout << "SELECT * FROM " << element.first << ";" << std::endl;
        element.second.select();
    }
}

bool SQL::drop_table(std::string name) {
    for (const auto& element : this->tables) {
        if (element.first == name) {
            std::cout << name << " table deleted" << std::endl;
            tables.erase(name);
            return true;
        }
    }
    std::cout << name << " table not found" << std::endl;
    return false;
}

std::string SQL::get_name() {
    return this->db_name;
}

std::string SQL::get_password() {
    return this->db_password;
}

Table* SQL::operator()(std::string name) {
    for (const auto& element : this->tables) {
        if (element.first == name) {
            return &this->tables[name];
        }
    }
    return nullptr;
}
