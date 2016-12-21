#ifndef SLIDER_H
#define SLIDER_H

#include <GL/gl.h>

#include <usercontrol.h>


class Slider : public UserControl
{
public:
    Slider(float x, float y,
           float width, float height);
    void draw();
    void setValue(float v);

    void clickEvent();
    void pointerMoveEvent(float pointer_x, float pointer_y);

    void setOnUserChangedValueCallback(PlayerController *c, std::function<void(PlayerController&, float)> f);

private:
    float value; // in [0; 1]

    std::function<void(PlayerController&, float)> onUserChangedValue;
    PlayerController *onUserChangedValueController;
};

#endif // SLIDER_H
