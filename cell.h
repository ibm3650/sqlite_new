#pragma once

#include "iterator.h"
#include <string>
#include <memory>


/**
 * @brief Перечисление типов данных SQLite.
 * @warning Оператор должен быть подготовлен, иначе, поведение не определено.
 * @warning Индекс должен быть в допустимом диапазоне, иначе, поведение не определено
 */
enum SQLITE_TYPE : int {
    INTEGER = SQLITE_INTEGER, /**< Целочисленный тип данных */
    FLOAT = SQLITE_FLOAT, /**< Вещественный тип данных */
    TEXT = SQLITE_TEXT, /**< Текстовый тип данных */
    BLOB = SQLITE_BLOB, /**< Двоичные данные */
    Null = SQLITE_NULL /**< Нулевое значение */
};

/**
 * @brief Класс column представляет собой ячейку данных из результата запроса SQLite.
 */
class column {
public:
    column() = default; /**< Конструктор по умолчанию */

    ~column() = default; /**< Деструктор */

    column &operator=(const column &) = default; /**< Оператор присваивания */

    column(const column &) = default; /**< Конструктор копирования */

    column &operator=(column &&) = default; /**< Оператор перемещения */

    column(column &&) = default; /**< Конструктор перемещения */

    /**
     * @brief Конструктор, привязывающий ячейку к подготовленному запросу SQLite.
     * @param stmt Указатель на подготовленный запрос SQLite.
     * @param pos Позиция ячейки в запросе.
     */
    explicit column(const std::shared_ptr<sqlite3_stmt> &stmt, int pos) noexcept: stmt_{stmt}, pos_{pos} {}

    /**
     * @brief Преобразует данные в ячейке в формат двоичных данных.
     * @return Указатель на данные в формате blob.
     * @warning Если длинна равна 0 - возвращается nullptr
     */
    [[nodiscard]] const void *to_blob() const noexcept;

    /**
 * @brief Преобразует данные в ячейке в целочисленный тип.
 * @return Целочисленное значение.
 */
    [[nodiscard]] int to_integer() const noexcept;

    /**
 * @brief Преобразует данные в ячейке в 64-битное целочисленное значение.
 * @return 64-битное целочисленное значение.
 */
    [[nodiscard]] int64_t to_integer64() const noexcept;

    /**
 * @brief Преобразует данные в ячейке в вещественное число.
 * @return Вещественное число.
 */
    [[nodiscard]] double to_double() const noexcept;

    /**
 * @brief Преобразует данные в ячейке в текстовый формат.
 * @return Указатель на текстовые данные.
 */
    [[nodiscard]] const char *to_text() const noexcept;

    /**
 * @brief Преобразует данные в ячейке в формат текста UTF-16.
 * @return Указатель на данные в формате UTF-16.
 */
    [[nodiscard]] const wchar_t *to_text16() const noexcept;

    /**
 * @brief Возвращает длину данных в ячейке.
 * @return Длина данных в байтах.
 */
    [[nodiscard]] size_t length() const noexcept;

    /**
 * @brief Возвращает длину данных в ячейке в формате UTF-16.
 * @return Длина данных в байтах в формате UTF-16.
 */
    [[maybe_unused]] [[nodiscard]] size_t length16() const noexcept;

    /**
* @brief Возвращает тип данных в ячейке результата запроса.
* @return Тип данных в ячейке (SQLITE_TYPE).
*/
    [[nodiscard]] SQLITE_TYPE type() const noexcept;

    /**
 * @brief Возвращает имя столбца, к которому привязана данная ячейка.
 * @return Указатель на строку с именем столбца в формате UTF-8.
 */
    [[maybe_unused]] [[nodiscard]] const char *name_assigned() const noexcept;

    /**
 * @brief Возвращает имя столбца, к которому привязана данная ячейка, в формате UTF-16.
 * @return Указатель на строку с именем столбца в формате UTF-16.
 */
    [[maybe_unused]] [[nodiscard]] const wchar_t *name_assigned16() const noexcept;


    [[nodiscard]] bool is_valid() const noexcept;


    /**
 * @brief Оператор преобразования в const void* .
     * Преобразует данные в ячейке в указатель на данные в формате blob.
 * @return Указатель на данные в формате blob.
 */
    explicit operator const void *() const noexcept{
        return to_blob();
    }

    /**
* @brief Оператор преобразования в double .
  * Преобразует данные в ячейке в указатель на данные в вещественное число.
* @return Числовое значение в типа double.
*/
    explicit operator double() const noexcept{
        return to_double();
    }

    /**
* @brief Оператор преобразования в int .
* Преобразует данные в ячейке в указатель на данные в целое число.
* @return Числовое значение в типа int.
*/
    explicit operator int() const noexcept{
        return to_integer();
    }

    /**
* @brief Оператор преобразования в int64 .
* Преобразует данные в ячейке в указатель на данные в 64-битное целое число.
* @return Числовое значение в типа int64_t.
*/
    explicit operator int64_t() const noexcept{
        return to_integer64();
    }

    /**
* @brief Оператор преобразования в const char* .
* Преобразует данные в ячейке в указатель на строку.
* @return строка типа  const char*.
*/
    explicit operator const char *() const noexcept{
        return to_text();
    }

    /**
* @brief Оператор преобразования в const wchar_t* .
* Преобразует данные в ячейке в указатель на UTF-16 строку.
* @return строка типа  const wchar_t*.
*/
    explicit operator const wchar_t *() const noexcept{
        return to_text16();
    }

    /**
* @brief Оператор преобразования в std::string .
* Преобразует данные в ячейке в указатель на UTF-16 строку.
* @return строка типа  std::string.
*/
    explicit operator std::string() const noexcept{
        return {reinterpret_cast<const char *>(sqlite3_column_text(stmt_.get(), pos_))};
    }

    /**
* @brief Оператор преобразования в std::wstring  .
* Преобразует данные в ячейке в указатель на UTF-16 строку.
* @return строка типа  std::string .
*/
    explicit operator std::wstring() const noexcept{
        return {to_text16()};
    }

    explicit operator std::basic_string<uint8_t>() const noexcept{
        return {static_cast<const uint8_t*>(to_blob()), length()};
    }


    /**
 * @brief Преобразует объект в логическое значение, указывающее на наличие подготовленного запроса.
 * @return true, если есть подготовленный запрос, иначе false.
 */
    explicit operator bool() const noexcept{
        return is_valid();
    }

private:
    std::shared_ptr<sqlite3_stmt> stmt_;
    int pos_ = 0;
};


class cell_it final : public iterator<column> {
public:
    cell_it() = default;

    ~cell_it() override = default;

    cell_it &operator=(const cell_it &) = delete;

    cell_it(const cell_it &) = delete;

    cell_it(cell_it &&) = default;

    cell_it &operator=(cell_it &&) = default;

    explicit cell_it(const std::shared_ptr<sqlite3_stmt> &stmt, int pos = 0) : iterator(stmt, pos) {}

    column operator*() final {
        return column(stmt_, pos_);
    }
};