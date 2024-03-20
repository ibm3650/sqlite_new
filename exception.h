//
// Created by kandu on 20.03.2024.
//

#ifndef SQLITE_NEW_EXCEPTION_H
#define SQLITE_NEW_EXCEPTION_H

#include <exception>
#include <string>
#include "sqlite3.h"

class exception : public std::runtime_error {
public:
    exception(sqlite3* db) : errorCode(sqlite3_errcode(db)), std::runtime_error(sqlite3_errmsg(db)) {}
    exception(int code) : errorCode(code), std::runtime_error(sqlite3_errstr(code)) {}

    const char* what() const noexcept override {
        return sqlite3_errstr(errorCode);
    }

    int getErrorCode() const {
        return errorCode;
    }

private:
    int errorCode;
   // std::string errorMsg;
};



#endif //SQLITE_NEW_EXCEPTION_H
