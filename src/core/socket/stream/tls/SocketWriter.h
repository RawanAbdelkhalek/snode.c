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

#ifndef CORE_SOCKET_STREAM_TLS_SOCKETWRITER_H
#define CORE_SOCKET_STREAM_TLS_SOCKETWRITER_H

#include "core/socket/stream/SocketWriter.h"
#include "core/socket/stream/tls/TLSHandshake.h"

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#include "core/socket/stream/tls/ssl_utils.h"
#include "log/Logger.h"

#include <openssl/err.h>
#include <openssl/ssl.h>

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace core::socket::stream::tls {

    template <typename SocketT>
    class SocketWriter : public core::socket::stream::SocketWriter<SocketT> {
    private:
        using Super = core::socket::stream::SocketWriter<SocketT>;
        using Super::Super;

        ssize_t write(const char* junk, std::size_t junkLen) override {
            int ret = 0;
            int ssl_err = sslErr;

            if (ssl_err == SSL_ERROR_NONE) {
                ret = SSL_write(ssl, junk, static_cast<int>(junkLen));

                if (ret <= 0) {
                    ssl_err = SSL_get_error(ssl, ret);
                }
            }

            switch (ssl_err) {
                case SSL_ERROR_NONE:
                    break;
                case SSL_ERROR_WANT_READ:
                    LOG(INFO) << "SSL/TLS start renegotiation on write";
                    doSSLHandshake(
                        [](void) -> void {
                            LOG(INFO) << "SSL/TLS renegotiation on write success";
                        },
                        [](void) -> void {
                            LOG(WARNING) << "SSL/TLS renegotiation on write timed out";
                        },
                        [this](int ssl_err) -> void {
                            ssl_log("SSL/TLS renegotiation", ssl_err);
                            sslErr = ssl_err;
                        });
                    errno = EINTR; // We simulate EINTR in case of a SSL_ERROR_WANT_READ (EAGAIN)
                    ret = -1;
                    break;
                case SSL_ERROR_WANT_WRITE:
                    errno = EINTR; // We simulate EINTR in case of a SSL_ERROR_WANT_READ (EAGAIN)
                    ret = -1;
                    break;
                case SSL_ERROR_ZERO_RETURN: // shutdown cleanly
                    ret = -1;               // on the write side this means a TCP broken pipe
                    break;
                case SSL_ERROR_SYSCALL:
                    ret = -1;
                    break;
                default:
                    ssl_log("SSL/TLS write failed", ssl_err);
                    errno = EPIPE;
                    ret = -1;
                    break;
            }

            return ret;
        }

    protected:
        void terminate() override {
            Super::terminate();
        }

        virtual void doSSLHandshake(const std::function<void()>& onSuccess,
                                    const std::function<void()>& onTimeout,
                                    const std::function<void(int)>& onError) = 0;

        SSL* ssl = nullptr;

        int sslErr = SSL_ERROR_NONE;
    };

} // namespace core::socket::stream::tls

#endif // CORE_SOCKET_STREAM_TLS_SOCKETWRITER_H
