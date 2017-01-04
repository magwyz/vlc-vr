#ifndef BUTTON_H
#define BUTTON_H

#include <GL/gl.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include <usercontrol.h>


template <class Controller> class Button : public UserControl<Controller>
{
public:
    Button(float x, float y,
           float width, float height,
           std::string texturePath,
           GLuint shaderProgram)
        : UserControl<Controller>(x, y, width, height, shaderProgram)
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

        // Alpha null to tell the shader to apply the texture.
        glColor4f(0.f, 0.f, 0.f, 0.f);
        glBindTexture(GL_TEXTURE_2D, texture[0]);

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

        glPopMatrix();
    }

private:
    GLuint texture[1];
};

#endif // BUTTON_H
