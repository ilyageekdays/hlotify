#include <iostream>
#include <cstdlib>
#include <iterator>
#include <ranges>

#include "notification_manager.h"
#include "hlotify_exception.h"
#include "config.h"

void printVector(const std::vector<std::string>& vec) {
    std::ranges::copy(vec, std::ostream_iterator<std::string>(std::cout, "\n"));
}

int main(int argc, char* argv[]) {
    std::system("clear");
    NotificationManager hlotifyManager;
    std::unique_ptr<HlotifyConfig> config;

    try {
        config = std::make_unique<HlotifyConfig>();
    } catch (const HlConfigLoadException& e) {
        std::cerr << "Caught HlConfigLoadException: " << e.what() << "\n";
        return EXIT_FAILURE;
    } catch (const HlConfigSaveException& e) {
        std::cerr << "Caught HlConfigSaveException: " << e.what() << "\n";
        return EXIT_FAILURE;
    } catch (const HlConfigCreateException& e) {
        std::cerr << "Caught HlConfigCreateException: " << e.what() << "\n";
        return EXIT_F       AILURE;
    } catch (const HlConfigException& e) {
        std::cerr << "Caught HlConfigException: " << e.what() << "\n";
        return EXIT_FAILURE;
    }

    int choice;

    do {
        std::cout << "1. Create Notification\n"
                  << "2. Create Timed Notification\n"
                  << "3. Read Notifications\n"
                  << "4. Update Notification\n"
                  << "5. Delete Notification\n"
                  << "6. Tree View of Config\n"
                  << "7. Get List of All Config Sections\n"
                  << "8. Set Value in Config\n"
                  << "9. Get Value in Config\n"
                  << "10. Remove Key in Config\n"
                  << "11. Remove Section in Config\n"
                  << "12. Exit\n"
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
                hlotifyManager.createNotification(title, message);
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
                hlotifyManager.createTimedNotification(title, message, duration);
                break;
            }
            case 3:
                std::system("clear");
                hlotifyManager.readNotifications();
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
                hlotifyManager.updateNotification(index, newTitle, newMessage);
                break;
            }
            case 5: {
                size_t index;
                std::cout << "Enter index of notification to delete: ";
                std::cin >> index;
                hlotifyManager.deleteNotification(index);
                break;
            }
            case 6: {
                std::system("clear");
                std::cout << config->treeView() << "\n\n";
                break;
            }
            case 7: {
                std::system("clear");
                std::cout << "List of Sections:\n";
                std::vector<std::string> vec = config->getSections();
                printVector(vec);
                std::cout << "\n\n";
                break;
            }
            case 8: {
                std::string section;
                std::string key;
                std::string value;

                std::system("clear");
                std::cout << "Enter section name: ";
                std::cin.ignore();
                std::cin >> section;
                std::cout << "Enter key name: ";
                std::cin.ignore();
                std::cin >> key;
                std::cout << "Enter value: ";
                std::cin.ignore();
                std::cin >> value;

                config->setValue(section, key, value);
                std::cout << "Value set successfully." << std::endl;
                break;
            }
            case 9: {
                std::string section;
                std::string key;

                std::system("clear");
                std::cout << "Enter section name: ";
                std::cin.ignore();
                std::cin >> section;
                std::cout << "Enter key name: ";
                std::cin.ignore();
                std::cin >> key;
                std::cout << "Requested value: " << config->getValue(section, key) << "\n";

                break;
            }
            case 10: {
                std::string section;
                std::string key;

                std::system("clear");
                std::cout << "Enter section name: ";
                std::cin.ignore();
                std::cin >> section;
                std::cout << "Enter key name: ";
                std::cin.ignore();
                std::cin >> key;
                config->deleteKey(section, key);
                std::cout << "Requested Key is Deleted!\n";

                break;
            }
            case 11: {
                std::string section;

                std::system("clear");
                std::cout << "Enter section name: ";
                std::cin.ignore();
                std::cin >> section;
                config->deleteSection(section);
                std::cout << "Requested Section is Deleted!\n";

                break;
            }
            case 12:
                std::cout << "Exiting..." << std::endl;
                break;
            default:
                std::cout << "Invalid choice!" << std::endl;
        }
    } while (choice != 12);

    return 0;
}