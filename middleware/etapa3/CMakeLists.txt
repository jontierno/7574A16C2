
project(7574MIDDLEWARE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(HEADERS_FILES
    include/connector.h
    include/constants.h
    include/logger.h
    include/middleware.h
    include/msg.h
    include/resources.h
    include/utils.h)

add_executable(Destructor destructor.cxx ${HEADERS_FILES})
add_executable(MiddlewareServer middlewareserver.cxx ${HEADERS_FILES})
add_executable(MiddlewareClient middlewareclient.cxx ${HEADERS_FILES})
add_executable(MiddlewareWorker middlewareworker.cxx ${HEADERS_FILES})
add_executable(Cliente cliente.cxx ${HEADERS_FILES})
add_executable(Traductor traductor.cxx ${HEADERS_FILES})

