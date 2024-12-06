#ifndef IMAGEELEMENT_H
#define IMAGEELEMENT_H

#include "drawable.h"

class HlImageElement : public HlDrawable {
public:
    HlImageElement(const std::string& name, float relX, float relY, float relWidth, float relHeight, const std::string& imagePath)
        : HlNotificationElement(HlElementType::Image, name), HlDrawable(relX, relY, relWidth, relHeight), imagePath(imagePath) {}

    ~HlImageElement() = default;

    void draw() override;

private:
    std::string imagePath;
};

#endif