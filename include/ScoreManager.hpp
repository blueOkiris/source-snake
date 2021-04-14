#pragma once

#include <string>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <GameLoop.hpp>
#include <ResourceManager.hpp>
#include <Game.hpp>

namespace snake {
    class ScoreManager : public link::Updatable, public sf::Drawable {
        private:
            GameState _state;
            sf::Vector2u _winSize;
            sf::Font _font;
            
            int _score, _highScore;
            const std::string _highScorePath = ".highscore.txt";
            
        public:
            ScoreManager(const link::ResourceManager &resMan);
            
            void setHighScore();
            void addToScore(int value);
            void resetScore(void);
            
            int score(void);
            int highScore(void);
            
            void update(const double delta, link::GameLoop &loop) override;
            void draw(
                sf::RenderTarget &target, sf::RenderStates states
            ) const override;
    };
}
