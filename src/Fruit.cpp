#include <random>
#include <functional>
#include <any>
#include <Game.hpp>
#include <SFML/Graphics.hpp>
#include <GameLoop.hpp>
#include <ResourceManager.hpp>
#include <Fruit.hpp>

using namespace snake;

Fruit::Fruit(
        const sf::Vector2f minPos, const sf::Vector2f maxPos,
        const link::ResourceManager &resMan) :
        _resMan(resMan),
        _minPos(minPos), _maxPos(maxPos), _playing(false) {
    newPos();
}

sf::Vector2f Fruit::pos() const {
    return _pos;
}

void Fruit::newBounds(const sf::Vector2f minPos, const sf::Vector2f maxPos) {
    _minPos = minPos;
    _maxPos = maxPos;
}

void Fruit::newPos(void) {
    _pos.x = static_cast<int>(
        _resMan.random() * (_maxPos.x - _minPos.x) + _minPos.x
    );
    _pos.y = static_cast<int>(
        _resMan.random() * (_maxPos.y - _minPos.y) + _minPos.y
    );
}

void Fruit::update(const double delta, link::GameLoop &loop) {
    auto state = std::any_cast<GameState>(loop.globalGameState["state"]);
    _playing = state == GameState::Playing;
}

void Fruit::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    if(!_playing) {
        return;
    }
    
    auto fruitTex = _resMan.texture("fruit");
    sf::Sprite fruitDrawable(fruitTex);
    auto fruitSize = fruitTex.getSize();
    fruitDrawable.setOrigin(sf::Vector2f(fruitSize.x / 2, fruitSize.y / 2));
    fruitDrawable.setPosition(_pos);
    target.draw(fruitDrawable, states);
}
