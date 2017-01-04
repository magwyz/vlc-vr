#ifndef SCREEN_H
#define SCREEN_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include <usercontrol.h>
#include <player.h>


template <class Controller> class Screen : public UserControl<Controller>
{
public:
    Screen(float x, float y,
           float width, float height, GLuint shaderProgram)
        : UserControl<Controller>(x, y, width, height, shaderProgram)
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

        /*glBegin(GL_QUADS);

        glTexCoord2f(0.0, 1.0);
        glVertex3f( 0.f, 0.f, 0.f);

        glTexCoord2f(0.0, 0.0);
        glVertex3f( 0.f, this->height, 0.f);

        glTexCoord2f(1.0, 0.0);
        glVertex3f( this->width, this->height, 0.f);

        glTexCoord2f(1.0, 1.0);
        glVertex3f( this->width, 0.f, 0.f);

        glEnd();*/

        GLfloat *vertexCoord;
        GLfloat *textureCoord;
        unsigned nbVertices;
        GLushort *indices;
        unsigned nbIndices;

        BuildSphere(1, &vertexCoord, &textureCoord,
                    &nbVertices, &indices, &nbIndices);

        this->drawMesh(vertexCoord, textureCoord, nbVertices, indices,
                       nbIndices, GL_TRIANGLES);

        free(vertexCoord);
        free(textureCoord);

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

    void BuildSphere(unsigned nbPlanes,
                     GLfloat **vertexCoord, GLfloat **textureCoord, unsigned *nbVertices,
                     GLushort **indices, unsigned *nbIndices)
    {
        #define SPHERE_RADIUS 1.f
        unsigned nbLatBands = 128;
        unsigned nbLonBands = 128;

        *nbVertices = (nbLatBands + 1) * (nbLonBands + 1);
        *nbIndices = nbLatBands * nbLonBands * 3 * 2;

        *vertexCoord = (GLfloat *)malloc(*nbVertices * 3 * sizeof(GLfloat));
        if (*vertexCoord == NULL)
            return;
        *textureCoord = (GLfloat *)malloc(nbPlanes * *nbVertices * 2 * sizeof(GLfloat));
        if (*textureCoord == NULL)
        {
            free(*vertexCoord);
            return;
        }
        *indices = (GLushort *)malloc(*nbIndices * sizeof(GLushort));
        if (*indices == NULL)
        {
            free(*textureCoord);
            free(*vertexCoord);
            return;
        }

        for (unsigned lat = 0; lat <= nbLatBands; lat++) {
            float theta = lat * (float) M_PI / nbLatBands;
            float sinTheta, cosTheta;

            sincosf(theta, &sinTheta, &cosTheta);

            for (unsigned lon = 0; lon <= nbLonBands; lon++) {
                float phi = lon * 2 * (float) M_PI / nbLonBands;
                float sinPhi, cosPhi;

                sincosf(phi, &sinPhi, &cosPhi);

                float x = cosPhi * sinTheta;
                float y = cosTheta;
                float z = sinPhi * sinTheta;

                unsigned off1 = (lat * (nbLonBands + 1) + lon) * 3;
                (*vertexCoord)[off1] = SPHERE_RADIUS * x;
                (*vertexCoord)[off1 + 1] = SPHERE_RADIUS * y;
                (*vertexCoord)[off1 + 2] = SPHERE_RADIUS * z;

                for (unsigned p = 0; p < nbPlanes; ++p)
                {
                    unsigned off2 = (p * (nbLatBands + 1) * (nbLonBands + 1)
                                    + lat * (nbLonBands + 1) + lon) * 2;
                    float width = 1.f;
                    float height = 1.f;
                    float u = (float)lon / nbLonBands * width;
                    float v = (float)lat / nbLatBands * height;
                    (*textureCoord)[off2] = u;
                    (*textureCoord)[off2 + 1] = v;
                }
            }
        }

        for (unsigned lat = 0; lat < nbLatBands; lat++) {
            for (unsigned lon = 0; lon < nbLonBands; lon++) {
                unsigned first = (lat * (nbLonBands + 1)) + lon;
                unsigned second = first + nbLonBands + 1;

                unsigned off = (lat * nbLatBands + lon) * 3 * 2;

                (*indices)[off] = first;
                (*indices)[off + 1] = second;
                (*indices)[off + 2] = first + 1;

                (*indices)[off + 3] = second;
                (*indices)[off + 4] = second + 1;
                (*indices)[off + 5] = first + 1;
            }
        }
    }

private:
    GLuint texture[1];
};


#endif // SCREEN_H
