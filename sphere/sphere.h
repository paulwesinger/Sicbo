#ifndef CSPHERE_H
#define CSPHERE_H

/*
 * File:   CSphere.h
 * Author: paul
 *
 * Created on 27. Oktober 2019, 20:03
 */

#include <cstdio>

#include <SDL2/SDL_image.h>

#include <glm/glm.hpp>
//#include <glm/vec2.hpp>
//#include <glm/vec3.hpp>
//#include <glm/vec4.hpp>

#include "../shaders/shader.h"
#include "../baseobject/baseobject.h"
#include "../camera/camera.h"


typedef struct {
    glm::vec3 vertex;
    glm::vec2 tex;
} SpherePointStruct;

class CSphere  : public BaseObject {
public:
    CSphere();
    CSphere(int points);
    CSphere(int points, GLfloat rad);

    CSphere(const CSphere& orig);

    CSphere(glm::vec3 origin, glm::mat4 pro);
    CSphere(glm::vec3 origin, glm::mat4 pro, int points);
    CSphere(glm::vec3 origin, glm::mat4 pro, int points, GLfloat rad,Shader * shad);

    CSphere(vec3 origin , vec4 color, glm::mat4 pro);
    CSphere(vec3 origin , vec4 color, glm::mat4 pro, int points);
    CSphere(vec3 origin , vec4 color, glm::mat4 pro, int points, GLfloat rad, Shader * shad);
    ~CSphere();
    virtual void Draw(Camera * cam );//, GLuint &shaderprog);
    virtual void SetColor(vec4 color);
    void SetRadian(float rad);
    void SetPoints( int countPoints);  // dieAuflösung
protected:
    // einen Vektor für den GPU-Mem vorbereiten
    void Add2GPU(float*v, int &index, GLfloat vx, GLfloat vy, GLfloat vz);
    void Add2GPU(float*v, int &index,glm::vec3 vec);

private:
    // Buffers for Opengl

    GLuint  _Vao, _VertexBuffer,_Ebo_npol;
    GLint   mv_location;
    GLint   color_location;
    GLuint shaderprogram;
    int countVertex;
   // GLushort * npol_indices;

    Shader * shader;
    GLfloat _Radius;
    int _CountPoints;
    void setUp();
    void calc( GLfloat * v);
};


#endif // CSPHERE_H
