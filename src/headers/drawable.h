#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "notification_element.h"

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

#endif