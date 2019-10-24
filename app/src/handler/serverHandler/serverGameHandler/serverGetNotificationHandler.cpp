#include <telebar/handler/serverHandler/serverGameHandler/serverGetNotificationHandler.hpp>


std::string serverGetNotificationHandler(ORM &orm, User user, std::string payload) {
    auto notifications = orm.getByField<UserNotification>("relatedUserId", std::to_string(user.getId()));
    if (notifications.empty())
        return "NONE";

    UserNotification notification = notifications[0];
    orm.remove(notifications[0]);
    return std::string("OK,") + notification.serialize();
}
