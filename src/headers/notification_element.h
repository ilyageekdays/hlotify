#ifndef NOTIFELEMENT_H
#define NOTIFELEMENT_H

#include <string>

enum class HlElementType {Text, Image, Button};

class HlNotificationElement {
public:
    HlElementType getType() const;
    std::string getName() const;
    virtual ~HlNotificationElement() = default;

protected:
    HlNotificationElement(HlElementType type, const std::string& name)
        : type(type), elementName(name) {}
    HlElementType type;
    std::string elementName;
};

#endif // NOTIFELEMENTS_H