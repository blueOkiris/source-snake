#include <chrono>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <Snake.hpp>
#include <Fruit.hpp>
#include <Game.hpp>
#include <FileManager.hpp>
#include <Window.hpp>

using namespace sourcesnake;

Window &Window::getInstance() {
    static Window instance;
    return instance;
}

Window::Window() :
        sf::RenderWindow(sf::VideoMode(300, 300), ""),
        _wallTex(loadTexture("img/wall.png")),
        _grassTex(loadTexture("img/grass.png")),
        _font(loadFont("fonts/Ubuntu-R.ttf")),
        _eatSndBuff(sourcesnake::loadSound("audio/eat.wav")),
        _dieSndBuff(sourcesnake::loadSound("audio/die.wav")) {
    setTitle(_title);
    setSize(_defSize);
    _snake.setPosition(sf::Vector2f(_defSize.x / 2, _defSize.y / 2));
    
    _state = GameState::Start;
    _forcedResize = false;
    _spaceHeld = _spaceJustPressed = _spaceJustReleased = false;
    _upHeld = _upJustPressed = _upJustReleased = false;
    _downHeld = _downJustPressed = _downJustReleased = false;
    _leftHeld = _leftJustPressed = _leftJustReleased = false;
    _rightHeld = _rightJustPressed = _rightJustReleased = false;
    
    _eatSound.setBuffer(_eatSndBuff);
    _dieSound.setBuffer(_dieSndBuff);
}

void Window::_update(const double delta) {
    switch(_state) {
        case GameState::Start: {
            auto winSize = getSize();
            auto snakeSize = _snake.headTex.getSize();
            _snake.setPosition(sf::Vector2f(
                winSize.x / 2 + snakeSize.x, 3 * (winSize.y / 4)
            ));
            _snake.resetBody();
            
            if(_spaceJustReleased) {
                _state = GameState::Playing;
                auto snakePos = sf::Vector2f(
                    winSize.x / 2 + snakeSize.x, winSize.y / 2
                );
                auto wallSize = _wallTex.getSize();
                snakePos.x = (snakePos.x / wallSize.x) * wallSize.x;
                snakePos.y = (snakePos.y / wallSize.y) * wallSize.y;
                snakePos.x -= _snake.headTex.getSize().x / 2;
                _snake.setPosition(snakePos);
                _snake.resetBody();
                _fruit = new Fruit(
                    sf::Vector2f(
                        Fruit::fruitTex.getSize().x + _wallTex.getSize().x,
                        Fruit::fruitTex.getSize().y + _wallTex.getSize().y
                    ), sf::Vector2f(
                        winSize.x - _wallTex.getSize().x
                            - Fruit::fruitTex.getSize().x,
                        winSize.y - _wallTex.getSize().y
                            - Fruit::fruitTex.getSize().y
                    )
                );
            }
        } break;
        
        case GameState::Playing: {
            auto winSize = getSize();
            auto wallTexSize = _wallTex.getSize();
            auto snakeEvent = _snake.update(
                delta,
                _upJustPressed, _downJustPressed,
                _leftJustPressed, _rightJustPressed,
                _fruit, winSize, wallTexSize
            );
            if(snakeEvent.first) {
                FileManager::getInstance().addToScore(1);
                free(_fruit);
                _fruit = new Fruit(
                    sf::Vector2f(
                        Fruit::fruitTex.getSize().x + _wallTex.getSize().x,
                        Fruit::fruitTex.getSize().y + _wallTex.getSize().y
                    ), sf::Vector2f(
                        winSize.x - _wallTex.getSize().x
                            - Fruit::fruitTex.getSize().x,
                        winSize.y - _wallTex.getSize().y
                            - Fruit::fruitTex.getSize().y
                    )
                );
                _eatSound.play();
            }
            if(snakeEvent.second) {
                _dieSound.play();
                FileManager::getInstance().setHighScore();
                _state = GameState::Dead;
                if(_fruit != nullptr) {
                    free(_fruit);
                    _fruit = nullptr;
                }
            }
        } break;
        
        case GameState::Dead:
            if(_spaceJustReleased) {
                _state = GameState::Start;
                _snake.resetBody();
                FileManager::getInstance().resetScore();
            }
            break;
    }
}

void Window::_render(void) {
    switch(_state) {
        case GameState::Start: {
            auto winSize = getSize();
            
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
                winSize.x / 2,
                winSize.y / 2 - titleText.getGlobalBounds().height
            ));
            
            sf::Text highScoreText;
            highScoreText.setFont(_font);
            highScoreText.setString(
                "High Score: "
                    + std::to_string(FileManager::getInstance().highScore())
            );
            highScoreText.setCharacterSize(30);
            highScoreText.setFillColor(sf::Color(0xFFFFFFFF));
            auto highScoreCenter = sf::Vector2f(
                highScoreText.getGlobalBounds().width / 2,
                highScoreText.getGlobalBounds().height / 2
            );
            highScoreText.setOrigin(highScoreCenter);
            highScoreText.setPosition(sf::Vector2f(
                winSize.x / 2,
                winSize.y / 2 + highScoreText.getGlobalBounds().height
            ));
            
            draw(titleText);
            draw(highScoreText);
            draw(_snake);
        } break;
        
        case GameState::Playing: {
            auto wallSize = getSize();
            auto grassTexSize = _grassTex.getSize();
            for(size_t y = 0; y < wallSize.y / grassTexSize.y; y++) {
                for(size_t x = 0; x < wallSize.x / grassTexSize.x; x++) {
                    sf::Sprite grassSpr(_grassTex);
                    grassSpr.setPosition(sf::Vector2f(
                        x * grassTexSize.x, y * grassTexSize.y
                    ));
                    draw(grassSpr);
                }
            }
            auto wallTexSize = _wallTex.getSize();
            for(size_t y = 0; y < wallSize.y / wallTexSize.y; y++) {
                for(size_t x = 0; x < wallSize.x / wallTexSize.x; x++) {
                    if(x == 0 || x == (wallSize.x / wallTexSize.x) - 1
                            || y == 0
                            || y == (wallSize.y / wallTexSize.y) - 1) {
                        sf::Sprite wallSpr(_wallTex);
                        wallSpr.setPosition(sf::Vector2f(
                            x * wallTexSize.x, y * wallTexSize.y
                        ));
                        draw(wallSpr);
                    }
                }
            }
            
            draw(_snake);
            draw(*_fruit);
            
            sf::Text scoreText;
            scoreText.setFont(_font);
            scoreText.setString(
                "Score: " + std::to_string(FileManager::getInstance().score())
            );
            scoreText.setCharacterSize(16);
            scoreText.setFillColor(sf::Color(0xFFFFFFFF));
            scoreText.setPosition(sf::Vector2f(48, 48));
            draw(scoreText);
        } break;
        
        case GameState::Dead:
            auto wallSize = getSize();
            auto wallTexSize = _wallTex.getSize();
            for(size_t y = 0; y < wallSize.y / wallTexSize.y; y++) {
                for(size_t x = 0; x < wallSize.x / wallTexSize.x; x++) {
                    if(x == 0 || x == (wallSize.x / wallTexSize.x) - 1
                            || y == 0
                            || y == (wallSize.y / wallTexSize.y) - 1) {
                        sf::Sprite wallSpr(_wallTex);
                        wallSpr.setPosition(sf::Vector2f(
                            x * wallTexSize.x, y * wallTexSize.y
                        ));
                        draw(wallSpr);
                    }
                }
            }
            draw(_snake);
            
            auto winSize = getSize();
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
                winSize.x / 2,
                winSize.y / 2 - 2 * deathText.getGlobalBounds().height
            ));
            draw(deathText);
            
            sf::Text highScoreText;
            highScoreText.setFont(_font);
            highScoreText.setString(
                "High Score: "
                    + std::to_string(FileManager::getInstance().highScore())
            );
            highScoreText.setCharacterSize(30);
            highScoreText.setFillColor(sf::Color(0xFFFFFFFF));
            auto highScoreCenter = sf::Vector2f(
                highScoreText.getGlobalBounds().width / 2,
                highScoreText.getGlobalBounds().height / 2
            );
            highScoreText.setOrigin(highScoreCenter);
            highScoreText.setPosition(sf::Vector2f(
                winSize.x / 2,
                winSize.y / 2 + 2 * highScoreText.getGlobalBounds().height
            ));
            draw(highScoreText);
            
            sf::Text scoreText;
            scoreText.setFont(_font);
            scoreText.setString(
                "Score: " + std::to_string(FileManager::getInstance().score())
            );
            scoreText.setCharacterSize(30);
            auto scoreTextCenter = sf::Vector2f(
                scoreText.getGlobalBounds().width / 2,
                scoreText.getGlobalBounds().height / 2
            );
            scoreText.setOrigin(scoreTextCenter);
            scoreText.setPosition(sf::Vector2f(
                winSize.x / 2,
                winSize.y / 2
                    + 2 * highScoreText.getGlobalBounds().height
                    + 2 * scoreText.getGlobalBounds().height
            ));
            scoreText.setFillColor(sf::Color(0xFFFFFFFF));
            draw(scoreText);
            break;
    }
}

void Window::run() {
    auto acc = 0.0;
    auto oldTime = std::chrono::system_clock::now();
    
    while(isOpen()) {
        auto currTime = std::chrono::system_clock::now();
        std::chrono::duration<double> delta = currTime - oldTime;
        acc += delta.count();
        oldTime = currTime;
        
        while(acc > 1.0 / _fps) {
            _pollEvents();
            _update(acc);
            _updateKeyJusts();
            acc -= 1.0 / _fps;
        }
        
        clear();
        _render();
        display();
    }
}

void Window::_pollEvents(void) {
    sf::Event event;
    while(pollEvent(event)) {
        switch(event.type) {
            case sf::Event::Closed:
                close();
                break;
            
            case sf::Event::Resized: {
                _handleResize(event);
            } break;
            
            case sf::Event::KeyPressed:
                _handleKeyEvents(event, true);
                break;
            
            case sf::Event::KeyReleased:
                _handleKeyEvents(event, false);
                break;
            
            default:
                break;
        }
    }
}

Window::~Window() {
    if(_fruit != nullptr) {
        free(_fruit);
    }
}

void Window::_updateKeyJusts(void) {
    _spaceJustPressed = _spaceJustReleased = false;
    _upJustPressed = _upJustReleased = false;
    _downJustPressed = _downJustReleased = false;
    _leftJustPressed = _leftJustReleased = false;
    _rightJustPressed = _rightJustReleased = false;
}

void Window::_handleKeyEvents(const sf::Event &event, const bool isPress) {
    switch(event.key.code) {
        case sf::Keyboard::Escape:
            close();
            break;
            
        case sf::Keyboard::Space:
            if(!_spaceHeld && isPress) {
                _spaceHeld = true;
                _spaceJustPressed = true;
            } else if(_spaceHeld && !isPress) {
                _spaceHeld = false;
                _spaceJustReleased = true;
            }
            break;
        
        case sf::Keyboard::Up:
            if(!_upHeld && isPress) {
                _upHeld = true;
                _upJustPressed = true;
            } else if(_upHeld && !isPress) {
                _upHeld = false;
                _upJustReleased = true;
            }
            break;
        
        case sf::Keyboard::Down:
            if(!_downHeld && isPress) {
                _downHeld = true;
                _downJustPressed = true;
            } else if(_downHeld && !isPress) {
                _downHeld = false;
                _downJustReleased = true;
            }
            break;
        
        case sf::Keyboard::Left:
            if(!_leftHeld && isPress) {
                _leftHeld = true;
                _leftJustPressed = true;
            } else if(_leftHeld && !isPress) {
                _leftHeld = false;
                _leftJustReleased = true;
            }
            break;
        
        case sf::Keyboard::Right:
            if(!_rightHeld && isPress) {
                _rightHeld = true;
                _rightJustPressed = true;
            } else if(_rightHeld && !isPress) {
                _rightHeld = false;
                _rightJustReleased = true;
            }
            break;
        
        default:
            break;
    }
}

void Window::_handleResize(const sf::Event &event) {
    _state = GameState::Start;
    
    sf::FloatRect visibleArea(
        0, 0, event.size.width, event.size.height
    );
    setView(sf::View(visibleArea));
    _snake.setPosition(sf::Vector2f(
        event.size.width / 2, event.size.height / 2
    ));
    
    if(!_forcedResize) {
        const auto width =
            event.size.width < _minSize.x ?
                _minSize.x :
                event.size.width;
        const auto height =
            event.size.height < _minSize.y ?
                _minSize.y :
                event.size.height;
        if(width != event.size.width
                || height != event.size.height) {
            _forcedResize = true;
            setSize(sf::Vector2u(width, height));
        }
    } else {
        _forcedResize = false;
    }
}