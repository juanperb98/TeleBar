#ifndef _TELEBAR_HANDLER_SERVERGAMEHANDLER_
#define _TELEBAR_HANDLER_SERVERGAMEHANDLER_

#include <string>

#include <telebar/entity/User.hpp>
#include <telebar/utils/ORM.hpp>


std::string serverGameHandlerOrchestrator(ORM& orm, User user, std::string action, std::string payload);

std::string serverStartGameHandler(ORM &orm, User user, std::string payload);

#endif