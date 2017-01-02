#ifndef SLIDER_H
#define SLIDER_H

#include <GL/gl.h>

#include <usercontrol.h>


template <class Controller> class Slider : public UserControl<Controller>
{
public:
    Slider(float x, float y,
           float width, float height)
        : UserControl<Controller>(x, y, width, height),
          value(0)
    { }

    void draw()
    {
        if (this->getState() == UNVISIBLE)
            return;

        UserControl<Controller>::draw();

        /* Move to the control space. */
        glPushMatrix();
        glTranslatef(this->x, this->y, 0.f);

        /* Draw the control */

        /* Draw the line. */
        #define LINE_HEIGHT 0.005

        glPushMatrix();
        glTranslatef(0.f, this->height / 2 - LINE_HEIGHT / 2, 0.f);
        glColor4f(0.f, 1.f, 0.f, 1.f);

        glBegin(GL_QUADS);
        glVertex3f( 0.f, 0.f, 0.f);
        glVertex3f( 0.f, LINE_HEIGHT, 0.f);
        glVertex3f( this->width, LINE_HEIGHT, 0.f);
        glVertex3f( this->width, 0.f, 0.f);
        glEnd();

        glPopMatrix();

        /* Draw the picker. */
        #define PICKER_SIZE 0.01

        glPushMatrix();
        glTranslatef(value * this->width - PICKER_SIZE / 2,
                     this->height / 2 - PICKER_SIZE / 2, 0.0005);
        glColor4f(0.f, 1.f, 1.f, 1.f);

        glBegin(GL_QUADS);
        glVertex3f(0.f, 0.f, 0.f);
        glVertex3f(0.f, PICKER_SIZE, 0.f);
        glVertex3f(PICKER_SIZE, PICKER_SIZE, 0.f);
        glVertex3f(PICKER_SIZE, 0.f, 0.f);
        glEnd();

        glPopMatrix();
        glPopMatrix();
    }

    void setValue(float v)
    {
        value = v;
    }

    void clickEvent()
    {
        if (this->getState() == LOCKED)
            this->setState(REST);
        else
            this->setState(LOCKED);
    }

    void pointerMoveEvent(float pointer_x, float pointer_y)
    {
        if (this->getState() == LOCKED)
        {
            float v = (pointer_x - this->x) / this->width;
            v = std::min(v, 1.f);
            v = std::max(v, 0.f);

            setValue(v);

            if (onUserChangedValue != NULL)
                onUserChangedValue(*this->controller, v);
        }
    }

    void setOnUserChangedValueCallback(std::function<void(PlayerController&, float)> f)
    {
        onUserChangedValue = f;
    }

private:
    float value; // in [0; 1]

    std::function<void(PlayerController&, float)> onUserChangedValue;
};

#endif // SLIDER_H
