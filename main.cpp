#include "engine/engine.h"


int main(int argc,char ** argv)
{


    CEngine * init = new CEngine("Engine started");

    if ( ! init ) {
        printf( "Failed to Create Object !!! \n Aborting ......");
        return 1;
    }

    if ( init->InitSDL2()) {

        init->Run();

    }
    else {
        printf(" Init SDL2 failed !! \n aborting ....");
        return 2;
    }
    init->Done();

    delete init;
}
