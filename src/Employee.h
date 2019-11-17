#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <QHash>

#include "Request.h"
#include "Singleton.h"

class Employee : public Request {

    Q_OBJECT

public:
    Q_INVOKABLE explicit Employee(QObject * = nullptr);

    bool exec(const QVariantMap &) override;
    bool match(const QString &) const override;

private:
    bool requestEmployeeList(const QVariantMap &);
    bool requestSendMail(const QVariantMap &);

    typedef bool (Employee::*RequestCallback)(const QVariantMap &);
    static const QHash<QString, RequestCallback> requests;
};

#endif // EMPLOYEE_H
