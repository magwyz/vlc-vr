#ifndef SLIDER_H
#define SLIDER_H

#include <GL/gl.h>

#include <usercontrol.h>


template <class Controller> class Slider : public UserControl<Controller>
{
public:
    Slider(float x, float y,
           float width, float height,
           std::string texturePath,
           GLuint shaderProgram)
        : UserControl<Controller>(x, y, width, height, shaderProgram),
          value(0)
    {
        SDL_Surface *surface;

        if ((surface = IMG_Load(texturePath.c_str())))
        {
            glGenTextures(1, texture);
            glBindTexture(GL_TEXTURE_2D, texture[0]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                         surface->w, surface->h, 0, GL_RGBA,
                         GL_UNSIGNED_BYTE, surface->pixels);

            glEnable(GL_TEXTURE_2D);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_PRIORITY, 1.0);
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }

        if (surface)
            SDL_FreeSurface(surface);
    }

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

        glColor4f(255.f / 255, 136.f / 255, 0.f / 255, 1.f);

        glBegin(GL_QUADS);
        glVertex3f( 0.f, 0.f, 0.f);
        glVertex3f( 0.f, LINE_HEIGHT, 0.f);
        glVertex3f( value * this->width, LINE_HEIGHT, 0.f);
        glVertex3f( value * this->width, 0.f, 0.f);
        glEnd();

        glColor4f(117.f / 255, 117.f / 255, 117.f / 255, 1.f);

        glBegin(GL_QUADS);
        glVertex3f( value * this->width, 0.f, 0.f);
        glVertex3f( value * this->width, LINE_HEIGHT, 0.f);
        glVertex3f( this->width, LINE_HEIGHT, 0.f);
        glVertex3f( this->width, 0.f, 0.f);
        glEnd();

        glPopMatrix();

        /* Draw the picker. */
        #define PICKER_SIZE 0.013

        glPushMatrix();
        glTranslatef(value * this->width - PICKER_SIZE / 2,
                     this->height / 2 - PICKER_SIZE / 2, 0.0005);      

        // Alpha null to tell the shader to apply the texture.
        glColor4f(0.f, 0.f, 0.f, 0.f);
        glBindTexture(GL_TEXTURE_2D, texture[0]);

        GLfloat vertexCoord[] = {
            0.f, 0.f, 0.f,
            0.f, PICKER_SIZE, 0.f,
            PICKER_SIZE, PICKER_SIZE, 0.f,
            PICKER_SIZE, 0.f, 0.f
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
    GLuint texture[1];

    std::function<void(PlayerController&, float)> onUserChangedValue;
};

#endif // SLIDER_H
