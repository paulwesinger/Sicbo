
#include "baseobject.h"
/*
 * File:   baseobject.cpp
 * Author: paul
 *
 * Created on 11. August 2018, 16:31
 */
#include "../logs/logs.h"
#include "../utils/utils.h"
#include "../imageloader/loadimage.h"

#include "baseobject.h"

BaseObject::BaseObject() {
    vec3 v = vec3(0.0,0.0,0.0);
    Translate(v);
    Scale(v);
    Rotate(v);
    SetColor(vec4(v,1.0));
    _IsOrtho = false;
    _FirstTranslate = true;
    _HasTextures = false;
}

BaseObject::BaseObject(const BaseObject& orig) {

}

BaseObject::BaseObject(vec3 pos,vec3 rotate,vec3 scale , vec4 col) {
    Translate(pos);
    Rotate(rotate);
    Scale(scale);
    SetColor(col);

    _IsOrtho = false;
    _FirstTranslate = true;
    _HasTextures = false;
}

BaseObject::~BaseObject() {
}

void BaseObject::Draw( Camera * cam, GLuint shaderprog){}

bool BaseObject::addTexture(std::vector<std::string> path, std::string obj) {


    _HasTextures = false;
    for ( uint i = 0; i< path.size(); i++) {

        SDL_Surface * surface = CLoadImage::getSurface(path[i], "BaseObject::Init");
        if ( surface ) {
            int width = surface->w;
            int height = surface->h;
            char * data = static_cast<char*>(surface->pixels);
            _HasTextures = true;
            if ( data ) {

                GLuint tex;
                glGenTextures(1,&tex);
                switch (i) {
                    case 0: glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D,tex); break;
                    case 1: glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D,tex); break;
                    case 2: glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D,tex); break;
                    case 3: glActiveTexture(GL_TEXTURE3); glBindTexture(GL_TEXTURE_2D,tex); break;
                    case 4: glActiveTexture(GL_TEXTURE4); glBindTexture(GL_TEXTURE_2D,tex); break;
                }
                // *******************************************
                // Generate Textures
                // *******************************************
                _Textures[i] = tex;
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                logimage("Texture "+ path[i]+ " geladen. Index = " + IntToString(i),obj);
            }
        }  // if surface
        else
            logwarn("Texture  nicht geladen !"," BaseObject::addTexture");

        if ( ++ _CountTextures > MAX_TEXTURES ) {
            logwarn( "No more Textures available ! MAX_TEXTURES = 5","BaseObject::addTextures");
            break;
        }
    }  // for

    loginfo("addTexture :: _CountTextures: " + IntToString(_CountTextures),"Baseobjec::Init");
    if (_HasTextures)
        return true;
    return  false;
}

void BaseObject::initShader(ShaderType s, GLuint prog) {
    switch (s) {

        case COLOR_SHADER:      perspectiveColorShader = prog; break;
        case TEXTURE_SHADER:    perspectiveTextureShader = prog; break;
        case LIGHT_SHADER:      lightshader = prog; break;
    }
}

void BaseObject::setActiveShader(ShaderType t){

    switch (t) {
        case COLOR_SHADER: currentShader = perspectiveColorShader; break;
        case TEXTURE_SHADER: currentShader = perspectiveTextureShader; break;
        case LIGHT_SHADER: currentShader = lightshader; break;
    }
}
 // Transormatioons

 vec3 BaseObject::GetRotate()       { return _rotate; }
 vec3 BaseObject::GetScale()        { return _scale ; }
 vec3 BaseObject::GetTranslate ()    { return _translate;}

 void BaseObject::Rotate(vec3 rotate)    {  _rotate    = rotate; }
 void BaseObject::Scale(vec3 scale)      {  _scale     = scale;  }
 void BaseObject::Translate(vec3 trans)  {  _translate = trans;  }

 void BaseObject::SetColor( vec4 col)       { _color = col;         }
 vec4 BaseObject::GetColor()                { return _color;        }

 void BaseObject::StepRotate(vec3 step)     { _rotate_step = step; _rotate += step; }
 void BaseObject::StepTranslate(vec3 step)  { _trans_step = step;  _translate += step; }
 void BaseObject::StepScale(vec3 step)      { _scale_step = step; _scale += step;  }

 void BaseObject::SetFirstTranslate(bool ok){ _FirstTranslate = ok;    }
 bool BaseObject::GetFirstTranslate()       { return _FirstTranslate;  }

 void BaseObject::SetProjection(glm::mat4 pro, bool orthomode )  {      _Projection = pro; _IsOrtho = orthomode;  }
 glm::mat4 & BaseObject::GetProjection()    {  return _Projection;      }

 bool BaseObject::HasTextures()              { return  _HasTextures; }
 void BaseObject::SetHasTextures(bool hasATexture) { _HasTextures = hasATexture; }

