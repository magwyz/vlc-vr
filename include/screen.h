#ifndef SCREEN_H
#define SCREEN_H

#include <GL/gl.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include <usercontrol.h>
#include <player.h>


template <class Controller> class Screen : public UserControl<Controller>
{
public:
    Screen(float x, float y,
           float width, float height)
        : UserControl<Controller>(x, y, width, height)
    {
        /* Create The Texture */
        glGenTextures(1, texture);

        /* Typical Texture Generation Using Data From The Bitmap */
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        printf("texture[0]: %d\n", texture[0]);

        /* Linear Filtering */
        glEnable(GL_TEXTURE_2D);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_PRIORITY, 1.0);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
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

        glBindTexture(GL_TEXTURE_2D, texture[0]);
        // Alpha null to tell the shader to apply the texture.
        glColor4f(0.f, 0.f, 0.f, 0.f);

        glBegin(GL_QUADS);

        glTexCoord2f(0.0, 1.0);
        glVertex3f( 0.f, 0.f, 0.f);

        glTexCoord2f(0.0, 0.0);
        glVertex3f( 0.f, this->height, 0.f);

        glTexCoord2f(1.0, 0.0);
        glVertex3f( this->width, this->height, 0.f);

        glTexCoord2f(1.0, 1.0);
        glVertex3f( this->width, 0.f, 0.f);

        glEnd();

        glPopMatrix();
    }

    void updateTexture(Player *p)
    {
        std::lock_guard<std::mutex> lock(p->mutex);

        /* Generate The Texture */
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, p->i_width,
                     p->i_height, 0, GL_BGR,
                     GL_UNSIGNED_BYTE, p->p_imgData);
    }

private:
    GLuint texture[1];
};


#endif // SCREEN_H
