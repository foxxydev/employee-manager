#include <QDebug>
#include <QMetaType>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>

#include "Employee.h"
#include "src/MailClient.h"

#define EMPLOYEE_REQUEST_CLASS "employee."

static const auto requestClass = QStringLiteral(EMPLOYEE_REQUEST_CLASS);

static const auto requestEmployeeListKey =
        QStringLiteral(EMPLOYEE_REQUEST_CLASS "employeeList");

static const auto requestSendMailKey =
        QStringLiteral(EMPLOYEE_REQUEST_CLASS "sendMail");

Employee::Employee(QObject *parent) : Request(requestClass, parent)
{
}

bool Employee::match(const QString &request) const
{
    qCDebug(logRequest).nospace() << "Employee::match(" << request << ")";
    return true;
}

bool Employee::exec(const QVariantMap &request)
{
    qCDebug(logRequest) << "Employee::exec()";

    Q_ASSERT(request.value("request").canConvert<QString>());
    const auto requestName = request.value("request").toString();
    Q_ASSERT(requestName.startsWith(requestClass));

    if (requests.contains(requestName)) {
        return (this->*requests[requestName])(request);
    } else {
        emitError(request,
                  QStringLiteral("Invalid request: '%1'").arg(requestName));
        return true;
    }
    return true;
}

bool Employee::requestEmployeeList(const QVariantMap &)
{
    // [HV] TBI : actual function, not just an always working one :)

    qDebug() << "WORKS";
    return true;
}

bool Employee::requestSendMail(const QVariantMap &request)
{
    MailClient mailClient;
    auto username="employeemanagerapp@gmail.com";
    auto receiver="tudor.gosa@gmail.com";
    auto subject="testing class";
    auto message="testing 123456789";

    if (mailClient.sendMail(username, receiver, subject, message)){
        emitCompleted(request, QStringLiteral("Mail sent succesfully!"));
    } else {
        emitError(request, QStringLiteral("Error in sending the mail!"));
    }

    return true;
}


const QHash<QString, Employee::RequestCallback> Employee::requests = {
    {requestEmployeeListKey, &Employee::requestEmployeeList},
    {requestSendMailKey, &Employee::requestSendMail}
};
