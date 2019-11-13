#ifndef REQUEST_H
#define REQUEST_H

#include <memory>

#include <QLoggingCategory>
#include <QMap>
#include <QMetaObject>
#include <QObject>
#include <QSettings>
#include <QString>
#include <QVariant>

Q_DECLARE_LOGGING_CATEGORY(logRequest)

class Request : public QObject {

    Q_OBJECT

public:
    Request(const QString &, QObject * = nullptr);

    virtual const QString &name() const;
    virtual bool exec(const QVariantMap &) = 0;
    virtual bool match(const QString &) const;

protected:
    std::unique_ptr<QSettings> loadSettings(const QVariantMap &,
                                            const QString &);
    void emitError(const QVariantMap &, const QVariant &);
    void emitCompleted(const QVariantMap &, const QVariant &);
    void emitProgress(const QVariantMap &, const QVariant &);

signals:
    void data(const QByteArray &);
    void error(const QVariantMap &);
    void completed(const QVariantMap &);
    void progress(const QVariantMap &);

private:
    const QString mName;
};

#endif // REQUEST_H
