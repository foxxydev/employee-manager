#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QWebSocket>

// Fix circular dependency with forward declaration.
class Daemon;
class Client final : public QObject {

    Q_OBJECT

public:
    Client(QWebSocket *, Daemon* = nullptr);
    ~Client();

private slots:
    void request(const QString &);
    void onNotification(const QVariantMap &);

private:
    void handleCommand(const QVariantMap &);

private:
    QWebSocket *mSocket;
    Daemon *mParent;
};

#endif // CLIENT_H
