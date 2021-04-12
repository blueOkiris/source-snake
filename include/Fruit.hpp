#pragma once

#include <random>
#include <SFML/Graphics.hpp>

namespace sourcesnake {
    class Fruit : public sf::Drawable {
        private:
            const std::uniform_int_distribution<int> _xDist, _yDist;
            const sf::Vector2f _pos;
        
        public:
            const static sf::Texture fruitTex;
            
            Fruit(const sf::Vector2f minPos, const sf::Vector2f maxPos);
            sf::Vector2f pos() const;
            void draw(
                sf::RenderTarget &target, sf::RenderStates states
            ) const override;
    };
}
