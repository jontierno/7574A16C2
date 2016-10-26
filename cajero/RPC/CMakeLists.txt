cmake_minimum_required(VERSION 3.5)
project(tradicional)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_SERVER
    cajero.h
    cajero_server.cpp
    cajero_svc.c
    cajero_xdr.c
    constants.h)

set(SOURCE_CLIENT
        cajero.h
        cajero_client.cpp
        cajero_clnt.c
        cajero_xdr.c
        constants.h)





add_executable(server ${SOURCE_SERVER})
add_executable(client ${SOURCE_CLIENT})