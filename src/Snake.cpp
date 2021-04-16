#include <vector>
#include <utility>
#include <iostream>
#include <map>
#include <memory>
#include <any>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <GameLoop.hpp>
#include <InputManager.hpp>
#include <ResourceManager.hpp>
#include <Game.hpp>
#include <Snake.hpp>

using namespace snake;

void SnakeStart::update(
        Snake &parent,
        const double delta, const link::GameLoop &loop,
        const link::ResourceManager &resMan,
        const link::InputManager &inpMan,
        const double moveDelay, double &moveTimer,
        SnakeDirection &nextDir, SnakeDirection &dir,
        std::vector<BodyInfo> &bodyInfos, sf::Vector2f &headPos) const {
    auto winSize = loop.getSize();
    auto headSize = resMan.texture("snake-head").getSize();
    parent.setPosition(sf::Vector2f(
        winSize.x / 2 + headSize.x, 3 * (winSize.y / 4)
    ));
    parent.resetBody();
}

void SnakeStart::draw(
        sf::RenderTarget &target, sf::RenderStates states) const {
    
}

void SnakePlaying::update(
        Snake &parent,
        const double delta, const link::GameLoop &loop,
        const link::ResourceManager &resMan,
        const link::InputManager &inpMan,
        const double moveDelay, double &moveTimer,
        SnakeDirection &nextDir, SnakeDirection &dir,
        std::vector<BodyInfo> &bodyInfos, sf::Vector2f &headPos) const {
    // Move if necessary
    moveTimer += delta;
    if(moveTimer >= moveDelay) {
        moveTimer = 0;
        _move(resMan, dir, nextDir, bodyInfos, headPos);
    }
    
    // Change direction on key press (but update when move triggers)
    nextDir = inpMan.held("up") ?
        SnakeDirection::Up :
        inpMan.held("down") ?
            SnakeDirection::Down :
            inpMan.held("left") ?
                SnakeDirection::Left :
                inpMan.held("right") ?
                    SnakeDirection::Right :
                    nextDir;
}

void SnakePlaying::_move(
        const link::ResourceManager &resMan,
        SnakeDirection &dir, SnakeDirection &nextDir,
        std::vector<BodyInfo> &bodyInfos, sf::Vector2f &headPos) const {
    // Update body parts
    for(size_t i = bodyInfos.size() - 1; i > 0; i--) {
        bodyInfos[i].first = bodyInfos[i - 1].first;
        bodyInfos[i].second = bodyInfos[i - 1].second;
    }
    bodyInfos[0].first = headPos;
    bodyInfos[0].second = dir;
    
    // Move the head to the new direction
    dir = nextDir;
    auto headSize = sf::Vector2i(resMan.texture("snake-head").getSize());
    headPos.x +=
        dir == SnakeDirection::Right ?
            headSize.x :
            (dir == SnakeDirection::Left ? -headSize.x : 0);
    headPos.y +=
        dir == SnakeDirection::Down ?
            headSize.y :
            (dir == SnakeDirection::Up ? -headSize.y : 0);
}

void SnakePlaying::draw(
        sf::RenderTarget &target, sf::RenderStates states) const {
    
}

void SnakeDead::update(
        Snake &parent,
        const double delta, const link::GameLoop &loop,
        const link::ResourceManager &resMan,
        const link::InputManager &inpMan,
        const double moveDelay, double &moveTimer,
        SnakeDirection &nextDir, SnakeDirection &dir,
        std::vector<BodyInfo> &bodyInfos, sf::Vector2f &headPos) const {
}

void SnakeDead::draw(
        sf::RenderTarget &target, sf::RenderStates states) const {
}

Snake::Snake(
        const link::ResourceManager &resMan, const link::InputManager &inpMan) :
        _moveDelayDec(0.005), _defMoveDelay(0.2),
        _minParts(3), _resMan(resMan), _inpMan(inpMan),
        _headPos(sf::Vector2f(0, 0)), _bodyInfos(std::vector<BodyInfo>()),
        _dir(SnakeDirection::Right), _nextDir(SnakeDirection::Right),
        _moveTimer(0), _moveDelay(_defMoveDelay),
        _eatBuffer(resMan.soundBuffer("eat")),
        _deathBuffer(resMan.soundBuffer("die")) {
    _behaviors.emplace(GameState::Start, std::make_shared<SnakeStart>());
    _behaviors.emplace(GameState::Playing, std::make_shared<SnakePlaying>());
    _behaviors.emplace(GameState::Dead, std::make_shared<SnakeDead>());
    
    _eatSound.setBuffer(_eatBuffer);
    _deathSound.setBuffer(_deathBuffer);
    
    for(size_t i = 0; i < _minParts - 1; i++) {
        _bodyInfos.push_back(
            std::make_pair(sf::Vector2f(0, 0), SnakeDirection::Right)
        );
    }
}

void Snake::update(const double delta, link::GameLoop &loop) {
    auto state = std::any_cast<GameState>(loop.globalGameState["state"]);
    _behaviors[state]->update(
        *this, delta, loop, _resMan, _inpMan,
        _moveDelay, _moveTimer, _nextDir, _dir, _bodyInfos, _headPos
    );
}

bool Snake::hasDied(const sf::Vector2u winSize) {
    // Check if the head is colliding with the body
    auto headTex = _resMan.texture("snake-head");
    auto headSize = headTex.getSize();
    sf::FloatRect rect(
        sf::Vector2f(_headPos.x - headSize.x / 2, _headPos.y - headSize.y / 2),
        sf::Vector2f(headSize)
    );
    auto bodyTex = _resMan.texture("snake-body");
    auto bodySize = bodyTex.getSize();
    for(const auto &bodyInfo : _bodyInfos) {
        sf::FloatRect infoRect(
            sf::Vector2f(
                bodyInfo.first.x - (bodySize.x * 0.75) / 2,
                bodyInfo.first.y - (bodySize.y * 0.75) / 2
            ), sf::Vector2f(bodySize.x * 0.75, bodySize.y * 0.75)
        );
        
        if(rect.intersects(infoRect)) {
            _deathSound.play();
            return true;
        }
    }
    
    // Check if the head has hit a wall
    auto wallTexSize = _resMan.texture("wall").getSize();
    if(_headPos.x - headSize.x / 2 < wallTexSize.x
            || _headPos.y - headSize.y / 2 < wallTexSize.y
            || _headPos.x + headSize.x / 2 > winSize.x - wallTexSize.x
            || _headPos.y + headSize.y / 2 > winSize.y - wallTexSize.y) {
        _deathSound.play();
        return true;
    }
    
    return false;
}

bool Snake::ateFruit(const std::shared_ptr<Fruit> &fruit) {
    auto headTexSize = _resMan.texture("snake-head").getSize();
    auto fruitSize = _resMan.texture("fruit").getSize();
    sf::FloatRect headRect(
        sf::Vector2f(
            _headPos.x - headTexSize.x / 2, _headPos.y - headTexSize.y / 2
        ), sf::Vector2f(headTexSize)
    );
    sf::FloatRect fruitRect(
        sf::Vector2f(
            fruit->pos().x - fruitSize.x / 2,
            fruit->pos().y - fruitSize.y / 2
        ), sf::Vector2f(fruitSize)
    );
    
    if(headRect.intersects(fruitRect)) {
        _bodyInfos.push_back(std::make_pair(
            _bodyInfos[_bodyInfos.size() - 1].first,
            _bodyInfos[_bodyInfos.size() - 1].second
        ));
        _moveDelay -= _moveDelayDec;
        if(_moveDelay < 0) {
            _moveDelay = 0;
        }
        
        _eatSound.play();
        
        return true;
    }
    
    return false;
}

void Snake::setPosition(const sf::Vector2f pos) {
    _headPos = pos;
}

void Snake::resetBody(void) {
    auto headTex = _resMan.texture("snake-head");
    auto bodyTex = _resMan.texture("snake-body");
    
    _dir = _nextDir = SnakeDirection::Right;
    while(_bodyInfos.size() > _minParts - 1) {
        _bodyInfos.pop_back();
    }
    _bodyInfos[0] = std::make_pair(
        sf::Vector2f(_headPos.x - headTex.getSize().x, _headPos.y),
        SnakeDirection::Right
    );
    _bodyInfos[1] = std::make_pair(
        sf::Vector2f(
            _headPos.x - headTex.getSize().x - bodyTex.getSize().x, _headPos.y
        ), SnakeDirection::Right
    );
    
    _moveDelay = _defMoveDelay;
}

void Snake::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    // Draw head texture as a sprite w/ center at _headPos
    auto headTex = _resMan.texture("snake-head");
    sf::Sprite headDrawable(headTex);
    auto headSize = headTex.getSize();
    headDrawable.setOrigin(sf::Vector2f(headSize.x / 2, headSize.y / 2));
    headDrawable.setPosition(_headPos);
    switch(_dir) {
        case SnakeDirection::Up:
            break;
        case SnakeDirection::Right:
            headDrawable.rotate(90);
            break;
        case SnakeDirection::Down:
            headDrawable.rotate(180);
            break;
        case SnakeDirection::Left:
            headDrawable.rotate(270);
            break;
    }
    target.draw(headDrawable, states);
    
    // Draw the body parts that follow
    auto bodyTex = _resMan.texture("snake-body");
    BodyInfo lastBody = std::make_pair(_headPos, _dir);
    for(const auto &bodyInfo : _bodyInfos) {
        sf::Sprite bodyDrawable(bodyTex);
        auto bodySize = bodyTex.getSize();
        if(bodyInfo != lastBody
                && (
                    (
                        bodyInfo.second == SnakeDirection::Up
                        && lastBody.second == SnakeDirection::Right
                    ) || (
                        bodyInfo.second == SnakeDirection::Left
                        && lastBody.second == SnakeDirection::Down
                    ) || (
                        bodyInfo.second == SnakeDirection::Right
                        && lastBody.second == SnakeDirection::Up
                    ) || (
                        bodyInfo.second == SnakeDirection::Down
                        && lastBody.second == SnakeDirection::Left
                    )
                )) {
            bodyDrawable.rotate(45);
        } else if(bodyInfo != lastBody
                && (
                    (
                        bodyInfo.second == SnakeDirection::Down
                        && lastBody.second == SnakeDirection::Right
                    ) || (
                        bodyInfo.second == SnakeDirection::Up
                        && lastBody.second == SnakeDirection::Left
                    ) || (
                        bodyInfo.second == SnakeDirection::Right
                        && lastBody.second == SnakeDirection::Down
                    ) || (
                        bodyInfo.second == SnakeDirection::Left
                        && lastBody.second == SnakeDirection::Up
                    ) 
                )) {
            bodyDrawable.rotate(-45);
        } else {
            switch(bodyInfo.second) {
                case SnakeDirection::Up:
                    break;
                case SnakeDirection::Right:
                    bodyDrawable.rotate(90);
                    break;
                case SnakeDirection::Down:
                    bodyDrawable.rotate(180);
                    break;
                case SnakeDirection::Left:
                    bodyDrawable.rotate(270);
                    break;
            }
        }
        bodyDrawable.setOrigin(sf::Vector2f(bodySize.x / 2, bodySize.y / 2));
        bodyDrawable.setPosition(bodyInfo.first);
        target.draw(bodyDrawable, states);
        lastBody = bodyInfo;
    }
}
