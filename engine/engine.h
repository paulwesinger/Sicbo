#ifndef CENGINE_H
#define CENGINE_H

#include "../initgl.h"
#include "../buttons/button.h"
#include "../textrenderer/textrender.h"
#include "../fileutils/fileutil.h"
#include "../defines.h"
#include <vector>

const std::string BUTTONS_CFG = "config/Buttons/";
const std::string OBJECT3D_CFG =  "config/3DObjects/";
const std::string SPACE = " ";


const int CFG_BUTTON_SIZE = 11;
const int CFG_3D_SIZE = 18;

class CEngine : public InitGL
{
public:
    CEngine(std::string titel = "OpenGL 4.5 Engine");
    void Run() override;
    void Done() override;

    static void EngineTestFunc1();   // Stellvertretend für alle zukünftigen funcs ->zum testen
    static void EngineTestFunc2();
    static void EngineTestFunc3();

protected:
    void loadButtons();

    // Lists for all objects:
    std::vector<std::string> btnList;
    std::vector<std::string> object2DList;
    std::vector<std::string> object3DList;

private:
    void InitButtons();
    void Init2D();
    void Init3D();

    bool initButtonStruct(sButtonStruct & bs,std::vector<std::string> cfg);
    bool init3DStruct(s3DStruct & d3s, std::vector<std::string> &cfg);
    std::string &getValueItem(std::string &s, std::string erasestring);
    void getStringPart(std::string &s,std::string key,std::size_t &p, std::string & part);
    void stringPart(std::string &s, std::string key, std::vector<std::string> & parts);

    TextRender * textrenderer;
    fileUtil * fu;
};

#endif // CENGINE_H
