#pragma once

#include <SFML/Graphics.hpp>
#include <Snake.hpp>
#include <Fruit.hpp>
#include <Game.hpp>
#include <memory>

namespace sourcesnake {
    class Window : public sf::RenderWindow {
        private:
            const std::string _title = "Snake";
            const sf::Vector2u _defSize = sf::Vector2u(640, 480);
            const sf::Vector2u _minSize = sf::Vector2u(300, 300);
            const int _fps = 60;
            const sf::Texture _wallTex, _grassTex;
            const sf::Font _font;
            const sf::SoundBuffer _eatSndBuff;
            const sf::SoundBuffer _dieSndBuff;
            
            Snake _snake;
            GameState _state;
            bool _forcedResize;
            bool _spaceHeld, _spaceJustPressed, _spaceJustReleased;
            bool _upHeld, _upJustPressed, _upJustReleased;
            bool _downHeld, _downJustPressed, _downJustReleased;
            bool _leftHeld, _leftJustPressed, _leftJustReleased;
            bool _rightHeld, _rightJustPressed, _rightJustReleased;
            Fruit *_fruit;
            sf::Sound _eatSound, _dieSound;
            
            void _pollEvents(void);
            void _handleKeyEvents(const sf::Event &event, const bool isPress);
            void _updateKeyJusts(void);
            void _handleResize(const sf::Event &event);
            
            void _update(double);
            void _render(void);
            Window();
            ~Window();
            
        public:
            static Window &getInstance();
            void run();
    };
}
