// database.cpp

#include "database_help_function_header.h" // يجب أن يضمن ملف الـ Header الخاص به
#include <iostream>
#include <limits>

// تعريف المتغير العام الذي أعلناه في ملف الـ h
sqlite3 *db = nullptr;

// دالة داخلية لإنشاء الجداول (لا نعلن عنها في الـ h لأنها للاستخدام الداخلي فقط)
void setup_database() {
    const char *sql_create_tables = 
        "CREATE TABLE IF NOT EXISTS Users ("
        "  id               INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  username         TEXT    UNIQUE NOT NULL,"
        "  password         TEXT    NOT NULL,"
        "  user_permissions INTEGER DEFAULT 0"
        ");"
        "CREATE TABLE IF NOT EXISTS Books ("
        "  id         INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  book_name  TEXT    NOT NULL,"
        "  author     TEXT,"
        "  price      REAL,"
        "  quantity   INTEGER"
        ");";

    char *errMsg = nullptr;
    int rc = sqlite3_exec(db, sql_create_tables, nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL error during setup: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    } else {
        std::cout << "Database tables are ready." << std::endl;
    }
}

// تنفيذ دالة فتح الاتصال
void connect_with_db() {
    int rc = sqlite3_open("bookstore.db", &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        exit(1);
    } else {
        std::cout << "Opened database successfully." << std::endl;
        setup_database(); // استدعاء الدالة الداخلية
    }
}

// تنفيذ دالة إغلاق الاتصال
void close_db() {
    if (db) {
        sqlite3_close(db);
        std::cout << "Database closed." << std::endl;
    }
}

// تنفيذ باقي الدوال...
bool user_exists(const std::string& username) {
    const char *sql_check = "SELECT 1 FROM Users WHERE username = ? LIMIT 1;";
    sqlite3_stmt *stmt;
    bool exists = false;

    int rc = sqlite3_prepare_v2(db, sql_check, -1, &stmt, nullptr);
    if (rc == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
        rc = sqlite3_step(stmt);
        exists = (rc == SQLITE_ROW);
    }

    sqlite3_finalize(stmt);
    return exists;
}

bool get_user_password(const std::string& username, std::string& password) {
    const char *sql_get = "SELECT password FROM Users WHERE username = ?;";
    sqlite3_stmt *stmt;
    bool found = false;

    int rc = sqlite3_prepare_v2(db, sql_get, -1, &stmt, nullptr);
    if (rc == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
        rc = sqlite3_step(stmt);

        if (rc == SQLITE_ROW) {
            const unsigned char* retrieved_pass = sqlite3_column_text(stmt, 0);
            password = reinterpret_cast<const char*>(retrieved_pass);
            found = true;
        }
    }

    sqlite3_finalize(stmt);
    return found;
}

bool register_new_user(const std::string& username, const std::string& password) {
    if (password.empty()) {
        std::cerr << "Error: Password cannot be empty." << std::endl;
        return false;
    }

    if (user_exists(username)) {
        std::cerr << "Error: Username '" << username << "' is already taken." << std::endl;
        return false;
    }

    const char *sql_insert = "INSERT INTO Users (username, password) VALUES (?, ?);";
    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(db, sql_insert, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Error preparing registration statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_TRANSIENT);
    
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc == SQLITE_DONE) {
        std::cout << "User '" << username << "' registered successfully." << std::endl;
        return true;
    } else {
        std::cerr << "Failed to register user: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
}