#include <fstream>
#include <string>
#include <FileManager.hpp>

using namespace sourcesnake;

FileManager::FileManager() :
        _score(0), _highScore(0) {
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

FileManager &FileManager::getInstance(void) {
    static FileManager instance;
    return instance;
}

void FileManager::setHighScore() {
    if(_score > _highScore) {
        std::ofstream highScoreFile(_highScorePath);
        highScoreFile << _score;
        highScoreFile.close();
        _highScore = _score;
    }
}

void FileManager::addToScore(int value) {
    _score += value;
}

void FileManager::resetScore(void) {
    _score = 0;
}

int FileManager::highScore(void) {
    return _highScore;
}

int FileManager::score(void) {
    return _score;
}
