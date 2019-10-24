#ifndef _TELEBAR_SERVERGAMENEWGAMEHANDLER_
#define _TELEBAR_SERVERGAMENEWGAMEHANDLER_

#include <string>

#include <telebar/entity/User.hpp>
#include <telebar/utils/ORM.hpp>
#include <telebar/entity/Game.hpp>
#include <telebar/configuration/serverConfiguration.hpp>
#include <telebar/configuration/gameConfiguration.hpp>
#include <telebar/entity/UserNotification.hpp>

/**
 * this user handles a, user that wants to join a new game
 * the user will get a new or already existing game, and the user will get a gameId set in their object
 * @param orm the database
 * @param user the requesting user
 * @param payload the desired number of players in the game
 * @return string the OK or WAIT response, followed by the serialized game
 */
std::string serverStartGameHandler(ORM &orm, User user, std::string payload);

#endif