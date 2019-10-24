#ifndef _TELEBAR_HANDLER_SERVERGAMEHANDLER_
#define _TELEBAR_HANDLER_SERVERGAMEHANDLER_

#include <string>

#include <telebar/entity/User.hpp>
#include <telebar/utils/ORM.hpp>
#include <telebar/entity/Game.hpp>
#include <telebar/configuration/serverConfiguration.hpp>
#include <telebar/handler/serverHandler/serverGameHandler/serverStartGameHandler.hpp>
#include <telebar/handler/serverHandler/serverGameHandler/serverNewMessageHandler.hpp>
#include <telebar/handler/serverHandler/serverGameHandler/serverGetMessageHandler.hpp>
#include <telebar/handler/serverHandler/serverGameHandler/serverGetNotificationHandler.hpp>
#include <telebar/handler/serverHandler/serverGameHandler/serverGetBoardHandler.hpp>
#include <telebar/configuration/gameConfiguration.hpp>

/**
 * this function orchestrates and redirects the request to its handler
 * @param orm the database object
 * @param user the requesting user
 * @param action the desired action to be handlerd
 * @param payload the data related to the action
 * @return string response to be send to the client
 */
std::string serverGameHandlerOrchestrator(ORM& orm, User user, std::string action, std::string payload);

#endif
