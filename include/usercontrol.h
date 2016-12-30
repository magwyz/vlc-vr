#ifndef USERCONTROL_H
#define USERCONTROL_H

#include <iostream>
#include <functional>

#include <math.h>
#include <string>

class PlayerController;


typedef enum
{
    REST,
    FOCUSSED,
    ACTIVATING,
    LOCKED,
    UNVISIBLE,
    UNSELECTABLE,
} state;


struct Position
{
    Position(float x, float y)
        : x(x), y(y) { }
    float x, y;
};


class UserControl
{
public:
    UserControl(float x, float y,
                float width, float height);

    virtual void draw();

    void setState(state s);
    state getState();
    Position getCenter();
    float getDistance(UserControl *c);

    bool intersect(float x1, float y1);

    void setVisibility(bool visible);
    void setOnCLickCallback(PlayerController *c, std::function<void(PlayerController&)> f);
    void setOnLockCallback(PlayerController *c, std::function<void(PlayerController&)> f);
    void setOnUnlockCallback(PlayerController *c, std::function<void(PlayerController&)> f);

    virtual void clickEvent();

    virtual void pointerMoveEvent(float pointer_x, float pointer_y);


protected:
    float x, y;
    float width, height;

    std::function<void(PlayerController&)> onClick;
    PlayerController *onClickController;

    std::function<void(PlayerController&)> onLock;
    PlayerController *onLockController;

    std::function<void(PlayerController&)> onUnlock;
    PlayerController *onUnlockController;

    std::function<void(PlayerController&, float, float)> pointerMove;
    PlayerController *pointerMoveController;

private:
    state currentState;
};

#endif // USERCONTROL_H
