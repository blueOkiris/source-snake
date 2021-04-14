#pragma once

#include <SFML/Graphics.hpp>
#include <ResourceManager.hpp>
#include <GameLoop.hpp>
#include <Game.hpp>

namespace snake {
    class UiRenderer : public link::Updatable, public sf::Drawable {
        private:
            GameState _state;
            sf::Vector2u _winSize;
            sf::Font _font;
        
        public:
            UiRenderer(const link::ResourceManager &resMan);
            
            void update(const double delta, link::GameLoop &loop) override;
            void draw(
                sf::RenderTarget &target, sf::RenderStates states
            ) const override;
    };
}
