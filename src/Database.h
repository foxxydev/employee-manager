#ifndef DATABASE_H
#define DATABASE_H

#include <QString>

class Database
{
public:
    Database();
    bool addNewAnnualLeave(const QString &, const QString &, const QString &, const QString & = QString());
};

#endif // DATABASE_H
