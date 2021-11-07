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

#ifndef WEB_WEBSOCKET_SUBPROTOCOLPLUGININTERFACE_H
#define WEB_WEBSOCKET_SUBPROTOCOLPLUGININTERFACE_H

#include "log/Logger.h"

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include <string>

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace web::websocket {

    template <typename SubProtocolT>
    class SubProtocolFactory {
    public:
        using SubProtocol = SubProtocolT;

        SubProtocolFactory(const std::string& name)
            : name(name) {
        }

        SubProtocolFactory() = delete;

        virtual ~SubProtocolFactory() = default;

        virtual SubProtocol* createSubProtocol() {
            SubProtocol* subProtocol = create();

            if (subProtocol != nullptr) {
                subProtocol->setName(name);
                refCount++;
            }

            return subProtocol;
        }

        virtual void deleteSubProtocol(SubProtocol* subProtocol) {
            delete subProtocol;

            refCount--;
        }

        std::string getName() {
            return name;
        }

        virtual SubProtocol* create() = 0;

    protected:
        std::size_t refCount = 0;

        std::string name;
    };

} // namespace web::websocket

#endif // WEB_WEBSOCKET_SUBPROTOCOLPLUGININTERFACE_H
