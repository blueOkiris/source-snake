#include <iostream>
#include <any>
#include <memory>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <GameLoop.hpp>
#include <ResourceManager.hpp>
#include <InputManager.hpp>
#include <Game.hpp>
#include <RoomManager.hpp>
#include <UiRenderer.hpp>
#include <Snake.hpp>

using namespace snake;

int main() {
    // Create main window and global game vars
    link::GameLoop loop(
        "Test", sf::Vector2u(640, 480), sf::Vector2u(300, 300), 60
    );
    loop.globalGameState.emplace("state", GameState::Start);
    
    // Handle utilities
    link::ResourceManager resMan;
    link::InputManager inpMan;
    try {
        resMan.loadFont("font", "fonts/Ubuntu-R.ttf");
        resMan.loadSound("die", "audio/die.wav");
        resMan.loadSound("eat", "audio/eat.wav");
        resMan.loadTexture("fruit", "img/fruit.png");
        resMan.loadTexture("snake-head", "img/snake-head.png");
        resMan.loadTexture("snake-body", "img/snake-body.png");
        resMan.loadTexture("wall", "img/wall.png");
        resMan.loadTexture("grass", "img/grass.png");
    } catch(const link::ResourceFailureException &rfe) {
        std::cout << rfe.what() << std::endl;
        return 1;
    }
    inpMan.add("quit", sf::Keyboard::Escape, loop);
    inpMan.add("start", sf::Keyboard::Space, loop);
    inpMan.add("up", sf::Keyboard::Up, loop);
    inpMan.add("down", sf::Keyboard::Down, loop);
    inpMan.add("left", sf::Keyboard::Left, loop);
    inpMan.add("right", sf::Keyboard::Right, loop);
    
    // Create game objects
    auto snake = std::make_shared<Snake>(resMan, inpMan);
    auto scoreManager = std::make_shared<ScoreManager>(resMan);
    auto roomManager = std::make_shared<RoomManager>(
        snake, scoreManager, resMan, inpMan, loop
    );
    auto uiRenderer = std::make_shared<UiRenderer>(resMan);
    
    // Attach updatables in order
    loop.attach(std::dynamic_pointer_cast<link::Updatable>(snake));
    loop.attach(std::dynamic_pointer_cast<link::Updatable>(roomManager));
    loop.attach(std::dynamic_pointer_cast<link::Updatable>(uiRenderer));
    loop.attach(std::dynamic_pointer_cast<link::Updatable>(scoreManager));
    
    // Attach drawables in order
    loop.attach(std::dynamic_pointer_cast<sf::Drawable>(roomManager));
    loop.attach(std::dynamic_pointer_cast<sf::Drawable>(snake));
    loop.attach(std::dynamic_pointer_cast<sf::Drawable>(uiRenderer));
    loop.attach(std::dynamic_pointer_cast<sf::Drawable>(scoreManager));
    
    // Attach triggerables in order
    loop.attach(std::dynamic_pointer_cast<link::Triggerable>(roomManager));
    
    try {
        loop.run();
    } catch(const std::out_of_range &oor) {
        std::cout
            << "Out of range exception!" << std::endl
            << "Error in " << oor.what() << std::endl;
        return 2;
    }
    
    return 0;
}
