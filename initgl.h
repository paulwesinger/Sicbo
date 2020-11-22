#ifndef INITGL_H
#define INITGL_H




#include <cstdio>
#include <cstdlib>
#include <string>


#include <GL/glew.h>
//#include <glad.h>



//#include <glm/mat4x4.hpp>
//#include "glm/gtc/matrix_transform.hpp"

#include <SDL2/SDL.h>
#include <irrklang/irrKlang.h>

#include "logs/logs.h"
#include "utils/utils.h"

#include "shaders/shader.h"
#include "cube/cube.h"
#include "sphere/sphere.h"
#include "skybox/skybox.h"
#include "base2d/base2d.h"
#include "camera/camera.h"
#include "defines.h"
#include "fileutils/fileutil.h"
#include "objectclasses/object.h"
#include "projection/projection.h"
#include "textrenderer/textrender.h"
#include "baseobject/baseobject.h"

#include "meshobject/meshobject.h"
#include "objloader/objloader.h"


#include "buttons/button.h"
#include "lights/light.h"


typedef struct {
    GLfloat x;
    GLfloat y;
    GLfloat z;
    GLfloat w;
    // use for colors too..
}GLFLOAT4;



class InitGL {
public:
    InitGL( std::string titel = "OpenGL");
    InitGL(const InitGL& orig);
    // Create Context and Init SDL2
    bool InitSDL2();

    virtual void Run();
    virtual void Done();
    // Methode zum abbrechen
    void sdl_die( std::string msg);
    virtual ~InitGL();

    virtual void Prepare2D(); // Flags für Rendern einstellen
    virtual void Restore3D(); // Flags zurücksetzen

protected:
    // ---------------------------------
    // Wieder löschen
    static void TestFunction();
    // ---------------------------------

    //----------------------------------
    // Look and feel
    //----------------------------------
    void SetClearColor(float r, float g, float b, float a);
    void setClearColor(float r, float g, float b);


    // HandleInput wird wahrscheinlich überflussig....
    virtual int HandleInput(SDL_Event e , uint &mox, uint &moy);

    // .... ersetzt durch HandleEvent und die virtuellen Handler dazu
    virtual uint HandleEvent(SDL_Event e);
    // HandlerFuncs for Mouse
    virtual void OnMouseMove(int &x, int &y, uint buttonstate);
    virtual void OnLeftMouseButtonClick(int &x, int &y);

    //----------------------------------
    // Object creation
    // ---------------------------------
    void add3Dobject(CCube * obj);
    void add2Dobject(Base2D * obj);
    void addButton(CButton* obj);


    void safeDelete(BaseObject * bo);

    CCube * cube;
    CCube * cube2;
    CCube * cube3;

    //CMeshObject *me;
    //COBJLoader * objLoader;

    CSphere *sphere1;
    CSphere *lightSource;   // Damit wir wissen wo die ichtquekke wirjkich  ist

    Base2D * base2d;
    Base2D * cockpit;
    Camera * camera;  // = View !!
    SkyBox * skybox;

    fileUtil * filestream = nullptr;
    Projection * projection;

    TextRender * textrender;

    light * ambientLight = nullptr;


    // Liste die alle 3D objekte enthält
    std::vector<CCube *> objects3D;
    // Liste für die 2D Objekte
    std::vector<Base2D *> objects2D;
    // Eine Button liste
    std::vector<CButton*>  buttons;


    SDL_Event e;
    // Members for navigating with the  mouse...
    MOUSE _Mouse;

    irrklang::ISoundEngine * soundengine;
    // Test Shader
    Shader * shader;
    GLuint cubeshaderprog_color;
    GLuint cubeshaderprog_tex;
    GLuint cubeshaderprog_normals;

    GLuint sphereshader_color;
    GLuint currentShader;
    bool _FullScreen ;
    // Global Resolution vars
    int _ResX;
    int _ResY;
    std::string caption;

    GLFLOAT4 _ClearColor;


private:

    void InitEngineObject();
    void InitUtils();  // Fileutils,logs ,etc....
    void InitShaders();
    void InitMatrices();
    void DeleteUtils();
    void DeleteMatrices();
    void DeleteShaders();

    SDL_Window * window ;
    SDL_GLContext maincontext;

    double angleX,angleY ; //  Testwinkel für camerabewegung
    double moveZ;          // Kamera entlag der Z achse verschieben
};

// -------------------------------------
// Abgeleitete Testklsse
// --------------------------------------




#endif // INITGL_H
