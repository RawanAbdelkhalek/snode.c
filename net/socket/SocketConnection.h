#ifndef SOCKETCONNECTION_H
#define SOCKETCONNECTION_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <functional>

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#include "socket/InetAddress.h"

class SocketConnection {
public:
    SocketConnection(const SocketConnection&) = delete;
    SocketConnection& operator=(const SocketConnection&) = delete;

    virtual ~SocketConnection() = default;

    void setContext(void* context) {
        this->context = context;
    }

    void* getContext() {
        return this->context;
    }

    virtual void enqueue(const char* buffer, int size) = 0;

    virtual void end() = 0;

    virtual void stashReader() = 0;
    virtual void unstashReader() = 0;

    virtual void stashWriter() = 0;
    virtual void unstashWriter() = 0;

    virtual InetAddress& getRemoteAddress() = 0;
    virtual void setRemoteAddress(const InetAddress& remoteAddress) = 0;

protected:
    SocketConnection() = default;

private:
    void* context{nullptr};
};

#endif // SOCKETCONNECTION_H
