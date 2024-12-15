#ifndef NOTIFICATIONMANAGER_H
#define NOTIFICATIONMANAGER_H

#include <vector>
#include <shared_mutex>
#include <vector>

#include "notification.h"
#include "notification_container.h"

class NotificationManager {
public:
    void createNotification(const std::string &title, const std::string &message);
    void createTimedNotification(const std::string &title, const std::string &message, int duration);
    void readNotifications();
    void updateNotification(size_t index, std::string &newTitle, std::string &newMessage);
    void deleteNotification(size_t index);

private:
    //NotificationContainer<std::shared_ptr<Notification>> notifications;
    std::vector<> notifications;
    std::shared_mutex vec_mutex;
};

#endif // NOTIFICATIONMANAGER_H
