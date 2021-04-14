#pragma once

#include <random>
#include <SFML/Graphics.hpp>
#include <GameLoop.hpp>

namespace snake {
    class Fruit :
            public sf::Drawable, public link::Updatable {
        private:
            const link::ResourceManager &_resMan;
            sf::Vector2f _minPos, _maxPos;
            sf::Vector2f _pos;
            
            bool _playing;
        
        public:
            Fruit(
                const sf::Vector2f minPos, const sf::Vector2f maxPos,
                const link::ResourceManager &resMan
            );
            sf::Vector2f pos() const;
            
            void newBounds(
                const sf::Vector2f minPos, const sf::Vector2f maxPos
            );
            void newPos(void);
        
            void update(const double delta, link::GameLoop &loop) override;
            void draw(
                sf::RenderTarget &target, sf::RenderStates states
            ) const override;
    };
}
