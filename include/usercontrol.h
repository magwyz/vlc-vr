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
                float width, float height,
                GLuint program)
          : x(x), y(y),
            width(width), height(height),
            program(program),
            currentState(REST),
            drawFocusIndicator(true) {}

    virtual void draw()
    {
       /* Move to the control space. */
       glPushMatrix();
       glTranslatef(x, y, 0.f);

       /* Draw lines if the control has the focus. */
       if (drawFocusIndicator
           && (currentState == FOCUSSED
               || currentState == LOCKED))
       {
           glPushMatrix();
           glTranslatef(0.f, 0.f, 0.001f);

           glColor4f(255.f / 255, 136.f / 255, 0.f / 255, 1.f);

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

    void setDrawFocusIndicator(bool d)
    {
        drawFocusIndicator = d;
    }


protected:
    float x, y;
    float width, height;

    bool drawFocusIndicator;

    Controller *controller;

    std::function<void(Controller&)> onClick;

    std::function<void(Controller&)> onLock;

    std::function<void(Controller&)> onUnlock;

    std::function<void(Controller&, float, float)> pointerMove;

    void drawBoundingRectangle()
    {
        GLfloat vertexCoord[] = {
            0.f, 0.f, 0.f,
            0.f, this->height, 0.f,
            this->width, this->height, 0.f,
            this->width, 0.f, 0.f
        };

        GLfloat textureCoord[] = {
            0.0, 1.0,
            0.0, 0.0,
            1.0, 0.0,
            1.0, 1.0,
        };

        GLushort indices[] = {
            0, 1, 2, 3
        };

        this->drawMesh(vertexCoord, textureCoord,
                       sizeof(vertexCoord) / sizeof(GLfloat) / 3,
                       indices, sizeof(indices) / sizeof(GLushort),
                       GL_QUADS);
    }

    void drawMesh(GLfloat *vertexCoord, GLfloat *textureCoord, unsigned nbVertices,
                  GLushort *indices, unsigned nbIndices, GLenum mode)
    {
        GLuint vertexbuffer;
        glGenBuffers(1, &vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, nbVertices * 3 * sizeof(GLfloat), vertexCoord, GL_STATIC_DRAW);

        glEnableVertexAttribArray(glGetAttribLocation(program, "vPosition"));
        glVertexAttribPointer(glGetAttribLocation(program, "vPosition"), 3, GL_FLOAT, 0, 0, 0);

        GLuint texturebuffer;
        glGenBuffers(1, &texturebuffer);
        glBindBuffer(GL_ARRAY_BUFFER, texturebuffer);
        glBufferData(GL_ARRAY_BUFFER, nbVertices * 2 * sizeof(GLfloat), textureCoord, GL_STATIC_DRAW);

        glEnableVertexAttribArray(glGetAttribLocation(program, "MultiTexCoord0"));
        glVertexAttribPointer(glGetAttribLocation(program, "MultiTexCoord0"), 2, GL_FLOAT, 0, 0, 0);

        GLuint elementbuffer;
        glGenBuffers(1, &elementbuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, nbIndices * sizeof(GLushort), indices, GL_STATIC_DRAW);

        glDrawElements(mode, nbIndices, GL_UNSIGNED_SHORT, 0);

        glDeleteBuffers(1, &vertexbuffer);
        glDeleteBuffers(1, &texturebuffer);
        glDeleteBuffers(1, &elementbuffer);
    }

        GLuint program;

private:
    state currentState;
};

#endif // USERCONTROL_H
