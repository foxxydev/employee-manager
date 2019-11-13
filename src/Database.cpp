#include <QDebug>
#include <QLoggingCategory>

// Database libraries
#include <QSqlDatabase>
#include <QSqlQuery>

#include "Database.h"

static QLoggingCategory logDatabase("Database", QtCriticalMsg);

Database::Database()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");

    // [HV] Maybe storing these in a non
    // hardcoded way would be a good idea...
    db.setHostName("localhost");
    db.setUserName("root");
    db.setPassword("password");
    db.setDatabaseName("company");

    if (!db.open()) {
        qCCritical(logDatabase) << "Failed to open the database!";
        return;
    }

    qCDebug(logDatabase) << "The database connection has been successfully established!";
}
