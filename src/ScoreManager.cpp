#include <string>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <GameLoop.hpp>
#include <ResourceManager.hpp>
#include <ScoreManager.hpp>

using namespace snake;

ScoreManager::ScoreManager(const link::ResourceManager &resMan) :
        _score(0), _highScore(0) {
    _font = resMan.font("font");
    
    std::ifstream highScoreFile(_highScorePath);
    if(!highScoreFile.is_open()) {
        std::ofstream outHighScoreFile(_highScorePath);
        outHighScoreFile << 0;
        outHighScoreFile.close();
    } else {
        highScoreFile >> _highScore;
        highScoreFile.close();
    }
}

void ScoreManager::update(const double delta, link::GameLoop &loop) {
    auto state = std::any_cast<GameState>(loop.globalGameState["state"]);
    _state = state;
    _winSize = loop.getSize();
}

void ScoreManager::draw(
        sf::RenderTarget &target, sf::RenderStates states) const {
    switch(_state) {
        case GameState::Start: {
            sf::Text highScoreText;
            highScoreText.setFont(_font);
            highScoreText.setString(
                "High Score: " + std::to_string(_highScore)
            );
            highScoreText.setCharacterSize(30);
            highScoreText.setFillColor(sf::Color(0xFFFFFFFF));
            auto highScoreCenter = sf::Vector2f(
                highScoreText.getGlobalBounds().width / 2,
                highScoreText.getGlobalBounds().height / 2
            );
            highScoreText.setOrigin(highScoreCenter);
            highScoreText.setPosition(sf::Vector2f(
                _winSize.x / 2,
                _winSize.y / 2 + highScoreText.getGlobalBounds().height
            ));
            target.draw(highScoreText);
        } break;
        
        case GameState::Playing: {
            sf::Text scoreText;
            scoreText.setFont(_font);
            scoreText.setString("Score: " + std::to_string(_score));
            scoreText.setCharacterSize(16);
            scoreText.setFillColor(sf::Color(0xFFFFFFFF));
            scoreText.setPosition(sf::Vector2f(48, 48));
            target.draw(scoreText);
        } break;
        
        case GameState::Dead: {
            sf::Text highScoreText;
            highScoreText.setFont(_font);
            highScoreText.setString(
                "High Score: " + std::to_string(_highScore)
            );
            highScoreText.setCharacterSize(30);
            highScoreText.setFillColor(sf::Color(0xFFFFFFFF));
            auto highScoreCenter = sf::Vector2f(
                highScoreText.getGlobalBounds().width / 2,
                highScoreText.getGlobalBounds().height / 2
            );
            highScoreText.setOrigin(highScoreCenter);
            highScoreText.setPosition(sf::Vector2f(
                _winSize.x / 2,
                _winSize.y / 2 + 2 * highScoreText.getGlobalBounds().height
            ));
            target.draw(highScoreText);
            
            sf::Text scoreText;
            scoreText.setFont(_font);
            scoreText.setString("Score: " + std::to_string(_score));
            scoreText.setCharacterSize(30);
            auto scoreTextCenter = sf::Vector2f(
                scoreText.getGlobalBounds().width / 2,
                scoreText.getGlobalBounds().height / 2
            );
            scoreText.setOrigin(scoreTextCenter);
            scoreText.setPosition(sf::Vector2f(
                _winSize.x / 2,
                _winSize.y / 2
                    + 2 * highScoreText.getGlobalBounds().height
                    + 2 * scoreText.getGlobalBounds().height
            ));
            scoreText.setFillColor(sf::Color(0xFFFFFFFF));
            target.draw(scoreText);
        } break;
    }
}

void ScoreManager::setHighScore() {
    if(_score > _highScore) {
        std::ofstream highScoreFile(_highScorePath);
        highScoreFile << _score;
        highScoreFile.close();
        _highScore = _score;
    }
}

void ScoreManager::addToScore(int value) {
    _score += value;
}

void ScoreManager::resetScore(void) {
    _score = 0;
}

int ScoreManager::highScore(void) {
    return _highScore;
}

int ScoreManager::score(void) {
    return _score;
}
