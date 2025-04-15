#include "DogMatchingGame.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>
#include <fstream>
#include <ctime>
// Using wide character supported ncurses (path may vary on different systems)
#ifdef __linux__ 
  #include <ncursesw/curses.h>
#else
  #include <ncurses.h>
#endif
#include <cstring>
#include <unistd.h>
#include <clocale>
#include <locale>
#include <string.h>

// Initialize ncurses
void DogMatchingGame::initNcurses() {
    // Set localization to support UTF-8
    setlocale(LC_ALL, "");
    
    initscr();            // Initialize ncurses screen
    cbreak();             // Disable line buffering
    noecho();             // Don't display typed characters
    keypad(stdscr, TRUE); // Enable function keys
    start_color();        // Enable colors
    use_default_colors(); // Use terminal default colors
    
    // Define color pairs
    init_pair(1, COLOR_GREEN, -1);  // Green text
    init_pair(2, COLOR_RED, -1);    // Red text
    init_pair(3, COLOR_YELLOW, -1); // Yellow text
    init_pair(4, COLOR_CYAN, -1);   // Cyan text
}

// Clear screen function
void DogMatchingGame::clearScreen() {
    clear();
    refresh();
}

// Wait for keypress
int DogMatchingGame::waitForKeypress() {
    return getch();
}

// Display text, optional with color
void DogMatchingGame::printText(int y, int x, const std::string& text, int colorPair) {
    if (colorPair > 0) {
        attron(COLOR_PAIR(colorPair));
    }
    mvprintw(y, x, "%s", text.c_str());
    if (colorPair > 0) {
        attroff(COLOR_PAIR(colorPair));
    }
    refresh();
}

// Get string input
std::string DogMatchingGame::getStringInput(int y, int x, const std::string& prompt) {
    echo(); // Enable echo
    std::string input;
    char buf[256];
    
    mvprintw(y, x, "%s", prompt.c_str());
    refresh();
    
    // Get user input
    int ch = getch();
    int pos = 0;
    
    while (ch != '\n' && pos < 255) {
        if (ch == KEY_BACKSPACE || ch == 127) {
            if (pos > 0) {
                pos--;
                mvprintw(y, x + prompt.length() + pos, " ");
                move(y, x + prompt.length() + pos);
            }
        } else if (ch >= 32 && ch <= 126) { // Printable characters
            buf[pos++] = ch;
            mvaddch(y, x + prompt.length() + pos - 1, ch);
        }
        refresh();
        ch = getch();
    }
    
    buf[pos] = '\0';
    input = buf;
    
    noecho(); // Disable echo
    return input;
}

// Format dog breed name for consistent display
std::string DogMatchingGame::formatDogBreedName(const DogBreed& breed) {
    return breed.name + " / " + breed.englishName;
}

// Initialize all dog breed data
void DogMatchingGame::initializeDogBreeds() {
    // Initialize 12 dog breeds based on provided information
    allDogBreeds = {
        {"","Golden Retriever", {"Friendly and gentle", "Double golden coat", "Highly intelligent and trainable"}},
        {"","German Shepherd", {"Loyal and protective", "Black and tan short coat", "Common police and military dog"}},
        {"","Labrador Retriever", {"Active and friendly", "Short waterproof coat", "Excellent guide dog breed"}},
        {"","Poodle", {"Curly styled coat", "Non-shedding and hypoallergenic", "Extremely obedient"}},
        {"","French Bulldog", {"Bat ears and wrinkled face", "Short nose and flat build", "Urban apartment-friendly"}},
        {"","Siberian Husky", {"Blue eyes and tri-color face", "Double insulating thick coat", "Independent sled dog temperament"}},
        {"","Corgi", {"Short legs and perky rear", "Welsh royal pet", "Smart but stubborn"}},
        {"","Border Collie", {"Classic black and white coloring", "World's smartest dog", "Strong herding instinct"}},
        {"","Shiba Inu", {"Fox-like face and curled tail", "Japanese national treasure", "Clean and independent"}},
        {"","Samoyed", {"Smiling angel face", "Snow-white double coat", "Arctic working dog heritage"}},
        {"","Bulldog", {"Exaggerated facial wrinkles", "British symbol breed", "Calm and brave temperament"}},
        {"","Chihuahua", {"World's smallest dog breed", "Apple head and large eyes", "Alert with loud bark"}}
    };
}

// Display all dog breeds and traits
void DogMatchingGame::displayAllBreeds() {
    clearScreen();
    printText(0, 0, "==== Dog Breeds and Traits List ====", 3);
    
    int row = 2;
    for (size_t i = 0; i < allDogBreeds.size(); i++) {
        std::string line = std::to_string(i+1) + ". " + formatDogBreedName(allDogBreeds[i]);
        printText(row++, 0, line);
        
        std::string traits = "   Traits: ";
        for (size_t j = 0; j < allDogBreeds[i].traits.size(); j++) {
            traits += "- " + allDogBreeds[i].traits[j] + " ";
        }
        printText(row++, 0, traits);
        row++; // Empty line
    }
    
    printText(row++, 0, "Remember these dog breeds and traits. Are you ready?");
    printText(row, 0, "Press Enter to start the game...");
    
    // Wait for Enter key
    int ch;
    while ((ch = getch()) != '\n' && ch != KEY_ENTER);
}

// Randomly select 6 breeds for the game
void DogMatchingGame::selectGameBreeds() {
    // Create vector of indices from 0 to 11
    std::vector<int> indices(allDogBreeds.size());
    for (size_t i = 0; i < indices.size(); i++) {
        indices[i] = i;
    }
    
    // Randomly shuffle indices
    auto rng = std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());
    std::shuffle(indices.begin(), indices.end(), rng);
    
    // Select first 6 indices corresponding to dog breeds
    gameBreeds.clear();
    for (int i = 0; i < 6; i++) {
        gameBreeds.push_back(allDogBreeds[indices[i]]);
    }
}

// Display game interface
void DogMatchingGame::displayGameInterface(int questionNumber, int timeLeft) {
    clearScreen();
    std::string title = "==== Dog Matching Game - " + playerName + " ====";
    printText(0, 0, title, 3);
    
    std::string status = "Question " + std::to_string(questionNumber) + "/6    Correct: " + 
                   std::to_string(correctAnswers) + "/" + std::to_string(questionNumber-1) + 
                   "    Completion: " + std::to_string(questionNumber > 1 ? correctAnswers * 100 / (questionNumber-1) : 0) + 
                   "%    Time left: " + std::to_string(timeLeft) + "s";
    printText(2, 0, status);
    
    // Display current question's trait hint
    const DogBreed& currentDog = gameBreeds[questionNumber - 1];
    printText(4, 0, "Guess the dog breed based on the following trait:", 4);
    
    // Randomly select a trait to display
    int traitIndex = rand() % currentDog.traits.size();
    std::string trait = "Trait: " + currentDog.traits[traitIndex];
    printText(6, 0, trait, 1);
    
    printText(8, 0, "Enter the dog breed name: ");
    refresh();
}

// Handle user input, supporting UTF-8 input
bool DogMatchingGame::handleUserInput(const DogBreed& currentDog, int timeLimit) {
    std::string userAnswer;
    bool answered = false;
    
    // Set non-blocking input (timeout 1 second)
    timeout(1000); // 1000ms = 1s
    
    int timeLeft = timeLimit;
    int inputY = 8;
    
    // Dynamically calculate the length of the prompt text instead of using hard - coded values. 
    std::string promptText = "Enter the dog breed name: ";
    int inputX = promptText.length();
    
    // Redisplay the prompt text and make sure it is fully shown. 
    move(inputY, 0);
    clrtoeol();
    printText(inputY, 0, promptText);
    refresh();
    
    // Input loop
    while (timeLeft > 0 && !answered) {
        // Update time display
        std::string status = "Question " + std::to_string(currentQuestion) + "/6    Correct: " + 
                       std::to_string(correctAnswers) + "/" + std::to_string(currentQuestion-1) + 
                       "    Completion: " + std::to_string(currentQuestion > 1 ? correctAnswers * 100 / (currentQuestion-1) : 0) + 
                       "%    Time left: " + std::to_string(timeLeft) + "s";
        move(2, 0);
        clrtoeol(); // Clear that line
        printText(2, 0, status);
        
        // Get character
        int ch = getch();
        
        if (ch == ERR) {
            // Timeout (one second passed)
            timeLeft--;
            continue;
        } else if (ch == '\n' || ch == KEY_ENTER) {
            // User submitted answer
            answered = true;
            break;
        } else if (ch == KEY_BACKSPACE || ch == 127) {
            // Backspace handling
            if (!userAnswer.empty()) {
                // Delete last character, handle UTF-8 encoding
                const char* str = userAnswer.c_str();
                int len = userAnswer.length();
                int i = 0, pos = 0;
                
                // Find start position of last character
                while (i < len) {
                    int charLen = 1;
                    if ((str[i] & 0xC0) == 0xC0) {
                        // This is the start of a multi-byte UTF-8 character
                        if ((str[i] & 0xE0) == 0xC0) charLen = 2;      // 2-byte character
                        else if ((str[i] & 0xF0) == 0xE0) charLen = 3; // 3-byte character
                        else if ((str[i] & 0xF8) == 0xF0) charLen = 4; // 4-byte character
                    }
                    if (i + charLen <= len) pos = i;
                    i += charLen;
                }
                
                // Delete the last complete character
                userAnswer = userAnswer.substr(0, pos);
                
                // Clear current input line and redisplay
                move(inputY, inputX);
                clrtoeol();
                printText(inputY, 0, promptText); // reprint the prompt text 
                printText(inputY, inputX, userAnswer);
            }
        } else if (ch >= 32) {
            // Normal ASCII character
            if (ch <= 127) {
                userAnswer += (char)ch;
            } 
            // Handle UTF-8 multi-byte characters
            else {
                // Handle multi-byte input
                int bytes[3] = {0}; // Store up to 3 subsequent bytes
                int nBytes = 0;
                
                // Determine number of subsequent bytes based on UTF-8 encoding rules
                if ((ch & 0xE0) == 0xC0) nBytes = 1;      // 2-byte character
                else if ((ch & 0xF0) == 0xE0) nBytes = 2; // 3-byte character
                else if ((ch & 0xF8) == 0xF0) nBytes = 3; // 4-byte character
                
                // Add first byte
                userAnswer += (char)ch;
                
                // Read remaining bytes
                for (int i = 0; i < nBytes; i++) {
                    bytes[i] = getch();
                    if (bytes[i] != ERR) {
                        userAnswer += (char)bytes[i];
                    }
                }
            }
            
            // Make sure to reprint the prompt text before each display of user input. 
            move(inputY, 0);
            clrtoeol();
            printText(inputY, 0, promptText);
            printText(inputY, inputX, userAnswer);
        }
        
        refresh();
    }
    
    // Restore blocking input
    timeout(-1);
    
    // If time is up but no answer
    if (timeLeft <= 0 && !answered) {
        printText(10, 0, "Time's up! You didn't answer.", 2);
        userAnswer = "";
    }
    
    // Check if answer is correct (case-insensitive for English answers)
    std::string userAnswerLower = userAnswer;
    std::string dogNameLower = currentDog.name;
    std::string dogEngNameLower = currentDog.englishName;
    
    // Convert English to lowercase for comparison (don't modify Chinese characters)
    std::transform(userAnswerLower.begin(), userAnswerLower.end(), userAnswerLower.begin(), 
             [](unsigned char c){ return std::tolower(c); });
    std::transform(dogEngNameLower.begin(), dogEngNameLower.end(), dogEngNameLower.begin(), 
             [](unsigned char c){ return std::tolower(c); });
    
    // Check if answer matches exactly
    bool isCorrect = (userAnswerLower == dogNameLower) || 
                     (userAnswerLower == dogEngNameLower);
    
    // Simplified matching: check if answer contains or partially matches
    if (!isCorrect) {
        // Check if user input is part of the Chinese name (exact substring)
        size_t namePos = dogNameLower.find(userAnswerLower);
        
        // Check if user input is part of the English name (case-insensitive)
        size_t engNamePos = dogEngNameLower.find(userAnswerLower);
        
        // Chinese matching requires exact match or longer input
        // English matching can be partial if it's substantial
        if (namePos != std::string::npos && userAnswer.length() >= 2) {
            isCorrect = true;
        }
        // If user input is a part of the English name and longer than half of name's length
        else if (engNamePos != std::string::npos && userAnswer.length() >= dogEngNameLower.length() / 2) {
            isCorrect = true;
        }
    }
    
    // Display correct answer
    if (isCorrect) {
        std::string message = "Correct! " + formatDogBreedName(currentDog);
        printText(10, 0, message, 1);
    } else {
        std::string message = "Wrong! The correct answer is: " + formatDogBreedName(currentDog);
        printText(10, 0, message, 2);
    }
    
    refresh();
    sleep(2); // Pause for 2 seconds
    return isCorrect;
}

// Display game results
void DogMatchingGame::displayGameResult() {
    clearScreen();
    std::string title = "==== Game Over - " + playerName + " ====";
    printText(0, 0, title, 3);
    
    if (correctAnswers == 6) {
        // If all answers are correct, display congratulations message
        displayCongratulations();
    } else {
        std::string result = "You got " + std::to_string(correctAnswers) + "/6 questions correct.";
        std::string score = "Final score: " + std::to_string(correctAnswers * 100 / 6) + "%";
        
        printText(2, 0, result);
        printText(3, 0, score);
        printText(5, 0, "Keep going, you'll do better next time!", 3);
    }
    
    // Save game result
    saveGameResult();
    
    printText(7, 0, "Press any key to continue...");
    getch();
}

// Display congratulations message
void DogMatchingGame::displayCongratulations() {
    // Try to open congratulations file
    std::ifstream congratsFile("congratulations.txt");
    
    if (congratsFile.is_open()) {
        printText(2, 0, "🎉 Congratulations! You got all 6 questions correct! 🎉", 1);
        
        // Read and display line by line
        std::string line;
        int row = 4;
        while (std::getline(congratsFile, line)) {
            printText(row++, 0, line);
        }
        congratsFile.close();
    } else {
        // If file can't be opened, display simple congratulations
        printText(2, 0, "🎉 Congratulations! You got all 6 questions correct! 🎉", 1);
        printText(4, 0, "You are a dog breed expert!", 1);
    }
}

// Save game result
void DogMatchingGame::saveGameResult() {
    // Open file to append result
    std::ofstream resultFile("game_results.txt", std::ios::app);
    if (resultFile.is_open()) {
        // Get current time
        time_t now = time(0);
        char dt[26];
        strftime(dt, sizeof(dt), "%Y-%m-%d %H:%M:%S", localtime(&now));
        std::string datetime(dt);
        
        resultFile << "Player: " << playerName << " | Score: " << correctAnswers << "/6 (" 
                  << (correctAnswers * 100 / 6) << "%) | Date: " << datetime << std::endl;
        resultFile.close();
    }
}

// Constructor
DogMatchingGame::DogMatchingGame() {
    initializeDogBreeds();
    currentQuestion = 0;
    correctAnswers = 0;
    gameRunning = false;
    playerName = "Player";
}

// Destructor
DogMatchingGame::~DogMatchingGame() {
    // Ensure ncurses is closed
    endwin();
}

// Start game
void DogMatchingGame::startGame() {
    // Set random seed
    srand(time(NULL));
    
    // Initialize ncurses
    initNcurses();
    
    // Display welcome screen
    clearScreen();
    printText(0, 0, "==== Welcome to the Dog Matching Game ====", 3);
    
    printText(2, 0, "Game Rules:");
    printText(3, 0, "1. You will see traits describing a dog");
    printText(4, 0, "2. You need to guess the corresponding dog breed");
    printText(5, 0, "3. There are 6 questions, with 30 seconds for each");
    printText(6, 0, "4. Please enter the complete dog breed name");
    
    // Get player name
    echo(); // Enable input echo
    printText(8, 0, "Enter your name: ");
    char name[50];
    getstr(name);
    playerName = name;
    noecho(); // Disable input echo
    
    if (playerName.empty()) {
        playerName = "Player";
    }
    
    std::string readyMsg = "Are you ready, " + playerName + "? Press Enter to see all dog breeds...";
    printText(10, 0, readyMsg);
    
    // Wait for Enter key
    int ch;
    while ((ch = getch()) != '\n' && ch != KEY_ENTER);
    
    // Display all dog breeds
    displayAllBreeds();
    
    // Randomly select 6 breeds
    selectGameBreeds();
    
    gameRunning = true;
    currentQuestion = 1;
    correctAnswers = 0;
    
    // Main game loop
    while (gameRunning && currentQuestion <= 6) {
        displayGameInterface(currentQuestion, 30);
        
        // Handle this question
        bool isCorrect = handleUserInput(gameBreeds[currentQuestion - 1], 30);
        if (isCorrect) {
            correctAnswers++;
        }
        
        currentQuestion++;
    }
    
    // Display results
    displayGameResult();
}

// Display history
void DogMatchingGame::showHistory() {
    // Initialize ncurses (if not already initialized)
    initNcurses();
    
    clearScreen();
    printText(0, 0, "==== Game History ====", 3);
    
    std::ifstream historyFile("game_results.txt");
    if (historyFile.is_open()) {
        std::string line;
        int row = 2;
        while (std::getline(historyFile, line) && row < LINES - 2) {
            printText(row++, 0, line);
        }
        historyFile.close();
    } else {
        printText(2, 0, "No history records.");
    }
    
    printText(LINES-2, 0, "Press any key to return...");
    getch();
}

// Main menu
void DogMatchingGame::showMainMenu() {
    // Initialize ncurses
    initNcurses();
    
    bool exitGame = false;
    while (!exitGame) {
        clearScreen();
        printText(0, 0, "==== Dog Matching Game - Main Menu ====", 3);
        
        printText(2, 0, "1. Start New Game");
        printText(3, 0, "2. View History");
        printText(4, 0, "3. Exit Game");
        
        printText(6, 0, "Please choose (1-3): ");
        
        int choice = getch();
        
        switch (choice) {
            case '1':
                startGame();
                break;
            case '2':
                showHistory();
                break;
            case '3':
                exitGame = true;
                break;
            default:
                printText(8, 0, "Invalid choice, please try again.", 2);
                refresh();
                sleep(1);
        }
    }
    
    // Display exit message
    clearScreen();
    printText(0, 0, "Thanks for playing! Goodbye!", 1);
    refresh();
    sleep(2);
    
    // Close ncurses
    endwin();
} 