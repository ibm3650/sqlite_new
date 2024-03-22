//
// Created by ibm3650 on 26.02.2024.
//

#pragma once

#include <memory>
#include "cell.h"
#include "iterator.h"

class row_t {
public:
    row_t() = default;

    ~row_t() = default;

    row_t(const row_t&) = default;

    row_t& operator=(const row_t&) = default;

    row_t(row_t&& other) noexcept = default;

    row_t& operator=(row_t&& other) noexcept = default;

    explicit row_t(const std::shared_ptr<sqlite3_stmt>& stmt) : stmt_{ stmt } {}

    column operator[](int pos) {
        return column(stmt_, pos);
    }

    column operator[](int pos) const {
        return column(stmt_, pos);
    }

    [[nodiscard]] cell_it begin() const{
        return cell_it(stmt_);
    }

    [[nodiscard]] cell_it end() const{
        return cell_it(stmt_, count());
    }

    [[nodiscard]] bool empty() const {
        return count() == 0;
    }

    [[nodiscard]] int count() const {
        if (!stmt_)
            return 0;
        return sqlite3_column_count(stmt_.get());
    }
private:
    std::shared_ptr<sqlite3_stmt> stmt_;
};





class row_it : public iterator<row_t> {
public:
    row_it() = default;

    ~row_it() = default;

    row_it& operator=(const row_it&) = delete;

    row_it(const row_it&) = delete;

    row_it(row_it&&) = default;

    row_it& operator=(row_it&&) = default;

    explicit row_it(const std::shared_ptr<sqlite3_stmt>& stmt) : iterator(stmt, 0) {}

    row_t operator*() final {
        return row_t{ stmt_ };
    }
private:
    [[nodiscard]] bool pre_compare() const final {
        return (sqlite3_step(stmt_.get()) == SQLITE_ROW) && stmt_;
    }
};