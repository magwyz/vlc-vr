#include <label.h>


Label::Label(float x, float y, int fontSize, std::string text)
    : UserControl(x, y, 0, 0),
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


Label::~Label()
{
    SDL_FreeSurface(textSurface);
    TTF_CloseFont(police);
}


void Label::setText(std::string t)
{
    text = t;
    mustRenderText = true;
}


void Label::renderText()
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

        width = textSurface->w * PIXEL_WORD_RATIO;
        height = textSurface->h * PIXEL_WORD_RATIO;

        SDL_FreeSurface(textSurface);
    }
}


void Label::draw()
{
    if (getState() == UNVISIBLE)
        return;

    UserControl::draw();

    if (mustRenderText)
    {
        renderText();
        mustRenderText = false;
    }

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
