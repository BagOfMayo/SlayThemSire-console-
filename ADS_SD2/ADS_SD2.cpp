#include <iostream>
#include <iomanip>
#include <fstream>
#include <windows.h>
#include <nlohmann/json.hpp>
#include <string>

#include <vector>
#include <map>
#include <queue>
#include <list>
#include <deque>

#include <ctime>
#include <cmath>
#include <algorithm>
#include <random>


using json = nlohmann::json;
using namespace std;


const int UniqueCardAmount = 17;
const int MaxCardsInHand = 7;

int bgColor = 0;

struct Card
{
    string name;
    string type;
    int cost = 0;
    int attackValue = 0;
    int defenceValue = 0;
    int repeat = 0;
    int str = 0;
    int mstr = 0;
    int vuln = 0;
    int weak = 0;
    int metal = 0;
    int selfd = 0;
    int eplus = 0;
    int draw = 0;
    int color = 7;
    string image[12];
};

class Entity
{
public:
    string name = "Ironchad";
    int maxhealth = 0;
    int health = 0;
    int block = 0;
    int maxEnergy = 0;
    int energy = 0;
    int status[5] = { 0 };  //S ; MinusS; VCount; WCount; Metallicize
    int color = 7;
    list<Card> deck;
    list<Card> hand;
    queue<Card> pile;
    string image[20];

    Entity(): maxhealth(0), health(0), maxEnergy(3), energy(3) {}
    Entity(int a): maxhealth(a), health(a), maxEnergy(3), energy(3) {}
    ~Entity()
    {
        deck.clear();
        hand.clear();
        while (!pile.empty())
        {
            pile.pop();
        }
        image->clear();
    }
};

Card card_from_json(const json& j);
void SetColor(int textColor, int bgColor);
void GotoXY(int x, int y);

void CoutCardXY(Card card, int x, int y, int color);
void CoutHand(list<Card> hand, int choosing, bool chose, int centerX, int centerY);
void CoutEnemy(Entity* enemy, int choosing, bool chose, int centerX, int centerY);
void CoutLog(deque<string>& log);
void CoutRewards(Entity*& player, vector<Card> uniqueCards);
void CoutEndofBattleScreen(Entity*& player, int level, bool& playing, bool& inGame);
void CoutHelp(int X, int Y);
void CoutPlayer(Entity*& player, vector<Card> uniqueCards, int choosing, bool chose, int X, int Y);


void CreateDeck(list<Card>& deck, vector<Card> uniqueCards);
void CreateDeckE(list<Card>& deck, vector<Card> uniqueCards, int start, int finish);
void CreatePile(queue<Card>& pile, list<Card> deck, list<Card>& hand);

void GiveHand(list<Card>& hand, queue<Card>& pile, list<Card> deck, int amount);
void EnemyBuilder(Entity& enemy, vector<Card> uniqueCards, int type);
void AddCardToPile(queue<Card>& pile, vector<Card> uniqueCards, int index, int amount);
void PlayCard(int cardInHandIndex, Entity*& entity, Entity*& target, deque<string>& log, vector<Card> uniqueCards, int turn);
bool EnoughEnergy(Entity*& player, int cardInHandIndex);
void BuffDecrease(Entity*& entity);

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    ifstream fin("cards.json", ios::in);
    if (!fin) { return 1; }
    json j;
    fin >> j;

    vector<Card> uniqueCards;
    for (const auto& item : j) {
        uniqueCards.push_back(card_from_json(item));
    }

    deque<string> log;

    deque<Entity> entities;

    Entity player1(125);
    CreateDeck(player1.deck, uniqueCards);
    CreatePile(player1.pile, player1.deck, player1.hand);

    Entity enemy1;


    entities.push_front(player1);
    entities.push_back(enemy1);

    Entity* enemy;
    Entity* player = &entities.front();
    
    int randColor;

    bool inGame = true;
    bool playing = false;
    bool inBattle = false;
    bool playerTurn = false;

    int level = 0;
    int turn = 1;
    int enemyType = 0;


    int chosenCard = 0;
    int chose = false;
    int enemyChose = 0;
    int action = true;
    srand(time(NULL));

    while (inGame)
    {
        system("cls");
        GotoXY(90, 21);
        cout << "   WELOCME TO";

        GotoXY(90, 24);
        cout << "P - Play";
        GotoXY(90, 25);
        cout << "H - Help";
        GotoXY(90, 26);
        cout << "E - Exit";


        int level = 0;
        int turn = 1;
        int enemyType = 0;

        player1 = Entity();
        player1.maxhealth = 125;
        player1.health = player1.maxhealth;
        CreateDeck(player1.deck, uniqueCards);
        CreatePile(player1.pile, player1.deck, player1.hand);

        entities.pop_front();
        entities.push_front(player1);

        player = &entities.front();

        while (true)
        {
            log.clear();
            GotoXY(90, 22);
            randColor = rand() % 6 + 9;
            SetColor(randColor, 0);
            cout << "SLAY THEM, SIRE!";
            SetColor(7, 0);

            if ((GetAsyncKeyState('P') & 0x8000))
            {
                playing = true;
                break;
            }
            if ((GetAsyncKeyState('E') & 0x8000))
            {
                GotoXY(90, 50);
                return 0;
            }
            if ((GetAsyncKeyState('H') & 0x8000))
            {
                CoutHelp(160, 2);
            }
            Sleep(200);
        }
        
        
        while (playing)
        {
            if (enemyType != 1 && enemyType != 2)
            {
                level++;
                
                if (level == 3) { enemyType = 4; }
                else { enemyType = level - 1; }

                enemy1 = Entity();
                EnemyBuilder(enemy1, uniqueCards, enemyType);
                CreatePile(enemy1.pile, enemy1.deck, enemy1.hand);
                GiveHand(enemy1.hand, enemy1.pile, enemy1.deck, 3);

                entities.pop_back();
                entities.push_back(enemy1);

                enemy = &entities.back();

                system("cls");


                for (int i = 0; i < 3;i++)
                {
                    CoutRewards(player, uniqueCards);
                    system("cls");
                    Sleep(1000);
                }

                GotoXY(90, 30);
                cout << " _____________ ";
                GotoXY(90, 31);
                cout << "| LEVEL " << level << " / 3 |";
                GotoXY(90, 32);
                cout << "|_____________|";

                CreatePile(player->pile, player->deck, player->hand);
                GiveHand(player->hand, player->pile, player->deck, 5);
                for (int i = 0; i < 5; i++) { player->status[i] = 0; }
                player->block = 0;

                Sleep(2000);
                system("cls");
                turn = 1;
                log.clear();
            }
            else
            {
                enemyType++;
                enemy1 = Entity();
                EnemyBuilder(enemy1, uniqueCards, enemyType);
                CreatePile(enemy1.pile, enemy1.deck, enemy1.hand);
                GiveHand(enemy1.hand, enemy1.pile, enemy1.deck, 3);

                entities.pop_back();
                entities.push_back(enemy1);

                enemy = &entities.back();
            }
            
            Sleep(1000);
            inBattle = true;
            enemyChose = rand() % enemy->hand.size();
            playerTurn = true;
            inBattle = true;
            while (inBattle)
            {
                if (playerTurn)
                {
                    if ((GetAsyncKeyState('A') & 0x8000))
                    {
                        chosenCard--;
                        action = true;
                    }
                    if ((GetAsyncKeyState('D') & 0x8000))
                    {
                        chosenCard++;
                        action = true;
                    }
                    if ((GetAsyncKeyState('W') & 0x8000) && chose == false)
                    {
                        chose = true;
                        action = true;
                    }
                    if ((GetAsyncKeyState('S') & 0x8000) && chose == true)
                    {
                        chose = false;
                        action = true;
                    }
                    

                    if ((GetAsyncKeyState(VK_SPACE) & 0x8000) && player->hand.size() > 0 && chose == true && EnoughEnergy(player, chosenCard))
                    {
                        action = true;
                        PlayCard(chosenCard, player, enemy, log, uniqueCards, turn);
                        chose = false;
                    }

                    if ((GetAsyncKeyState('N') & 0x8000) && playerTurn == true)
                    {
                        action = true;
                        playerTurn = false;
                    }

                    if ((GetAsyncKeyState('M') & 0x8000))
                    {
                        action = true;
                        playing = false;
                        inBattle = false;
                    }
                    if ((GetAsyncKeyState('E') & 0x8000))
                    {
                        system("cls");
                        inGame = false;
                        playing = false;
                        inBattle = false;
                        return 0;
                    }
                }

                if (!playerTurn)
                {
                    player->block += player->status[4];

                    enemy->block = 0;

                    system("cls");
                    CoutEnemy(enemy, enemyChose, true, 92, 11);
                    CoutPlayer(player, uniqueCards, -1, false, 63, 46);
                    CoutLog(log);
                    CoutHelp(160, 2);
                    Sleep(1000);

                    auto it = enemy->hand.begin();
                    advance(it, enemyChose);
                    int isAnAttack = false;
                    if (it->type == "Attack") { isAnAttack = true; }

                    PlayCard(enemyChose, enemy, player, log, uniqueCards, turn);

                    if (player->health <= 0 || enemy->health <= 0)
                    {
                        inBattle = false;
                        Sleep(500);
                        break;
                    }
                    Sleep(1000);
                    
                    if(isAnAttack){ bgColor = 4; }
                    
                    system("cls");
                    CoutEnemy(enemy, -1, false, 92, 11);
                    CoutPlayer(player, uniqueCards, -1, false, 63, 46);
                    CoutLog(log);
                    CoutHelp(160, 2);
                    Sleep(200);
                    bgColor = 0;

                    

                    system("cls");
                    CoutEnemy(enemy, -1, false, 92, 11);
                    CoutPlayer(player, uniqueCards, -1, false, 63, 46);
                    CoutLog(log);
                    CoutHelp(160, 2);

                    Sleep(1000);

                    //deleting 1 card from player
                    if (player->hand.size() > 0)
                    {
                        int discardCard = rand() % player->hand.size() + (rand() % player->hand.size()*2);
                        auto it = player->hand.begin();
                        while (discardCard != 0)
                        {
                            it++;
                            if (it == player->hand.end())
                            {
                                it = player->hand.begin();
                            }
                            discardCard--;
                        }

                        player->hand.erase(it);
                    }

                    system("cls");
                    CoutEnemy(enemy, -1, false, 92, 11);
                    CoutPlayer(player, uniqueCards, -1, false, 63, 46);
                    CoutLog(log);
                    CoutHelp(160, 2);
                    Sleep(1000);

                    //giving 3 cards
                    for (int i = 0; i < 3; i++)
                    {
                        GiveHand(player->hand, player->pile, player->deck, 1);

                        system("cls");
                        CoutEnemy(enemy, -1, false, 92, 11);
                        CoutPlayer(player, uniqueCards, -1, false, 63, 46);
                        CoutLog(log);
                        CoutHelp(160, 2);
                        Sleep(1000);
                    }


                    GiveHand(enemy->hand, enemy->pile, enemy->deck, 1);


                    system("cls");
                    CoutEnemy(enemy, -1, false, 92, 11);
                    CoutPlayer(player, uniqueCards, -1, false, 63, 46);
                    CoutLog(log);
                    CoutHelp(160, 2);
                    Sleep(1000);

                    system("cls");
;
                    enemyChose = rand() % enemy->hand.size();


                    playerTurn = true;

                    BuffDecrease(enemy);
                    BuffDecrease(player);
                    player->block = 0;

                    chosenCard = 0;

                    turn++;
                }

                if (action)
                {
                    system("cls");
                    action = false;
                }

                CoutEnemy(enemy, enemyChose, false, 92, 11);
                CoutPlayer(player, uniqueCards, chosenCard, chose, 63, 46);
                CoutLog(log);
                CoutHelp(160, 2);

                Sleep(200);


                if (enemy->color == 4)
                {
                    if (enemy->name == "Jaw Cat") { enemy->color = 3; }
                    else if (enemy->name == "The Champ") { enemy->color = 15; }
                    else { enemy->color = 10; }
                }

                if (player->health <= 0 || enemy->health <= 0)
                {
                    inBattle = false;
                    Sleep(500);
                }
            }

            if (enemyType != 1 && enemyType != 2)
            {
                if (inGame && playing)
                {
                    CoutEndofBattleScreen(player, level, playing, inGame);
                }
            }
        }
    }
}

Card card_from_json(const json& j)
{
    Card c;
    c.name = j.value("name", "");
    c.type = j.value("type", "");
    c.cost = j.value("cost", 0);
    c.attackValue = j.value("attackValue", 0);
    c.defenceValue = j.value("defenceValue", 0);
    c.repeat = j.value("repeat", 0);
    c.str = j.value("str", 0);
    c.mstr = j.value("mstr", 0);
    c.vuln = j.value("vuln", 0);
    c.weak = j.value("weak", 0);
    c.metal = j.value("metal", 0);
    c.selfd = j.value("selfd", 0);
    c.eplus = j.value("eplus", 0);
    c.draw = j.value("draw", 0);
    c.color = j.value("color", 7);

    if (j.contains("image") && j["image"].is_array())
    {
        for (size_t i = 0; i < 12 && i < j["image"].size();i++)
        {
            c.image[i] = j["image"][i].get<string>();
        }
    }
    return c;
}

void SetColor(int textColor, int bgColor)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (bgColor << 4) | textColor);
}

void GotoXY(int x, int y)
{
    SetConsoleCursorPosition(
        GetStdHandle(STD_OUTPUT_HANDLE), { static_cast<SHORT>(x), static_cast<SHORT>(y) });
}

void CreateDeck(list<Card>& deck, vector<Card> uniqueCards)
{
    deck.push_front(uniqueCards[0]);
    deck.push_front(uniqueCards[0]);
    deck.push_front(uniqueCards[0]);
    deck.push_front(uniqueCards[0]);

    deck.push_front(uniqueCards[1]);
    deck.push_front(uniqueCards[1]);
    deck.push_front(uniqueCards[1]);
    deck.push_front(uniqueCards[1]);

    deck.push_front(uniqueCards[2]);
    deck.push_front(uniqueCards[2]);
}

void CreateDeckE(list<Card>& deck, vector<Card> uniqueCards, int start, int finish)
{
    for (int i = start; i < finish;i++)
    {
        deck.push_front(uniqueCards[i]);
        deck.push_front(uniqueCards[i]);
        deck.push_front(uniqueCards[i]);
    }
}

void CreatePile(queue<Card>& pile, list<Card> deck, list<Card>& hand)
{
    map<string, int> handCardCount;
    for (auto& card : hand) { handCardCount[card.name]++; }

    vector<Card> tempVector(deck.begin(), deck.end());

    for (int i = 0; i < tempVector.size();i++)
    {
        if (handCardCount[tempVector[i].name] > 0)
        {
            handCardCount[tempVector[i].name]--;
            tempVector.erase(tempVector.begin() + i);
            i--;
        }
    }

    random_device rd;
    mt19937 g(rd());
    shuffle(tempVector.begin(), tempVector.end(), g);

    queue<Card> newPile;

    for (auto& card : tempVector) { newPile.push(card); }

    pile = move(newPile);
}

void GiveHand(list<Card>& hand, queue<Card>& pile, list<Card> deck, int amount)
{
    while (amount > 0 && hand.size() < MaxCardsInHand)
    {
        if (pile.empty())
        {
            CreatePile(pile, deck, hand);
        }
        Card tempCard;
        tempCard = pile.front();
        pile.pop();

        hand.push_back(tempCard);
        amount--;

        Sleep(200);
    }
}

void CoutCardXY(Card card, int x, int y, int color)
{
    for (int i = 0; i <= 11;i++)
    {
        GotoXY(x, y+i);
        SetColor(color, 0);
        cout << card.image[i];
        SetColor(7, 0);
    }
}

void CoutHand(list<Card> hand, int choosing, bool chose, int centerX, int centerY)
{
    int handSize = hand.size();
    
    int offset = 0;
    centerY -= 6;
    

    if (hand.size() <= 5) { offset = (15 - hand.size() * 3); }

    int totalWidth = handSize * 10 + (handSize - 1) * offset;
    int startX = centerX - totalWidth / 2;

    int xPos = 0;
    int xPos2 = 0;

    int tempChoosing = choosing;
    auto it = hand.begin();
    Card* highlight = nullptr;

    while (tempChoosing != 0 && !hand.empty())
    {
        if (tempChoosing < 0)
        {
            if (it == hand.begin())
            {
                it = hand.end();
            }
            it--;
            tempChoosing++;
        }
        else
        {
            it++;
            if (it == hand.end())
            {
                it = hand.begin();
            }
            tempChoosing--;
        }
    }
    
    int i = 0;
    for (auto& card : hand)
    {
        xPos = startX + (i * (10 + offset));
        if (&card == &(*it))
        {
            highlight = &card;
            xPos2 = xPos;
        }
        else
        {
            if (chose) { CoutCardXY(card, xPos, centerY, 8); }
            else { CoutCardXY(card, xPos, centerY, 7); }
        }
        
        i++;
    }

    if (highlight != nullptr)
    {
        if (chose) { CoutCardXY(*highlight, xPos2, centerY-2, 15); }
        else { CoutCardXY(*highlight, xPos2, centerY-1, 15); }
    }
}

void EnemyBuilder(Entity& enemy, vector<Card> uniqueCards, int type)
{
    int cardStart = UniqueCardAmount + type * 3;
    int cardEnd = UniqueCardAmount + (type + 1) * 3;
    if (type >= 3)
    {
        cardStart--;
        cardEnd--;
    }
    CreateDeckE(enemy.deck, uniqueCards, cardStart, cardEnd);

    switch (type)
    {
    case 0:
        enemy.name = "Jaw Cat";
        enemy.color = 3;
        enemy.health = 50;

        //enemy.image[0] = "--█▄▀▓▒░-----------------|-------------------------";
        enemy.image[0] = u8"                                                   ";
        enemy.image[1] = u8"                                                   ";
        enemy.image[2] = u8"                                                   ";
        enemy.image[3] = u8"                                                   ";
        enemy.image[4] = u8"                                                   ";
        enemy.image[5] = u8"                      ▄██                          ";
        enemy.image[6] = u8"              ▄▀    ▄████                          ";
        enemy.image[7] = u8"       ▄  ▀▄▄██████▓█████▄▄▄ ▄▄                    ";
        enemy.image[8] = u8"     ▄ ▄██████████████████████▄ ▄                  ";
        enemy.image[9] = u8"    ▄████████████████████████████▄ ▄▄              ";
        enemy.image[10] = u8"  ▄█████████▓  ██████▓██▓██████▓████▄               ";
        enemy.image[11] = u8"  ██████████████████▒▓█▒▓███████▓▓█████▄▀           ";
        enemy.image[12] = u8"  ▀░▄░▀ ▀█▀▀██████████████████████▓████████▄▄▄▄     ";
        enemy.image[13] = u8"  ▀███▄█▄▄████████████████▓▒▒██████████▀▀███▀▀▀██▄▄ ";
        enemy.image[14] = u8"    ▀▀████████████████▓▒████▓▒▒▀▀▀▀▀▀               ";
        enemy.image[15] = u8"                ▒ ▒░ ▒    ████▓▒                    ";
        enemy.image[16] = u8"                 ▄▄ ▄▄█████▀▀                       ";
        enemy.image[17] = u8"                ▀  █▀  ▀█                           ";
        enemy.image[18] = u8"                                                   ";
        enemy.image[19] = u8"                                                   ";
        break;
    case 1:
        enemy.name = "Acid Slime (L)";
        enemy.color = 10;
        enemy.health = 65;

        //enemy.image[0] = "--█▄▀▓▒░-------------------------------------------";
        enemy.image[0] = u8"                                                   ";
        enemy.image[1] = u8"                                                   ";
        enemy.image[2] = u8"                                                   ";
        enemy.image[3] = u8"      ░░                                           ";
        enemy.image[4] = u8"                          ░                        ";
        enemy.image[5] = u8"                    ░                              ";
        enemy.image[6] = u8"    ░      ▄▄█████████████▒▄▄▄     ░░░             ";
        enemy.image[7] = u8"         ▄█▒▒▓██████████████▓███▄▄                 ";
        enemy.image[8] = u8"      ▄▄███████▓▓███████████▓███████▄   ░          ";
        enemy.image[9] = u8"    ▄█████▓ ▓████████████████████████▄▄            ";
        enemy.image[10] = u8" ▄██████████████████▓ ▓█████████████████▄          ";
        enemy.image[11] = u8" ███▓ ▓█████▒▒▒▒▒███████████████████████████  ░    ";
        enemy.image[12] = u8" ▒███████████▒▒▓▓▒▒█████▓ ▓██████████████████▄▄    ";
        enemy.image[13] = u8" █▓█████▓ ▓███▓▓██████████████████████████████▓▒▒  ";
        enemy.image[14] = u8" █▓██████████████▓ ▓███████████████████▓███████    ";
        enemy.image[15] = u8"  ▀████████████████████▓█████████████▓▒████▀▀      ";
        enemy.image[16] = u8" ░░  ▀▀█████████▀▀▀▀▀███▓▒▒▓███████████▀▀          ";
        enemy.image[17] = u8"       ░░░    ░░░         ░░▀▀▀▀▀▀▀▀▀▀░░░          ";
        enemy.image[18] = u8"                               ░░░                 ";
        enemy.image[19] = u8"                                                   ";
        break;
    case 2:
        enemy.name = "Acid Slime (M)";
        enemy.color = 10;
        enemy.health = 35;

        //enemy.image[0] = "--█▄▀▓▒░-------------------------------------------";
        enemy.image[0] = u8"                                                   ";
        enemy.image[1] = u8"                                                   ";
        enemy.image[2] = u8"                                                   ";
        enemy.image[3] = u8"                                                   ";
        enemy.image[4] = u8"                                                   ";
        enemy.image[5] = u8"                                                   ";
        enemy.image[6] = u8"                                                   ";
        enemy.image[7] = u8"                                                   ";
        enemy.image[8] = u8"              ░                                    ";
        enemy.image[9] = u8"                   ▄▄▄▄▄▄▄        ░                 ";
        enemy.image[10] = u8"      ░    ▄▄▄███████████▓▓▓▒▒▄▄▄▄▄                ";
        enemy.image[11] = u8"        ▄▄██████████████████████████▄              ";
        enemy.image[12] = u8"       ▄██████▒▒▒▒█████▓ ▓████████████▓▓     ░     ";
        enemy.image[13] = u8"      ▄█▓ ▓█████████████████████████████▒▒         ";
        enemy.image[14] = u8"     ▄████████▓ ▓███████████████████▓████▓▓        ";
        enemy.image[15] = u8"      ▀████████████████▓███████████████▒▓▓█▀       ";
        enemy.image[16] = u8"   ░░   ▀█████▀▀▀▀▀███▓▒▒▓███████▓█▓▓              ";
        enemy.image[17] = u8"                                       ░           ";
        enemy.image[18] = u8"                                                   ";
        enemy.image[19] = u8"                                                   ";
        break;
    case 3:
        enemy.name = "Acid Slime (S)";
        enemy.color = 10;
        enemy.health = 20;

        //enemy.image[0] = "--█▄▀▓▒░-------------------------------------------";
        enemy.image[0] = u8"                                                   ";
        enemy.image[1] = u8"                                                   ";
        enemy.image[2] = u8"                                                   ";
        enemy.image[3] = u8"                                                   ";
        enemy.image[4] = u8"                                                   ";
        enemy.image[5] = u8"                                                   ";
        enemy.image[6] = u8"                                                   ";
        enemy.image[7] = u8"                                                   ";
        enemy.image[8] = u8"                                                   ";
        enemy.image[9] = u8"                         ░                         ";
        enemy.image[10] = u8"                                                   ";
        enemy.image[11] = u8"                   ▄▄████████▄▄▄▄▄                 ";
        enemy.image[12] = u8"               ▄▄▄█████▓ ▓██████████▄    ░         ";
        enemy.image[13] = u8"            ▄█████████████████████████▄            ";
        enemy.image[14] = u8"        ░  ██████████▒▒▒████████████████           ";
        enemy.image[15] = u8"            ▀█████████████████████████▀            ";
        enemy.image[16] = u8"              ▀█████▀▀▀▀▀█████████▓█▀    ░         ";
        enemy.image[17] = u8"                                                   ";
        enemy.image[18] = u8"                                                   ";
        enemy.image[19] = u8"                                                   ";
        break;
    case 4:
        enemy.name = "The Champ";
        enemy.color = 15;
        enemy.health = 250;

        //enemy.image[0] = "--█▄▀▓▒░-------------------------------------------";
        enemy.image[0] = u8"                     ██▄ ▄██▄ ▄██                  ";
        enemy.image[1] = u8"                     ████████████                  ";
        enemy.image[2] = u8"                     █░█░█░█░████                  ";
        enemy.image[3] = u8"                     ████████████                  ";
        enemy.image[4] = u8"                     ████████████     ▄▄▄▄         ";
        enemy.image[5] = u8"          ▒▒▒▒▒▒▒████▓▓▓▓▓▓▓▓▓▓▓▓███▓███████       ";
        enemy.image[6] = u8"         ▒▒▒▒▒█████████████████████▓██████████     ";
        enemy.image[7] = u8"         ▒▒▒███████████████████████▓███████████    ";
        enemy.image[8] = u8"          ▒▓████████████████████████▓▒██▓███████   ";
        enemy.image[9] = u8"          ▒▓████████████████████████▓▒███▓██████   ";
        enemy.image[10] = u8"          ▒▓▓████████▓██████████████▓▒███▓██████    ";
        enemy.image[11] = u8"           ▒▒▓▓▓███████████▓▓▓▓▓▓▓▓▒▒▒▒█████████    ";
        enemy.image[12] = u8"           ▒▒███████████████████████▒▒▒████████     ";
        enemy.image[13] = u8"           ▒▒▒██████████████████████▒  ████████     ";
        enemy.image[14] = u8"          ▒▒▒▒▒█████████████████████  ███████       ";
        enemy.image[15] = u8"         ▒▒▒▒▒ ████████████████████  ███████        ";
        enemy.image[16] = u8"        ░░░░░   ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓  ██████          ";
        enemy.image[17] = u8"        ░░░░░   ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓ ▓█▓█▓█          ";
        enemy.image[18] = u8"                ▒▒▒▒           ▒▒▒▒                 ";
        enemy.image[19] = u8"               ▒▒▒▒▒          ▒▒▒▒▒                 ";
        break;
    default:
        break;
    }
}

void CoutEnemy(Entity* enemy, int choosing, bool chose, int centerX, int centerY)
{
    
    for (int i = 0; i < 20; i++)
    {
        GotoXY(centerX - 25, centerY - 10 + i);
        SetColor(enemy->color, 0);
        cout << enemy->image[i];
        SetColor(7, 0);
    }
    GotoXY(centerX - 14, centerY + 11);
    cout << "ENEMY HEALTH: ";
    SetColor(4, 0);
    cout << enemy->health;
    SetColor(7, 0);
    GotoXY(centerX + 5, centerY + 11);
    cout << " BLOCK: ";
    SetColor(9, 0);
    cout << enemy->block;
    SetColor(7, 0);
    GotoXY(centerX - 17, centerY + 12);
    cout << "STATUS: ";
    //int status[5] = { 0 };  //S ; MinusS; VCount; WCount; Metallicize
    SetColor(12, 0);
    cout << "S: " << enemy->status[0];

    SetColor(6, 0);
    cout << " MS: " << enemy->status[1];

    SetColor(5, 0);
    cout << " V: " << enemy->status[2];

    SetColor(1, 0);
    cout << " W: " << enemy->status[3];

    SetColor(3, 0);
    cout << " M: " << enemy->status[4];
    SetColor(7, 0);

    CoutHand(enemy->hand, choosing, chose, centerX-2, centerY + 21);
}

void AddCardToPile(queue<Card>& pile, vector<Card> uniqueCards, int index, int amount)
{
    for (int i = 0; i < amount;i++)
    {
        pile.push(uniqueCards[index]);
    }

    vector<Card> tempv;
    while (!pile.empty())
    {
        tempv.push_back(pile.front());
        pile.pop();
    }

    random_device rd;
    mt19937 g(rd());
    shuffle(tempv.begin(), tempv.end(), g);

    for (auto& card : tempv) { pile.push(card); }
}

void PlayCard(int cardInHandIndex, Entity*& entity, Entity*& target, deque<string>& log, vector<Card> uniqueCards, int turn)
{
    Card card;
    auto it = entity->hand.begin();

    int tempChoosing = cardInHandIndex;
    while (tempChoosing != 0)
    {
        if (tempChoosing < 0)
        {
            if (it == entity->hand.begin())
            {
                it = entity->hand.end();
            }
            it--;
            tempChoosing++;
        }
        else
        {
            it++;
            if (it == entity->hand.end())
            {
                it = entity->hand.begin();
            }
            tempChoosing--;
        }
    }

    card = *it;
    entity->hand.erase(it);

    entity->energy -= card.cost;
    
    log.push_front("Turn " + to_string(turn) + ((turn < 10) ? " " : "") + "| " + entity->name + " played '" + card.name + "'");
    string spacing = (turn < 10) ? "         " : "        ";

    if (card.type == "Attack") { target->color = 4; }

    if (card.attackValue > 0)
    {
        int damageValue = ceil(((card.attackValue + entity->status[0]) * (1 + target->status[2] * 0.25) * (1 - min(0.5, entity->status[3] * 0.25))));
        GotoXY(50, 50);

        if (card.name == "Shield Slam")
        {
            damageValue = ceil(((entity->block + entity->status[0]) * (1 + target->status[2] * 0.25) * (1 - min(0.5, entity->status[3] * 0.25))));
        }
        if (card.name == "Heavy Blade")
        {
            damageValue = ceil(((card.attackValue + entity->status[0] * 3) * (1 + target->status[2] * 0.25) * (1 - min(0.5, entity->status[3] * 0.25))));
        }

        int temp = card.repeat+1;
        for (int i = 0; i < temp;i++)
        {
            if (target->block >= damageValue)
            {
                if (damageValue != 0)
                {
                    log.push_front(spacing + target->name + " blocked " + to_string(damageValue) + " D");
                    target->block -= damageValue;
                }
            }
            else
            {
                if (target->block > 0)
                {
                    log.push_front(spacing + target->name + " blocked " + to_string(target->block) + " D");
                }
                target->health -= damageValue - target->block;
                log.push_front(spacing + target->name + " took " + to_string(damageValue - target->block) + " D");
                target->block = 0;
            }
        }
    }

    if (card.defenceValue > 0)
    {
        log.push_front(spacing + entity->name + " raised it's block by " + to_string(card.defenceValue));
        entity->block += card.defenceValue;
    }

    if (card.str > 0)
    {
        log.push_front(spacing + entity->name + " gained " + to_string(card.str) + " S");
        entity->status[0] += card.str;
    }

    if (card.mstr > 0)
    {
        log.push_front(spacing + entity->name + " gained " + to_string(card.mstr) + " MS");
        entity->status[1] += card.mstr;
    }

    if (card.vuln > 0)
    {
        log.push_front(spacing + entity->name + " applyed " + to_string(card.vuln) + " V to " + target->name);
        target->status[2] += card.vuln;
    }

    if (card.weak > 0)
    {
        log.push_front(spacing + entity->name + " applyed " + to_string(card.weak) + " W to " + target->name);
        target->status[3] += card.weak;
    }

    if (card.metal > 0)
    {
        log.push_front(spacing + entity->name + " gained " + to_string(card.metal) + " M");
        entity->status[4] += card.metal;
    }

    if (card.eplus > 0)
    {
        if (card.name == "Dropkick")
        {
            if (target->status[2] > 0)
            {
                log.push_front(spacing + entity->name + " gained " + to_string(card.eplus) + " E");
                entity->energy += card.eplus;
            }
        }
        else
        {
            log.push_front(spacing + entity->name + " gained " + to_string(card.eplus) + " E");
            entity->energy += card.eplus;
        }
    }

    if (card.selfd > 0)
    {
        log.push_front(spacing + entity->name + " sacraficed " + to_string(card.selfd) + " Health");
        entity->health -= card.selfd;
        
    }

    if (card.draw > 0)
    {
        if (card.name == "Dropkick")
        {
            if (target->status[2] > 0)
            {
                GiveHand(entity->hand, entity->pile, entity->deck, card.draw);
                log.push_front(spacing + entity->name + " draws " + to_string(card.draw) + " C");
            }
        }
        else
        {
            GiveHand(entity->hand, entity->pile, entity->deck, card.draw);
            log.push_front(spacing + entity->name + " draws " + to_string(card.draw) + " C");
        }
    }

    if (card.name == "Power Through")
    {
        AddCardToPile(entity->pile, uniqueCards, 16, 2);
        log.push_front(spacing + "2 'Wound' were added to the draw pile of " + entity->name);
    }

    if (card.name == "Acid Spit...")
    {
        AddCardToPile(target->pile, uniqueCards, 16, 2);
        log.push_front(spacing + "2 'Wound' were added to the draw pile of " + target->name);
    }

    if (card.name == "Acid Spit.." || card.name == "Acid Spit.")
    {
        AddCardToPile(target->pile, uniqueCards, 16, 1);
        log.push_front(spacing + "1 'Wound' were added to the draw pile of " + target->name);
    }

    if (card.name == "Anger")
    {
        entity->status[1] = 0;
        entity->status[2] = 0;
        entity->status[3] = 0;
    }

    if (entity->health < 0) { entity->health = 0; }
    if (target->health < 0) { target->health = 0; }
}

void CoutLog(deque<string>& log)
{
    for (int i = 0; i < log.size();i++)
    {
        if (i > 60) { break; }
        GotoXY(2, 60-i);
        cout << log.at(i);
    }
}

void CoutRewards(Entity*& player, vector<Card> uniqueCards)
{
    system("cls");

    int randcard1 = rand() % 12 + 3;
    int randcard2 = rand() % 12 + 3;
    while(randcard2 == randcard1) { randcard2 = rand() % 12 + 3; }
    int randcard3 = rand() % 12 + 3;
    while (randcard3 == randcard1 || randcard3 == randcard2) { randcard3 = rand() % 12 + 3; }

    list<Card> rewards;
    rewards.push_front(uniqueCards[randcard1]);
    rewards.push_front(uniqueCards[randcard2]);
    rewards.push_front(uniqueCards[randcard3]);

    int chosenCard = 0;
    bool chose = false;
    bool action = true;
    while (true)
    {
        if ((GetAsyncKeyState('A') & 0x8000))
        {
            chosenCard--;
            action = true;
        }
        if ((GetAsyncKeyState('D') & 0x8000))
        {
            chosenCard++;
            action = true;
        }
        if ((GetAsyncKeyState('W') & 0x8000) && chose == false)
        {
            chose = true;
            action = true;
        }
        if ((GetAsyncKeyState('S') & 0x8000) && chose == true)
        {
            chose = false;
            action = true;
        }
        if ((GetAsyncKeyState(VK_SPACE) & 0x8000) && chose == true)
        {
            action = true;
            auto it = rewards.begin();

            int tempChoosing = chosenCard;
            while (tempChoosing != 0)
            {
                if (tempChoosing < 0)
                {
                    if (it == rewards.begin())
                    {
                        it = rewards.end();
                    }
                    it--;
                    tempChoosing++;
                }
                else
                {
                    it++;
                    if (it == rewards.end())
                    {
                        it = rewards.begin();
                    }
                    tempChoosing--;
                }
            }

            player->deck.push_back(*it);
            break;
        }


        if (action)
        {
            system("cls");
            action = false;
        }

        GotoXY(76, 24);
        cout << "CHOOSE A CARD TO ADD TO YOUR DECK";
        CoutHand(rewards, chosenCard, chose, 90, 35);

        Sleep(200);
    }
}

void CoutEndofBattleScreen(Entity*& player, int level, bool& playing, bool& inGame)
{
    system("cls");
    int menuX = 90;
    int menuY = 25;
    if (player->health <= 0)
    {
        GotoXY(menuX, menuY);
        cout << "YOU'VE LOST!";
    }
    else
    {
        GotoXY(menuX, menuY);
        cout << "YOU'VE WON!";
        if (level < 3)
        {
            GotoXY(menuX, menuY + 1);
            cout << "N - CONTINUE";
            menuY++;
        }
    }
    GotoXY(menuX, menuY + 1);
    cout << "M - TO THE MENU";
    GotoXY(90, menuY + 2);
    cout << "E - EXIT";


    while (true)
    {
        if ((GetAsyncKeyState('E') & 0x8000))
        {
            inGame = false;
            playing = false;
            break;
        }
        if ((GetAsyncKeyState('M') & 0x8000))
        {
            playing = false;
            break;
        }
        if ((GetAsyncKeyState('N') & 0x8000) && player->health > 0 && level < 3)
        {
            playing = true;
            break;
        }

        Sleep(200);
    }

    //player.health = player.maxhealth;
    player->energy = player->maxEnergy;
    player->hand.clear();

    while (!player->pile.empty()) { player->pile.pop(); }
}

bool EnoughEnergy(Entity*& player, int cardInHandIndex)
{
    if (player->hand.size() < 1) { return false; }
    auto it = player->hand.begin();

    
    int tempChoosing = cardInHandIndex;
    while (tempChoosing != 0)
    {
        if (tempChoosing < 0)
        {
            if (it == player->hand.begin())
            {
                it = player->hand.end();
            }
            it--;
            tempChoosing++;
        }
        else
        {
            it++;
            if (it == player->hand.end())
            {
                it = player->hand.begin();
            }
            tempChoosing--;
        }
    }
    

    if (player->energy >= it->cost)
    {
        return true;
    }
    return false;
}

void CoutHelp(int X, int Y)
{
    int currentY = Y;
    X -= 5;

    GotoXY(X, currentY++);
    cout << "-----------------HELP-----------------";
    GotoXY(X, currentY++);
    cout << "At the end of every turn player discards 1 Card and draws 2 from their draw pile.";
    GotoXY(X, currentY++);
    cout << "At the end of every turn enemy draws 1 Card from their draw pile.";
    GotoXY(X, currentY++);
    cout << "Cannot play cards whose Energy is greater that your current Energy";
    GotoXY(X, currentY++);
    cout << "Press N to end your turn";
    GotoXY(X, currentY++);
    cout << "Press E to exit";
    GotoXY(X, currentY++);
    cout << "Press M to go to the menu";
    GotoXY(X, currentY++);
    cout << "Press A/D to cycle between cards";
    GotoXY(X, currentY++);
    cout << "Press W/S to select/deselect a card";
    GotoXY(X, currentY++);
    cout << "Press SPACE to play selected card";
    GotoXY(X, currentY++);
    cout << "--------------CARD VALUES--------------";
    GotoXY(X, currentY++);
    cout << "D - Damage";
    GotoXY(X, currentY++);
    cout << "S - Strength";
    GotoXY(X, currentY++);
    cout << "MS - At the end of a turn: minus X S and become 0";
    GotoXY(X, currentY++);
    cout << "V - Vulnerability, decreases by 1 every turn";
    GotoXY(X, currentY++);
    cout << "W - Weakness, decreases by 1 every turn";
    GotoXY(X, currentY++);
    cout << "M - At the end of a turn: plus X Block";
    GotoXY(X, currentY++);
    cout << "E - Energy";
    GotoXY(X, currentY++);
    cout << "C - Card";
    GotoXY(X, currentY++);
    cout << "Reapeat X - Action will be executed X times";
    GotoXY(X, currentY++);
    cout << "Damage Calculation:";
    GotoXY(X, currentY++);
    cout << "ceil(((D + S) * (1 + V(enemy) * 0.25) * (1 - min(0.5, W * 0.25))))";
    GotoXY(X, currentY++);
    cout << "Block - negates X Damage";
    GotoXY(X, currentY++);
    cout << "Block becomes 0 at the start of a turn";
    
}

void CoutPlayer(Entity*& player, vector<Card> uniqueCards, int choosing, bool chose, int X, int Y)
{
    GotoXY(X, Y - 2);
    cout << "ENERGY: ";
    SetColor(6, 0);
    cout << player->energy << " / " << player->maxEnergy;
    SetColor(7, 0);

    CoutHand(player->hand, choosing, chose, X + 27, Y + 7);


    GotoXY(X + 73, Y);
    cout << "DRAW PILE";
    GotoXY(X + 73, Y + 1);
    CoutCardXY(uniqueCards[uniqueCards.size() - 1], 133, 47, 8);

    SetColor(7, bgColor);
    GotoXY(X + 12, Y + 14);
    cout << "      HEALTH: ";
    SetColor(4, bgColor);
    cout << player->health;
    SetColor(7, bgColor);
    GotoXY(X + 30, Y + 14);
    cout << " BLOCK: ";
    SetColor(9, bgColor);
    cout << player->block << ((player->block < 10) ? "      " : "     ");
    SetColor(7, bgColor);
    GotoXY(X + 12, Y + 15);
    cout << "STATUS: ";
    SetColor(12, bgColor);
    cout << "S: " << player->status[0];

    SetColor(6, bgColor);
    cout << " MS: " << player->status[1];

    SetColor(5, bgColor);
    cout << " V: " << player->status[2];

    SetColor(1, bgColor);
    cout << " W: " << player->status[3];

    SetColor(3, bgColor);
    cout << " M: " << player->status[4];
    SetColor(7, 0);
}

void BuffDecrease(Entity*& entity)
{
    entity->energy = entity->maxEnergy;
    entity->status[0] -= entity->status[1];
    entity->status[1] = 0;
    entity->status[2]--;
    entity->status[3]--;
    if (entity->status[2] < 0) { entity->status[2] = 0; }
    if (entity->status[3] < 0) { entity->status[3] = 0; }
}