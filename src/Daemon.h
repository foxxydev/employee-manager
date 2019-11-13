#ifndef DAEMON_H
#define DAEMON_H

#include <QObject>
#include <QWebSocketServer>

class Daemon : public QObject {

    Q_OBJECT

public:
    explicit Daemon(QObject * = nullptr);

    int port() const;
    void setPort(int);

public slots:
    void start();

private slots:
    void clientConnected();

private:
    int mPort;
    QWebSocketServer *mWebSocket;
};

#endif // DAEMON_H
