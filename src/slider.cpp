#include "slider.h"


Slider::Slider(float x, float y,
               float width, float height)
    : UserControl(x, y, width, height),
      value(0)
{ }



void Slider::draw()
{
    if (getState() == UNVISIBLE)
        return;

    UserControl::draw();

    /* Move to the control space. */
    glPushMatrix();
    glTranslatef(x, y, 0.f);

    /* Draw the control */

    /* Draw the line. */
    #define LINE_HEIGHT 0.005

    glPushMatrix();
    glTranslatef(0.f, height / 2 - LINE_HEIGHT / 2, 0.f);
    glColor4f(0.f, 1.f, 0.f, 1.f);

    glBegin(GL_QUADS);
    glVertex3f( 0.f, 0.f, 0.f);
    glVertex3f( 0.f, LINE_HEIGHT, 0.f);
    glVertex3f( width, LINE_HEIGHT, 0.f);
    glVertex3f( width, 0.f, 0.f);
    glEnd();

    glPopMatrix();

    /* Draw the picker. */
    #define PICKER_SIZE 0.01

    glPushMatrix();
    glTranslatef(value * width - PICKER_SIZE / 2,
                 height / 2 - PICKER_SIZE / 2, 0.0005);
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


void Slider::setValue(float v)
{
    value = v;
}


void Slider::clickEvent()
{
    if (getState() == LOCKED)
        setState(REST);
    else
        setState(LOCKED);
}


void Slider::pointerMoveEvent(float pointer_x, float pointer_y)
{
    if (getState() == LOCKED)
    {
        float v = (pointer_x - x) / width;
        v = std::min(v, 1.f);
        v = std::max(v, 0.f);

        setValue(v);

        if (onUserChangedValue != NULL)
            onUserChangedValue(*onUserChangedValueController, v);
    }
}


void Slider::setOnUserChangedValueCallback(PlayerController *c, std::function<void(PlayerController&, float)> f)
{
    onUserChangedValueController = c;
    onUserChangedValue = f;
}
