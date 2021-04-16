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
            // Helpers for draw
            static bool _isTopRightOrBottomLeft(
                const SnakeDirection oldDir, const SnakeDirection newDir
            );
            static bool _isTopLeftOrBottomRight(
                const SnakeDirection oldDir, const SnakeDirection newDir
            );
            
            const double _moveDelayDec; // Amount to decrease timer speed by
            const double _defMoveDelay; // Default timer speed
            const size_t _minParts; // Minimum number of parts to spawn with
            const link::ResourceManager &_resMan;
            const link::InputManager &_inpMan;
            
            sf::Vector2f _headPos; // Head for the snake
            std::vector<BodyInfo> _bodyInfos; // Dir and position of body pieces
            double _moveTimer, _moveDelay; // Counter for when to move forward
            sf::SoundBuffer _eatBuffer, _deathBuffer; // Eat and death audio
            sf::Sound _eatSound, _deathSound; // Eat and death audio players
            SnakeDirection _dir; // Current HEAD direction
            
            // List of behaviors for the update based on game state
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
            
            // Core of class
            void update(const double delta, link::GameLoop &loop) override;
            void draw(
                sf::RenderTarget &target, sf::RenderStates states
            ) const override;
    };
}
