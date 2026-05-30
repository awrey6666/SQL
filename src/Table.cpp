#include "Table.h"
#include "utils.h"
#include "file_system.h"

#include <iostream>

Table::Table() {}

Table::Table(std::vector<std::string> arr) {
    rows.push_back(arr);
}

void Table::insert(std::string name, std::vector<std::string> arr) {
    if (rows.empty() || arr.size() == rows[0].size()) {
        rows.push_back(arr);
    }
    FileSystem& fs = FileSystem::get_instance();
    for (int i = 0; i < static_cast<int>(arr.size()); ++i) {
        fs.write_file(name, arr[i]);
        if (i < static_cast<int>(arr.size()) - 1) {
            fs.write_file(name, "-");
        }
    }
    fs.write_file(name, "\r\n");
    std::cout << "row inserted" << std::endl;
}

void Table::insert(std::vector<std::string> arr) {
    if (rows.empty() || arr.size() == rows[0].size()) {
        rows.push_back(arr);
    }
}

Table Table::select() const {
    if(rows.empty()){
        std::cout << "Empty table\n";
        return *this;
    }
    std::cout << std::endl;
    for (int repeat = 0; repeat < 18 * static_cast<int>(this->rows[0].size()) + 1; ++repeat) {
        std::cout << "_";
    }
    std::cout << std::endl;

    for (int i = 0; i < static_cast<int>(this->rows.size()); ++i) {
        std::cout << "| ";
        for (int j = 0; j < static_cast<int>(this->rows[i].size()); ++j) {
            std::cout << this->rows[i][j];
            for (int repeat = 0; repeat < (15 - static_cast<int>(this->rows[i][j].length())); ++repeat) {
                std::cout << " ";
            }
            std::cout << " | ";
        }
        std::cout << std::endl;
        if (i == 0) {
            std::cout << "|";
            for (int repeat = 0; repeat < 18 * static_cast<int>(this->rows[0].size()) - 1; ++repeat) {
                std::cout << "-";
            }
            std::cout << "|";
            std::cout << std::endl;
        }
    }

    return *this;
}
std::string Table::to_string(){
    if(rows.empty()){
        return "Empty table\n";
    }
   std::string result = "";
    for (int repeat = 0; repeat < 18 * static_cast<int>(this->rows[0].size()) + 1; ++repeat) {
        result += "_";
    }
    result += "\n";

    for (int i = 0; i < static_cast<int>(this->rows.size()); ++i) {
         result += "| ";
        for (int j = 0; j < static_cast<int>(this->rows[i].size()); ++j) {
             result += this->rows[i][j];
            for (int repeat = 0; repeat < (15 - static_cast<int>(this->rows[i][j].length())); ++repeat) {
                 result += " ";
            }
             result += " | ";
        }
         result += "\n";
        if (i == 0) {
             result += "|";
            for (int repeat = 0; repeat < 18 * static_cast<int>(this->rows[0].size()) - 1; ++repeat) {
                 result += "-";
            }
             result += "|";
             result += "\n";
        }
    }

    return result;
}

Table Table::select(std::string key, std::string value) const {
    Table result;
    int index = -1;
    for (int i = 0; i < static_cast<int>(this->rows[0].size()); ++i) {
        if (this->rows[0][i] == key) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        std::cout << "Key not found" << std::endl;
        return result;
    }

    result.insert(this->rows[0]);

    bool found = false;
    for (int i = 1; i < static_cast<int>(this->rows.size()); ++i) {
        if (this->rows[i][index] == value) {
            result.insert(this->rows[i]);
            found = true;
        }
    }
    if (found) {
        result.select();
    } else {
        std::cout << "No rows found for " << key << " = " << value << std::endl;
    }
    return result;
}

void Table::delete_from() {
    for (int i = 0; i < static_cast<int>(this->rows.size()); ++i) {
        this->rows.pop_back();
    }
    std::cout << "Table deleted" << std::endl;
}

void Table::delete_from(std::string key, std::string value) {
    if (rows.empty()) {
        std::cout << "Table is empty" << std::endl;
        return;
    }

    int index = -1;
    for (int i = 0; i < static_cast<int>(rows[0].size()); ++i) {
        if (rows[0][i] == key) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        std::cout << "Key not found" << std::endl;
        return;
    }

    for (int i = 1; i < static_cast<int>(rows.size()); ) {
        if (index < static_cast<int>(rows[i].size()) && rows[i][index] == value) {
            rows.erase(rows.begin() + i);
        } else {
            i++;
        }
    }
}

void Table::update_set(std::string key1, std::string current_value, std::string new_value) {
    int index = -1;
    for (int i = 0; i < static_cast<int>(this->rows[0].size()); ++i) {
        if (this->rows[0][i] == key1) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        std::cout << "Key " << key1 << " not found" << std::endl;
        return;
    }
    for (int i = 1; i < static_cast<int>(this->rows.size()); ++i) {
        if (this->rows[i][index] == current_value) {
            this->rows[i][index] = new_value;
        }
    }
}

void Table::update_set(std::string key1, std::string current_value,
                std::string key2, std::string new_value) {
    current_value = clean(current_value);
    new_value = clean(new_value);

    int index1 = -1;
    int index2 = -1;

    for (int i = 0; i < static_cast<int>(this->rows[0].size()); ++i) {
        if (this->rows[0][i] == key1) index1 = i;
        if (this->rows[0][i] == key2) index2 = i;
    }

    if (index1 == -1 || index2 == -1) {
        std::cout << "Key not found" << std::endl;
        return;
    }

    for (int i = 1; i < static_cast<int>(this->rows.size()); ++i) {
        if (index1 >= static_cast<int>(this->rows[i].size()) || index2 >= static_cast<int>(this->rows[i].size())) {
            continue;
        }
        if (this->rows[i][index1] == current_value) {
            this->rows[i][index2] = new_value;
        }
    }
}