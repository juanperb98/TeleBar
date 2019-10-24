#ifndef _TELEBAR_SERVERGETMESSAGEHANDLER_
#define _TELEBAR_SERVERGETMESSAGEHANDLER_

#include <string>

#include <telebar/entity/User.hpp>
#include <telebar/utils/ORM.hpp>
#include <telebar/entity/Game.hpp>
#include <telebar/configuration/serverConfiguration.hpp>
#include <telebar/configuration/gameConfiguration.hpp>
#include <telebar/entity/Message.hpp>
#include <telebar/entity/UserNotification.hpp>

/**
 * returns a message from a given message id
 * @param orm the database
 * @param user the requesting user
 * @param payload message id
 * @return string OK + the serialized message or ERROR if the message was not found
 */
std::string serverGetMessageHandler(ORM &orm, User user, std::string payload);

#endif