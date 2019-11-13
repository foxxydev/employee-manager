#include <QDebug>

#include "Request.h"

Q_LOGGING_CATEGORY(logRequest, "Request", QtCriticalMsg)

static const auto idKey = QStringLiteral("id");
static const auto statusKey = QStringLiteral("status");
static const auto statusSuccess = QStringLiteral("success");
static const auto statusFailure = QStringLiteral("failure");
static const auto statusProgress = QStringLiteral("progress");

static const auto resultKey = QStringLiteral("result");

static QVariantMap buildReply(const QVariantMap &request,
                              const QVariant &result, const QString &status)
{
    auto reply = QVariantMap({
        {statusKey, status},
        {resultKey, result}
    });

    if (request.contains(idKey)) {
        reply[idKey] = request.value(idKey);
    }

    return reply;
}

Request::Request(const QString &name, QObject *parent)
    : QObject(parent), mName(name)
{
}

std::unique_ptr<QSettings> Request::loadSettings(const QVariantMap &request,
                                                 const QString &fileName)
{
    std::unique_ptr<QSettings> settings{
        new QSettings(fileName, QSettings::IniFormat)};
    if (settings->status() == QSettings::AccessError) {
        emitError(request, QStringLiteral("Access denied for '%1'.")
                                      .arg(settings->fileName()));
    } else if (settings->status() == QSettings::FormatError) {
        emitError(request, QStringLiteral("Invalid format of '%1'.")
                                      .arg(settings->fileName()));
    }

    return settings;
}

const QString &Request::name() const
{
    return mName;
}

bool Request::match(const QString &name) const
{
    qCDebug(logRequest).nospace() << "Request::match(" << name << ")";
    return !mName.compare(name);
}

void Request::emitError(const QVariantMap &request, const QVariant &result)
{
    qCDebug(logRequest) << "Request" << request << "failed, result" << result;
    emit error(buildReply(request, result, statusFailure));
}

void Request::emitCompleted(const QVariantMap &request, const QVariant &result)
{
    qCDebug(logRequest) << "Request" << request << "succeded, result" << result;
    emit completed(buildReply(request, result, statusSuccess));
}

void Request::emitProgress(const QVariantMap &request, const QVariant &result)
{
    qCDebug(logRequest) << "Request" << request << "progress, result" << result;
    emit progress(buildReply(request, result, statusProgress));
}
