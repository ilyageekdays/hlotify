#ifndef NOTIFICATIONMANAGER_H
#define NOTIFICATIONMANAGER_H

#include "notification.h"
#include <vector>

class NotificationManager {
public:
    void createNotification(const std::string &title, const std::string &message);
    void createTimedNotification(const std::string &title, const std::string &message, int duration);
    void readNotifications() const;
    void updateNotification(size_t index, const std::string &newTitle, const std::string &newMessage);
    void deleteNotification(size_t index);

private:
    std::vector<Notification> notifications;
};

#endif // NOTIFICATIONMANAGER_H
