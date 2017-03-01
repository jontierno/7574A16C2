
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

add_executable(Cliente cliente.cxx ${HEADERS_FILES})
add_executable(Destructor destructor.cxx ${HEADERS_FILES})
add_executable(MiddlewareServer middlewareserver.cxx ${HEADERS_FILES})
add_executable(MiddlewareWorker middlewareworker.cxx ${HEADERS_FILES})

add_executable(MiddlewareClient middlewareclient.cxx ${HEADERS_FILES})
add_executable(Traductor traductor.cxx ${HEADERS_FILES})


add_custom_command(TARGET Cliente POST_BUILD 
  COMMAND "${CMAKE_COMMAND}" -E copy
     "${CMAKE_CURRENT_BINARY_DIR}/Cliente"
     "${CMAKE_SOURCE_DIR}/demo/cliente" 
  COMMENT "Copiando Cliente al directorio demo")


add_custom_command(TARGET Destructor POST_BUILD 
  COMMAND "${CMAKE_COMMAND}" -E copy
     "${CMAKE_CURRENT_BINARY_DIR}/Destructor"
     "${CMAKE_SOURCE_DIR}/demo/cliente" 
  COMMENT "Copiando Destructor al directorio demo")

add_custom_command(TARGET MiddlewareServer POST_BUILD 
  COMMAND "${CMAKE_COMMAND}" -E copy
     "${CMAKE_CURRENT_BINARY_DIR}/MiddlewareServer"
     "${CMAKE_SOURCE_DIR}/demo/cliente" 
  COMMENT "Copiando MiddlewareServer al directorio demo")

add_custom_command(TARGET MiddlewareWorker POST_BUILD 
  COMMAND "${CMAKE_COMMAND}" -E copy
     "${CMAKE_CURRENT_BINARY_DIR}/MiddlewareWorker"
     "${CMAKE_SOURCE_DIR}/demo/cliente" 
  COMMENT "Copiando MiddlewareWorker al directorio demo")




add_custom_command(TARGET MiddlewareClient POST_BUILD 
  COMMAND "${CMAKE_COMMAND}" -E copy
     "${CMAKE_CURRENT_BINARY_DIR}/MiddlewareClient"
     "${CMAKE_SOURCE_DIR}/demo/traductor" 
  COMMENT "Copiando MiddlewareClient al directorio demo")


add_custom_command(TARGET Traductor POST_BUILD 
  COMMAND "${CMAKE_COMMAND}" -E copy
     "${CMAKE_CURRENT_BINARY_DIR}/Traductor"
     "${CMAKE_SOURCE_DIR}/demo/traductor" 
  COMMENT "Copiando Traductor al directorio demo")
