#ifndef LABEL_H
#define LABEL_H

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <GL/gl.h>

#include <usercontrol.h>


template <class Controller> class Label : public UserControl<Controller>
{
public:
    Label(float x, float y, int fontSize,
          std::string text, GLuint shaderProgram)
        : UserControl<Controller>(x, y, 0, 0, shaderProgram),
          text(text),
          police(NULL), textSurface(NULL),
          mustRenderText(true)
    {
        police = TTF_OpenFont("../timeless.ttf", fontSize);

        glGenTextures(1, texture);
        glBindTexture(GL_TEXTURE_2D, texture[0]);

        glEnable(GL_TEXTURE_2D);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_PRIORITY, 1.0);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    ~Label()
    {
        SDL_FreeSurface(textSurface);
        TTF_CloseFont(police);
    }

    void draw()
    {
        if (this->getState() == UNVISIBLE)
            return;

        UserControl<Controller>::draw();

        if (mustRenderText)
        {
            renderText();
            mustRenderText = false;
        }

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

    void renderText()
    {
        SDL_Color whiteColor = {255, 255, 255};
        textSurface = TTF_RenderUTF8_Blended(police, text.c_str(), whiteColor);

        if (textSurface != NULL)
        {
            glBindTexture(GL_TEXTURE_2D, texture[0]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                         textSurface->w, textSurface->h, 0, GL_RGBA,
                         GL_UNSIGNED_BYTE, textSurface->pixels);

            #define PIXEL_WORD_RATIO 0.001f

            this->width = textSurface->w * PIXEL_WORD_RATIO;
            this->height = textSurface->h * PIXEL_WORD_RATIO;

            SDL_FreeSurface(textSurface);
        }
    }

    void setText(std::string t)
    {
        text = t;
        mustRenderText = true;
    }


private:
    GLuint texture[1];
    std::string text;
    bool mustRenderText;

    TTF_Font *police;
    SDL_Surface *textSurface;
};

#endif // LABEL_H
