#include "button.h"
#include "../logs/logs.h"

CButton::CButton(int resx, int resy) :
    Base2D(resx,resy) {

    init();
}
CButton::CButton(int resx, int resy, std::string path) :
    Base2D(resx, resy,path){
    init();
}

void CButton::init() {

     _ButtonColors = colorscheme.setScheme(DARK);

    setWidth(DEFAULT_BUTTON_WIDTH);
    setHeight(DEFAULT_BUZTTON_HEIGHT);
    textPos.x = _Pos.x + X_MARGIN;
    textPos.y = _Pos.y + Y_MARGIN;
    _Text = "NO";
    _AlphaText    = ALPHA_TEXT_ENABLED;
    _Alpha_Image  = ALPHA_IMAGE_ENABLED;

    _ImageCol = BTN_COLOR_DEFAULT_IMAGE;
    _TextCol  = BTN_COLOR_DEFAULT_TEXT;
}
CButton::~CButton() {

}

// ----------------------------------------------
// Look and feel
// ----------------------------------------------

void CButton::setText(std::string t) { _Text = t; }

void CButton::setEnable() {
    _Enabled = true;
    _AlphaText   = ALPHA_TEXT_ENABLED;
    _Alpha_Image = ALPHA_IMAGE_ENABLED;
}

void CButton::setDisable() {
    _Enabled = false;
    _AlphaText    = ALPHA_TEXT_DISABLED;
    _Alpha_Image  = ALPHA_IMAGE_DISABLED;
}

std::string CButton::Text() {return  _Text; }
bool CButton::Enabled() { return _Enabled; }


// Handler Func
void CButton::AddHandler(FP handler) {
    clickFunc = handler;
}

void CButton::setbuttonColors(glm::vec3 imagecol, glm::vec3 textcol) {
    _ImageCol = imagecol;
    _TextCol  = textcol;

    Base2D::setColor(glm::vec4(imagecol,_Alpha_Image));
}
// -----------------------------------------------
// Event Handling
// -----------------------------------------------
void CButton::OnClick() {
    clickFunc();
}  // Must be overwritten

void CButton::OnFocus() {
    loginfo("OnFocus","CButton::OnFocus");
}

void CButton::Render( ) {

    Base2D::Render();

}


//--------------------------------------------------
// Defaullt Buttons for use
// -------------------------------------------------

CDefaultButton::CDefaultButton( int resx, int  resy) :
    CButton(resx, resy)

{


    textPos.x = _Pos.x + X_MARGIN;
    textPos.y = _Pos.y + Y_MARGIN;
    // init stuff
    textImage = new Base2D(resx, resy, "images/Add.png");
    textImage->setPos(0,0);

}

CDefaultButton::CDefaultButton(int resx, int resy, std::string path):
    CButton(resx, resy, path){

    textImage = new Base2D(resx, resy, "images/Add.png");
    textImage->setColor(glm::vec4(BTN_COLOR_DEFAULT_TEXT,_AlphaText));
    textImage->setPos(0,0);
    // init stuff
}

void CDefaultButton::setbuttonColors(glm::vec3 imagecol, glm::vec3 textcol)  {

    CButton::setbuttonColors(imagecol, textcol);
    textImage->setColor(glm::vec4(textcol,_AlphaText));
}


// overritten

void CDefaultButton::setPos(int x, int y) {
    Base2D::setPos(x,y);

    textPos.x = _Pos.x + X_MARGIN;
    textPos.y = _Pos.y + Y_MARGIN;

    textImage->setPos(_Pos.x,_Pos.y);
}
void CDefaultButton::setSize(int w, int h) {
    Base2D::setSize(w, h);
    if (textImage != nullptr) {
        textImage->setSize(w, h);
    }
}

void CDefaultButton::Render() {


    Base2D::setColor(glm::vec4(1.0,1.0,1.0,_Alpha_Image));
    Base2D::Render();

    textImage->Base2D::setColor(glm::vec4(1.0,1.0,1.0,_AlphaText));
    textImage->Base2D::Render();
}

void CDefaultButton::OnClick() {
    clickFunc();
}
