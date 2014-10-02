
//
// Disclamer:
// ----------
//
// This code will work only if you selected window, graphics and audio.
//
// Note that the "Run Script" build phase will copy the required frameworks
// or dylibs to your application bundle so you can execute it on any OS X
// computer.
//
// Your resource files (images, sounds, fonts, ...) are also copied to your
// application bundle. To get the path to these resource, use the helper
// method resourcePath() from ResourcePath.hpp
//


// Here is a small helper for you ! Have a look.
#include "ResourcePath.hpp"
#include "MainGame.h"
#include "MainGame_SDL.h"
int main(int, char const**)
{
    MainGame_SDL game;
}
