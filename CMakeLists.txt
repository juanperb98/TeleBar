cmake_minimum_required (VERSION 2.8.11)

project (TeleBar)

enable_testing()

set(CLIENT_TARGET
    app/src/client.cpp
)

set(SERVER_TARGET
    app/src/server.cpp
)

set(TESTS_TARGET
    app/src/tests/tests.cpp
)

set(CLIENT_SOURCES
)

set(SERVER_SOURCES
)

set(COMMON_SOURCES
)

include_directories("${CMAKE_SOURCE_DIR}/include")

add_executable(server ${SERVER_TARGET} ${SERVER_SOURCES} ${COMMON_SOURCES})
add_executable(client ${CLIENT_TARGET} ${CLIENT_SOURCES} ${COMMON_SOURCES})
# add_executable(tests ${TEST_TARGET} ${CLIENT_SOURCES} ${SERVER_SOURCES} ${COMMON_SOURCES})
