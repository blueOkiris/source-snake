#include <random>
#include <functional>
#include <Game.hpp>
#include <SFML/Graphics.hpp>
#include <Fruit.hpp>

using namespace sourcesnake;

const sf::Texture Fruit::fruitTex = loadTexture("img/fruit.png");

Fruit::Fruit(
        const sf::Vector2f minPos, const sf::Vector2f maxPos) :
        _xDist(minPos.x, maxPos.x), _yDist(minPos.y, maxPos.y),
        _pos(sf::Vector2f(
            std::bind(_xDist, getRandomGenerator())(),
            std::bind(_yDist, getRandomGenerator())()
        )) {
}

sf::Vector2f Fruit::pos() const {
    return _pos;
}

void Fruit::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    sf::Sprite fruitDrawable(fruitTex);
    auto fruitSize = fruitTex.getSize();
    fruitDrawable.setOrigin(sf::Vector2f(fruitSize.x / 2, fruitSize.y / 2));
    fruitDrawable.setPosition(_pos);
    target.draw(fruitDrawable, states);
}
