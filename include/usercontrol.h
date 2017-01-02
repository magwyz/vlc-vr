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


template <class Controller> class UserControl
{
public:
    UserControl(float x, float y,
                float width, float height)
          : x(x), y(y),
          width(width), height(height),
          currentState(REST) {}

    virtual void draw()
    {
       /* Move to the control space. */
       glPushMatrix();
       glTranslatef(x, y, 0.f);

       /* Draw lines if the control has the focus. */
       if (currentState == FOCUSSED
           || currentState == LOCKED)
       {
           glPushMatrix();
           glTranslatef(0.f, 0.f, 0.001f);

           glColor4f(0.f, 0.f, 1.f, 1.f);

           glLineWidth(3.f);

           glBegin(GL_LINE_STRIP);
           glVertex3f( 0.f, 0.f, 0.f);
           glVertex3f( 0.f, height, 0.f);
           glVertex3f( width, height, 0.f);
           glVertex3f( width, 0.f, 0.f);
           glVertex3f( 0.f, 0.f, 0.f);
           glEnd();

           glPopMatrix();
       }

       glPopMatrix();
    }

    void setState(state s)
    {
        if (currentState == LOCKED && s != LOCKED)
            onUnlock(*controller);
        if (currentState != LOCKED && s == LOCKED)
            onLock(*controller);

        currentState = s;
    }

    state getState()
    {
        return currentState;
    }

    Position getCenter()
    {
        return Position((2 * x + width) / 2,
                        (2 * y + height) / 2);
    }

    float getDistance(UserControl *c)
    {
        Position c1 = getCenter();
        Position c2 = c->getCenter();
        float xDiff = c1.x - c2.x;
        float yDiff = c1.y - c2.y;
        return sqrtf(xDiff * xDiff + yDiff * yDiff);
    }

    bool intersect(float x1, float y1)
    {
        bool ret = false;
        if (currentState != UNVISIBLE
            && x1 >= x && x1 <= x + width
            && y1 >= y && y1 <= y + height)
            ret = true;
        return ret;
    }

    void setVisibility(bool visible)
    {
        setState(visible ? REST : UNVISIBLE);
    }

    void setOnCLickCallback(std::function<void(PlayerController&)> f)
    {
        onClick = f;
    }

    void setOnLockCallback(std::function<void(PlayerController&)> f)
    {
        onLock = f;
    }

    void setOnUnlockCallback(std::function<void(PlayerController&)> f)
    {
        onUnlock = f;
    }

    void setController(Controller *c)
    {
        controller = c;
    }

    virtual void clickEvent()
    {
        onClick(*controller);
    }

    virtual void pointerMoveEvent(float pointer_x, float pointer_y)
    {
        pointerMove(*controller, pointer_x, pointer_y);
    }

    void setPosition(float new_x, float new_y)
    {
        x = new_x;
        y = new_y;
    }

    void setSize(float w, float h)
    {
        width = w;
        height = h;
    }


protected:
    float x, y;
    float width, height;

    Controller *controller;

    std::function<void(Controller&)> onClick;

    std::function<void(Controller&)> onLock;

    std::function<void(Controller&)> onUnlock;

    std::function<void(Controller&, float, float)> pointerMove;

private:
    state currentState;
};

#endif // USERCONTROL_H
