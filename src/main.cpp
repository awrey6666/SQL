#include <iostream>
#include <string>

#include "SQL.h"
#include "compiler.h"
#include "SocketManager.h"
#include "thread_pool.h"

int main() {

    SQL db("sql", "1234");

    SocketManager socket_manager;
    socket_manager.start(db);

    db.find_files("DB");
    db.select_all();

    ThreadPool pool(2);

    while (true) {

        std::cout << "SQL|> \n";

        std::string code;
        std::getline(std::cin, code);

        if (code == "help") {

            std::cout << "Commands:\n";
            std::cout << "CREATE TABLE ...;\n";
            std::cout << "INSERT INTO ...;\n";
            std::cout << "SELECT ...;\n";
            std::cout << "DELETE ...;\n";
            std::cout << "UPDATE ...;\n";
            std::cout << "EXIT;\n";

            continue;
        }

        if (code == "EXIT;" || code == "exit;") {
            break;
        }

        pool.enqueue([&, code]() {
            std::cout << SQL_compiler(db, code);
        });
    }

    socket_manager.stop();

    return 0;
}