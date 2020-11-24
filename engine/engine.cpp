#include "engine.h"
#include "../utils/utils.h"

CEngine::CEngine(std::string titel) :
        InitGL(titel)
{}

void CEngine::Done() {
    InitGL::Done();
}

void CEngine::Run() {

    Init2D();
    Init3D();
    InitButtons();
    SetClearColor(0.2, 0.2, 0.7, 1.0);
    //logimage("Erstelle Cockpt............");
    //cockpit= new Base2D(_ResX, _ResY,"images/blackWidowAlpha1920x1200.png");

    InitGL::Run();
}

// ---------------------------------------------------------------
// Place here Functons for button handler..
// ---------------------------------------------------------------
void CEngine::EngineTestFunc1() {
     loginfo("TestFunc1","Engine:: EngineTestFunc 1");
}
void CEngine::EngineTestFunc2() {
     loginfo("TestFunc2","Engine:: EngineTestFunc 2");
}
void CEngine::EngineTestFunc3() {
    loginfo("TestFunc3","Engine:: EngineTestFunc 3");
}

// --------------------------------------------------------------
// Init Methods for Engines
// --------------------------------------------------------------
void CEngine::Init2D() {

    //================================
    // Init 2D Objects
    // ===============================

    // Testweise mal einfügen
    loginfo("============================");
    loginfo("Erstelle 2D Objekte.........");
    loginfo("============================");

    loginfo("============================");
    loginfo("Erstelle Spielfeld..........");
    loginfo("============================");
    spielFeld = new Base2D(_ResX,_ResY,"../blender/Sicbo/Spielfeld.png");
    logimage("Done Spielfeld    ..........");


    //--------------------------------------
    loginfo("============================");
    loginfo("Erstelle Jetons.............");
    loginfo("============================");
    jetonRed = new Base2D(_ResX,_ResY,IMAGE_PATH +"JetonRed.png");
    jetonRed->setPos(10,1500);
    jetonBlue = new Base2D(_ResX,_ResY,IMAGE_PATH +"JetonBlue.png");
    jetonBlue->setPos(100,1500);
    jetonGreen = new Base2D(_ResX,_ResY,IMAGE_PATH + "JetonGreen.png");
    jetonGreen->setPos(200,1500);
    jetonYellow = new Base2D(_ResX,_ResY,IMAGE_PATH + "JetonGelb.png");
    jetonYellow->setPos(300,1500);

    add2Dobject(spielFeld);
    add2Dobject(jetonRed);
    add2Dobject(jetonBlue);
    add2Dobject(jetonGreen);
    add2Dobject(jetonYellow);

    logimage("Done jetons       ..........");



  }

void CEngine::Init3D(){
    logEmptyLine();
    loginfo("============================");
    loginfo("Erstelle 3D Objekte.........");
    loginfo("============================");
   fileUtil * fileutil = new fileUtil();
   bool ok;

   ok = fileutil->readLine(OBJECT3D_CFG + "Objects3DList.cfg", object3DList);
   if (ok) {
       loginfo("Lade Datei |ObjectList3D.cfg|","CEngine::Init3D");

        // Liste mit Objekten abarbeiten :
        for (unsigned int i = 0; i < object3DList.size(); i++) {

            std::string path = OBJECT3D_CFG + object3DList[i];

            loginfo("Erstelle Object: .......< " + path+ " >","Engine::Init3D");

            fileUtil * objreader = new fileUtil;
            std::vector<std::string> objconfig;
            objreader->readLine(path, objconfig);


            if( ! objconfig.empty() ) {

                s3DStruct s3D;

                if (init3DStruct(s3D,objconfig)) {
                    CCube * obj = new CCube(glm::vec3(0.0,0.0,0.0),glm::vec4(s3D.color.x, s3D.color.y, s3D.color.z, s3D.color.w),projection->GetPerspective());

                    //obj->SetColor(glm::vec4(s3D.color.x, s3D.color.y, s3D.color.z, s3D.color.w));
                    if ( s3D.textures == "" )
                        obj->SetHasTextures( false);
                    else
                        obj->SetHasTextures( true);

                    obj->SetFirstTranslate( ( s3D.firstTranslate == 1) ? true: false);
                    obj->Rotate(glm::vec3(s3D.trans.rotate.x, s3D.trans.rotate.y, s3D.trans.rotate.z) );
                    obj->Translate(glm::vec3(s3D.trans.translate.x, s3D.trans.translate.y, s3D.trans.translate.z));
                    obj->Scale(glm::vec3(s3D.trans.scale.x, s3D.trans.scale.y, s3D.trans.scale.z));

                    //----------------------------------------
                    // Add textures , if we have some
                    // ---------------------------------------
                    bool texturesok;
                    std::vector<std::string> images;

                    std::string path = s3D.textures;

                    if ( s3D.textures != "" ) {
                        fileUtil fu;

                        texturesok =  fu.readLine(path, images);
                        if (texturesok)
                            obj->addTexture(images,"InitGL::add3DObject");
                        else
                            logwarn("Init::Cube2 :  konnte Textures nicht laden ! ","InitGL::Init::cube2::addTexture");
                    }

                    add3Dobject(obj);
                    loginfo("s3D initialisisert ","CEngine::init3D");
                }
                else
                    logwarn("konnte s3D nicht initialisieren !!", "CEngine::init3D" );
                   // Hier die neuen stringpart functions einbauen

                logEmptyLine();
                loginfo("Prepare for next Object: ","CEngine::init3D");

           }
       }
   }
   else
       logwarn("Fehler: Datei | config/Object3DList.cfg nicht gefunden ! |");

   logEmptyLine() ;
   loginfo("----------------------------");
   loginfo(" Init 3D Objects ");
   loginfo("     READY");
   loginfo("----------------------------");
}



void CEngine::InitButtons() {

    logEmptyLine();
    loginfo("============================");
    loginfo("Erstelle Buttons ...........");
    loginfo("============================");

    loadButtons();

}

void CEngine::loadButtons() {
    // Open file
    bool ok;
    fu = new fileUtil();

    ok = fu->readLine(BUTTONS_CFG + "ButtonList.cfg", btnList);


    if (ok) {
        loginfo("Lade Datei |ButtonList.cfg|","CEngine::loadbuttons");
        logEmptyLine();

        for (unsigned int i = 0; i < btnList.size(); i++  ){

            std::string path = BUTTONS_CFG + btnList[i];

            fileUtil * btnreader = new fileUtil;
            std::vector<std::string> btnconfig;
            btnreader->readLine(path, btnconfig);



            if ( ! btnconfig.empty() ) {

                loginfo("Erstelle Button: .......< " + path+ " >", "Engine::loadButtons");

                sButtonStruct btnStruct;
                if  (initButtonStruct(btnStruct,btnconfig) ) {

                    CButton * btn = new CDefaultButton(_ResX,_ResY ,btnStruct.path);//   btnStruct.path);
                    btn->setPos(btnStruct.PosX,btnStruct.PosY);
                    btn->setSize(btnStruct.SizeX, btnStruct.SizeY);
                    btn->setbuttonColors(glm::vec3(btnStruct.ImageRed,btnStruct.ImageGreen,btnStruct.ImageBlue),
                                         glm::vec3(btnStruct.TextRed,btnStruct.TextGreen,btnStruct.TextBlue));

                    switch (i) {
                        case 0: btn ->AddHandler(EngineTestFunc1); break;
                        case 1: btn ->AddHandler(EngineTestFunc2); break;
                        case 2: btn ->AddHandler(EngineTestFunc3); break;
                    default:
                        btn ->AddHandler(EngineTestFunc1);
                    }
                    if (btnStruct.Enable > 0 )
                        btn->setEnable();
                    else
                        btn->setDisable();
                    btn->setText("Edit");
                    addButton(btn);

                    loginfo(".........Done", "Engine::loadButtons");
                    logEmptyLine();
                }

            }

        }


    }
    else
        logwarn("Fehler: Datei | config/ButtonList nicht gefunden ! |");


    logEmptyLine() ;
    loginfo("----------------------------");
    loginfo(" Load Button Objects ");
    loginfo("        READY");
    loginfo("----------------------------");

}

void CEngine::getStringPart(std::string &s,std::string key, std::size_t &p, std::string & part){

    //std::string temp = s;
    p = s.find(key,0);
    if ( p != s.npos  ) {
        char buf[50];
        s.copy(buf,p );

        s.erase(0,p);
        s.erase(0,1);

        part = (std::string) buf;

    }
    else
        part = s;
}

std::string &CEngine::getValueItem(std::string &s, std::string erasestring) {
    return s.erase(0,erasestring.length() ) ;
}

void CEngine::stringPart(std::string &s, std::string key,  std::vector<std::string> & parts) { // Achtung which beginnt bei 1 !!!
    // --------------------------------------------------------------------
    // Argument which = 1. 2. oder n. teilstreing im String..
    // key = trennszeichen zwschen den string part,
    // bsp.: 'Heute ist nebel': Der 3. TeilString "nebel" nach dem 2. "space"
    //----------------------------------------------------------------------

    // erstmal den string zerlegen
    std::size_t pos;
    int i = 0;
    std::string part;

    std::string partstring = "";
    getStringPart(s,key, pos,partstring);

    while (  pos != s.npos && i < 20) {

        //parts.push_back(partstring);
        parts.push_back( s) ;

        getStringPart(s,key, pos, partstring);

        i++;
    }

    parts.push_back( s) ;
}

bool CEngine::init3DStruct(s3DStruct &d3s, std::vector<std::string> &cfg){
    if (cfg.size() >= CFG_3D_SIZE ) {

        //+---------------------------------------------------------------------+
        //+     VORGEHEN :                                                      |
        //+     Liste abarbeiten, Teilstring bis " " ermitteln,                 |
        //+     Variablen name = Teilstring --> 2. Teilstring in Wert wandeln   |
        //+     und in der s3DStruct zuweisen                                   |
        //+---------------------------------------------------------------------+

        for (int i=0; i < cfg.size(); i++ ) {

            std::string s = cfg.at(i);
            std::vector<std::string> partlist;
            std::string  part;
            stringPart(s,SPACE, partlist); // Config zeile, schlüssel = " " , liste mit teilstrings )

            if ( ! partlist.empty() )     {
                part = partlist.at(0);

            }

            else
                part = "";



            //-----------------------
            // Origin
            //-----------------------
            if (part == "originX" )
               d3s.origin.x    = StringToFloat(partlist[1]);// Value


            if (part == "originY" )
               d3s.origin.y    = StringToFloat(partlist[1]);// Value

            if (part == "originZ" )
               d3s.origin.z    = StringToFloat(partlist[1]);// Value

            // ----------------------
            // color
            // ----------------------
            if (part == "colorRed" )
               d3s.color.x    = StringToFloat(partlist[1]);// Value

            if (part == "colorGreen" )
               d3s.color.x    = StringToFloat(partlist[1]);// Value

            if (part == "colorBlue" )
               d3s.color.z    = StringToFloat(partlist[1]);// Value

            if (part == "colorAlpha" )
               d3s.color.w    = StringToFloat(partlist[1]);// Value

            //------------------------
            // haslight
            // -----------------------

            logimage("part " + part);
            if (part == "hasLight" )
               d3s.hasLight    = StringToInt(partlist[1]);// Value


            // -----------------------
            // texture(s)
            //------------------------
            if (part == "textures" ){
                //datei name auslesen:
                if (partlist.size() > 2)
                    d3s.textures = partlist[1];
                else
                    d3s.textures = "";
            }

            //------------------------
            //translate
            //------------------------

            if (part == "translateX")
                d3s.trans.translate.x = StringToFloat(partlist[1]);

            if (part == "translateY")
                d3s.trans.translate.y = StringToFloat(partlist[1]);

            if (part == "translateZ")
                d3s.trans.translate.z = StringToFloat(partlist[1]);


            if (part == "rotateX")
                d3s.trans.rotate.x = StringToFloat(partlist[1]);

            if (part == "rotateY")
                d3s.trans.rotate.y = StringToFloat(partlist[1]);

            if (part == "rotateZ")
                d3s.trans.rotate.z = StringToFloat(partlist[1]);


            if (part == "scaleX")
                d3s.trans.scale.x = StringToFloat(partlist[1]);

            if (part == "scaleY")
                d3s.trans.scale.y = StringToFloat(partlist[1]);

            if (part == "scaleZ")
                d3s.trans.scale.z = StringToFloat(partlist[1]);

            //-------------------------
            //   firstTranslate
            //-------------------------
            if (part == "firstTranslate")
                 d3s.firstTranslate = StringToInt(partlist[1]);
        }
        return true;

    }
    return false;
}

bool CEngine::initButtonStruct(sButtonStruct &bs, std::vector<std::string> cfg) {

    if (cfg.size() >= CFG_BUTTON_SIZE ) {
        bs.path = getValueItem(cfg[0],"Image: ");
        loginfo("Image Path: "+ bs.path);

        bs.PosX = StringToFloat(getValueItem(cfg[1],"PositionX: "));

        bs.PosY = StringToFloat(getValueItem(cfg[2],"PositionY: "));

        bs.SizeX =  StringToFloat(getValueItem(cfg[3],"SizeX: "));

        bs.SizeY =  StringToFloat(getValueItem(cfg[4],"SizeY: "));

        bs.Enable = StringToInt(getValueItem(cfg[5],"Enable: "));

        bs.ImageRed = StringToFloat(getValueItem(cfg[6],"BGRed: "));

        bs.ImageGreen = StringToFloat(getValueItem(cfg[7],"BGGreen: "));

        bs.ImageBlue = StringToFloat(getValueItem(cfg[8],"BGBlue: "));

        bs.TextRed = StringToFloat(getValueItem(cfg[9],"TextColorRed: "));

        bs.TextGreen = StringToFloat(getValueItem(cfg[10],"TextColorGreen: "));

        bs.TextBlue = StringToFloat(getValueItem(cfg[11],"TextColorBlue: "));

        return true;
    }
    else
        return false;
}





