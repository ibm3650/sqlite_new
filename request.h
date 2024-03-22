#pragma once

#include <memory>

#include "row.h"
#include "exception.h"
#include "sqlite3.h"
class statement {
public:
    //using ptr = std::shared_ptr<sqlite3_stmt>;

    statement() = default;

    statement(const statement&) = default;

    statement& operator=(const statement&) = default;

    statement(statement&&) noexcept = default;

    statement& operator=(statement&&) noexcept = default;

    ~statement() = default;

    explicit statement(sqlite3_stmt* const stmt) :
            stmt_{ stmt, [&](sqlite3_stmt* obj) {sqlite3_finalize(obj); } } {}





    [[nodiscard]] row_it begin() const{
        if(is_done_)
            row_it{ };
        return row_it{ stmt_ };
    }

    [[nodiscard]] static row_it end(){
        return row_it{};
    }

    explicit operator bool() const {
        return static_cast<bool>(stmt_);
    }

    void bind(int pos, double value){
        int ret = sqlite3_bind_double(stmt_.get(), pos, value);
        if(ret != SQLITE_OK)
            throw exception(ret);
    }

    void bind(int pos,const void* data, int length, void(*deleter)(void*)= SQLITE_STATIC){
        int ret = sqlite3_bind_blob(stmt_.get(), pos, data, length, deleter);
        if(ret != SQLITE_OK)
            throw exception(ret);
    }



    void bind(int pos, int value){
        int ret = sqlite3_bind_int(stmt_.get(), pos, value);
        if(ret != SQLITE_OK)
            throw exception(ret);
    }

    void bind(int pos, int64_t value){
        int ret = sqlite3_bind_int64(stmt_.get(), pos, value);
        if(ret != SQLITE_OK)
            throw exception(ret);
    }


    void bind(int pos, const char* data, int length, void(*deleter)(void*)= SQLITE_STATIC){
        int ret = sqlite3_bind_text(stmt_.get(), pos, data, length, deleter);
        if(ret != SQLITE_OK)
            throw exception(ret);
    }

    void bind(int pos, const wchar_t* data, int length, void(*deleter)(void*)= SQLITE_STATIC){
        int ret = sqlite3_bind_text16(stmt_.get(), pos, data, length, deleter);
        if(ret != SQLITE_OK)
            throw exception(ret);
    }

    [[maybe_unused]] void bind_null(int pos){
        int ret = sqlite3_bind_null(stmt_.get(), pos);
        if(ret != SQLITE_OK)
            throw exception(ret);
    }

    int changes(){
        return sqlite3_changes(sqlite3_db_handle(stmt_.get()));
    }

    int execute(){
        if(is_done_)
            throw exception("Statement is done");
        int ret = sqlite3_step(stmt_.get());
        switch (ret) {
            case SQLITE_BUSY:
                throw exception(ret);
            case SQLITE_ERROR:
                is_done_ = true;
                throw exception(ret);
            case SQLITE_MISUSE:
                is_done_ = true;
                throw exception(ret);
            case SQLITE_DONE:
                is_done_ = true;
                return changes();
            case SQLITE_ROW:
                throw exception("Use foreach. Statement has several rows");
            default:
                throw exception(ret);
        }
    }

    [[maybe_unused]] int parameter_count(){
        return sqlite3_bind_parameter_count(stmt_.get());
    }

    [[maybe_unused]] int parameter_index(const char* name){
        int ret = sqlite3_bind_parameter_index(stmt_.get(), name);
        if(ret == 0)
            throw exception("Name no exists");
        return ret;
    }

    [[maybe_unused]] const char* parameter_name(int index){
        const char* ret = sqlite3_bind_parameter_name(stmt_.get(), index);
        if(ret == nullptr)
            throw exception("Index out of range");
        return ret;
    }

    [[maybe_unused]] void clear_bindings(){
        sqlite3_clear_bindings(stmt_.get());
    }
private:
    bool is_done_ = false;
    std::shared_ptr<sqlite3_stmt> stmt_;
};