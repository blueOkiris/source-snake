#pragma once

#include <chrono>
#include <random>
#include <exception>
#include <string>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

namespace sourcesnake {
    enum class GameState {
        Start, Playing, Dead
    };
    
    inline std::default_random_engine &getRandomGenerator() {
        static std::default_random_engine randGenerator;
        static bool g_hasBeenSeeded = false;
        
        if(!g_hasBeenSeeded) {
            randGenerator.seed(
                std::chrono::system_clock::now().time_since_epoch().count()
            );
        }
        
        return randGenerator;
    }
    
    struct ResourceFailureException : public std::exception {
        private:
            const std::string _msg;
            
        public:
            ResourceFailureException(const std::string &src) :
                    _msg("Failed to load resource '" + src + "'") {
            }
            
            const char *what() const throw() {
                return _msg.c_str();
            }
    };

    inline const sf::Texture loadTexture(const std::string &src) {
        sf::Texture tex;
        if(!tex.loadFromFile(src)) {
            throw ResourceFailureException(src);
        }
        return tex;
    }

    inline const sf::Font loadFont(const std::string &src) {
        sf::Font font;
        if(!font.loadFromFile(src)) {
            throw ResourceFailureException(src);
        }
        return font;
    }

    inline const sf::SoundBuffer loadSound(const std::string &src) {
        sf::SoundBuffer sound;
        if(!sound.loadFromFile(src)) {
            throw ResourceFailureException(src);
        }
        return sound;
    }
}
