//
// Created by kandu on 28.02.2024.
//

#pragma once

#include <memory>
#include "sqlite3.h"

template<typename T>
class iterator {
public:
    iterator() = default;

    virtual ~iterator() = default;

    iterator(const iterator&) = default;

    iterator& operator=(const iterator&) = default;

    iterator(iterator&& other) noexcept = default;

    iterator& operator=(iterator&& other) noexcept = default;

    explicit iterator(const std::shared_ptr<sqlite3_stmt>& stmt, int pos) :pos_{ pos }, stmt_{ stmt } {}

    bool operator !=(const iterator& other) const {
        if (!pre_compare())
            return false;
        return pos_ != other.pos_;
    }

    iterator& operator++() {
        if (pos_ != -1)
            pos_++;
        return *this;
    }

    virtual T operator*() = 0;
protected:
    [[nodiscard]] virtual bool pre_compare() const {
        return static_cast<bool>(stmt_);
    }

    int pos_ = -1;

    std::shared_ptr<sqlite3_stmt> stmt_;
};