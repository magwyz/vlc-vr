#include <GL/gl.h>

#include <userinterface.h>
#include <quaternion.h>

UserInterface::UserInterface(float x, float y, float z,
                             float width, float height)
    : x(x), y(y), z(z),
      width(width), height(height),
      focussedControl(NULL)
{ }


void UserInterface::addControl(UserControl *control)
{
    controls.push_back(control);
}


void UserInterface::draw()
{
    /* Move to the user interface space. */
    glPushMatrix();
    glTranslatef(x, y, z);

    /* Draw the background */
    glColor4f(1.f, 0.f, 0.f, 1.f);

    glBegin(GL_QUADS);
    glVertex3f( 0.f, 0.f, 0.f);
    glVertex3f( 0.f, height, 0.f);
    glVertex3f( width, height, 0.f);
    glVertex3f( width, 0.f, 0.f);
    glEnd();

    glPushMatrix();
    glTranslatef(0.f, 0.f, 0.01f);

    /* Draw the controls */
    for (std::vector<UserControl *>::iterator it = controls.begin();
         it != controls.end(); ++it)
        (*it)->draw();

    glPopMatrix();
    glPopMatrix();
}


void UserInterface::pointerFocus(ohmd_device* hmd)
{
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
            UserControl *c = controls[i];
            if (c->intersect(intf_x, intf_y))
            {
                focussedControl = c;
                focussedControl->setState(FOCUSSED);
            }
        }
    }
}


void UserInterface::drawPointer(ohmd_device* hmd)
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

    glPointSize(20.f);
    glColor4f(0.f, 0.f, 1.f, 1.f);

    glBegin(GL_POINTS);
    glVertex3f(intersec[0], intersec[1], intersec[2]);
    glEnd();
}


void UserInterface::clickEvent()
{
    if (focussedControl != NULL)
    {
        focussedControl->clickEvent();
    }
}
