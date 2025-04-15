#ifndef DOG_BREED_H
#define DOG_BREED_H

#include <string>
#include <vector>

// Define dog breed structure
struct DogBreed {
    std::string name;          // Primary name (can be Chinese or localized name)
    std::string englishName;   // English name (international standard name)
    std::vector<std::string> traits; // Traits list
};

#endif // DOG_BREED_H 