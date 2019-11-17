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

static const auto dataKey = QStringLiteral("data");
static const auto usernameKey = QStringLiteral("username");
static const auto receiverKey = QStringLiteral("receiver");
static const auto subjectKey = QStringLiteral("subject");
static const auto messageKey = QStringLiteral("message");

Employee::Employee(QObject *parent) : Request(requestClass, parent)
{
}

bool Employee::match(const QString &request) const
{
    qCDebug(logRequest).nospace() << "Employee::match(" << request << ")";
    return request.startsWith(requestClass);;
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


    if (!request.contains(dataKey)) {
        emitError(request, QStringLiteral("Request contains no data."));
        return true;
    }
    const auto data = request.value(dataKey).toMap();

    if (!data.contains(usernameKey)) {
        emitError(request, QStringLiteral("Request is missing username."));
        return true;
    }

    if (!data.contains(receiverKey)) {
        emitError(request, QStringLiteral("Request is missing receiver."));
        return true;
    }

    if (!data.contains(subjectKey)) {
        emitError(request, QStringLiteral("Request is missing subject."));
        return true;
    }

    if (!data.contains(messageKey)) {
        emitError(request, QStringLiteral("Request is missing message."));
        return true;
    }

    const auto username = data.value(usernameKey).toString();
    const auto receiver = data.value(receiverKey).toString();
    const auto subject = data.value(subjectKey).toString();
    const auto message = data.value(messageKey).toString();

    if (!mailClient.sendMail(username, receiver, subject, message)) {
        emitError(request, QStringLiteral("Error in sending the mail!"));
        return true;
    }

    emitCompleted(request, QStringLiteral("Mail sent succesfully!"));
    return true;
}


const QHash<QString, Employee::RequestCallback> Employee::requests = {
    {requestEmployeeListKey, &Employee::requestEmployeeList},
    {requestSendMailKey, &Employee::requestSendMail}
};
