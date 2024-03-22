//
// Created by kandu on 20.03.2024.
//

#pragma once

#include <exception>
#include <string>
#include "sqlite3.h"

class exception : public std::runtime_error {
public:
    explicit exception(sqlite3* db) : errorCode(sqlite3_errcode(db)), std::runtime_error(sqlite3_errmsg(db)) {}
    explicit exception(int code) : errorCode(code), std::runtime_error(sqlite3_errstr(code)) {}
    explicit exception(const char* msg) : std::runtime_error(msg) {}

    [[nodiscard]] const char* what() const noexcept override {
        return sqlite3_errstr(errorCode);
    }

    [[nodiscard]] int getErrorCode() const {
        return errorCode;
    }

private:
    int errorCode=0;
   // std::string errorMsg;
};



