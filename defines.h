/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   defines.h
 * Author: paul
 *
 * Created on 05. Jänner 2019, 13:04
 */

#ifndef DEFINES_H
#define DEFINES_H

#include <vector>
#include <string>

const int MAX_SCHEMES = 6;   // siehe COLOR_SCHEMES
const std::string NO_FILE  = "NO FILE";


// Structs for Objects to read in
enum ObjectType {
    ZYLINER = 0 ,
    KUGEL       ,
    ECK4        ,
    ECK6        
};


enum COLOR_Sheme {
    DARK = 0,
    RED,
    BLUE,
    GREEN,
    YELLOW,
    SYSTEM
};

typedef void (*FP)();  // Zeiger auf Funktion wird für eventhandling gebraucht

typedef struct {
 int x;
 int y;
 int x1;
 int y1;
} sRect;


typedef struct{
  int x;
  int y;
} sPOINT;

struct sSize {
    int w;
    int h;
} ;

typedef struct _float2{
    float x;
    float y;
} FLOAT2;

typedef struct _float3{
    float x;
    float y;
    float z;
}FLOAT3;

typedef struct _float4{
    float x;
    float y;
    float z;
    float w;
}FLOAT4;

typedef struct {
    float x;
    float y;
    float z;
} VECTOR;

typedef struct _2D{
    long id;
    float x;
    float y;
}VECTOR2D;

typedef struct _3D{
    long id;
    _float3 vector;
    _float3 normale;
}VECTOR3D;

typedef struct _transform {
    _float3 translate;
    _float3 rotate;
    _float3 scale;
}TRANSFORM;

typedef struct _face {
    short p0;
    short p1;
    short p2;
}FACE;

typedef struct{
    float u;
    float v;
}TEXTURECOORDS;

typedef struct _color{
    _float4 color;
}COLOR;

typedef struct _material{
    _float3 ambient;
    _float3 diffuse;
    _float3 specular;
    float shiness;
} MATERIAL;

// Header for global defines 
// Mouse 
typedef struct {
    int lastx,x;
    int lasty,y;
} MOUSE;

enum Events{

    NO_INPUT                = 0,
    MOUSE_Move,
    // Input Key
    KEY_Esc,                 
    KEY_Left,               
    KEY_Right,               
    KEY_Up   ,               
    KEY_Down ,    
    NUM_0,
    NUM_1,
    NUM_2,
    NUM_3,
    NUM_4,
    NUM_5,
    NUM_6,
    NUM_7,
    NUM_8,
    NUM_9,
    // A S D E  für rotate Camera 
    KEY_A,
    KEY_S,
    KEY_D,
    KEY_E,
    //ortho bzw perspective mode:
    KEY_O,
    KEY_P,
    // Wired or Textured
    KEY_W,
    KEY_T,
    KEY_C,

    MOUSE_Move_Right = 100,              
    MOUSE_Move_Left ,         
    MOUSE_Move_Up,      
    MOUSE_Move_Down,
    
    MOUISE_BTN_Left_Clk   = 10000 ,    
    MOUSE_BTN_Right_Clk ,    
    MOUSE_BTN_MiddLe_Clk,    

    MOUSE_Wheel 
};

enum ShaderType {
    COLOR_SHADER = 0,
    TEXTURE_SHADER,
    LIGHT_SHADER
};

// ------------------------------------------------------
// needed for loading 2D and 3D objects from a file
// ------------------------------------------------------
typedef struct {

    bool firstTranslate;
    bool hasLight;
    TRANSFORM trans;
    FLOAT3 origin;
    FLOAT4 color;
    std::string textures;
}s3DStruct;
#endif /* DEFINES_H */

