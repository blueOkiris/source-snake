#include <memory>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <GameLoop.hpp>
#include <InputManager.hpp>
#include <ResourceManager.hpp>
#include <Snake.hpp>
#include <Game.hpp>
#include <RoomManager.hpp>

using namespace snake;

RoomManager::RoomManager(
        const std::shared_ptr<Snake> &player,
        const std::shared_ptr<ScoreManager> &scoreManager,
        const link::ResourceManager &resMan, const link::InputManager &inpMan,
        link::GameLoop &loop) :
        _resMan(resMan), _inpMan(inpMan),
        _player(player), _scoreManager(scoreManager), _fruit(nullptr) {
    auto fruitSize = _resMan.texture("fruit").getSize();
    auto wallSize = _resMan.texture("wall").getSize();
    auto winSize = loop.getSize();
    _fruit = std::make_shared<Fruit>(
        sf::Vector2f(
            fruitSize.x + wallSize.x, fruitSize.y + wallSize.y
        ), sf::Vector2f(
            winSize.x - wallSize.x - fruitSize.x,
            winSize.y - wallSize.y - fruitSize.y
        ), _resMan
    );
    loop.attach(std::dynamic_pointer_cast<link::Updatable>(_fruit));
    // draw it ourself!
    // loop.attach(std::dynamic_pointer_cast<sf::Drawable>(_fruit));
    
    _resetToStart = false;
    _winSize = winSize;
}

void RoomManager::update(const double delta, link::GameLoop &loop) {
    if(_inpMan.justReleased("quit")) {
        loop.close();
    }
    if(_resetToStart) {
        loop.globalGameState["state"] = GameState::Start;
    }
    
    auto state = std::any_cast<GameState>(loop.globalGameState["state"]);
    _state = state;
    switch(state) {
        case GameState::Start:
            if(_inpMan.justReleased("start")) {
                loop.globalGameState["state"] = GameState::Playing;
                auto snakeSize = _resMan.texture("snake-head").getSize();
                auto wallTex = _resMan.texture("wall");
                auto snakePos = sf::Vector2f(
                    _winSize.x / 2 + snakeSize.x, _winSize.y / 2
                );
                auto wallSize = wallTex.getSize();
                snakePos.x = (snakePos.x / wallSize.x) * wallSize.x;
                snakePos.y = (snakePos.y / wallSize.y) * wallSize.y;
                snakePos.x -= snakeSize.x / 2;
                _player->setPosition(snakePos);
                _player->resetBody();
            }
            _resetToStart = false;
            break;
        
        case GameState::Playing:
            if(_player->ateFruit(_fruit)) {
                _fruit->newPos();
                _scoreManager->addToScore(1);
            }
            
            if(_player->hasDied(_winSize)) {
                loop.globalGameState["state"] = GameState::Dead;
                _scoreManager->setHighScore();
            }
            
            break;
        
        case GameState::Dead:
            if(_inpMan.justReleased("start")) {
                loop.globalGameState["state"] = GameState::Start;
                _scoreManager->resetScore();
            }
            break;
    }
}

void RoomManager::trigger(const sf::Event &event, link::GameLoop &loop) {
    if(event.type == sf::Event::Resized) {
        auto fruitSize = _resMan.texture("fruit").getSize();
        auto wallSize = _resMan.texture("wall").getSize();
        auto winSize = loop.getSize();
        _winSize = winSize;
        _fruit->newBounds(
            sf::Vector2f(
                fruitSize.x + wallSize.x, fruitSize.y + wallSize.y
            ), sf::Vector2f(
                winSize.x - wallSize.x - fruitSize.x,
                winSize.y - wallSize.y - fruitSize.y
            )
        );
        
        _resetToStart = true;
    }
}

void RoomManager::draw(
        sf::RenderTarget &target, sf::RenderStates states) const {
    switch(_state) {
        case GameState::Start:
            break;
        
        case GameState::Playing: {
            auto grassTex = _resMan.texture("grass");
            auto grassSize = grassTex.getSize();
            for(size_t y = 0; y < _winSize.y / grassSize.y; y++) {
                for(size_t x = 0; x < _winSize.x / grassSize.x; x++) {
                    sf::Sprite grassSpr(grassTex);
                    grassSpr.setPosition(sf::Vector2f(
                        x * grassSize.x, y * grassSize.y
                    ));
                    target.draw(grassSpr);
                }
            }
            
            auto wallTex = _resMan.texture("wall");
            auto wallSize = wallTex.getSize();
            for(size_t y = 0; y < _winSize.y / wallSize.y; y++) {
                for(size_t x = 0; x < _winSize.x / wallSize.x; x++) {
                    if(x == 0 || x == (_winSize.x / wallSize.x) - 1
                            || y == 0 || y == (_winSize.y / wallSize.y) - 1) {
                        sf::Sprite wallSpr(wallTex);
                        wallSpr.setPosition(sf::Vector2f(
                            x * wallSize.x, y * wallSize.y
                        ));
                        target.draw(wallSpr);
                    }
                }
            }
            
            target.draw(*_fruit);
        } break;
        
        case GameState::Dead:
            break;
    }
}