#pragma once

#include <vector>
#include <utility>
#include <map>
#include <memory>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <GameLoop.hpp>
#include <ResourceManager.hpp>
#include <InputManager.hpp>
#include <Game.hpp>
#include <Fruit.hpp>

namespace snake {
    enum class SnakeDirection {
        Up, Down, Left, Right
    };
    
    typedef std::pair<sf::Vector2f, SnakeDirection> BodyInfo;
    
    // TODO: Cleanup while moving towards this direction
    /*
     * Interface for containing the functionality
     * "Pure functions" that can be swapped out
     */
    class Snake; // Since they depend on eachother
    class SnakeStateBehavior {
        public:
            virtual void update(
                Snake &parent,
                const double delta, link::GameLoop &loop
            ) const = 0;
    };
    
    class SnakeStart : public SnakeStateBehavior {
        public:
            void update(
                Snake &parent, const double delta, link::GameLoop &loop
            ) const override;
    };
    
    class SnakePlaying : public SnakeStateBehavior {
        public:
            void update(
                Snake &parent, const double delta, link::GameLoop &loop
            ) const override;
    };
    
    class SnakeDead : public SnakeStateBehavior {
        public:
            void update(
                Snake &parent, const double delta, link::GameLoop &loop
            ) const override;
    };
    
    /*
     * Public interface to managing state of snake
     * as well as state itself
     * 
     * However, behavior is in the above states
     */
    class Snake : public sf::Drawable, public link::Updatable {
        private:
            const double _moveDelayDec, _defMoveDelay;
            const size_t _minParts;
            const link::ResourceManager &_resMan;
            const link::InputManager &_inpMan;
            
            sf::Vector2f _headPos;
            std::vector<BodyInfo> _bodyInfos;
            double _moveTimer, _moveDelay;
            sf::SoundBuffer _eatBuffer, _deathBuffer;
            sf::Sound _eatSound, _deathSound;
            SnakeDirection _dir;
            std::map<GameState, std::shared_ptr<SnakeStateBehavior>> _behaviors;
            
        public:
            SnakeDirection nextDir;
            
            Snake(
                const link::ResourceManager &resMan,
                const link::InputManager &inpMan
            );
            
            // State manipulation
            void setPosition(const sf::Vector2f pos);
            void moveForward(const double delta);
            void resetBody(void);
            const link::ResourceManager &resMan(void);
            const link::InputManager &inpMan(void);
            
            // State change possibilities
            bool ateFruit(const std::shared_ptr<Fruit> &fruit);
            bool hasDied(const sf::Vector2u winSize);
            
            void update(const double delta, link::GameLoop &loop) override;
            void draw(
                sf::RenderTarget &target, sf::RenderStates states
            ) const override;
    };
}
