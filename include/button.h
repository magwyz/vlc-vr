#ifndef BUTTON_H
#define BUTTON_H

#include <GL/gl.h>

#include <usercontrol.h>


class Button : public UserControl
{
public:
    Button(float x, float y,
           float width, float height,
           std::string texturePath);
    void draw();

private:
    GLuint texture[1];
};

#endif // BUTTON_H
