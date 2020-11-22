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

#define SD_WIDTH    2880//1280.0f//1024
#define SD_HEIGHT   1620//768.0f//768
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

    if (ambientLight != nullptr)
        delete  ambientLight;

    DeleteUtils();
    DeleteMatrices();
    DeleteShaders();
}

void InitGL::DeleteShaders() {

}

void InitGL::safeDelete(BaseObject * bo) {
    if (bo != NULL) {
        delete bo;
        bo = NULL;
    }
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

    int allDisplays = SDL_GetNumVideoDisplays();
    loginfo("Anzahl Displays " + IntToString(allDisplays));


    // Alle Display modes auflisten:
    // ------------------------------------------------------
    // ÄNDERN !!! -> fragen welcehes display genommen wird !!
    int currentdisplay = allDisplays - 1;
    // ------------------------------------------------------
    for (int i = 0; i < numDisplaymodes; i++ ) {
        SDL_DisplayMode dpm;
        if ( SDL_GetDisplayMode(currentdisplay,i,&dpm) == 0 ) {


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
        else {
            std::string s(SDL_GetError());
            logwarn(" Warning: " + s);

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
     glClearColor(r, g, b, 1.0);
}

void InitGL::SetClearColor(float r, float g, float b, float a) {
    _ClearColor.x = r;
    _ClearColor.y = g;
    _ClearColor.z = b;
    _ClearColor.w = a;

    glClearColor(r, g, b, a);
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


    logimage("Erstelle Text Renderer.....");
    textrender = new TextRender(_ResX, _ResY);
    loginfo("Erstelle Shaders........");
    InitShaders();
    loginfo("..... done all");
    loginfo("============================");
}

// --------------------------------------------
// Adding's
// --------------------------------------------
void InitGL::add3Dobject(BaseObject *obj) {

   // obj->initShader(COLOR_SHADER,cubeshaderprog_color);
   // obj->initShader(TEXTURE_SHADER,cubeshaderprog_tex);
   // obj->initShader(LIGHT_SHADER, cubeshaderprog_normals);
   // obj->setActiveShader(TEXTURE_SHADER);

   // obj->addLight(ambientLight);
    //objects3D.push_back(obj);
}

void InitGL::add2Dobject(Base2D *obj) {
    objects2D.push_back(obj);
}

void InitGL::addButton(CButton* obj) {
    buttons.push_back(obj);
}

void InitGL::Run() {


    bool quit = false;


    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


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

    std::vector<std::string> texts;
    textrender->AddString("Das ist die 1. Zeile");
    textrender->AddString("Das ist die 2. Zeile");

    textrender->SetHasBottom(true);
    textrender->SetHasHeader(true);
    textrender->SetHasBackground(true);
    textrender->SetHasTexture(true);
    textrender->SetAlignRight(false);

    while ( ! quit) {

        glClearColor(_ClearColor.x,_ClearColor.y, _ClearColor.z, _ClearColor.w);
        glClear(GL_COLOR);
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
              // currentShader = cubeshaderprog_color;
               break;
           }

           case KEY_T: {
              // currentShader = cubeshaderprog_tex;
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



