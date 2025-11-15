cmake_minimum_required(VERSION 3.10)
project(DigitalNotebook)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenSSL REQUIRED)
find_package(CURL REQUIRED)

include_directories(${OPENSSL_INCLUDE_DIR})
include_directories(${CURL_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/backend)

add_executable(notebook_server
    backend/server.cpp
    backend/User.cpp
    backend/Note.cpp
    backend/NoteList.cpp
    backend/AIIntegration.cpp
)

target_link_libraries(notebook_server
    ${OPENSSL_LIBRARIES}
    ${CURL_LIBRARIES}
    pthread
)

install(TARGETS notebook_server DESTINATION bin)
