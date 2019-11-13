#include "Const.h"
#include "RequestFactory.h"

namespace detail {

void RequestFactory::registerRequest(const Request *request)
{
    mRequests.append(request);
}

Request *RequestFactory::createRequest(const QString &name, QObject *parent)
{
    for (const auto &request : asConst(mRequests)) {
        if (request->match(name)) {
            const auto requestInstance = request->metaObject()->newInstance();
            Q_ASSERT(requestInstance);

            if (parent) {
                requestInstance->setParent(parent);
            }

            return qobject_cast<Request *>(requestInstance);
        }
    }

    return nullptr;
}

} // namespace detail
