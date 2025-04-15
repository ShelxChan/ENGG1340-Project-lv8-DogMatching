#include <locale>
#include <clocale>
#include "DogMatchingGame.h"

int main() {
    // Set localization to support multilingual characters
    std::setlocale(LC_ALL, "");
    // Ensure output streams use UTF-8 encoding
    std::locale::global(std::locale(""));
    
    DogMatchingGame game;
    game.showMainMenu();
    
    return 0;
} 