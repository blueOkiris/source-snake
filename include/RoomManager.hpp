#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
#include <GameLoop.hpp>
#include <ResourceManager.hpp>
#include <InputManager.hpp>
#include <ScoreManager.hpp>
#include <Snake.hpp>

namespace snake {
    class RoomManager :
            public link::Updatable, public link::Triggerable,
            public sf::Drawable {
        private:
            const link::ResourceManager _resMan;
            const link::InputManager _inpMan;
            std::shared_ptr<Snake> _player;
            std::shared_ptr<ScoreManager> _scoreManager;
            std::shared_ptr<Fruit> _fruit;
            
            bool _resetToStart;
            sf::Vector2u _winSize;
            GameState _state;
        
        public:
            RoomManager(
                const std::shared_ptr<Snake> &player,
                const std::shared_ptr<ScoreManager> &scoreManager,
                const link::ResourceManager &resMan,
                const link::InputManager &inpMan,
                link::GameLoop &loop
            );
            void update(const double delta, link::GameLoop &loop);
            void trigger(const sf::Event &event, link::GameLoop &loop) override;
            void draw(
                sf::RenderTarget &target, sf::RenderStates states
            ) const override;
    };
}
