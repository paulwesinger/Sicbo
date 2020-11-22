#include "initgl.h"


/*
 * File:   Init3D.cpp
 * Author: paul
 *
 * Created on 17. Mai 2018, 21:11
 */

#include "initgl.h"
//#include "vecmath/vecmath.h"
#include <vector>
//#include "3DLoader/load3ds.h"

// Res for windowed Mode

#define SD_WIDTH    1920//1280.0f//1024
#define SD_HEIGHT   1200//768.0f//768
#define FULLSCREEN_WIDTH   3840//1680.0f//1920.0f
#define FULLSCREEN_HEIGHT  2160//1050.0f//1200.0f

using namespace irrklang;

InitGL::InitGL (const std::string titel){

    caption = titel;
    window = nullptr;
    maincontext = nullptr;

    _FullScreen = false;

    _Mouse.lastx = 0;
    _Mouse.lasty = 0;
    _Mouse.x     = 0;
    _Mouse.y     = 0;

    cube    = NULL;
    cube2   = NULL;
    cube3   = NULL;
    sphere1 = NULL;
    lightSource = NULL;
    skybox  = NULL;
    base2d  = NULL;
   // cockpit = NULL;
    textrender = NULL;
   // soundengine = NULL;
    projection = nullptr;

    InitUtils();
    InitMatrices();

    // Irrklan init
 //   soundengine = createIrrKlangDevice();
 //   if ( soundengine )
 //       loginfo(" SoundEngine created ","InitGL::InitGL");
}

InitGL::InitGL(const InitGL& orig) {
}

InitGL::~InitGL() {
    safeDelete(cube);
    safeDelete(cube2);
    safeDelete(cube3);

    safeDelete(sphere1);
    safeDelete(lightSource);

    if ( skybox != NULL)
        delete skybox;
    if (base2d != NULL  )
        delete base2d;
    if (cockpit != NULL)
        delete cockpit;

    if (ambientLight != nullptr)
        delete  ambientLight;

    DeleteUtils();
    DeleteMatrices();
    DeleteShaders();
}

void InitGL::DeleteShaders() {
    glDeleteProgram(cubeshaderprog_color);
    glDeleteProgram(cubeshaderprog_tex);
    glDeleteProgram(sphereshader_color);
    glDeleteProgram(currentShader);
}

void InitGL::safeDelete(BaseObject * bo) {
    if (bo != NULL) {
        delete bo;
        bo = NULL;
    }
 }

void InitGL::TestFunction() {
    logwarn("Aus der Testfunction", "InitGL::TestFunction");

}


// ******************************************
// Utils
// ------------------------------------------
void InitGL::InitUtils() {
    filestream = new fileUtil();
    if (filestream == NULL) {
        logwarn ("Konnte Util FileStream nicht erstellen !!","InitGL::InitUtils");
    }
    else
        loginfo("Erstelle FileUtil...... DEone","InitGL::InitUtils");
}

void InitGL::InitShaders() {
    //================================================================
    //Test für CustomShader

    shader = new Shader();

    // Vertex Shader
    // ------------------------------------------------------------------------
    std::string v_source ="ShaderSources/cubevertexshader.vex";
    int vs = shader ->compileVertexShaderFromFile(v_source,filestream);
    //Fragment Shader Color
    v_source ="ShaderSources/colorshader.frg";
    int fs_Color = shader ->compileFragmentShaderFromFile(v_source,filestream);
    // Fragment Shader Texture
    v_source ="ShaderSources/cubefragmentshaderMulti.frg";
    int fs_Tex = shader ->compileFragmentShaderFromFile(v_source,filestream);
    // ColorCubeShader
    loginfo("Erstelle Cube Color Shader.................done");
    shader->CreateCustomShader(cubeshaderprog_color);
    shader->AttachCustomShader(cubeshaderprog_color,vs);
    shader->AttachCustomShader(cubeshaderprog_color,fs_Color);
    shader->CreateCustomProgram(cubeshaderprog_color);

    //Texture CubeShader
    loginfo("Erstelle Cube Texture Shader ..............done");
    shader->CreateCustomShader(cubeshaderprog_tex);
    shader->AttachCustomShader(cubeshaderprog_tex,vs);
    shader->AttachCustomShader(cubeshaderprog_tex,fs_Tex);
    shader->CreateCustomProgram(cubeshaderprog_tex);

    // Shader für lightning
    loginfo("Erstelle Cube Lightning Shader ..............done");
    v_source ="ShaderSources/cubevertexnormalshader.vex";
    int vsn = shader ->compileVertexShaderFromFile(v_source,filestream);
    //Fragment Shader Color
    v_source ="ShaderSources/cubefragmentshaderMultinormals.frg";
    int fsn = shader ->compileFragmentShaderFromFile(v_source,filestream);
    shader->CreateCustomShader(cubeshaderprog_normals);
    shader->AttachCustomShader(cubeshaderprog_normals,vsn);
    shader->AttachCustomShader(cubeshaderprog_normals,fsn);
    shader->CreateCustomProgram(cubeshaderprog_normals);

    glDetachShader(cubeshaderprog_color,vs);
    glDetachShader(cubeshaderprog_color,fs_Color);
    glDetachShader(cubeshaderprog_tex,fs_Tex);

    glDetachShader(cubeshaderprog_normals,fsn);
    glDetachShader(cubeshaderprog_normals,vsn);


    // =======================================================================
    //------------------------------------------------------------------------
    // Sphere Shader color:
    v_source = "ShaderSources/spherevertexshader.vex";
    vs = shader->compileVertexShaderFromFile(v_source,filestream);
    // Fragment sHader
    v_source ="ShaderSources/spherefragmentshader.frg";
    fs_Color = shader->compileVertexShaderFromFile(v_source,filestream);
    //Alles zusammenfügen:
    loginfo("Erstelle Sphere Color Shader ..............done");
    shader -> CreateCustomShader(sphereshader_color);
    shader -> AttachCustomShader(sphereshader_color,vs);
    shader -> AttachCustomShader(sphereshader_color,fs_Color);
    shader ->CreateCustomProgram(sphereshader_color);
    // ========================================================================
    //delete binares
    shader->deleteShader(vs);
    shader->deleteShader(fs_Color);
    shader->deleteShader(fs_Tex);

    shader->deleteShader(vsn);
    shader->deleteShader(fsn);
    // ========================================================================
    currentShader = cubeshaderprog_normals;
}

void InitGL::DeleteUtils() {
    if ( filestream != NULL) {
        delete filestream;
        loginfo("Deleted filestream","InitGL::DeleteUtils");
    }
}

void InitGL::DeleteMatrices() {
    if (projection != NULL) {
        delete projection;
        loginfo("Delete Prokjection Matrix Class","InitGL::DeleteMatrtices");
    }
}

// ******************************************
// MAtrices
// ------------------------------------------
void InitGL::InitMatrices() {
    projection = new Projection(0,_ResX,0,_ResY,0.01f,1000.0f);
    if (projection != NULL) {
        loginfo("Created Procjection Class","InitGL::InitMatrices");
    }
}

// ******************************************
// Heier der graphische Teil
// ------------------------------------------
bool InitGL::InitSDL2()  {


    if (  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO ) != 0 )
    {
            printf("Initialisierung fehlgeschlagen  FEHLERMELDUNG:  %s" , SDL_GetError());
            return(false);

    }
    atexit(SDL_Quit);

    printf("Init success \n");
    SDL_GL_LoadLibrary(NULL);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL,1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,5);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,24);

    int numDisplaymodes = SDL_GetNumDisplayModes(0);
    loginfo("Num Display modes: " + IntToString(numDisplaymodes), "InitGL::Init");

    // Alle Display modes auflisten:
    int currentdisplay = 1;
    for (int i = 0; i < numDisplaymodes; i++ ) {
        SDL_DisplayMode dpm;
        SDL_GetDisplayMode(currentdisplay,i,&dpm);
        std::string w = IntToString(dpm.w);
        std::string h = IntToString(dpm.h);
        std::string index =IntToString(i);
        try {

            // todo: Pixelformat auswerten
            loginfo("Resolution Mode[" + index+ "] : " + w + "x" + h ,"InitGL::Init");
        }
        catch ( ...) {
            logwarn("Konnte mode[" + index + "] nicht ermitteln");
        }

    }


    SDL_DisplayMode dm;
    SDL_GetDesktopDisplayMode(0, &dm );

    _ResX = FULLSCREEN_WIDTH;
    _ResY = FULLSCREEN_HEIGHT;

    if ( _FullScreen) {

        window = SDL_CreateWindow(
                caption.c_str(),
                0,//SDL_WINDOWPOS_UNDEFINED,
                0,//SDL_WINDOWPOS_UNDEFINED,
                _ResX,_ResY,
                SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_OPENGL
                //SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL
            );

        // Test für andreren Mode

        //dm.h = 600;
        //dm.w = 800;
        //if (SDL_SetWindowDisplayMode(window,&dm) == 0 )
        //    printf("Neuer Mode : %i x % i", dm.w,dm.h);
        //else
        //    printf(" Failed to set Mode !!! %s", SDL_GetError());

    }
    else {
            _ResX = SD_WIDTH;
            _ResY = SD_HEIGHT;
            window = SDL_CreateWindow(
                caption.c_str(),
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                _ResX,_ResY,
                SDL_WINDOW_OPENGL
    );

     //   dm.h = 720;
     //   dm.w = 1280;
     //   if (SDL_SetWindowDisplayMode(window,&dm) == 0 )
     //       printf("Neuer Mode : %i x % i", dm.w,dm.h);
     //   else
     //       printf(" Failed to set Mode !!! %s", SDL_GetError());
    }

    if ( window == NULL)  {
        sdl_die("Konnte Fenster nicht erzeugen");
        return false;
    }

    maincontext = SDL_GL_CreateContext(window);
    if ( ! maincontext) {
        sdl_die (" Konnte Context nicht erzeugen ");
        return false;
    }

    printf(" Geladene OpenGL version :");
    //gladLoadGLLoader(SDL_GL_GetProcAddress);
    // Alles Ok ,checken was initialisiert ist :
    printf("Vendor :  %s\n",glGetString(GL_VENDOR));
    printf("Renderer  %s\n",glGetString(GL_RENDERER));
    printf("Version   %s\n",glGetString(GL_VERSION));// glClearColor(0.5f,0.5f,0.0f,0.0f);


       GLenum err = glGetError();
       switch (err){
           case GL_NO_ERROR         :   loginfo (" Kein Fehler ","Base2D Render");  break;
           case GL_INVALID_ENUM     :   loginfo ("Invalid Enum","Base2D Render");   break;
           case GL_INVALID_VALUE    :   loginfo ("Invalid Value","Base2D REnder");  break;
           case GL_INVALID_OPERATION:   loginfo ("Invalid operation","Base2D Render"); break;
           case GL_INVALID_FRAMEBUFFER_OPERATION: loginfo ("Invalid Framebuffer Operation","Base2D Render"); break;
           case GL_OUT_OF_MEMORY    :   loginfo("Out of Memory","Base2D Render"); break;
           case GL_STACK_UNDERFLOW  :   loginfo ("Stack UNDERFLOW","Base2D Render"); break;
           case GL_STACK_OVERFLOW   :   loginfo ("Stack Overflow","Base2D Render"); break;
           default: loginfo("Undefined");
       }


    glewExperimental = GL_TRUE;
    glewInit();
    // Use v-sync
    SDL_GL_SetSwapInterval(1);

    // Testweise Displaymode ermitteln
    SDL_DisplayMode current;
    int error = SDL_GetCurrentDisplayMode(0, &current);
    if ( error == 0) {
       printf ("InitGL -- WindowSize:  %i x %i h \n",current.w,current.h);
    }
    else
        printf ("InitGL -- RES_X,RES_Y :  %i x %i h \n",_ResX,_ResY);

    InitEngineObject();


    // --------------------------------
    // Test Loader:
    //---------------------------------

    //me = new CMeshObject();
    //me->Load3DSMesh("Meshes/spaceship.3ds");

    //objLoader = new COBJLoader("Meshes/Quad.obj");

    //int errorid = objLoader->LoadOBJ();

    //logwarn(" OBJLoader return ID: " + IntToString(errorid));


    return true;
}

void InitGL::setClearColor(float r, float g, float b) {
    _ClearColor.x = r;
    _ClearColor.y = g;
    _ClearColor.z = b;
    _ClearColor.w = 1.0f;
}

void InitGL::SetClearColor(float r, float g, float b, float a) {
    _ClearColor.x = r;
    _ClearColor.y = g;
    _ClearColor.z = b;
    _ClearColor.w = a;
}

void InitGL::InitEngineObject() {
// ===============================================================
// Hier werden alle Objeckte initialisiert
// ===============================================================

    loginfo("Erstelle Viewport " + IntToString(_ResX) + " x " + IntToString(_ResY),"InitGL::InitEngineObject");
    glViewport(0,0, _ResX , _ResY );

    loginfo("Setze ClearColor auf Schwartz ...... done","InitGL::InitEngineObject");
    _ClearColor.x = 0.0f; _ClearColor.y = 0.0f; _ClearColor.z = 0.0f; _ClearColor.w = 1.0f;
    glClearColor(_ClearColor.x, _ClearColor.y, _ClearColor.z, _ClearColor.w);

    // ---------------------------------------
    // Camera View
    // ---------------------------------------
    loginfo("Erstelle Camera Objekt","InitGL::InitEngineObject");
    camera = new (Camera);
    loginfo("Erstelle Camera Objekt ...... done","InitGL::InitEngineObject");

    // ---------------------------------------
    // Light init
    //----------------------------------------
    ambientLight = new light;
    ambientLight->setColor(glm::vec3(1.0,0.0,1.0));
    ambientLight->setPos(glm::vec3(0.0,15.0,10.0));


    loginfo("Erstelle Standard Ambientes Licht ","InitGL::InitEngineObjects");

    // ---------------------------------------
    // Skybox ,objects init.
    // ---------------------------------------
    loginfo("Erstelle Standard SkyBox \"Dessert\" ........","InitGL::InitEngineObject");
    skybox = new SkyBox(projection->GetPerspective());
    std::vector<std::string> faces;

    faces.push_back("skybox/desert/desert_lf.tga");
    faces.push_back("skybox/desert/desert_rt.tga");
    faces.push_back("skybox/desert/desert_up.tga");
    faces.push_back("skybox/desert/desert_dn.tga");
    faces.push_back("skybox/desert/desert_ft.tga");
    faces.push_back("skybox/desert/desert_bk.tga");
    skybox -> Load(faces);
    loginfo("Erstelle Skybox ........Done","InitGL::InitEngineObject");



    //================================
    // Init 2D Objects
    // ===============================




    //  Diesen Block auslagern zur Engine

    /*

    loginfo("============================");
    loginfo("Erstelle 2D Objekte.........");
    loginfo("============================");
    logimage("Erstelle TestIcon..........");
    base2d = new Base2D(_ResX, _ResY,"icons/ibus-setup-hangul.png");
    logimage("Erstelle Cockpt............");
*/
    //cockpit= new Base2D(_ResX, _ResY,"images/blackWidowAlpha1920x1200.png");




    logimage("Erstelle Text Renderer.....");
    textrender = new TextRender(_ResX, _ResY);
    loginfo("Erstelle Shaders........");
    InitShaders();
    loginfo("..... done all");
    loginfo("============================");


    //========================================
    // Init 3D Objects
    //========================================

    loginfo("Erstelle 3D Objects .........");
    loginfo("Erstelle cube 1 - 3......done");

    // Liste mit Texture pfaden erstellen

    std::vector<std::string> cubeimages;
    fileUtil fu;

    cube  = new CCube(glm::vec3(0.0,0.0,0.0),glm::vec4(1.0,1.0,1.0,1.0), projection->GetPerspective());   // no scale
    // Cube Shader init
    cube->initShader(COLOR_SHADER,cubeshaderprog_color);
    cube->initShader(TEXTURE_SHADER,cubeshaderprog_tex);
    cube->initShader(LIGHT_SHADER, cubeshaderprog_normals);
    cube->setActiveShader(TEXTURE_SHADER);


    cube->addLight(ambientLight);



    // Texture loading
    bool texturesok =  fu.readLine("config/cubetextures.cfg",cubeimages);
    if (texturesok)
        cube->addTexture(cubeimages,"IniGL::  cube");
    else
        logwarn("Init::Cube :  konnte Textures nicht laden ! ","InitGL::Init::cube::AddTexture");

    cubeimages.clear();

    // Cube 2 init

    cube2 = new CCube(glm::vec3(0.0,0.0,0.0),glm::vec4(1.0,1.0,1.0,1.0), projection->GetPerspective());
    cube2->initShader(COLOR_SHADER,cubeshaderprog_color);
    cube2->initShader(TEXTURE_SHADER,cubeshaderprog_tex);
    cube2->initShader(LIGHT_SHADER,cubeshaderprog_normals);

    cube2->setActiveShader(LIGHT_SHADER);
    cube2->addLight(ambientLight);

    texturesok =  fu.readLine("config/cube2textures.cfg",cubeimages);
    if (texturesok)
        cube2->addTexture(cubeimages,"InitGL::cube2");
    else
        logwarn("Init::Cube2 :  konnte Textures nicht laden ! ","InitGL::Init::cube2::addTexture");
    cubeimages.clear();

    // Cube3 init
    cube3 = new CCube(glm::vec3(0.0,0.0,0.0),glm::vec4(1.0,1.0,1.0,1.0), projection->GetPerspective());
    cube3->initShader(COLOR_SHADER,cubeshaderprog_color);
    cube3->initShader(TEXTURE_SHADER,cubeshaderprog_tex);
    cube3->initShader(LIGHT_SHADER,cubeshaderprog_normals);

    cube3->setActiveShader(LIGHT_SHADER);
    cube3->addLight(ambientLight);
    // Texture loading
    texturesok =  fu.readLine("config/cube2textures.cfg",cubeimages);
    if (texturesok)
        cube3->addTexture(cubeimages,"InitGL::cube3");
    else
        logwarn("Init::Cube3 konnte Textures nicht laden ! ","InitGL::Init::cube2::addTexture");
    cubeimages.clear();


    //----------------------------------------------------
    // engine objects testen
    //- --------------------------------------------------

    for (int i = 0; i< objects3D.size(); i ++) {
        objects3D.at(i) -> initShader(COLOR_SHADER,cubeshaderprog_color);
        objects3D.at(i) -> initShader(TEXTURE_SHADER,cubeshaderprog_tex);
        objects3D.at(i) -> initShader(LIGHT_SHADER,cubeshaderprog_normals);
        objects3D.at(i) -> SetProjection(projection->GetPerspective());

        //if (objects3D.at(i)->HasTextures() )
        //    objects3D.at(i)->addTexture()

        objects3D.at(i)->setActiveShader(LIGHT_SHADER);
        objects3D.at(i)->addLight(ambientLight);
    }


    // Transformations
 //   cube->Translate(vec3(0,0,20));wird
 //   cube3->Translate(vec3(0,0,-2));
    // Sphere
    loginfo("Erstelle Sphere .........done");
    sphere1  = new CSphere(glm::vec3(0.0,0.0,0.0),glm::vec4(1.0,0.0,0.0,1.0), projection->GetPerspective(),12,(GLfloat)4.0,shader);
    sphere1->SetColor(glm::vec4(1.0,0.0,0.5,1.0));

    //-----------------------------------------
    // Lightsource as a spere
    //-----------------------------------------
    loginfo("Serstell LichtQuelle als wiesse spere....","InitGL::InitEngineObjects");
    lightSource = new CSphere(ambientLight->getPos(),glm::vec4(0.0,0.0,1.0,1.0),projection->GetPerspective(),24,(GLfloat)2.0,shader );



    loginfo("Done 3D Objects .............");

    angleX = 0.0f;
    angleY = 0.0f;
    moveZ = 0.0f;
}

// --------------------------------------------
// Adding's
// --------------------------------------------
void InitGL::add3Dobject(CCube *obj) {

    obj->initShader(COLOR_SHADER,cubeshaderprog_color);
    obj->initShader(TEXTURE_SHADER,cubeshaderprog_tex);
    obj->initShader(LIGHT_SHADER, cubeshaderprog_normals);
    obj->setActiveShader(TEXTURE_SHADER);

    obj->addLight(ambientLight);
    objects3D.push_back(obj);
}

void InitGL::add2Dobject(Base2D *obj) {
    objects2D.push_back(obj);
}

void InitGL::addButton(CButton* obj) {
    buttons.push_back(obj);
}

void InitGL::Run() {


    bool quit = false;
    static float rot_y = 1.0f;
    static float tx = 0.05f;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Diese transformations vectoren enthalten die "steps" für die Animation
    vec3 steptrans;
    vec3 steprotate;
    vec3 stepscale;

    bool left = true;
    bool up = true;

    steptrans  = vec3(0.0,-0.5,0.0);
    steprotate = vec3(0.5,0.8,0.0);
    stepscale  = vec3(0.0,0.0,0.0);

    cube->Translate(vec3(8.5,0.0,0.0));
    cube2->Translate(vec3(-8.0,-3.0,0.0));
    cube3->Translate(vec3(0.0,-2.0,0.0));
    sphere1->Translate(vec3(0.0,-4.0,0.0));




  //  cube->Rotate(vec3(15.0,0.0,0.0));
  //  cube2->Rotate(vec3(0.0,45,0.0));
  //  cube3->Rotate(vec3(0.0,60.0,0.0));
    // timetest
    Uint32 tickstart = SDL_GetTicks();
    Uint32 tickend   = tickstart;
    Uint32 elapsed = 0;
    Uint32 second  = 0; // Zähler für elapsed bis 1000 ms
    int event = 0;
    if (soundengine) {
      /*

    irrklang::ISound * snd = soundengine -> play2D("sounds/getout.ogg");
       if (snd) {
           snd ->setVolume(40);
          // snd->drop();
       }

       if (snd )
           snd ->drop();
        */
    }


    // TEstfunction

//    FP fp = TestFunction;
//       base2d->setHandler(fp);

    // Test Zeilen für Text Fenster

    std::vector<std::string> texts;
    textrender->AddString("Das ist die 1. Zeile");
    textrender->AddString("Das ist die 2. Zeile");
    //texts.push_back("Das ist die 3. Zeile");
    //texts.push_back("Da ist nochwas");
    //.push_back("Da ist noch Zeile 5");

    textrender->SetHasBottom(true);
    textrender->SetHasHeader(true);
    textrender->SetHasBackground(true);
    textrender->SetHasTexture(true);
    textrender->SetAlignRight(false);



    for (int i = 0; i < objects3D.size(); i++ ) {
        loginfo("TranslateX: " + FloatToString(objects3D.at(i)->GetTranslate().x));
        loginfo("TranslateY: " + FloatToString(objects3D.at(i)->GetTranslate().y));
        loginfo("TranslateZ: " + FloatToString(objects3D.at(i)->GetTranslate().z));
        logEmptyLine();
        loginfo("Color Red: " + FloatToString(objects3D.at(i)->GetColor().x));
        loginfo("Color Green : " + FloatToString(objects3D.at(i)->GetColor().y));
        loginfo("Color Blue: " + FloatToString(objects3D.at(i)->GetColor().z));
        loginfo("Color Alpha: " + FloatToString(objects3D.at(i)->GetColor().w));
        logEmptyLine(2);
    }








    while ( ! quit) {
        elapsed = tickend - tickstart;

        tickstart = tickend;
        // -------------------------------
        // Test für dynamische winkel
        // abhängig von verbrauchter zeit
        // -------------------------------

       double ups = 5.0f;  // umdrehung per sec...... min

       /*
        * TODO:
        * Klasse für SDL_GetTicks() erstellen ,
        * Funktionen für U/min bzw. U/sec. erstellen
        * verstrichene Zeit ist eben dann max. 60000 für Minute
        * oder 1000 für Secunde !!!
        */
       /* Dieser Bereich für DauerRotation der KAmeara
       double frames = 6000000.0 / second ;

       double mmin = 10.0f;
       double speed  = 0.2f; // geschwindigkeit

       second += elapsed;

       angleX = 360.0f * ups / frames   ;   // upm jetzt
       float ax = static_cast<float> (angleX);

       //checkdegree(ax);
       if (ax > 360.0f )
           ax -= 360;

       if ( second > 60000 )
          second = 0;

       glm::vec3 pos = camera -> GetOrgPos();
       pos = glm::rotateY(pos,ax);

       camera -> SetPos(pos);
       camera -> UpdateCamera();
       */
       uint motionx =0;
       uint motiony =0;




       e.type = 0;
       SDL_PollEvent( &e );

       event = HandleInput(e,motionx,motiony );

       SDL_Event testEvent = e;

       HandleEvent(testEvent);




       switch ( event) {
           case KEY_Esc  : quit = true;    break;

           case KEY_A: camera->MoveLeft(elapsed); break;
           case KEY_Left : camera->YawCameraLeft(elapsed); break;

           case KEY_D: camera->MoveRight(elapsed);break;
           case KEY_Right: camera->YawCameraRight(elapsed); break;


           case KEY_E: camera->MoveForward(elapsed); break;
           case KEY_Up: camera->PitchCameraUp(elapsed); break; break;

           case KEY_S:camera->MoveBackward(elapsed);break;
           case KEY_Down: camera ->PitchCameraDown(elapsed); break;

           // Key "C" und "T" sind für Color und Texture Shader
           case KEY_C: {
               currentShader = cubeshaderprog_color;
               break;
           }

           case KEY_T: {
               currentShader = cubeshaderprog_tex;
               break;
           }

           // ORtho oder perspective mode:
           case KEY_O : {
               // Hier sollte eine Objectliste durchlaufen werden ,
               // bei allen objecten jetzt projection auf orho setzen
               // Hier setzte ich mal alle 3 per "Hand"
           ////    cube->SetProjection(projection->GetOrtho(), true);
           ////    cube2->SetProjection(projection->GetOrtho(), true);
           ////    cube3->SetProjection(projection->GetOrtho(), true);
               //skybox->SetProjection(projection->GetOrtho());
               break;
           }
           case KEY_P: {  // Wired
            ////   cube->SetProjection(projection->GetPerspective(), false);
            ////   cube2->SetProjection(projection->GetPerspective(), false);
            ////   cube3->SetProjection(projection->GetPerspective(), false);
               //skybox->SetProjection(projection->GetOrtho());
               break;
           }

           // Roll Camera left
           case NUM_1: {
               camera->YawEyeLeft(elapsed);
               break;
           }
           // Roll Camera right
           case NUM_3: {
               camera-> YawEyeRight(elapsed);
               break;
           }
           case NUM_2: {
               // Pitch Camera Doun
               camera -> PitchEyeDown(elapsed);
               break;
           }

           case NUM_5: {
               // Pitch camera Up
               camera -> PitchEyeUp(elapsed);//
               break;
           }
       }

       glEnable(GL_DEPTH_TEST);
       glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

       vec3 dummy = steptrans;
     //  cube->StepTranslate(dummy);

        dummy=steprotate;

        cube->StepRotate(steprotate);
        cube->SetColor(vec4(1.0,0.0,0.0,1.0));
        /**
        * Alle mit 3 / wieder enklammern !!!
        */
        cube->SetFirstTranslate(true);
     //   cube ->Draw( camera, currentShader);

        cube2 -> SetColor(vec4(1.0,1.0, 1.0,1.0));
     //   cube2 -> Calc( rot_y + 0.3f,tx + 0.05f);
        if ( left ) {
            dummy.x -= steptrans.x;
            //printf(" dymmy v. cube2  %f \n",dummy.x );
            if (dummy.x < 1.0)
                left = false;
        }
        else{
            dummy.x += steptrans.x;
            //printf(" dymmy v. cube2  %f \n",dummy.x );
            if (dummy.x > 1.0 )
                left = true;
        }
        ///dummy.y += steprotate.y;TEXTURE
        cube2->SetFirstTranslate(false);
        cube2->StepRotate(dummy);
        //cube2->Draw( camera, currentShader);

        cube3 -> SetColor(vec4(0.0,1.0,0.0,1.0));




     //   cube3 -> Calc( rot_y + 0.4,0.4);
        //      dummy = steprotate;

        dummy = vec3(1.5,3.0,0.0);
        //dummy.y += steprotate.y;//0.07;
        cube3->StepRotate(dummy);

        dummy = steptrans;
        //dummy.y += steptrans.y;//0.05;
        //cube3->StepTranslate(dummy);
        cube3->Translate(dummy);
        cube3->StepRotate(dummy);

        cube3->SetFirstTranslate(false);
        cube3->Draw ( camera, currentShader);
        //glUseProgram(0);

        dummy = vec3(1.0,2.0,3.0);
        sphere1->SetFirstTranslate(true);
        sphere1 ->StepRotate(dummy);


        //meshObject
        dummy = vec3(1.0,0.0,0.0);
      //  me->SetFirstTranslate(true);
      //  me ->StepRotate(dummy);
      //  me->StepTranslate(glm::vec3(2.0,0.5,0.0));
      //  me->Translate(glm::vec3(dummy));
      //  me->Draw(camera, currentShader);


        // lightsource
        dummy = vec3(1.0,0.0,0.0);
        lightSource->SetFirstTranslate(true);
        lightSource ->StepRotate(dummy);



        //sphere1->SetColor(vec4(1.0,0.4,0.8,1.0));
       // sphere1->StepTranslate(glm::vec3(2.0,0.5,0.0));
        //sphere1->Translate(glm::vec3(dummy));
        //currentShader = sphereshader_color;
        sphere1->Draw(camera);//,sphereshader_color);
        // lightsource
        lightSource->Draw(camera);

        // ===================================
        // Das beste zum Schluss : Skybox
        // ===================================
        skybox->Draw(camera->GetView());

        if (! objects3D.empty() ) {
            for (unsigned int i=0;i < objects3D.size(); i++ ) {
                dummy = vec3(1.0 * (float) i ,2.0,3.0);
                objects3D[i]->SetProjection(projection->GetPerspective());

                objects3D[i]->Translate(dummy);
                objects3D[i]->StepRotate(dummy);
                objects3D[i]->Draw(camera,currentShader);

            }
        }

        // ===================================
        // Alles für 2D Projektion vorbereiten
        //====================================

        Prepare2D();

        //cockpit->Render(0.0f,0.0f,camera);

        textrender -> Render(100.0f, 400.0f);


        if ( !  objects2D.empty() ) {
            for (uint i =0; i < objects2D.size(); i++) {
                (objects2D[i])->Render();
            }
        }

        if ( ! buttons.empty() ) {
            for ( uint i = 0; i < buttons.size(); i++) {
                buttons[i]->Render();

            }
        }

        Restore3D();

        SDL_GL_SwapWindow(window);
        tickend = SDL_GetTicks();

     }
}

void InitGL::Done() {
}

void InitGL::Prepare2D() {
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glDepthMask(0);
}

void InitGL::Restore3D() {
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    glFrontFace(GL_CCW);
    glEnable(GL_BLEND);
    glDepthMask(1);
}



uint InitGL::HandleEvent(SDL_Event e) {
    switch (e.type) {
        case SDL_MOUSEMOTION : OnMouseMove(e.motion.x, e.motion.y, e.button.state); break;
        case SDL_MOUSEBUTTONUP: {
            if ( e.button.button == SDL_BUTTON_LEFT ) {
                OnLeftMouseButtonClick(e.motion.x, e.motion.y);
            }
            break;
        }
    }
    return  e.type; // Falls wo gebraucht wird
}

//-------------------------------------------------
// Mouse events
//-------------------------------------------------
void InitGL::OnMouseMove(int &x, int &y, uint buttonstate) {

}
void InitGL::OnLeftMouseButtonClick(int &x, int &y) {

    if ( ! buttons.empty() ) {
        for (uint i = 0; i < buttons.size(); i++) {
            if (buttons[i]->intersect(x, y) )
                buttons[i]->OnClick();
        }
    }
}


int InitGL::HandleInput(SDL_Event e, uint &mox, uint &moy) {

    switch (e.type) {
   //
        case SDL_MOUSEWHEEL :   return  MOUSE_Wheel;

        // Keyboard
        case SDL_KEYDOWN : {
            switch(e.key.keysym.sym ) {
                case SDLK_RIGHT     : return KEY_Right;
                case SDLK_LEFT      : return KEY_Left;
                case SDLK_UP        : return KEY_Up;
                case SDLK_DOWN      : return KEY_Down;
                case SDLK_ESCAPE    : return KEY_Esc;
                // Ziffernblock:
                case SDLK_KP_0      : return NUM_0;
                case SDLK_KP_1      : return NUM_1;
                case SDLK_KP_2      : return NUM_2;
                case SDLK_KP_3      : return NUM_3;
                case SDLK_KP_4      : return NUM_4;
                case SDLK_KP_5      : return NUM_5;
                case SDLK_KP_6      : return NUM_6;
                case SDLK_KP_7      : return NUM_7;
                case SDLK_KP_8      : return NUM_8;
                case SDLK_KP_9      : return NUM_9;

                case SDLK_a         : return KEY_A;
                case SDLK_s         : return KEY_S;
                case SDLK_d         : return KEY_D;
                case SDLK_e         : return KEY_E;
                case SDLK_p         : return KEY_P;  // PerspectiveMode
                case SDLK_o         : return KEY_O;  // Orthomode
                case SDLK_c         : return KEY_C;  // Colored Draw
                case SDLK_t         : return KEY_T;  // Textured Draw

                default : return NO_INPUT;
            }
       }
        // Mouse Buttons
        case SDL_MOUSEBUTTONUP : {
            switch (e.button.button) {
                case SDL_BUTTON_LEFT    :   return MOUISE_BTN_Left_Clk;
                case SDL_BUTTON_MIDDLE  :   return MOUSE_BTN_MiddLe_Clk;
                case SDL_BUTTON_RIGHT   :   return MOUSE_BTN_Right_Clk;
            }
        }

        case SDL_MOUSEMOTION: {

            // Camera Yaw

            int stepx = e.motion.x - ( _ResX / 2);//_Mouse.lastx;
            int stepy = e.motion.y - ( _ResY / 2);//_Mouse.lasty;

             /*
            if ( stepx > 0 ) {

                mox = MOUSE_Move_Right;
                logwarn("Motion X " + IntToString(mox),"InitGL::HandleInput");
            }
            else if ( stepx < 0) {
                mox = MOUSE_Move_Left;
                logwarn("Motion X " + IntToString(mox),"InitGL::HandleInput");
            }
            // Das ganse für pitch

            if (stepy > 0 ) {
                moy =  MOUSE_Move_Down;
                logwarn("Motion Y " + IntToString(moy),"InitGL::HandleInput");
            }
            else if (stepy < 0 ) {
                moy =  MOUSE_Move_Up;
                logwarn("Motion Y " + IntToString(moy),"InitGL::HandleInput");

            }
            else
                return NO_INPUT;
            */
            mox = e.motion.x;
            moy = e.motion.y;

            _Mouse.lastx = e.motion.x;
            _Mouse.lasty = e.motion.y;
            return MOUSE_Move;
        }
        default : return NO_INPUT;
    }

    return NO_INPUT;
}

void InitGL::sdl_die( std::string msg)
{
    fprintf(stderr,"%s: %s \n", msg.c_str(),SDL_GetError());

}



