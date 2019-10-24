#ifndef _TELEBAR_HANDLER_SERVERHANDLER_
#define _TELEBAR_HANDLER_SERVERHANDLER_

#include <string>

#include <telebar/entity/User.hpp>
#include <telebar/utils/ORM.hpp>
#include <telebar/handler/serverHandler/serverGameHandler/serverGameHandlerOrchestrator.hpp>


std::string serverHandlerOrchestrator(ORM& orm, std::string request);

User getUserFromToken(ORM& orm, const std::string& token);

std::string logInHandler(ORM& orm, const std::string& loginPayload);

std::string registerHandler(ORM& orm, const std::string& loginPayload);


#endif