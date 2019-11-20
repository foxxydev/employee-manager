#include <QDebug>
#include <QMetaType>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>

#include "Employee.h"

#define EMPLOYEE_REQUEST_CLASS "employee."

static const auto requestClass = QStringLiteral(EMPLOYEE_REQUEST_CLASS);

static const auto requestEmployeeListKey =
        QStringLiteral(EMPLOYEE_REQUEST_CLASS "sendAnnualLeaveRequest");

static const auto dataKey = QStringLiteral("data");
static const auto emailKey = QStringLiteral("email");
static const auto startDateKey = QStringLiteral("startDate");
static const auto endDateKey = QStringLiteral("endDate");
static const auto detailsKey = QStringLiteral("details");

Employee::Employee(QObject *parent) : Request(requestClass, parent)
{
    mDatabase = new Database();
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
        return (this->*requests[requestName])(request.value(dataKey).toMap());
    } else {
        emitError(request,
                  QStringLiteral("Invalid request: '%1'").arg(requestName));
        return true;
    }
    return true;
}

bool Employee::requestEmployeeList(const QVariantMap &request)
{
    // [HV] TBI : actual function, not just an always working one :)

    if (!request.contains(dataKey)) {
        emitError(request,
                  QStringLiteral("Request contains no data."));
        return true;
    }

    const auto data = request.value(dataKey).toMap();
    qDebug() << "DATA: " << data;

    if (!data.contains(emailKey)) {
        emitError(request,
                  QStringLiteral("Request is missing data, i.e. email"));
        return true;
    }

    if (!data.contains(startDateKey)) {
        emitError(request,
                  QStringLiteral("Request is missing data, i.e. start date"));
        return true;
    }

    if (!data.contains(endDateKey)) {
        emitError(request,
                  QStringLiteral("Request is missing data, i.e. end date"));
        return true;
    }

    const auto email = data.value(emailKey).toString();
    const auto startDate = data.value(startDateKey).toString();
    const auto endDate = data.value(endDateKey).toString();

    if (data.contains(detailsKey)) {
        const auto details = data.value(detailsKey).toString();
        mDatabase.addNewAnnualLeave(email, startDate, endDate, details);

    }

    mDatabase.addNewAnnualLeave(email, startDate, endDate, details);
    qDebug() << "All info:\n"
             << email << "\n"
             << startDate << "\n"
             << endDate;

    return true;
}


const QHash<QString, Employee::RequestCallback> Employee::requests = {
    {requestEmployeeListKey, &Employee::requestEmployeeList}
};
