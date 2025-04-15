# Dog Breed Matching Game

This is a cross-platform C++ terminal-based game that tests players' knowledge of different dog breed characteristics.

## Game Introduction

The game features 12 well-known dog breeds, each with unique traits. In each game session, the system randomly selects 6 dog breeds and provides one trait as a hint. Players need to guess the corresponding dog breed within a time limit.

## Game Features

- Clean terminal interface
- Real-time countdown display
- Game progress and score tracking
- History record saving
- Complete game completion congratulations
- Cross-platform support (Linux/Unix systems)
- Bilingual support (English)

## Dog Breed List

The game includes the following 12 dog breeds:

1. Golden Retriever - Friendly and gentle, double golden coat, highly intelligent and trainable
2. German Shepherd - Loyal and protective, black and tan short coat, common police and military dog
3. Labrador Retriever - Active and friendly, short waterproof coat, excellent guide dog breed
4. Poodle - Curly styled coat, non-shedding and hypoallergenic, extremely obedient
5. French Bulldog - Bat ears and wrinkled face, short nose and flat build, urban apartment-friendly
6. Siberian Husky - Blue eyes and tri-color face, double insulating thick coat, independent sled dog temperament
7. Corgi - Short legs and perky rear, Welsh royal pet, smart but stubborn
8. Border Collie - Classic black and white coloring, world's smartest dog, strong herding instinct
9. Shiba Inu - Fox-like face and curled tail, Japanese national treasure, clean and independent
10. Samoyed - Smiling angel face, snow-white double coat, Arctic working dog heritage
11. Bulldog - Exaggerated facial wrinkles, British symbol breed, calm and brave temperament
12. Chihuahua - World's smallest dog breed, apple head and large eyes, alert with loud bark

## Dependencies

This project depends on the following libraries:
- ncurses - for terminal interface operations

## How to Compile and Run

### Installing Dependencies
On most Linux distributions, you can install ncurses via package manager:

```bash
# Debian/Ubuntu
sudo apt-get install libncurses5-dev libncursesw5-dev

# Fedora
sudo dnf install ncurses-devel

# CentOS/RHEL
sudo yum install ncurses-devel
```

### Compilation
Compile using the provided Makefile:

```bash
# Compile
make

# Run
./dog_matching

# Clean compiled files
make clean
```

Or manually compile:

```bash
g++ -o dog_matching main.cpp -std=c++11 -lncurses
```

## System Requirements

- Linux/Unix operating system
- C++11 or higher
- ncurses library
- Terminal with UTF-8 support

## Game Tips

- Before the game starts, all dog breeds and their traits will be displayed, please memorize them
- Input is case-insensitive
- Each question has a 30-second time limit

## File Description

- `main.cpp` - Game main program source code
- `Makefile` - Compilation configuration file
- `congratulations.txt` - Congratulatory message displayed upon successful game completion
- `game_results.txt` - Game history record file (automatically created)

## Author

- [Your Name] - Initial version 