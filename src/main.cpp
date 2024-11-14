#include <iostream>
#include <cstdlib>
#include <iterator>
#include <ranges>

#include "notification_elements.h"
#include "notification_manager.h"
#include "config.h"

int main(int argc, char* argv[]) {

    std::vector<std::unique_ptr<HlNotificationElement>> elements;
    std::system("clear");
    int choice;
    do {
        std::cout << "1. Elements List\n"
                  << "2. Create Element\n"
                  << "3. Remove Element\n"
                  << "4. Get Element Info\n"
                  << "5. Draw Element\n"
                  << "6. Click Element\n"
                  << "7. Exit\n"
                  << "Choose an option: ";
        std::cin >> choice;
        std::system("clear");
        switch(choice) {
            case 1: {
                std::array<std::string, 3> typeNames = { "Text", "Image", "Button" };
                int index = 0;
                std::cout << "List of elements:\n";
                for (const auto& elem : elements) {
                    std::cout << "[" << index << "]: " << typeNames[static_cast<std::size_t>(elem->getType())] << " \"" << elem->getName() << "\"\n";
                    index++;
                }
                std::cout << "\n";
                break;
            }
            case 2: {
                int typeChoice;
                float relX;
                float relY;
                float relWidth;
                float relHeight;
                std::string name;
                std::cout << "Enter an element name: ";
                std::cin.ignore();
                std::cin >> name;
                std::cout << "Select an element type(1 - Text, 2 - Image, 3 - Button): ";
                std::cin >> typeChoice;

                switch (typeChoice)
                {
                    case 1: {
                        std::string text;
                        std::cout << "Enter the text of the textElement: ";
                        std::cin >> text;
                        std::cout << "Enter layout data of textElement(relX, relY, relWidth, relHeight): ";
                        std::cin >> relX;
                        std::cin >> relY;
                        std::cin >> relWidth;
                        std::cin >> relHeight;
                        elements.push_back(make_unique<HlTextElement>(name, relX, relY, relWidth, relHeight, text));
                        break;
                    }
                    case 2: {
                        std::string path;
                        std::cout << "Enter the path of the image: ";
                        std::cin >> path;
                        std::cout << "Enter layout data of imageElement(relX, relY, relWidth, relHeight): ";
                        std::cin >> relX;
                        std::cin >> relY;
                        std::cin >> relWidth;
                        std::cin >> relHeight;
                        elements.push_back(make_unique<HlImageElement>(name, relX, relY, relWidth, relHeight, path));
                        break;
                    }
                    case 3: {
                        std::string label;
                        std::cout << "Enter the label of the button: ";
                        std::cin >> label;
                        std::cout << "Enter layout data of buttonElement(relX, relY, relWidth, relHeight): ";
                        std::cin >> relX;
                        std::cin >> relY;
                        std::cin >> relWidth;
                        std::cin >> relHeight;
                        elements.push_back(make_unique<HlButtonElement>(name, relX, relY, relWidth, relHeight, label));
                        break;
                    }
                    default:
                        std::cout << "Invalid choice!\n";
                        break;
                    }
                    std::cout << "Element created successfully\n";
                break;
            }
            case 3: {
                int index;
                std::cout << "Enter the index of the element to be removed: ";
                std::cin >> index;
                if (index < 0 || index >= elements.size()) {
                    std::cout << "Incorrect index!\n";
                    break;
                }
                elements.erase(elements.begin() + index);
                std::cout << "Element removed successfully\n";
                break;
            }
            case 4: {
                std::array<std::string, 3> typeNames = { "Text", "Image", "Button" };
                int index;
                std::cout << "Enter the index of the element: ";
                std::cin >> index;
                if (index < 0 || index >= elements.size()) {
                    std::cout << "Incorrect index!\n";
                    break;
                }
                std::cout << typeNames[static_cast<std::size_t>(elements[index]->getType())] << "\n";
                break;
            }
            case 5: {
                int index;
                std::cout << "Enter the index of the element: ";
                std::cin >> index;
                if (index < 0 || index >= elements.size()) {
                    std::cout << "Incorrect index!\n";
                    break;
                }
                if (auto drawable = dynamic_cast<HlDrawable*>(elements[index].get())) {
                    drawable->draw();
                } else {
                    std::cout << "Selected element is not drawable!\n";
                }
                break;
            }
            case 6: {
                int index;
                std::cout << "Enter the index of the element: ";
                std::cin >> index;
                if (index < 0 || index >= elements.size()) {
                    std::cout << "Incorrect index!\n";
                    break;
                }
                if (auto clickable = dynamic_cast<HlClickable*>(elements[index].get())) {
                    clickable->onClick();
                } else {
                    std::cout << "Selected element is not clickable!\n";
                    std::cout << clickable;
                }
                break;
            }
            case 7: 
                break;
            default:
                std::cout << "Invalid choice!\n";
        }
    } while (choice != 7);

    return 0;
}
