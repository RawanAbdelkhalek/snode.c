/*
 * snode.c - a slim toolkit for network communication
 * Copyright (C) 2020, 2021, 2022 Volker Christian <me@vchrist.at>
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

#include "express/WebApp.h"

#include "core/SNodeC.h"

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace express {

    bool WebApp::initialized = false;

    WebApp::WebApp(const Router& router)
        : Router(router) {
    }

    void WebApp::init(int argc, char* argv[]) {
        core::SNodeC::init(argc, argv);
        WebApp::initialized = true;
    }

    int WebApp::start(const utils::Timeval& timeOut) {
        return core::SNodeC::start(timeOut);
    }

    void WebApp::stop() {
        core::SNodeC::stop();
    }

    core::TickStatus WebApp::tick(const utils::Timeval& timeOut) {
        return core::SNodeC::tick(timeOut);
    }

    void WebApp::free() {
        core::SNodeC::free();
    }

} // namespace express
