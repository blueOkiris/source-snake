#pragma once

#include <string>
#include <fstream>
#include <memory>
#include <map>
#include <SFML/Graphics.hpp>
#include <GameLoop.hpp>
#include <ResourceManager.hpp>
#include <Game.hpp>

namespace snake {
    class ScoreManager;
    
    class ScoreManagerDrawBehavior {
        public:
            virtual void draw(
                const ScoreManager &parent,
                sf::RenderTarget &target, sf::RenderStates states
            ) const = 0;
    };
    
    class ScoreManagerStart : public ScoreManagerDrawBehavior {
        public:
            void draw(
                const ScoreManager &parent,
                sf::RenderTarget &target, sf::RenderStates states
            ) const override;
    };
    
    class ScoreManagerPlaying : public ScoreManagerDrawBehavior {
        public:
            void draw(
                const ScoreManager &parent,
                sf::RenderTarget &target, sf::RenderStates states
            ) const override;
    };
    
    class ScoreManagerDead: public ScoreManagerDrawBehavior {
        public:
            void draw(
                const ScoreManager &parent,
                sf::RenderTarget &target, sf::RenderStates states
            ) const override;
    };
    
    class ScoreManager : public link::Updatable, public sf::Drawable {
        private:
            GameState _state;
            sf::Vector2u _winSize;
            sf::Font _font;
            
            int _score, _highScore;
            const std::string _highScorePath = ".highscore.txt";
            
            std::map<GameState, std::shared_ptr<ScoreManagerDrawBehavior>>
                _drawBehaviors;
            
        public:
            ScoreManager(const link::ResourceManager &resMan);
            
            void setHighScore(); // Possibly set highscore to score and save
            void addToScore(int value);
            void resetScore(void);
            
            // State access
            const sf::Font &font(void) const;
            sf::Vector2u winSize(void) const;
            int score(void) const;
            int highScore(void) const;
            
            void update(const double delta, link::GameLoop &loop) override;
            void draw(
                sf::RenderTarget &target, sf::RenderStates states
            ) const override;
    };
}
