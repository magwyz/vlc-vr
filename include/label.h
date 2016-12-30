#ifndef LABEL_H
#define LABEL_H

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <GL/gl.h>

#include <usercontrol.h>


class Label : public UserControl
{
public:
    Label(float x, float y, int fontSize, std::string text);
    ~Label();
    void draw();
    void renderText();
    void setText(std::string t);

private:
    GLuint texture[1];
    std::string text;
    bool mustRenderText;

    TTF_Font *police;
    SDL_Surface *textSurface;
};

#endif // LABEL_H
