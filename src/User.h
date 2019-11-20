#ifndef USER_H
#define USER_H

#include <QtCore>
#include <QJsonArray>
#include <QJsonValue>

#include "Request.h"

class User : public Request {

    Q_OBJECT

public:
    Q_INVOKABLE explicit User(QObject * = nullptr);

    bool exec(const QVariantMap &) override;
    bool match(const QString &) const override;

private:
    bool requestAuthentication(const QVariantMap &);
    bool requestChangePassword(const QVariantMap &);
    bool requestResetPassword(const QVariantMap &);

    void createUserDataFile();

    QString getDefaultServicePassword();
    QJsonArray getAccountValues();
    QJsonValue getSetting(QString setting);
    bool setSetting(QString setting, QJsonValue value);
    bool setNewPassword(QString account, QString password, QString newPassword, bool verifyPassword = true);

    typedef bool (User::*RequestCallback)(const QVariantMap &);
    static const QHash<QString, RequestCallback> requests;
};

#endif // USER_H
