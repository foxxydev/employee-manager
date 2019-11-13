#ifndef REQUESTFACTORY_H
#define REQUESTFACTORY_H

#include <QMap>
#include <QString>
#include <QMetaObject>

#include "Singleton.h"

#include "Request.h"

namespace detail {
class RequestFactory
{
public:
    void registerRequest(const Request *);
    Request *createRequest(const QString &, QObject * = nullptr);

private:
    QList<const Request *> mRequests;
};
} // namespace detail

typedef Singleton<detail::RequestFactory> RequestFactory;

#endif // REQUESTFACTORY_H
