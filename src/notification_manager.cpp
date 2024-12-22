#include "notification_manager.h"
#include <iostream>

void NotificationManager::createNotification(const std::string &title, const std::string &message) {
    std::shared_lock lock(vec_mutex);
    notifications.add(std::make_shared<Notification>(title, message));
}

void NotificationManager::createTimedNotification(const std::string &title, const std::string &message, int duration) {
    std::shared_lock lock(vec_mutex);
    notifications.add(std::make_shared<Notification>(title, message, duration));
    if (duration > 0) {
            (*(notifications.back())).startTimer();
    }
}

void NotificationManager::readNotifications() {
    std::shared_lock lock(vec_mutex);
    for (const auto &notification : notifications) {
        std::cout << notification;
    }
}

void NotificationManager::updateNotification(size_t index, std::string &newTitle, std::string &newMessage) {
    std::shared_lock lock(vec_mutex);
    if (index < notifications.size()) {
        (*(notifications[index])).setTitle(std::move(newTitle));
        (*(notifications[index])).setMessage(std::move(newMessage));
    } else {
        std::cerr << "Invalid index!\n";
    }
}

void NotificationManager::deleteNotification(size_t index) {
    std::shared_lock lock(vec_mutex);
    if (index < notifications.size()) {
        notifications.remove(index);
    } else {
        std::cerr << "Invalid index!\n";
    }
}
