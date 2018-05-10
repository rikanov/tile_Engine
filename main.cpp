
#include "engine.h"

#include <iostream>
using namespace std;

Engine * test = nullptr;

void exit()
{
    cout << "Engine: Finishing SDL..." << endl;
    std::cout << "IMG Quit... " << std::endl;
    IMG_Quit();
    std::cout << "Done. " << std::endl;
    std::cout << "SDL Quit... " << std::endl;
    SDL_Quit();
    std::cout << "Done. " << std::endl;
    delete test;
    std::cout<<"Finish program..."<<std::endl;
}

int main(int argc, char *argv[])
{
    
    atexit(exit); 
    test = new Engine(Ally::OWN, new View2D("First board", 1920, 1080));
    test->loop();
    return EXIT_SUCCESS;
}
