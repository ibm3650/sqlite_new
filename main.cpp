#include <iostream>
#include "db.h"

int main() {
     database db("C:\\Users\\kandu\\CLionProjects\\sqlite_new\\Chinook_Sqlite.sqlite");
     std::cout<<db.filename();
    //request_t req = db.request("SELECT * FROM sqlite_master");
    request_t req = db.request("SELECT * FROM Customer");
    for (const auto& row: req) {
        std::cout << row[0].to_text() << ", ";
        std::wcout << row[1].to_text16() << L' ' << row[2].to_text16() << std::endl;
    }
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
