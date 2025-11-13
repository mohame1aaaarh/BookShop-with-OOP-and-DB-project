// main.cpp

#include <iostream>
#include <string>
#include <limits>

// تضمين ملف الواجهة الخاص بقاعدة البيانات
#include "database_help_function_header.h"

// استخدام النطاق القياسي
using namespace std;

// دالة مساعدة لتنظيف مخزن الإدخال
void clear_input_buffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int main() {
    // الاتصال بقاعدة البيانات باستخدام الدالة من ملف database.cpp
    connect_with_db();

    int choice;
    do {
        cout << "\n--- Bookstore User Management ---\n";
        cout << "1. Register a new user\n";
        cout << "2. Login (check user and get password)\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            string username, password;
            cout << "Enter new username: ";
            clear_input_buffer();
            getline(cin, username);
            
            cout << "Enter password: ";
            getline(cin, password);

            register_new_user(username, password);

        } else if (choice == 2) {
            string username, password_from_db;
            cout << "Enter username to check: ";
            clear_input_buffer();
            getline(cin, username);

            if (user_exists(username)) {
                cout << "User '" << username << "' exists." << endl;
                if (get_user_password(username, password_from_db)) {
                    cout << "The stored password for this user is: " << password_from_db << endl;
                }
            } else {
                cout << "User '" << username << "' does not exist." << endl;
            }
        } else if (choice == 3) {
            cout << "Exiting..." << endl;
        } else {
            cout << "Invalid choice. Please try again." << endl;
            clear_input_buffer();
        }

    } while (choice != 3);

    // إغلاق الاتصال بقاعدة البيانات
    close_db();
    
    cout << "Program finished." << endl;
    return 0;
}