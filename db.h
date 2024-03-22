//
// Created by kandu on 09.03.2024.
//

#pragma once


#include "sqlite3.h"
#include "request.h"
#include "exception.h"


enum DBSTATUS:int{
    LOOKASIDE_USED = SQLITE_DBSTATUS_LOOKASIDE_USED,
    CACHE_USED = SQLITE_DBSTATUS_CACHE_USED,
    SCHEMA_USED = SQLITE_DBSTATUS_SCHEMA_USED,
    STMT_USED = SQLITE_DBSTATUS_STMT_USED,
    LOOKASIDE_HIT = SQLITE_DBSTATUS_LOOKASIDE_HIT,
    LOOKASIDE_MISS_SIZE = SQLITE_DBSTATUS_LOOKASIDE_MISS_SIZE,
    LOOKASIDE_MISS_FULL = SQLITE_DBSTATUS_LOOKASIDE_MISS_FULL,
    CACHE_HIT =SQLITE_DBSTATUS_CACHE_HIT,
    CACHE_MISS = SQLITE_DBSTATUS_CACHE_MISS,
    CACHE_WRITE= SQLITE_DBSTATUS_CACHE_WRITE,
    DEFERRED_FKS = SQLITE_DBSTATUS_DEFERRED_FKS,
    CACHE_USED_SHARED = SQLITE_DBSTATUS_CACHE_USED_SHARED,
    CACHE_SPILL = SQLITE_DBSTATUS_CACHE_SPILL,
};

enum DBACCESS:int{
    READONLY,
    READWRITE,
    UNKNOWN
};
class database {
public:
    database(const char *filename) {
        int ret  = sqlite3_open(filename, &db);
        if (ret != SQLITE_OK)
            throw exception(ret);
        is_open_  = true;
    }

    database(const wchar_t *filename) {
        int ret  = sqlite3_open16(filename, &db);
        if (ret != SQLITE_OK)
            throw exception(ret);
        is_open_  = true;
    }

    database(const char* filename, int flags, const char* vfs_name=nullptr) {
        int ret = sqlite3_open_v2(
                filename,   /* Database filename (UTF-8) */
                &db,         /* OUT: SQLite db handle */
                flags,              /* Flags */
                vfs_name        /* Name of VFS module to use */
        );
        if (ret != SQLITE_OK)
            throw exception(ret);
        is_open_  = true;
    }

    ~database() {
        sqlite3_close_v2(db);//ALWAYS_OK
    }

    statement request(const char *sql, int sql_length=-1, const char** tail=nullptr, int flags=0){
        sqlite3_stmt * stmt;
        int ret = sqlite3_prepare_v3(
                db,            /* Database handle */
                sql,       /* SQL statement, UTF-8 encoded */
                sql_length,              /* Maximum length of zSql in bytes. */
                flags,
                &stmt,  /* OUT: Statement handle */
                tail     /* OUT: Pointer to unused portion of zSql */
        );
        if(ret == SQLITE_OK)return {stmt};
        throw exception(db);

    }

    statement request(const wchar_t *sql, int sql_length=-1, const void ** tail=nullptr, int flags=0){
        sqlite3_stmt * stmt;
        int ret = sqlite3_prepare16_v3(
                db,            /* Database handle */
                reinterpret_cast<const void*>(sql),       /* SQL statement, UTF-16 encoded */
                sql_length,              /* Maximum length of zSql in bytes. */
                flags,
                &stmt,  /* OUT: Statement handle */
                tail     /* OUT: Pointer to unused portion of zSql */
        );
        if(ret == SQLITE_OK)return {stmt};
        throw exception(db);
    }

    const char* filename(const char* db_name="main"){
        return sqlite3_db_filename(db, db_name);
    }

    int status_max(DBSTATUS variant, bool clear=true){
        int currentCacheUsed = 0;
        int highestCacheUsed = 0;
        int status = sqlite3_db_status(db,
                                       variant,
                                       &currentCacheUsed,
                                       &highestCacheUsed,
                                       clear);
        if(status != SQLITE_OK)
            throw exception(db);
        return highestCacheUsed;
    }

    int status_current(DBSTATUS variant){
        int currentCacheUsed = 0;
        int highestCacheUsed = 0;
        int status = sqlite3_db_status(db,
                                       variant,
                                       &currentCacheUsed,
                                       &highestCacheUsed,
                                       0);
        if(status!= SQLITE_OK)
            throw exception(db);
        return currentCacheUsed;
    }

    DBACCESS access(const char* name="main"){
        int isReadOnly = sqlite3_db_readonly(db, name);
        if(isReadOnly == 1)
            return DBACCESS::READONLY;
        else if(isReadOnly == 0)
            return DBACCESS::READWRITE;
        return DBACCESS::UNKNOWN;
    }

private:

    bool is_open_ = false;
    sqlite3* db{};
};

