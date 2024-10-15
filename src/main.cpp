#include <iostream>
#include <cstdlib>
#include "notificationmanager.h"

int main() {
    NotificationManager manager;
    int choice;

    do {
        std::cout << "1. Create Notification\n"
                  << "2. Create Timed Notification\n"
                  << "3. Read Notifications\n"
                  << "4. Update Notification\n"
                  << "5. Delete Notification\n"
                  << "6. Exit\n"
                  << "Choose an option: ";
        std::cin >> choice;

        std::system("clear");

        switch (choice) {
            case 1: {
                std::string title;
                std::string message;

                std::cin.ignore();
                std::cout << "Enter title: ";
                std::getline(std::cin, title);
                std::cout << "Enter message: ";
                std::getline(std::cin, message);
                manager.createNotification(title, message);
                break;
            }
            case 2: {
                std::string title;
                std::string message;
                int duration;

                std::cin.ignore();
                std::cout << "Enter title: ";
                std::getline(std::cin, title);
                std::cout << "Enter message: ";
                std::getline(std::cin, message);
                std::cout << "Enter duration in seconds: ";
                std::cin >> duration;
                manager.createTimedNotification(title, message, duration);
                break;
            }
            case 3:
                std::system("clear");
                manager.readNotifications();
                break;
            case 4: {
                size_t index;
                std::string newTitle;
                std::string newMessage;

                std::cout << "Enter index of notification to update: ";
                std::cin >> index;
                std::cin.ignore();
                std::cout << "Enter new title: ";
                std::getline(std::cin, newTitle);
                std::cout << "Enter new message: ";
                std::getline(std::cin, newMessage);
                manager.updateNotification(index, newTitle, newMessage);
                break;
            }
            case 5: {
                size_t index;
                std::cout << "Enter index of notification to delete: ";
                std::cin >> index;
                manager.deleteNotification(index);
                break;
            }
            case 6:
                std::cout << "Exiting..." << std::endl;
                break;
            default:
                std::cout << "Invalid choice!" << std::endl;
        }
    } while (choice != 6);

    return 0;
}
