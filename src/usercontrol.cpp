#include <GL/gl.h>

#include <usercontrol.h>


UserControl::UserControl(float x, float y,
            float width, float height)
    : x(x), y(y),
      width(width), height(height),
      currentState(REST),
      onClickController(NULL),
      onLockController(NULL),
      onUnlockController(NULL),
      pointerMoveController(NULL) {}


void UserControl::draw() {
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


void UserControl::setState(state s)
{
    if (currentState == LOCKED && s != LOCKED
        && onUnlockController != NULL)
        onUnlock(*onUnlockController);
    if (currentState != LOCKED && s == LOCKED
        && onLockController != NULL)
        onLock(*onLockController);

    currentState = s;
}


state UserControl::getState()
{
    return currentState;
}


Position UserControl::getCenter()
{
    return Position((2 * x + width) / 2,
                    (2 * y + height) / 2);
}


float UserControl::getDistance(UserControl *c)
{
    Position c1 = getCenter();
    Position c2 = c->getCenter();
    float xDiff = c1.x - c2.x;
    float yDiff = c1.y - c2.y;
    return sqrtf(xDiff * xDiff + yDiff * yDiff);
}


bool UserControl::intersect(float x1, float y1)
{
    bool ret = false;
    if (currentState != UNVISIBLE
        && x1 >= x && x1 <= x + width
        && y1 >= y && y1 <= y + height)
        ret = true;
    return ret;
}


void UserControl::setVisibility(bool visible)
{
    setState(visible ? REST : UNVISIBLE);
}


void UserControl::setOnCLickCallback(PlayerController *c, std::function<void(PlayerController&)> f)
{
    onClick = f;
    onClickController = c;
}


void UserControl::clickEvent()
{
    if (onClickController != NULL)
        onClick(*onClickController);
}


void UserControl::pointerMoveEvent(float pointer_x, float pointer_y)
{
    if (pointerMoveController != NULL)
        pointerMove(*pointerMoveController, pointer_x, pointer_y);
}


void UserControl::setOnLockCallback(PlayerController *c, std::function<void(PlayerController&)> f)
{
    onLockController = c;
    onLock = f;
}


void UserControl::setOnUnlockCallback(PlayerController *c, std::function<void(PlayerController&)> f)
{
    onUnlockController = c;
    onUnlock = f;
}
