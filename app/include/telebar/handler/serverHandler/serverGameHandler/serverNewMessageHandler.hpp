#ifndef _TELEBAR_SERVERNEWMESSAGEHANDLER_
#define _TELEBAR_SERVERNEWMESSAGEHANDLER_

#include <string>

#include <telebar/entity/User.hpp>
#include <telebar/utils/ORM.hpp>
#include <telebar/entity/Game.hpp>
#include <telebar/configuration/serverConfiguration.hpp>
#include <telebar/configuration/gameConfiguration.hpp>
#include <telebar/entity/Message.hpp>

/**
 * publish a new message and make new message notifications to all the users in the same game
 * a message can be send only if the user is in a game
 * @param orm the database
 * @param user the requesting user
 * @param payload the message
 * @return just the OK ACK
 */
std::string serverNewMessageHandler(ORM &orm, User user, std::string payload);

#endif