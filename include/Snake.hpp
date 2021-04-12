#pragma once

#include <vector>
#include <utility>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <Fruit.hpp>

namespace sourcesnake {
    enum class SnakeDirection {
        Up, Down, Left, Right
    };
    
    typedef std::pair<sf::Vector2f, SnakeDirection> BodyInfo;
    
    class Snake : public sf::Drawable{
        private:
            const double _moveDelayDec;
            const size_t _minParts;
            
            sf::Vector2f _headPos;
            std::vector<BodyInfo> _bodyInfos;
            
            SnakeDirection _dir, _nextDir;
            double _moveTimer;
            double _moveDelay;
            
            void _move(void);
            bool _ateFruit(const Fruit *fruit);
            bool _hasDied(
                const sf::Vector2u winSize, const sf::Vector2u wallTexSize
            ) const;
        
        public:
            const sf::Texture headTex, bodyTex;
            
            Snake();
            void resetBody(void);
            void setPosition(const sf::Vector2f pos);
            
            std::pair<bool, bool> update(
                const double delta,
                const bool upPressed, const bool downPressed,
                const bool leftPressed, const bool rightPressed,
                const Fruit *fruit,
                const sf::Vector2u winSize, const sf::Vector2u wallTexSize
            );
            void draw(
                sf::RenderTarget &target, sf::RenderStates states
            ) const override;
    };
}
