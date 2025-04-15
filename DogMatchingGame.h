#ifndef DOG_MATCHING_GAME_H
#define DOG_MATCHING_GAME_H

#include <string>
#include <vector>
#include "DogBreed.h"

// Game class
class DogMatchingGame {
private:
    std::vector<DogBreed> allDogBreeds; // All dog breeds
    std::vector<DogBreed> gameBreeds;   // Breeds selected for this round
    int currentQuestion;           // Current question
    int correctAnswers;            // Number of correct answers
    bool gameRunning;              // Is the game running
    std::string playerName;        // Player name
    
    // Initialize ncurses
    void initNcurses();
    
    // Clear screen function
    void clearScreen();
    
    // Wait for keypress
    int waitForKeypress();
    
    // Display text, optional with color
    void printText(int y, int x, const std::string& text, int colorPair = 0);
    
    // Get string input
    std::string getStringInput(int y, int x, const std::string& prompt);
    
    // Format dog breed name for consistent display
    std::string formatDogBreedName(const DogBreed& breed);
    
    // Initialize all dog breed data
    void initializeDogBreeds();
    
    // Display all dog breeds and traits
    void displayAllBreeds();
    
    // Randomly select 6 breeds for the game
    void selectGameBreeds();
    
    // Display game interface
    void displayGameInterface(int questionNumber, int timeLeft);
    
    // Handle user input
    bool handleUserInput(const DogBreed& currentDog, int timeLimit);
    
    // Display game results
    void displayGameResult();
    
    // Display congratulations message
    void displayCongratulations();
    
    // Save game result
    void saveGameResult();
    
public:
    // Constructor
    DogMatchingGame();
    
    // Destructor
    ~DogMatchingGame();
    
    // Start game
    void startGame();
    
    // Display history
    void showHistory();
    
    // Main menu
    void showMainMenu();
};

#endif // DOG_MATCHING_GAME_H 