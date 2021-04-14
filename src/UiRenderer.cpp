#include <iostream>
#include <SFML/Graphics.hpp>
#include <GameLoop.hpp>
#include <Game.hpp>
#include <UiRenderer.hpp>

using namespace snake;

UiRenderer::UiRenderer(const link::ResourceManager &resMan) {
    _font = resMan.font("font");
}

void UiRenderer::update(const double delta, link::GameLoop &loop) {
    auto state = std::any_cast<GameState>(loop.globalGameState["state"]);
    _state = state;
    _winSize = loop.getSize();
}

void UiRenderer::draw(
        sf::RenderTarget &target, sf::RenderStates states) const {
    switch(_state) {
        case GameState::Start: {
            sf::Text titleText;
            titleText.setFont(_font);
            titleText.setString("Snake");
            titleText.setCharacterSize(120);
            titleText.setFillColor(sf::Color(0xFFFFFFFF));
            auto titleCenter = sf::Vector2f(
                titleText.getGlobalBounds().width / 2,
                titleText.getGlobalBounds().height / 2
            );
            titleText.setOrigin(titleCenter);
            titleText.setPosition(sf::Vector2f(
                _winSize.x / 2,
                _winSize.y / 2 - titleText.getGlobalBounds().height
            ));
            target.draw(titleText);
        } break;
        
        case GameState::Playing:
            break;
        
        case GameState::Dead: {
            sf::Text deathText;
            deathText.setFont(_font);
            deathText.setString("You died!");
            deathText.setCharacterSize(50);
            deathText.setFillColor(sf::Color(0xFFFFFFFF));
            auto deathCenter = sf::Vector2f(
                deathText.getGlobalBounds().width / 2,
                deathText.getGlobalBounds().height / 2
            );
            deathText.setOrigin(deathCenter);
            deathText.setPosition(sf::Vector2f(
                _winSize.x / 2,
                _winSize.y / 2 - 2 * deathText.getGlobalBounds().height
            ));
            target.draw(deathText);
        } break;
    }
}
