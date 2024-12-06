#ifndef CLICKABLE_H
#define CLICKABLE_H

#include "notification_element.h"

class HlClickable : virtual public HlNotificationElement {
public:
    virtual void onClick() = 0;
    virtual ~HlClickable() = default;
};

#endif