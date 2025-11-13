// database.h

#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <sqlite3.h>

// إعلان المتغير العام كـ "extern" 
// هذا يخبر المترجم أن هذا المتغير موجود، ولكن تعريفه وتخصيص الذاكرة له سيكون في ملف .cpp آخر
extern sqlite3 *db;

// إعلانات الدوال (Function Prototypes)
// هذه هي الواجهة التي سيستخدمها باقي مشروعك

/**
 * @brief تفتح الاتصال بقاعدة البيانات وتجهز الجداول.
 */
void connect_with_db();

/**
 * @brief تغلق الاتصال بقاعدة البيانات.
 */
void close_db();

/**
 * @brief تتحقق مما إذا كان اسم المستخدم موجوداً.
 * @param username اسم المستخدم المراد التحقق منه.
 * @return true إذا كان المستخدم موجوداً.
 * @return false إذا لم يكن المستخدم موجوداً.
 */
bool user_exists(const std::string& username);

/**
 * @brief تحصل على كلمة مرور مستخدم معين.
 * @param username اسم المستخدم.
 * @param password متغير سيتم تخزين كلمة المرور فيه (تمرير بالمرجع).
 * @return true إذا تم العثور على المستخدم وكلمة المرور.
 * @return false إذا لم يتم العثور على المستخدم.
 */
bool get_user_password(const std::string& username, std::string& password);

/**
 * @brief تسجل مستخدم جديد في قاعدة البيانات.
 * @param username اسم المستخدم الجديد.
 * @param password كلمة مرور المستخدم الجديد.
 * @return true إذا تم التسجيل بنجاح.
 * @return false إذا فشل التسجيل (اسم مستخدم مكرر أو كلمة مرور فارغة).
 */
bool register_new_user(const std::string& username, const std::string& password);


#endif // DATABASE_H