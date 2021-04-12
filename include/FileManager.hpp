#pragma once

#include <string>
#include <fstream>

namespace sourcesnake {
    class FileManager {
        private:
            int _score, _highScore;
            const std::string _highScorePath = ".highscore.txt";
            
            FileManager();
            
        public:
            static FileManager &getInstance(void);
            
            void setHighScore();
            void addToScore(int value);
            void resetScore(void);
            
            int score(void);
            int highScore(void);
    };
}
