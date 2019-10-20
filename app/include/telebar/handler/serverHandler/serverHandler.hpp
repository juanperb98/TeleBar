#ifndef _TELEBAR_HANDLER_SERVERHANDLER_
#define _TELEBAR_HANDLER_SERVERHANDLER_

#include <string>

#include <telebar/entity/User.hpp>
#include <telebar/utils/ORM.hpp>


std::string serverHandlerOrchestrator(std::string request);

User authenticationHandler(const std::string& token);

std::string logInHandler(const std::string& loginPayload);
std::string registerHandler(const std::string& loginPayload);
#endif