#include <string>
#include <fstream>
#include <memory>
#include <map>
#include <SFML/Graphics.hpp>
#include <GameLoop.hpp>
#include <ResourceManager.hpp>
#include <ScoreManager.hpp>

using namespace snake;

void ScoreManagerStart::draw(
        const ScoreManager &parent,
        sf::RenderTarget &target, sf::RenderStates states) const {
    sf::Text highScoreText;
    highScoreText.setFont(parent.font());
    highScoreText.setString(
        "High Score: " + std::to_string(parent.highScore())
    );
    highScoreText.setCharacterSize(30);
    highScoreText.setFillColor(sf::Color(0xFFFFFFFF));
    auto highScoreCenter = sf::Vector2f(
        highScoreText.getGlobalBounds().width / 2,
        highScoreText.getGlobalBounds().height / 2
    );
    highScoreText.setOrigin(highScoreCenter);
    highScoreText.setPosition(sf::Vector2f(
        parent.winSize().x / 2,
        parent.winSize().y / 2 + highScoreText.getGlobalBounds().height
    ));
    target.draw(highScoreText);
}

void ScoreManagerPlaying::draw(
        const ScoreManager &parent,
        sf::RenderTarget &target, sf::RenderStates states) const {
    sf::Text scoreText;
    scoreText.setFont(parent.font());
    scoreText.setString("Score: " + std::to_string(parent.score()));
    scoreText.setCharacterSize(16);
    scoreText.setFillColor(sf::Color(0xFFFFFFFF));
    scoreText.setPosition(sf::Vector2f(48, 48));
    target.draw(scoreText);
}

void ScoreManagerDead::draw(
        const ScoreManager &parent,
        sf::RenderTarget &target, sf::RenderStates states) const {
    sf::Text highScoreText;
    highScoreText.setFont(parent.font());
    highScoreText.setString(
        "High Score: " + std::to_string(parent.highScore())
    );
    highScoreText.setCharacterSize(30);
    highScoreText.setFillColor(sf::Color(0xFFFFFFFF));
    auto highScoreCenter = sf::Vector2f(
        highScoreText.getGlobalBounds().width / 2,
        highScoreText.getGlobalBounds().height / 2
    );
    highScoreText.setOrigin(highScoreCenter);
    highScoreText.setPosition(sf::Vector2f(
        parent.winSize().x / 2,
        parent.winSize().y / 2 + 2 * highScoreText.getGlobalBounds().height
    ));
    target.draw(highScoreText);
    
    sf::Text scoreText;
    scoreText.setFont(parent.font());
    scoreText.setString("Score: " + std::to_string(parent.score()));
    scoreText.setCharacterSize(30);
    auto scoreTextCenter = sf::Vector2f(
        scoreText.getGlobalBounds().width / 2,
        scoreText.getGlobalBounds().height / 2
    );
    scoreText.setOrigin(scoreTextCenter);
    scoreText.setPosition(sf::Vector2f(
        parent.winSize().x / 2,
        parent.winSize().y / 2
            + 2 * highScoreText.getGlobalBounds().height
            + 2 * scoreText.getGlobalBounds().height
    ));
    scoreText.setFillColor(sf::Color(0xFFFFFFFF));
    target.draw(scoreText);
}

ScoreManager::ScoreManager(const link::ResourceManager &resMan) :
        _score(0), _highScore(0) {
    _font = resMan.font("font");
    
    _drawBehaviors.emplace(
        GameState::Start, std::make_shared<ScoreManagerStart>()
    );
    _drawBehaviors.emplace(
        GameState::Playing, std::make_shared<ScoreManagerPlaying>()
    );
    _drawBehaviors.emplace(
        GameState::Dead, std::make_shared<ScoreManagerDead>()
    );
    
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

const sf::Font &ScoreManager::font(void) const {
    return _font;
}

sf::Vector2u ScoreManager::winSize(void) const {
    return _winSize;
}

void ScoreManager::update(const double delta, link::GameLoop &loop) {
    auto state = std::any_cast<GameState>(loop.globalGameState["state"]);
    _state = state;
    _winSize = loop.getSize();
}

void ScoreManager::draw(
        sf::RenderTarget &target, sf::RenderStates states) const {
    _drawBehaviors.at(_state)->draw(*this, target, states);
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

int ScoreManager::highScore(void) const {
    return _highScore;
}

int ScoreManager::score(void) const {
    return _score;
}
