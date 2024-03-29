cmake_minimum_required (VERSION 2.8.11)

project (TeleBar)

enable_testing()

set(CLIENT_TARGET
        app/src/mainClient.cpp
)

set(GAME_TARGET
        app/src/game.cpp
)

set(SERVER_TARGET
        app/src/server.cpp
)

set(TESTS_TARGET
        app/src/tests/tests.cpp
)

set(CLIENT_SOURCES
        app/src/kernel/Client.cpp
)

set(GAME_SOURCES
        app/src/kernel/Client.cpp
)

set(SERVER_SOURCES
        app/src/kernel/Server.cpp
        app/src/handler/serverHandler/serverHandler.cpp
        app/src/handler/serverHandler/serverGameHandler/serverGameHandlerOrchestrator.cpp
        app/src/handler/serverHandler/serverGameHandler/serverStartGameHandler.cpp
        app/src/handler/serverHandler/serverGameHandler/serverGetBoardHandler.cpp
        app/src/handler/serverHandler/serverGameHandler/serverSetBoardHandler.cpp
        app/src/handler/serverHandler/serverGameHandler/serverGetNotificationHandler.cpp
        app/src/handler/serverHandler/serverGameHandler/serverGetMessageHandler.cpp
        app/src/handler/serverHandler/serverGameHandler/serverNewMessageHandler.cpp
)

set(COMMON_SOURCES
        app/src/entity/User.cpp
        app/src/interface/SerializableInterface.cpp
        app/src/utils/userAuthenticationUtils.cpp
        app/src/entity/Player.cpp
        app/src/entity/Piece.cpp
        app/src/entity/Game.cpp
        app/src/entity/Message.cpp
        app/src/entity/UserNotification.cpp
        app/src/entity/GameAction.cpp
)




add_executable(server ${SERVER_TARGET} ${SERVER_SOURCES} ${COMMON_SOURCES})
add_executable(client ${CLIENT_TARGET} ${CLIENT_SOURCES} ${COMMON_SOURCES})
add_executable(game ${GAME_TARGET} ${GAME_SOURCES} ${COMMON_SOURCES})
# add_executable(tests ${TEST_TARGET} ${CLIENT_SOURCES} ${SERVER_SOURCES} ${COMMON_SOURCES})

target_link_libraries(server sqlite3)

target_include_directories(server
        PRIVATE
        ${PROJECT_SOURCE_DIR}/app/include
)

target_include_directories(client
        PRIVATE
        ${PROJECT_SOURCE_DIR}/app/include
)

target_include_directories(game
        PRIVATE
        ${PROJECT_SOURCE_DIR}/app/include
)

#target_include_directories(tests
#    PRIVATE
#       ${PROJECT_SOURCE_DIR}/app/include
#)
