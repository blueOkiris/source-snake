#include <iostream>
#include <Game.hpp>
#include <Window.hpp>

using namespace sourcesnake;

int main() {
    try {
        Window::getInstance().run();
    } catch(const ResourceFailureException &rfe) {
        std::cout
            << "Resource failure!" << std::endl << rfe.what() << std::endl;
    }
    return 0;
}
