
#include "engine.h"

#include <iostream>
using namespace std;

View2D * teszt = nullptr;

void exit()
{
    delete teszt;
    cout << "Engine: Finishing SDL..." << endl;
    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char *argv[])
{
    
    atexit(exit); 
    Engine test(Ally::OWN, new View2D("First board", 1920, 1080));
    test.loop();
    return EXIT_SUCCESS;
}
