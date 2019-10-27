#ifndef _TELEBAR_SERVERSETBOARDHANDLER_
#define _TELEBAR_SERVERSETBOARDHANDLER_

#include <string>

#include <telebar/entity/User.hpp>
#include <telebar/utils/ORM.hpp>
#include <telebar/entity/Game.hpp>
#include <telebar/configuration/serverConfiguration.hpp>
#include <telebar/configuration/gameConfiguration.hpp>
#include <telebar/entity/GameAction.hpp>
#include <telebar/entity/UserNotification.hpp>


std::string serverSetBoardHandler(ORM &orm, User user, std::string payload);

#endif