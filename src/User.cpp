#include <QProcess>

#include "User.h"

#define USER_REQUEST_CLASS "user."

static const auto requestClass = QStringLiteral(USER_REQUEST_CLASS);

static const auto requestAuthenticationKey =
    QStringLiteral(USER_REQUEST_CLASS "authenticate");
static const auto requestChangePasswordKey =
    QStringLiteral(USER_REQUEST_CLASS "changePassword");
static const auto requestResetPasswordKey =
    QStringLiteral(USER_REQUEST_CLASS "resetPassword");

static const auto dataKey = QStringLiteral("data");
static const auto accountKey = QStringLiteral("account");
static const auto passwordKey = QStringLiteral("password");
static const auto newPasswordKey = QStringLiteral("newPassword");

static const auto userDataPath = QStringLiteral("/var/www/userData/userInfo.json");

static const auto userAccount = QStringLiteral("user");
static const auto adminAccount = QStringLiteral("admin");
static const auto serviceAccount = QStringLiteral("service");

static const auto defaultUserPassword = QStringLiteral("userpassword");
static const auto defaultAdminPassword = QStringLiteral("adminpassword");

User::User(QObject *parent) : Request(requestClass, parent)
{
}

bool User::exec(const QVariantMap &request)
{
    qCDebug(logRequest) << "User::exec()";

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
}
bool User::match(const QString &request) const
{
    qCDebug(logRequest).nospace() << "User::match(" << request << ")";
    return request.startsWith(requestClass);
}

bool User::requestAuthentication(const QVariantMap &request)
{
    bool found = false;

    if (!request.contains(dataKey)) {
        emitError(request, QStringLiteral("Request contains no data."));
        return true;
    }
    const auto data = request.value(dataKey).toMap();

    if (!data.contains(accountKey)) {
        emitError(request, QStringLiteral("Request is missing account value."));
        return true;
    }
    const QString account = data.value(accountKey).toString();

    if (!data.contains(passwordKey)) {
        emitError(request, QStringLiteral("Request is missing password value."));
        return true;
    }
    const QString password = data.value(passwordKey).toString();

    QJsonArray array = getAccountValues();
    foreach (const QJsonValue & v, array) {
        QString accountIter = v.toObject().value("account").toString();
        QString passwordIter = v.toObject().value("password").toString();
        if ((QString::compare(accountIter, account, Qt::CaseSensitive)==0)
            && (QString::compare(passwordIter, password, Qt::CaseSensitive)==0)) {
            found = true;
        }
    }
    if (!found) {
        emitError(request,
                  QStringLiteral("Failed to authenticate, username and/or password were incorrect"));
    } else {
        emitCompleted(request,
                      QStringLiteral("Authenticated user '%1'.").arg(account));
    }

    return true;
}

bool User::requestChangePassword(const QVariantMap &request)
{
    if (!request.contains(dataKey)) {
        emitError(request, QStringLiteral("Request contains no data."));
        return true;
    }
    const auto data = request.value(dataKey).toMap();

    if (!data.contains(accountKey)) {
        emitError(request, QStringLiteral("Request is missing account value."));
        return true;
    }
    const QString account = data.value(accountKey).toString();

    if (!data.contains(passwordKey)) {
        emitError(request, QStringLiteral("Request is missing password value."));
        return true;
    }
    const QString password = data.value(passwordKey).toString();

    if (!data.contains(newPasswordKey)) {
        emitError(request, QStringLiteral("Request is missing new password value."));
        return true;
    }
    const QString newPassword = data.value(newPasswordKey).toString();

    if (!setNewPassword(account, password, newPassword)) {
        emitError(request,
                  QStringLiteral("Failed to authenticate, username and/or password were incorrect"));
    } else {
        emitCompleted(request,
                          QStringLiteral("Changed password for user '%1'.").arg(account));
    }

    return true;
}

bool User::requestResetPassword(const QVariantMap &request)
{
    if (!request.contains(dataKey)) {
        emitError(request, QStringLiteral("Request contains no data."));
        return true;
    }
    const auto data = request.value(dataKey).toMap();

    if (!data.contains(accountKey)) {
        emitError(request, QStringLiteral("Request is missing account value."));
        return true;
    }
    const QString account = data.value(accountKey).toString();

    bool found = false;
    if (account.compare(account, userAccount, Qt::CaseSensitive) == 0) {
        found = true;
        setNewPassword(account, "", defaultUserPassword, false);
    } else if (account.compare(account, adminAccount, Qt::CaseSensitive) == 0) {
        found = true;
        setNewPassword(account, "", defaultAdminPassword, false);
    } else if (account.compare(account, serviceAccount, Qt::CaseSensitive) == 0) {
        found = true;
        setNewPassword(account, "", getDefaultServicePassword(), false);
    }

    if (found) {
        emitCompleted(request,
                          QStringLiteral("Password has been reset for user '%1'.").arg(account));
    } else {
        emitError(request,
                        QStringLiteral("User '%1' password could not be reset."));
    }

    return true;
}

void User::createUserDataFile()
{
    QJsonObject userObject;
    userObject.insert("account", userAccount);
    userObject.insert("password", defaultUserPassword);

    QJsonObject adminObject;
    adminObject.insert("account", adminAccount);
    adminObject.insert("password", defaultAdminPassword);

    QJsonObject serviceObject;
    serviceObject.insert("account", serviceAccount);
    serviceObject.insert("password", getDefaultServicePassword());

    QJsonArray accountsArray;
    accountsArray.push_back(userObject);
    accountsArray.push_back(adminObject);
    accountsArray.push_back(serviceObject);


    QJsonObject docObject;
    docObject.insert("accountsArray", accountsArray);

    QFileInfo fileInfo(userDataPath);
    QDir dir = fileInfo.dir();

    if (!dir.exists()) {
        dir.mkpath(dir.path());
    }

    QJsonDocument doc(docObject);
    QFile file(fileInfo.filePath());

    if (!file.exists()) {
        file.open(QIODevice::WriteOnly);
        file.write(doc.toJson());
        file.close();
    }
}

QString User::getDefaultServicePassword()
{
    QString password = "service";

    std::unique_ptr<QSettings> embosserConf{
        new QSettings(QStringLiteral("/var/lib/embosser/embosser.conf"), QSettings::IniFormat)};

    const auto serialFileKey = QStringLiteral("Serial No");
    if (embosserConf->contains(serialFileKey)) {
        password += embosserConf->value(serialFileKey).toString();
    }

    embosserConf.reset(nullptr);
    return password;
}

QJsonArray User::getAccountValues()
{
    QFile file(userDataPath);
    if (!file.exists()) {
        createUserDataFile();
    }
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument document = QJsonDocument::fromJson(jsonData);
    QJsonObject object = document.object();
    QJsonValue value = object.value("accountsArray");
    QJsonArray array = value.toArray();

    return array;
}

QJsonValue User::getSetting(QString setting)
{
    QFile file(userDataPath);
    if (!file.exists()) {
        createUserDataFile();
    }
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument document = QJsonDocument::fromJson(jsonData);
    QJsonObject docObject = document.object();
    QJsonValue settings = docObject.value("settings");
    QJsonObject settingsObj = settings.toObject();

    return settingsObj.value(setting);
}

bool User::setSetting(QString setting, QJsonValue val)
{
    QFile file(userDataPath);
    if (!file.exists()) {
        createUserDataFile();
    }
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray jsonData = file.readAll();
    file.close();

    file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QJsonDocument document = QJsonDocument::fromJson(jsonData);

    QJsonObject docObject = document.object();
    QJsonValue value = docObject.value("settings");
    QJsonObject settingsObject = value.toObject();

    if (!val.isNull()) {
        settingsObject[setting] = val;
    }

    docObject.insert("settings", settingsObject);
    document.setObject(docObject);
    file.write(document.toJson());
    file.close();

    return true;
}

bool User::setNewPassword(QString account, QString password, QString newPassword, bool verifyPassword)
{
    bool found = false;
    QFile file(userDataPath);
    if (!file.exists()) {
        createUserDataFile();
    }
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument document = QJsonDocument::fromJson(jsonData);
    QJsonObject docObject = document.object();
    QJsonValue value = docObject.value("accountsArray");
    QJsonArray array = value.toArray();

    QJsonObject bufferObj;

    for (int i = 0; i < array.size(); ++i) {
        QJsonValue v = array[i];
        QString accountIter = v.toObject().value("account").toString();
        QString passwordIter = v.toObject().value("password").toString();
        if (QString::compare(accountIter, account, Qt::CaseSensitive) == 0) {
            if (!verifyPassword || (QString::compare(passwordIter, password, Qt::CaseSensitive) == 0)) {
                found = true;
                bufferObj = v.toObject();
                bufferObj.insert("password", newPassword);
                array.replace(i, bufferObj);
            }
            break;
        }
    }
    if (found) {
        docObject["accountsArray"] = array;
        file.open(QIODevice::WriteOnly | QIODevice::Truncate);
        document.setObject(docObject);
        file.write(document.toJson());
        file.close();
    }

    return found;
}

const QHash<QString, User::RequestCallback> User::requests = {
    {requestAuthenticationKey, &User::requestAuthentication},
    {requestChangePasswordKey, &User::requestChangePassword},
    {requestResetPasswordKey, &User::requestResetPassword}
};
