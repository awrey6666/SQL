#pragma once

#include <unordered_map>
#include <string>
#include <vector>

#include "Table.h"

class SQL {
private:
    std::string db_name;
    std::string db_password;

    std::unordered_map<std::string, Table> tables;

public:
    SQL(std::string name, std::string password);

    int create_table(std::vector<std::string> arr, std::string name);

    bool drop_table(std::string name);

    std::string get_name();
    std::string get_password();

    Table* operator()(std::string name);

    void find_files(const std::string& directory = ".");
    void select_all();
};