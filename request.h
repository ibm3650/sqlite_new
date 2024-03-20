//
// Created by kandu on 27.02.2024.
//

#pragma once

#include <memory>
#include "sqlite3.h"
#include "row.h"

class request_t {
public:
    request_t() = default;

    request_t(const request_t&) = default;

    request_t& operator=(const request_t&) = default;

    request_t(request_t&&) noexcept = default;

    request_t& operator=(request_t&&) noexcept = default;

    ~request_t() = default;

    request_t(sqlite3_stmt* const stmt) :
            stmt_{ stmt, [&](sqlite3_stmt* obj) {sqlite3_finalize(obj); } } {}

    [[nodiscard]] row_it begin() const{
        return row_it{ stmt_ };
    }

    [[nodiscard]] static row_it end(){
        return row_it{};
    }

    explicit operator bool() const {
        return static_cast<bool>(stmt_);
    }
private:
    std::shared_ptr<sqlite3_stmt> stmt_;
};