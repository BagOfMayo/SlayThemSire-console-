"Slay the Spire" Inspired Console Deck Builder Game

Turn-based deck-building game built in C++ and running entirely in the Windows console

Features
- Deck-building system (draw pile, hand, discard logic)
- Turn-based combat system
- Multiple card types (attack, defense, power, status)
- Simple enemy AI with random decision-making
- Status effects system (Strength, Weakness, Vulnerability, Metallicize)
- Energy system per turn
- Randomized card rewards after battles
- Console rendering with ASCII visuals
- Data-driven cards using JSON

Technologies Used
- C++
- STL (vector, list, map, ...)
- nlohmann/json (for card data)
- windows.h for console control
- OOP

Game Mechanics
- Each turn:
    Player uses energy to play cards.
    Enemy performs an action.
- Cards can:
    Deal damage.
    Apply buffs/debuffs.
    Draw more cards / Give more energy.
- Damage formula includes:
    Strength scaling.
    Weakness / Vulnerability modifiers.
    Block system.

Gameplay:
https://github.com/user-attachments/assets/0059a68a-dad7-4d5f-89b9-5de5c91c4c39



---This project was created for learning purposes and to practice C++ and game system design---
