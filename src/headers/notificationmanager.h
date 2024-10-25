#ifndef NOTIFICATIONMANAGER_H
#define NOTIFICATIONMANAGER_H

#include "notification.h"
#include <vector>
#include <mutex>

class NotificationManager {
public:
    void createNotification(const std::string &title, const std::string &message);
    void createTimedNotification(const std::string &title, const std::string &message, int duration);
    void readNotifications();
    void updateNotification(size_t index, std::string &newTitle, std::string &newMessage);
    void deleteNotification(size_t index);

private:
    std::vector<std::shared_ptr<Notification>> notifications;
    std::mutex vec_mutex;
};

#endif // NOTIFICATIONMANAGER_H
