#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <vector>

#include <openhmd.h>

#include <button.h>
#include <slider.h>
#include <label.h>


typedef enum
{
    LEFT_MOVE,
    RIGHT_MOVE,
    UP_MOVE,
    DOWN_MOVE
} move;


class UserInterface
{
public:
    UserInterface(float x, float y, float z,
                  float width, float height);
    void draw();
    void addControl(UserControl *control);
    void pointerFocus(ohmd_device* hmd);
    void drawPointer(ohmd_device* hmd);
    void clickEvent();

private:
    std::vector<UserControl *> controls;
    UserControl *focussedControl;

    float x, y, z;
    float width, height;
};

#endif // USERINTERFACE_H
