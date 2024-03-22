#include "../cell.h"

const void *column::to_blob() const noexcept {
    return sqlite3_column_blob(stmt_.get(), pos_);
}

int column::to_integer() const noexcept{
    return sqlite3_column_int(stmt_.get(), pos_);
}

int64_t column::to_integer64() const noexcept{
    return sqlite3_column_int64(stmt_.get(), pos_);
}

double column::to_double() const noexcept{
    return sqlite3_column_double(stmt_.get(), pos_);
}

const char* column::to_text() const noexcept{
    auto tmp = reinterpret_cast<const char*>(sqlite3_column_text(stmt_.get(), pos_));
    return (tmp ? tmp : "");
}

const wchar_t* column::to_text16() const noexcept{
    auto tmp = static_cast<const wchar_t*>(sqlite3_column_text16(stmt_.get(), pos_));
    return (tmp ? tmp : L"");
}

size_t column::length() const noexcept{
    return sqlite3_column_bytes(stmt_.get(), pos_);
}

[[maybe_unused]] size_t column::length16() const noexcept{
    return sqlite3_column_bytes16(stmt_.get(), pos_);
}

[[nodiscard]] SQLITE_TYPE column::type() const noexcept{
    return static_cast<SQLITE_TYPE>(sqlite3_column_type(stmt_.get(), pos_));
}

[[maybe_unused]] const char* column::name_assigned() const noexcept{
    return sqlite3_column_name(stmt_.get(), pos_);
}

[[maybe_unused]] const wchar_t* column::name_assigned16() const noexcept{
    return static_cast<const wchar_t*>(sqlite3_column_name16(stmt_.get(), pos_));
}

bool column::is_valid() const noexcept{
    return static_cast<bool>(stmt_);
}

