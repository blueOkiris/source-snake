#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <Game.hpp>
#include <Fruit.hpp>
#include <Snake.hpp>

using namespace sourcesnake;

Snake::Snake() :
        _moveDelayDec(0.005), _minParts(3),
        _headPos(sf::Vector2f(0, 0)), _bodyInfos(std::vector<BodyInfo>()),
        _dir(SnakeDirection::Right), _nextDir(SnakeDirection::Right),
        _moveTimer(0), _moveDelay(0.2),
        headTex(sourcesnake::loadTexture("img/snake-head.png")),
        bodyTex(sourcesnake::loadTexture("img/snake-body.png")) {
    for(size_t i = 0; i < _minParts - 1; i++) {
        _bodyInfos.push_back(
            std::make_pair(sf::Vector2f(0, 0), SnakeDirection::Right)
        );
    }
}

std::pair<bool, bool> Snake::update(
        const double delta,
        const bool upPressed, const bool downPressed,
        const bool leftPressed, const bool rightPressed,
        const Fruit *fruit,
        const sf::Vector2u winSize, const sf::Vector2u wallTexSize) {
    // Move if necessary
    _moveTimer += delta;
    if(_moveTimer >= _moveDelay) {
        _moveTimer = 0;
        _move();
    }
    
    // Change direction on key press (but update when move triggers)
    _nextDir = upPressed ?
        SnakeDirection::Up :
        downPressed ?
            SnakeDirection::Down :
            leftPressed ?
                SnakeDirection::Left :
                rightPressed ?
                    SnakeDirection::Right :
                    _nextDir;
    
    // Check if fruit collision -> "eat" fruit
    return std::make_pair(_ateFruit(fruit), _hasDied(winSize, wallTexSize));
}

bool Snake::_ateFruit(const Fruit *fruit) {
    auto headSize = headTex.getSize();
    sf::FloatRect rect(
        sf::Vector2f(_headPos.x - headSize.x / 2, _headPos.y - headSize.y / 2),
        sf::Vector2f(headSize)
    );
    auto fruitSize = Fruit::fruitTex.getSize();
    sf::FloatRect fruitRect(
        sf::Vector2f(
            fruit->pos().x - fruitSize.x / 2,
            fruit->pos().y - fruitSize.y / 2
        ), sf::Vector2f(fruitSize)
    );
    if(rect.intersects(fruitRect)) {
        _bodyInfos.push_back(std::make_pair(
            _bodyInfos[_bodyInfos.size() - 1].first,
            _bodyInfos[_bodyInfos.size() - 1].second
        ));
        _moveDelay -= _moveDelayDec;
        if(_moveDelay < 0) {
            _moveDelay = 0;
        }
        return true;
    }
    return false;
}

bool Snake::_hasDied(
        const sf::Vector2u winSize, const sf::Vector2u wallTexSize) const {
    // Check if the head is colliding with the body
    auto headSize = headTex.getSize();
    sf::FloatRect rect(
        sf::Vector2f(_headPos.x - headSize.x / 2, _headPos.y - headSize.y / 2),
        sf::Vector2f(headSize)
    );
    auto bodySize = bodyTex.getSize();
    for(const auto &bodyInfo : _bodyInfos) {
        sf::FloatRect infoRect(
            sf::Vector2f(
                bodyInfo.first.x - (bodySize.x * 0.75) / 2,
                bodyInfo.first.y - (bodySize.y * 0.75) / 2
            ), sf::Vector2f(bodySize.x * 0.75, bodySize.y * 0.75)
        );
        
        if(rect.intersects(infoRect)) {
            return true;
        }
    }
    
    // Check if the head has hit a wall
    if(_headPos.x - headSize.x / 2 < wallTexSize.x
            || _headPos.y - headSize.y / 2 < wallTexSize.y
            || _headPos.x + headSize.x / 2 > winSize.x - wallTexSize.x
            || _headPos.y + headSize.y / 2 > winSize.y - wallTexSize.y) {
        return true;
    }
    
    return false;
}

void Snake::_move(void) {
    // Update body parts
    for(size_t i = _bodyInfos.size() - 1; i > 0; i--) {
        _bodyInfos[i].first = _bodyInfos[i - 1].first;
        _bodyInfos[i].second = _bodyInfos[i - 1].second;
    }
    _bodyInfos[0].first = _headPos;
    _bodyInfos[0].second = _dir;
    
    // Move the head to the new direction
    _dir = _nextDir;
    auto headSize = sf::Vector2i(headTex.getSize());
    _headPos.x +=
        _dir == SnakeDirection::Right ?
            headSize.x :
            (_dir == SnakeDirection::Left ? -headSize.x : 0);
    _headPos.y +=
        _dir == SnakeDirection::Down ?
            headSize.y :
            (_dir == SnakeDirection::Up ? -headSize.y : 0);
}

void Snake::resetBody(void) {
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
}

void Snake::setPosition(const sf::Vector2f pos) {
    _headPos = pos;
}

void Snake::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    // Draw head texture as a sprite w/ center at _headPos
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
