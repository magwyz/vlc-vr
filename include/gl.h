/*
 * OpenHMD - Free and Open Source API and drivers for immersive technology.
 * Copyright (C) 2013 Fredrik Hultin.
 * Copyright (C) 2013 Jakob Bornecrantz.
 * Distributed under the Boost 1.0 licence, see LICENSE for full text.
 */

/* OpenGL Test - Interface For GL Helper Functions */

#ifndef GL_H
#define GL_H

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include <GL/glew.h>
#include <GL/gl.h>

#include "userinterface.h"

class PlayerController;

#define TEST_WIDTH 2160
#define TEST_HEIGHT 1200

#define EYE_WIDTH (TEST_WIDTH / 2)
#define EYE_HEIGHT (TEST_HEIGHT)

typedef struct {
	int w, h;
	SDL_Surface* screen;
} gl_ctx;


typedef enum {
    LEFT_EYE,
    RIGHT_EYE,
} eye;

struct Player;

void init_gl(gl_ctx* ctx, int w, int h);
GLuint compile_shader(const char* vertex, const char* fragment);
void create_fbo(int eye_width, int eye_height, GLuint* fbo, GLuint* color_tex, GLuint* depth_tex);
void drawEye(ohmd_device *hmd, eye curEye, GLuint fbo,
             UserInterface<PlayerController> *intf,
             UserInterface<PlayerController> *intfScreen);
void drawMesh(GLuint program, GLfloat *vertexCoord, GLfloat *textureCoord, unsigned nbVertices,
              GLushort *indices, unsigned nbIndices, GLenum mode);


#endif
