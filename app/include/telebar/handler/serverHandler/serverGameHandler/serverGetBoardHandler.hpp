#ifndef _TELEBAR_SERVERGETBOARDHANDLER_
#define _TELEBAR_SERVERGETBOARDHANDLER_

#include <string>

#include <telebar/entity/User.hpp>
#include <telebar/utils/ORM.hpp>
#include <telebar/entity/Game.hpp>
#include <telebar/configuration/serverConfiguration.hpp>
#include <telebar/configuration/gameConfiguration.hpp>

/**
 * this user handles a, user that wants to join a new game
 * the user will get a new or already existing game, and the user will get a gameId set in their object
 * @param orm the database
 * @param user the requesting user
 * @param payload not used
 * @return string the OK string followed by the serialized board or the ERROR string indicating that there is no board
 */
std::string serverGetBoardHandler(ORM &orm, User user, std::string payload);

#endif