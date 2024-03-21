//
// Created by kandu on 28.02.2024.
//

#pragma once
#include <string>
#include <memory>
#include "iterator.h"


enum SQLITE_TYPE: int{
    INTEGER = SQLITE_INTEGER,
    FLOAT = SQLITE_FLOAT,
    TEXT= SQLITE_TEXT,
    BLOB      = SQLITE_BLOB,
    Null      = SQLITE_NULL
};

//Если индекс ячейки неорректен - поведение не определено
//Если stmt неорректен - поведение не определено
class cell_t {
public:
    cell_t() = default;

    ~cell_t() = default;

    cell_t& operator=(const cell_t&) = default;

    cell_t(const cell_t&) = default;

    cell_t& operator=(cell_t&&) = default;

    cell_t(cell_t&&) = default;

    explicit cell_t(const std::shared_ptr<sqlite3_stmt>& stmt, int pos) :stmt_{ stmt }, pos_{ pos } {}

    [[nodiscard]] const void* to_blob() const {
        return sqlite3_column_blob(stmt_.get(), pos_);
    }

    [[nodiscard]] int to_integer() const {
        return sqlite3_column_int(stmt_.get(), pos_);
    }

    [[nodiscard]] int64_t to_integer64() const {
        return sqlite3_column_int64(stmt_.get(), pos_);
    }

    [[nodiscard]] double to_double() const {
        return sqlite3_column_double(stmt_.get(), pos_);
    }

    [[nodiscard]] const char* to_text() const {
        auto tmp = reinterpret_cast<const char*>(sqlite3_column_text(stmt_.get(), pos_));
        return (tmp ? tmp : "");
    }

    [[nodiscard]] const wchar_t* to_text16() const {
        auto tmp = static_cast<const wchar_t*>(sqlite3_column_text16(stmt_.get(), pos_));
        return (tmp ? tmp : L"");
    }

    [[nodiscard]] size_t length() const {
        return sqlite3_column_bytes(stmt_.get(), pos_);
    }

    [[nodiscard]] size_t length16() const {
        return sqlite3_column_bytes16(stmt_.get(), pos_);
    }

    explicit operator const void* () const {
        return to_blob();
    }

    explicit operator double() const {
        return to_double();
    }

    explicit operator int() const {
        return to_integer();
    }

    explicit operator int64_t() const {
        return to_integer64();
    }

    explicit operator const char* () const {
        return to_text();
    }

    explicit operator const wchar_t* () const {
        return to_text16();
    }

    explicit operator std::string() const {
        return {reinterpret_cast<const char *>(sqlite3_column_text(stmt_.get(), pos_))};
    }

    explicit operator std::wstring() const {
        return { reinterpret_cast<const wchar_t *>(sqlite3_column_text16(stmt_.get(), pos_))};
    }

    explicit operator std::basic_string<uint8_t>() const {
        return { static_cast<const uint8_t*>(to_blob()), length() };
    }

    [[nodiscard]] SQLITE_TYPE type() const {
        return static_cast<SQLITE_TYPE>(sqlite3_column_type(stmt_.get(), pos_));
    }

    [[nodiscard]] const char* name_assigned() const {
        return sqlite3_column_name(stmt_.get(), pos_);
    }

    [[nodiscard]] const wchar_t* name_assigned16() const {
        return static_cast<const wchar_t*>(sqlite3_column_name16(stmt_.get(), pos_));
    }

    explicit operator bool() const {
        return static_cast<bool>(stmt_);
    }

private:
    std::shared_ptr<sqlite3_stmt> stmt_;

    int pos_ = 0;
};



class cell_it final : public iterator<cell_t> {
public:
    cell_it() = default;

    ~cell_it() override = default;

    cell_it& operator=(const cell_it&) = delete;

    cell_it(const cell_it&) = delete;

    cell_it(cell_it&&) = default;

    cell_it& operator=(cell_it&&) = default;

    explicit cell_it(const std::shared_ptr<sqlite3_stmt>& stmt, int pos = 0) : iterator(stmt, pos) {}

    cell_t operator*() final {
        return cell_t(stmt_, pos_);
    }
};