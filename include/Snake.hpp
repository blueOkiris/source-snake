#pragma once

#include <vector>
#include <utility>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <GameLoop.hpp>
#include <ResourceManager.hpp>
#include <InputManager.hpp>
#include <Fruit.hpp>

namespace snake {
    enum class SnakeDirection {
        Up, Down, Left, Right
    };
    
    typedef std::pair<sf::Vector2f, SnakeDirection> BodyInfo;
    
    class Snake : public sf::Drawable, public link::Updatable {
        private:
            const double _moveDelayDec, _defMoveDelay;
            const size_t _minParts;
            const link::ResourceManager &_resMan;
            const link::InputManager &_inpMan;
            
            sf::Vector2f _headPos;
            std::vector<BodyInfo> _bodyInfos;
            SnakeDirection _dir, _nextDir;
            double _moveTimer, _moveDelay;
            sf::SoundBuffer _eatBuffer, _deathBuffer;
            sf::Sound _eatSound, _deathSound;
            
            void _move(void);
            
        public:
            Snake(
                const link::ResourceManager &resMan,
                const link::InputManager &inpMan
            );
            
            void setPosition(const sf::Vector2f pos);
            void resetBody(void);
            bool ateFruit(const std::shared_ptr<Fruit> &fruit);
            bool hasDied(const sf::Vector2u winSize);
            
            void update(const double delta, link::GameLoop &loop) override;
            void draw(
                sf::RenderTarget &target, sf::RenderStates states
            ) const override;
    };
}
