/*
 * snode.c - a slim toolkit for network communication
 * Copyright (C) 2020 Volker Christian <me@vchrist.at>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef HTTP_CLIENT_CLIENT_H
#define HTTP_CLIENT_CLIENT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <any>
#include <cstddef>
#include <functional>
#include <map>
#include <netinet/in.h>
#include <string>

#endif // DOXYGEN_SHOULD_SKIP_THIS

#include "http/client/ClientContext.hpp"
#include "log/Logger.h"

namespace http::client {

    template <typename SocketClientT, typename RequestT, typename ResponseT>
    class Client {
    public:
        using SocketClient = SocketClientT;
        using SocketConnection = typename SocketClient::SocketConnection;
        using SocketAddress = typename SocketConnection::SocketAddress;
        using Request = RequestT;
        using Response = ResponseT;

        Client(const std::function<void(SocketConnection*)>& onConnect,
               const std::function<void(Request&)>& onRequestBegin,
               const std::function<void(Response&)>& onResponse,
               const std::function<void(int, const std::string&)>& onResponseError,
               const std::function<void(SocketConnection*)>& onDisconnect,
               const std::map<std::string, std::any>& options = {{}})
            : socketClient(
                  [onResponse, onResponseError](SocketConnection* socketConnection) -> void { // onConstruct
                      ClientContext<Request, Response>* clientContext =
                          new ClientContext<Request, Response>(socketConnection, onResponse, onResponseError);
                      socketConnection->template setContext<ClientContextBase*>(clientContext);
                  },
                  [](SocketConnection* socketConnection) -> void { // onDestruct
                      socketConnection->template getContext<ClientContextBase*>([](ClientContextBase* clientContext) -> void {
                          delete clientContext;
                      });
                  },
                  [onConnect, onRequestBegin](SocketConnection* socketConnection) -> void { // onConnect
                      onConnect(socketConnection);

                      socketConnection->template getContext<ClientContextBase*>(
                          [&socketConnection, &onRequestBegin](ClientContextBase* clientContext) -> void {
                              Request& request = clientContext->getRequest();

                              request.setHost(socketConnection->getRemoteAddress().host());
                              onRequestBegin(request);
                          });
                  },
                  [onDisconnect](SocketConnection* socketConnection) -> void { // onDisconnect
                      onDisconnect(socketConnection);
                  },
                  [](SocketConnection* socketConnection, const char* junk, std::size_t junkLen) -> void { // onRead
                      socketConnection->template getContext<ClientContextBase*>([junk, junkLen](ClientContextBase* clientContext) -> void {
                          clientContext->receiveResponseData(junk, junkLen);
                      });
                  },
                  [](SocketConnection* socketConnection, int errnum) -> void { // onReadError
                      if (errnum != 0) {
                          PLOG(ERROR) << "Server: " << socketConnection->getRemoteAddress().host() << " (" << errnum << ")";
                      } else {
                          VLOG(0) << "Server: EOF";
                      }
                  },
                  [](SocketConnection* socketConnection, int errnum) -> void { // onWriteError
                      if (errnum != 0) {
                          PLOG(ERROR) << "Server: " << socketConnection->getRemoteAddress().host() << " (" << errnum << ")";
                      } else {
                          VLOG(0) << "Server: EOF";
                      }
                  },
                  options) {
        }

        void connect(const std::string& ipOrHostname, uint16_t port, const std::function<void(int err)>& onError) {
            socketClient.connect(SocketAddress(ipOrHostname, port), onError);
        }

    protected:
        SocketClient socketClient;
    };

} // namespace http::client

#endif // HTTP_CLIENT_CLIENT_H
