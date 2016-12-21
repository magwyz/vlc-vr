#include <SDL/SDL.h>
#include <SDL/SDL_image.h>


#include "button.h"

Button::Button(float x, float y,
               float width, float height,
               std::string texturePath)
    : UserControl(x, y, width, height)
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


void Button::draw()
{
    if (getState() == UNVISIBLE)
        return;

    UserControl::draw();

    /* Move to the control space. */
    glPushMatrix();
    glTranslatef(x, y, 0.f);

    /* Draw the control */

    // Alpha null to tell the shader to apply the texture.
    glColor4f(0.f, 0.f, 0.f, 0.f);
    glBindTexture(GL_TEXTURE_2D, texture[0]);

    glBegin(GL_QUADS);

    glTexCoord2f(0.0, 1.0);
    glVertex3f( 0.f, 0.f, 0.f);

    glTexCoord2f(0.0, 0.0);
    glVertex3f( 0.f, height, 0.f);

    glTexCoord2f(1.0, 0.0);
    glVertex3f( width, height, 0.f);

    glTexCoord2f(1.0, 1.0);
    glVertex3f( width, 0.f, 0.f);

    glEnd();

    glPopMatrix();
}
