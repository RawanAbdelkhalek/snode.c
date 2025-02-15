cmake_minimum_required(VERSION 3.0)

find_package(PkgConfig REQUIRED)

pkg_check_modules(BLUEZ bluez)

if(BLUEZ_FOUND)
    set(NET-L2_CPP
        SocketAddress.cpp
        config/ConfigAddress.cpp
        stream/Socket.cpp
        stream/config/ConfigClientSocket.cpp
        stream/config/ConfigServerSocket.cpp
        stream/legacy/SocketClient.cpp
        stream/legacy/SocketServer.cpp
        stream/tls/SocketClient.cpp
        stream/tls/SocketServer.cpp
    )

    set(NET-L2_H
        SocketAddress.h
        config/ConfigAddress.h
        stream/ClientSocket.h
        stream/ClientSocket.hpp
        stream/ServerSocket.h
        stream/ServerSocket.hpp
        stream/Socket.h
        stream/config/ConfigClientSocket.h
        stream/config/ConfigServerSocket.h
        stream/legacy/SocketClient.h
        stream/legacy/SocketServer.h
        stream/legacy/config/ConfigSocketClient.h
        stream/legacy/config/ConfigSocketServer.h
        stream/tls/SocketClient.h
        stream/tls/SocketServer.h
        stream/tls/config/ConfigSocketClient.h
        stream/tls/config/ConfigSocketServer.h
    )

    add_library(net-l2 SHARED ${NET-L2_CPP} ${NET-L2_H})
    add_library(snodec::net-l2 ALIAS net-l2)

    target_link_libraries(net-l2 PUBLIC snodec::net ${BLUEZ_LIBRARIES})

    target_include_directories(
        net-l2
        PUBLIC "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>"
               "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
               "$<INSTALL_INTERFACE:include/snode.c>"
    )

    set_target_properties(
        net-l2 PROPERTIES SOVERSION 1 OUTPUT_NAME snodec-net-l2
    )

    install(
        TARGETS net-l2
        EXPORT snodec_net-l2_Targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        INCLUDES
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/snode.c
    )

    install(
        DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/snode.c/net/l2cap
        FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp"
        PATTERN "cmake" EXCLUDE
    )

    install(
        EXPORT snodec_net-l2_Targets
        FILE snodec_net-l2_Targets.cmake
        NAMESPACE snodec::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/snodec
    )

else(BLUEZ_FOUND)
    message(
        WARNING
            " libbluetooth-dev not found:\n"
            "    If you want to get bluetooth support you should install libbluetooth-dev."
    )
endif(BLUEZ_FOUND)
