cmake_minimum_required(VERSION 3.0)

set(HTTP_SOCKETCONTEXTUPGRADE_SERVER_COMPILE_LIBDIR
    ${HTTP_SOCKETCONTEXTUPGRADE_COMPILE_LIBDIR}/server
)

set(HTTP_SOCKETCONTEXTUPGRADE_SERVER_INSTALL_LIBDIR
    ${HTTP_SOCKETCONTEXTUPGRADE_INSTALL_LIBDIR}/server
)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(PkgConfig REQUIRED)

pkg_check_modules(LIBMAGIC libmagic)

set(HTTPSERVER_CPP
    Request.cpp RequestParser.cpp Response.cpp SocketContextUpgradeFactory.cpp
    SocketContextUpgradeFactorySelector.cpp
)

set(HTTPSERVER_H
    Request.h
    RequestParser.h
    Response.h
    Server.h
    SocketContext.h
    SocketContext.hpp
    SocketContextFactory.h
    SocketContextUpgradeFactory.h
    SocketContextUpgradeFactorySelector.h
    ../legacy/in/Server.h
    ../legacy/in6/Server.h
    ../legacy/rf/Server.h
    ../tls/in/Server.h
    ../tls/in6/Server.h
    ../tls/rf/Server.h
)

add_library(httpserver SHARED ${HTTPSERVER_CPP} ${HTTPSERVER_h})
add_library(snodec::httpserver ALIAS httpserver)

if(LIBMAGIC_FOUND)
    target_compile_definitions(httpserver PUBLIC HAS_LIBMAGIC)
else(LIBMAGIC_FOUND)
    message(
        WARNING
            " libmagic-dev not found:\n"
            "    libmagic is used for the automatic detection of Mime-type files.\n"
            "    SNode.C has some built-in knowledge of mime types which, compared to libmime,\n"
            "    is not as reliable. So you should install libmagic-dev for better mime type detection."
    )
endif(LIBMAGIC_FOUND)

target_compile_definitions(
    httpserver
    PRIVATE
        HTTP_SOCKETCONTEXTUPGRADE_SERVER_COMPILE_LIBDIR="${HTTP_SOCKETCONTEXTUPGRADE_SERVER_COMPILE_LIBDIR}"
        HTTP_SOCKETCONTEXTUPGRADE_SERVER_INSTALL_LIBDIR="${CMAKE_INSTALL_PREFIX}/${HTTP_SOCKETCONTEXTUPGRADE_SERVER_INSTALL_LIBDIR}"
)

target_include_directories(
    httpserver
    PUBLIC "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>"
           "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
           "$<BUILD_INTERFACE:${LIBMAGIC_INCLUDE_DIRS}>"
           "$<INSTALL_INTERFACE:include/snode.c>"
)

target_link_libraries(httpserver PUBLIC snodec::http ${LIBMAGIC_LIBRARIES} dl)

set_target_properties(
    httpserver
    PROPERTIES SOVERSION 1
               OUTPUT_NAME snodec-http-server
               HTTP_SOCKETCONTEXTUPGRADE_SERVER_COMPILE_LIBDIR
               "${HTTP_SOCKETCONTEXTUPGRADE_SERVER_COMPILE_LIBDIR}"
               HTTP_SOCKETCONTEXTUPGRADE_SERVER_INSTALL_LIBDIR
               "${HTTP_SOCKETCONTEXTUPGRADE_SERVER_INSTALL_LIBDIR}"
               EXPORT_PROPERTIES
               "HTTP_SOCKETCONTEXTUPGRADE_SERVER_INSTALL_LIBDIR"
)

install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION include/snode.c/web/http/server
    FILES_MATCHING
    PATTERN "*.h"
    PATTERN "*.hpp"
    PATTERN "cmake" EXCLUDE
)

install(
    TARGETS httpserver
    EXPORT snodec_httpserver_Targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/snode.c
)

install(
    EXPORT snodec_httpserver_Targets
    FILE snodec_httpserver_Targets.cmake
    NAMESPACE snodec::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/snodec
)
