#ifndef _TELEBAR_SERVERGETNOTIFICATIONHANDLER_
#define _TELEBAR_SERVERGETNOTIFICATIONHANDLER_

#include <string>

#include <telebar/entity/User.hpp>
#include <telebar/utils/ORM.hpp>
#include <telebar/entity/Game.hpp>
#include <telebar/configuration/serverConfiguration.hpp>
#include <telebar/configuration/gameConfiguration.hpp>
#include <telebar/entity/UserNotification.hpp>

/**
 * gets a single notification for the requesting, if there is a notification, it returns it serialized to the user
 * and removes it from the database
 * @param orm the database
 * @param user the requesting user
 * @param payload not used
 * @return OK + a serialized stream or NONE if there are no notifications
 */
std::string serverGetNotificationHandler(ORM &orm, User user, std::string payload);

#endif