#include "notificationmanager.h"
#include <iostream>

void NotificationManager::createNotification(const std::string &title, const std::string &message) {
    notifications.emplace_back(title, message);
}

void NotificationManager::createTimedNotification(const std::string &title, const std::string &message, int duration) {
    notifications.emplace_back(title, message, duration);
}

void NotificationManager::readNotifications() const {
    for (const auto &notification : notifications) {
        notification.display();
        std::cout << std::endl;
    }
}

void NotificationManager::updateNotification(size_t index, const std::string &newTitle, const std::string &newMessage) {
    if (index < notifications.size()) {
        notifications[index] = Notification(newTitle, newMessage);
    } else {
        std::cerr << "Invalid index!" << std::endl;
    }
}

void NotificationManager::deleteNotification(size_t index) {
    if (index < notifications.size()) {
        notifications.erase(notifications.begin() + index);
    } else {
        std::cerr << "Invalid index!" << std::endl;
    }
}
