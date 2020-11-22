#include "utils.h"
#include <string>


std::string IntToString ( int value) {
    char buf [50];
    // return wert wird nicht gebraucht
    snprintf(buf,50,"%d", value);
    return (std::string(buf));
}

std::string FloatToString ( float value) {
    char buf [50];
    //return wert wird nicht gebraucht
    snprintf(buf,50,"%F", value);
    return (std::string(buf));
}

float StringToFloat( std::string value) {
     std::string::size_type sz;
     return std::stof(value,&sz);
}

int StringToInt(std::string value) {
    std::string::size_type sz;
    return std::stoi(value,&sz);
}
