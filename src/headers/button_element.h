#ifndef BUTTONELEMENT_H
#define BUTTONELEMENT_H

#include "clickable.h"
#include "drawable.h"

class HlButtonElement : public HlDrawable, HlClickable {
public:
    HlButtonElement(const std::string& name, float relX, float relY, float relWidth, float relHeight, const std::string& label)
        : HlNotificationElement(HlElementType::Button, name), HlDrawable(relX, relY, relWidth, relHeight), label(label) {}

    ~HlButtonElement() = default;

    void draw() override;
    void onClick() override;

private:
    std::string label;
};

#endif