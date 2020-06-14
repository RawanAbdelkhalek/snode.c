#ifndef SOCKETCONNECTIONBASE_H
#define SOCKETCONNECTIONBASE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#include "SocketConnection.h"


class SocketServer;

template <typename Reader, typename Writer>
class SocketConnectionBase
    : public SocketConnection
    , public Reader
    , public Writer {
public:
    void enqueue(const char* buffer, int size) override;

    void end() override;

    void stashReader() override;
    void unstashReader() override;

    void stashWriter() override;
    void unstashWriter() override;

    InetAddress& getRemoteAddress() override;
    void setRemoteAddress(const InetAddress& remoteAddress) override;

protected:
    SocketConnectionBase(int csFd, SocketServer* serverSocket,
                         const std::function<void(SocketConnection* cs, const char* junk, ssize_t n)>& readProcessor,
                         const std::function<void(SocketConnection* cs, int errnum)>& onReadError,
                         const std::function<void(SocketConnection* cs, int errnum)>& onWriteError);

private:
    void unmanaged() override;

    SocketServer* serverSocket;

    InetAddress remoteAddress{};
};

#endif // SOCKETCONNECTIONBASE_H
