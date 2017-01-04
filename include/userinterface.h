#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <vector>

#include <openhmd.h>

#include <GL/glew.h>
#include <GL/gl.h>

#include <button.h>
#include <slider.h>
#include <label.h>
#include <screen.h>
#include <quaternion.h>


typedef enum
{
    LEFT_MOVE,
    RIGHT_MOVE,
    UP_MOVE,
    DOWN_MOVE
} move;


template <class Controller> class UserInterface
{
public:
    UserInterface(float x, float y, float z,
                  float width, float height)
        : x(x), y(y), z(z),
          width(width), height(height),
          focussedControl(NULL),
          visible(true), drawContainer(true)
    { }

    void draw()
    {
        if (!visible)
            return;

        /* Move to the user interface space. */
        glPushMatrix();
        glTranslatef(x, y, z);

        /* Draw the background */
        if (drawContainer)
        {
            glColor4f(50.f / 255, 50.f / 255, 50.f / 255, 1.f);

            glBegin(GL_QUADS);
            glVertex3f( 0.f, 0.f, 0.f);
            glVertex3f( 0.f, height, 0.f);
            glVertex3f( width, height, 0.f);
            glVertex3f( width, 0.f, 0.f);
            glEnd();
        }

        glPushMatrix();
        glTranslatef(0.f, 0.f, 0.01f);

        /* Draw the controls */
        for (auto it = controls.begin();
             it != controls.end(); ++it)
            (*it)->draw();

        glPopMatrix();
        glPopMatrix();
    }

    void addControl(UserControl<Controller> *control)
    {
        controls.push_back(control);
    }

    void pointerFocus(ohmd_device* hmd)
    {
        if (!visible)
            return;

        float ori[4]; // Orientation quaternion.
        ohmd_device_getf(hmd, OHMD_ROTATION_QUAT, ori);

        Quaternion q(ori);

        // View direction.
        Quaternion vd = q * Quaternion(0.f, 0.f, -1.f, 1.f) * q.conjugate();

        // The interface must be perpandicular to the z axis.
        float d = z / vd.z;

        float intersec[3];
        intersec[0] = d * vd.x;
        intersec[1] = d * vd.y;
        intersec[2] = d * vd.z;

        float intf_x = intersec[0] - x;
        float intf_y = intersec[1] - y;


        if (focussedControl != NULL)
        {
            if (focussedControl->getState() == LOCKED)
            {
                focussedControl->pointerMoveEvent(intf_x, intf_y);
            }
            else
            {
                if (focussedControl->getState() != UNVISIBLE)
                    focussedControl->setState(REST);
                focussedControl = NULL;
            }
        }

        if (focussedControl == NULL
            || focussedControl->getState() != LOCKED)
        {
            for (unsigned i = 0; i < controls.size(); ++i)
            {
                UserControl<Controller> *c = controls[i];
                if (c->intersect(intf_x, intf_y))
                {
                    focussedControl = c;
                    focussedControl->setState(FOCUSSED);
                }
            }
        }
    }

    void drawPointer(ohmd_device* hmd)
    {
        float ori[4]; // Orientation quaternion.
        ohmd_device_getf(hmd, OHMD_ROTATION_QUAT, ori);

        Quaternion q(ori);

        // View direction.
        Quaternion vd = q * Quaternion(0.f, 0.f, -1.f, 1.f) * q.conjugate();

        // The interface must be perpandicular to the z axis.
        float d = -0.3 / vd.z;

        float intersec[3];
        intersec[0] = d * vd.x;
        intersec[1] = d * vd.y;
        intersec[2] = d * vd.z;

        glPointSize(10.f);
        glColor4f(255.f / 255, 136.f / 255, 0.f / 255, 1.f);

        glBegin(GL_POINTS);
        glVertex3f(intersec[0], intersec[1], intersec[2]);
        glEnd();
    }

    bool clickEvent()
    {
        bool ret = false;
        if (focussedControl != NULL)
        {
            focussedControl->clickEvent();
            ret = true;
        }
        return ret;
    }

    void setController(Controller *c)
    {
        for (auto it = controls.begin(); it != controls.end(); ++it)
            (*it)->setController(c);
    }

    void getPosition(float &x_ret, float &y_ret, float &z_ret)
    {
        x_ret = x;
        y_ret = y;
        z_ret = z;
    }

    void setPosition(float new_x, float new_y, float new_z)
    {
        x = new_x;
        y = new_y;
        z = new_z;
    }

    void setSize(float w, float h)
    {
        width = w;
        height = h;
    }

    bool getVisibility()
    {
        return visible;
    }

    void setVisibility(bool v)
    {
        visible = v;
    }

    void setDrawContainer(bool d)
    {
        drawContainer = d;
    }

private:
    std::vector<UserControl<Controller> *> controls;
    UserControl<Controller> *focussedControl;

    float x, y, z;
    float width, height;
    bool visible;
    bool drawContainer;
};

#endif // USERINTERFACE_H
