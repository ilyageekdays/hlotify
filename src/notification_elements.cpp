#include <iostream>

#include "notification_element.h"
#include "button_element.h"
#include "image_element.h"
#include "text_element.h"

HlElementType HlNotificationElement::getType() const {
    return this->type;
}

std::string HlNotificationElement::getName() const {
    return this->elementName;
}

void HlTextElement::draw() {
    std::cout << "Drawing text (" << elementName << "): '" << text << "' at position (" << relX << ", " << relY << ")"
                << " with size (" << relWidth << ", " << relHeight << ")" << std::endl;
}

void HlImageElement::draw() {
    std::cout << "Drawing image(" << elementName << ") from '" << imagePath << "' at position (" << relX << ", " << relY << ")"
                << " with size (" << relWidth << ", " << relHeight << ")" << std::endl;
}

void HlButtonElement::draw() {
    std::cout << "Drawing button(" << elementName << "): '" << label << "' at position (" << relX << ", " << relY << ")"
                << " with size (" << relWidth << ", " << relHeight << ")" << std::endl;
}

void HlButtonElement::onClick() {
    std::cout << "Button(" << elementName << "): '" << label << "' is pressed" << std::endl;
}