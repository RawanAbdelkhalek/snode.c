/*
 * snode.c - a slim toolkit for network communication
 * Copyright (C) 2020, 2021 Volker Christian <me@vchrist.at>
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

#ifndef WEB_WS_CLIENT_WSSERVERCONTEXT_HPP
#define WEB_WS_CLIENT_WSSERVERCONTEXT_HPP

#include "WSClientContext.h"
#include "log/Logger.h"
#include "net/socket/stream/SocketConnectionBase.h"
#include "web/ws/WSProtocol.h"

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#define CLOSE_SOCKET_TIMEOUT 10

namespace web::ws::client {

    template <typename WSProtocol>
    WSClientContext<WSProtocol>::WSClientContext()
        : WSContext(new WSProtocol()) {
        WSContext::setWSContext(this);
    }

    template <typename WSProtocol>
    void WSClientContext<WSProtocol>::sendMessageStart(uint8_t opCode, const char* message, std::size_t messageLength) {
        WSTransmitter::sendMessageStart(opCode, message, messageLength, true);
    }

    template <typename WSProtocol>
    void WSClientContext<WSProtocol>::sendMessageFrame(const char* message, std::size_t messageLength) {
        WSTransmitter::sendMessageFrame(message, messageLength, true);
    }

    template <typename WSProtocol>
    void WSClientContext<WSProtocol>::sendMessageEnd(const char* message, std::size_t messageLength) {
        WSTransmitter::sendMessageEnd(message, messageLength, true);
    }

    template <typename WSProtocol>
    void WSClientContext<WSProtocol>::sendMessage(uint8_t opCode, const char* message, std::size_t messageLength) {
        WSTransmitter::sendMessage(opCode, message, messageLength, true);
    }

} // namespace web::ws::client

#endif // WEB_WS_CLIENT_WSSERVERCONTEXT_HPP
