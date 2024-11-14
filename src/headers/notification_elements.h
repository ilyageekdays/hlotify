#ifndef NOTIFELEMENTS_H
#define NOTIFELEMENTS_H

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

class HlDrawable : virtual public HlNotificationElement {
public:
    virtual void draw() = 0;
    virtual ~HlDrawable() = default;
protected:
    HlDrawable(float relX, float relY, float relWidth, float relHeight)
        : relX(relX), relY(relY), relWidth(relWidth), relHeight(relHeight) {}
    float relX;
    float relY;
    float relWidth;
    float relHeight; 
};

class HlClickable : virtual public HlNotificationElement {
public:
    virtual void onClick() = 0;
    virtual ~HlClickable() = default;
};

class HlTextElement : public HlDrawable {
public:
    HlTextElement(const std::string& name, float relX, float relY, float relWidth, float relHeight, const std::string& text)
        : HlNotificationElement(HlElementType::Text, name), HlDrawable(relX, relY, relWidth, relHeight), text(text) {}

    ~HlTextElement() = default;

    void draw() override;

private:
    std::string text;
};

class HlImageElement : public HlDrawable {
public:
    HlImageElement(const std::string& name, float relX, float relY, float relWidth, float relHeight, const std::string& imagePath)
        : HlNotificationElement(HlElementType::Image, name), HlDrawable(relX, relY, relWidth, relHeight), imagePath(imagePath) {}

    ~HlImageElement() = default;

    void draw() override;

private:
    std::string imagePath;
};

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

#endif // NOTIFELEMENTS_H