#pragma once

#include <vector>
#include <string>

class Table {

private:
    std::vector<std::vector<std::string>> rows;

public:
    Table();
    Table(std::vector<std::string> arr);

    void insert(std::string name, std::vector<std::string> arr);
    void insert(std::vector<std::string> arr);

    Table select() const;
    Table select(std::string key, std::string value) const;

    void delete_from();
    void delete_from(std::string key, std::string value);

    void update_set(std::string key1,
                    std::string current_value,
                    std::string new_value);

    void update_set(std::string key1,
                    std::string current_value,
                    std::string key2,
                    std::string new_value);
    std::string to_string();
};