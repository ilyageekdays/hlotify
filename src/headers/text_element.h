#ifndef TEXTELEMENT_H
#define TEXTELEMENT_H

#include "drawable.h"

class HlTextElement : public HlDrawable {
public:
    HlTextElement(const std::string& name, float relX, float relY, float relWidth, float relHeight, const std::string& text)
        : HlNotificationElement(HlElementType::Text, name), HlDrawable(relX, relY, relWidth, relHeight), text(text) {}

    ~HlTextElement() = default;

    void draw() override;

private:
    std::string text;
};

#endif