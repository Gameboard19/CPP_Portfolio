// ConsoleSurvivalGame.cpp : This file contains the 'main' function. Program execution begins and ends there.
// purpose of this program is to be an entertaining fun little game for me to make. it is also to experiment with pointers.

#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <iomanip>
#include <cmath>
#include <ctime>
#include <random>
#include <map>
#include <conio.h>

/*      TO DO LIST
-CLEAN UP CODE
    -I WILL DO THIS, BUT NOT RIGHT NOW. 
-Multiplayer?
    -this would require a complete reworking of the game/ how the monsters behave.
        -fortunately, some framework for this new change is already present.
-Allow for different room sizes (recommended from myah)
    -specifically on x axis.
        -just have a global short variable xLength, have it default to 10, then change it to be whatever value room::loadedLevel[0].length(); is
    -It's a really good idea, so thank you, ma'am.
-TODO: while it is working most of the time, the game has gotten more unstable. you need to figure out whats causing this.
    -I am almost certain it is somewhere in the constructor room::room()
        -I guess put some cout lines in there for debugging and run the code a bunch
-Improve room spawning
    -use # in room file to prevent containers from spawning in key spots
        -then overwrite each '#' with ' '
            -this guarantees containers will (hopefully) not softlock you.
-Fix taking items from containers 
    -Currently, it just erases whatever item was selected from the inventory, but this results in an empty space that crashed the game when you try to put it in your inventory
    -fix this by additionally bringing every entry after it down 1 space and reduce the variable invSize by 1
    -I'll get to this eventually I swear.
        -It's just going to be annoying af to do and I've at least changed it for now so you can't crash it with this.
-Add more monster types ////DONE
    -this will be complicated but a good start would be to make NOTHINGTURN_MOVES into a variable assigned a value when a new monster is created.
    -the following are monster ideas
        -"Nothing" is the generic one, just have it move 2 times a turn.
        -"Ghost" moves only once a turn, but isn't visible. have him deal mind damage.
        -"Flicker" can only be seen every other turn. rn just have it move at the same speed as "Nothing"
        -"Demon" moves 3 times a turn. this one should be especially rare
        -"Glitch" does wacky stuff. elaborate on this later.
-Create function to delete previous room and generate a new one ////DONE
    -takes place in the following steps
        -delete contents of containers
        -delete containers
        -delete door
        -delete monster
            -set the bools relating to monster spawing to false
        -reset player->pos.x and player->pos.y
        -room = new room
        -monster = new monster
    -will be assigning '+' in player_action() to debugging this function.
    -this mfer is bugged (evidence suggests some error with assigning memory) until this gets fixed some debug text will be appearing
-(Eventually) remove the debug Keys in player_action() ////DONE
    -these include
        - '-'
        - '+'
-make it where the likelihood of specific items spawning is slightly modified by your stat conditions. ////DONE? (see item::statItem())
-change game to dynamically adapt the x coords length depending on the file's x length
    -this shouldn't be all that difficult it'll just take me 2 hours to do it.
-make a debug menu
    -clear up more room to use keys
    -debug menus are cool
    -can add wacky stuff
*/
using namespace std;

bool game_active = true;
struct coords {
    int x;
    int y;
};
//input handler
//these bools have to do with debug testing
bool neverHungry = false;
bool neverHurt = false;
bool alwaysSane = false;
bool debug_displayProcess = false;
void input_handler() {
    cin.clear();
    cin.ignore(1000, '\n');
    cout << "Bad input. Try again.";
}
void confirm() {
    cout << "Press anything to continue..." << endl;
    _getch();
    system("cls");
}
int random(int low, int high) {
    return low + rand() % (high - low + 1);
}
void EndGame(string cause) {
    cout << "You have died of " << cause << "..." << endl;
    int splashtextvalue = random(1, 10);
    switch (splashtextvalue) {
    case 1:
        cout << "Have you considered...\n...Like...\n...Not dying?" << endl;
        break;
    case 2:
        cout << "Dying sucks so maybe, I don't know, don't die?..." << endl;
        break;
    case 3:
        cout << "\"Dude I'm telling you " << cause << " needs to be nerfed istg man...\"" << endl;
        break;
    case 4:
        cout << "A loser is you!" << endl;
        break;
    case 5:
        cout << "\"I wonder if there's a wiki for this game...\" (there isn't)" << endl;
        break;
    case 6:
        cout << "That hurt." << endl;
        break;
    default:
        cout << "Better luck next time!" << endl;
        break;
    }
    game_active = false;
    confirm();
    return;
}
//this is a minigame where the player has a limited number of turns to add and subtract numbers to get to a specific number. this will be used for lockpicking
bool lockpickMinigame() {
    bool failed;
    int attempts = random(10, 13);
    int goalNumber = random(attempts * random(1, 5), attempts * 9);
    int currentValue = 0;
    bool adding = true;
    char inputValue = 0;
    while (attempts > 0) {
        system("cls");
        cout << "***** PICKING LOCK... *****" << endl;
        cout << "Attempts Left: " << attempts << endl;
        cout << "Goal Number: " << goalNumber << endl;
        cout << "Current Value: " << currentValue << endl;
        cout << "Mode: " << (adding ? "Adding" : "Subtracting") << endl;
        cout << "press a number between 1 and 9, or press \'+\' or \'-\'" << endl;
        inputValue = _getch();
        switch (inputValue) {
        case '1':
            if (adding) {
                currentValue += 1;
            }
            else {
                currentValue -= 1;
            }
            attempts--;
            break;
        case '2':
            if (adding) {
                currentValue += 2;
            }
            else {
                currentValue -= 2;
            }
            attempts--;
            break;
        case '3':
            if (adding) {
                currentValue += 3;
            }
            else {
                currentValue -= 3;
            }
            attempts--;
            break;
        case '4':
            if (adding) {
                currentValue += 4;
            }
            else {
                currentValue -= 4;
            }
            attempts--;
            break;
        case '5':
            if (adding) {
                currentValue += 5;
            }
            else {
                currentValue -= 5;
            }
            attempts--;
            break;
        case '6':
            if (adding) {
                currentValue += 6;
            }
            else {
                currentValue -= 6;
            }
            attempts--;
            break;
        case '7':
            if (adding) {
                currentValue += 7;
            }
            else {
                currentValue -= 7;
            }
            attempts--;
            break;
        case '8':
            if (adding) {
                currentValue += 8;
            }
            else {
                currentValue -= 8;
            }
            attempts--;
            break;
        case '9':
            if (adding) {
                currentValue += 9;
            }
            else {
                currentValue -= 9;
            }
            attempts--;
            break;
        case '+':
            adding = true;
            break;
        case '-':
            adding = false;
            break;
        default:
            break;
        }
        if (currentValue == goalNumber) {
            failed = false;
            break;
        }
        else {
            failed = true;
        }
    }
    if (failed) {
        cout << "You failed to pick the lock..." << endl;
    }
    else {
        cout << "You managed to pick the lock..." << endl;
    }
    return failed;
}
//bool key_exists will be used quite often in here so I will go ahead and define it here.
bool key_exists;
//this also will be relevant but unsure what to do with yet so up here it goes
short roomNum = 1;
//this will be used for determining what the lowest value stat of player is for item spawning
enum statRef {
    HEALTH,
    HUNGER,
    MIND
};
statRef lowest = HEALTH;
short xlength = 10;
//constants
const short OVEREAT_PENALTY = 125;
const short FOODPOISON_PENALTY = 60;
const short ITEM_SPAWNCHANCE = 60;
const short MRE_SPAWNCHANCE = 60;
const short DOOR_LOCKCHANCE = 75;
const short MOVE_HUNGERCOST = 1;
const short NOTHING_CHANCE = 30;
//these aren't actually the avg amount of moves its just a number I can shift up and down to universally shift how long the monster takes to spawn
const short MONSTERMOVES_AVGMIN = 10;
const short MONSTERMOVES_AVGMAX = 20;
const short MONSTERMOVES_MINDPENALTY = 1;
const short MONSTERWAKE_MINDPENALTY = 5;
// number of different types of numbers (for generation purposes.)
const unsigned int ITEM_TYPES = 9;
const unsigned int CONTAINER_MAXSIZE = 3;
//items are things the player can pick up. when a player opens a container it will generate one of a couple items. if the door to the next room is locked at least one container in the room is guaranteed to contain a key.
class door {
public:
    coords pos_left;
    coords pos_right;
    bool isLocked;
    char doorChars[2];
    void checkState() {
        if (isLocked) {
            doorChars[0] = '{';
            doorChars[1] = '}';
        }
        else {
            doorChars[0] = '[';
            doorChars[1] = ']';
        }
    }
    void setPos_right() {
        pos_right.x = pos_left.x + 1;
        pos_right.y = pos_left.y;
    }
    door() {
        //gets a random number to see if the door will be locked
        isLocked = random(1, 100) < DOOR_LOCKCHANCE;
        if (isLocked == false) {
            //this is currently bugged. it ends up generating a key anyway if it isn't locked. need to fix that.
            key_exists = true;
        }
        else {
            key_exists = false;
        }
        pos_left.x = random(1, xlength -3);
        pos_left.y = 9;
        setPos_right();
        checkState();
    }
    void resetDoor() {
        //gets a random number to see if the door will be locked
        isLocked = random(1, 100) < DOOR_LOCKCHANCE;
        if (isLocked == false) {
            //this is currently bugged. it ends up generating a key anyway if it isn't locked. need to fix that.
            key_exists = true;
        }
        else {
            key_exists = false;
        }
        pos_left.x = random(1, xlength - 3);
        pos_left.y = 9;
        setPos_right();
        checkState();
    }
};
class item {
    //Currently planned items include:
    //Medkit (Health [large])
    //Old Sandwich (Hunger [large, risky])
    //Key
    //Shard of Glass (Mind [trades health])
    //Rose (Mind [small])
    //Granola Bar (Hunger [small]
    //Bandages (Health [small])
    //
public:
    //these are only used for notes.
    string notes[5] = {"essay", "file1", "file2", "file3", "mark1"};
    short notetype;
    ifstream notefile;
    string itemName;
    string tooltip;
    //the following effects are for if the item is a consumable.
    bool isConsumable, isKey = false;
    short health, mind, hunger = 0;
    //likelihood of a food giving you food poisoning (1 - 100).
    short poisonchance = 0;
    //these functions are used to dedclare generate various items;
private:
    void note() {
        itemName = "Note";
        tooltip = "A piece of paper with words on it.";
        isConsumable = false;
        isKey = false;
        health = 0;
        hunger = 0;
        mind = 0;
        poisonchance = 0;
        notetype = random(0, 3);
    }
    void medkit() {
        itemName = "Medical Supplies";
        tooltip = "A bundle of quality medical supplies.";
        isConsumable = true;
        isKey = false;
        health = 60;
        hunger = 0;
        mind = 0;
        poisonchance = 0;
    }
    void sandwich() {
        itemName = "Old Sandwich";
        tooltip = "It's starting to mold, but it'll keep me going for a while.";
        isConsumable = true;
        isKey = false;
        health = 0;
        hunger = 45;
        mind = -10;
        poisonchance = 40;
    }
    void key() {
        itemName = "Key";
        tooltip = "It probably belongs to something nearby.";
        isConsumable = true;
        isKey = true;
        key_exists = true;
    }
    void glassShard() {
        itemName = "Shard of Glass";
        tooltip = "Razor sharp edges. I'll cut myself if I'm not careful.";
        isConsumable = true;
        isKey = false;
        health = -10;
        hunger = 0;
        mind = 20;
        poisonchance = 0;
    }
    void doll() {
        itemName = "Familiar Doll";
        tooltip = "Where have I seen it before?";
        isConsumable = true;
        isKey = false;
        health = 0;
        hunger = 0;
        mind = 40;
        poisonchance = 0;
    }
    void granola() {
        itemName = "Granola Bar";
        tooltip = "Not very nutritious, but it won't make me sick.";
        isConsumable = true;
        isKey = false;
        health = 0;
        hunger = 15;
        mind = 0;
        poisonchance = 0;
    }
    void bandage_dirty() {
        itemName = "Dirty Bandages";
        tooltip = "I bet whatever wound I bandage using these will get infected.";
        isConsumable = true;
        isKey = false;
        health = 10;
        hunger = 0;
        mind = -5;
        poisonchance = 0;
    }
    void mre() {
        itemName = "MRE";
        tooltip = "Meal Ready to Eat. It's... Edible.";
        isConsumable = true;
        isKey = false;
        health = 0;
        hunger = 60;
        mind = 0;
        poisonchance = 0;
    }
    void statItem() {
        switch (lowest) {
        case HEALTH:
            medkit();
            break;
        case HUNGER:
            mre();
            break;
        case MIND:
            doll();
            break;
        default:
            break;
        }
    }
public:
    void randomize() {
        short item_randomizer;
        //probably gonna move the thing that defines the objects over to containers so i can have it roll empty slots if I want it to.
        if (key_exists) {
            item_randomizer = random(1, ITEM_TYPES - 1);
        }
        else {
            item_randomizer = ITEM_TYPES;
        }
        switch (item_randomizer) {
        case 1:
            glassShard();
            break;
        case 2:
            sandwich();
            break;
        case 3:
            medkit();
            break;
        case 4:
            doll();
            break;
        case 5:
            granola();
            break;
        case 6:
            bandage_dirty();
            break;
        case 7:
            //trying to balance mre spawnrates
            if (random(1, 100) < MRE_SPAWNCHANCE) {
                mre();
            }
            else {
                sandwich();
            }
            break;
        case 8:
            statItem();
            break;
        case 9:
            key();
            break;
        }
        //this cout is for debug purposes
        //cout << itemName;
    }
};
//governs containers;
class container {
public:
    char containerChar = 'H';
    coords cPos;
    string containerName;
    const short CONTAINER_NAMECOUNT = 3;
    string names[4] = {"Locker", "Desk", "Chest", "Bag"};
    //FIX THIS
    const short LOCKEDCHANCE = 40;
    map<int, string> inventory_accessKey;
    map<int, item*> cInventory;
    bool isEmpty = true;
    short invSize = 0;
    string lastitem;
    //always equals false until further notice
    //  -further notice has been reached. I am implementing a minigame that opens these.
    bool isLocked;
    container() {
        //invsize may be variable in the future.
        containerName = names[random(1, CONTAINER_NAMECOUNT)];
        int itemchance = random(1, 100);
        lastitem = "1";
        isLocked = (random(1, 100) < LOCKEDCHANCE);
        bool isUnique = true;
        if (key_exists) {
            if (itemchance < ITEM_SPAWNCHANCE) {
                invSize = random(1, CONTAINER_MAXSIZE);
                for (int cInventory_i = 0; cInventory_i < invSize; cInventory_i++) {
                    isEmpty = false;
                    do {
                        item* randitem = new item;
                        randitem->randomize();
                        cInventory[cInventory_i] = randitem;
                        inventory_accessKey[cInventory_i] = randitem->itemName;
                        if (randitem->itemName == lastitem) {
                            isUnique = false;
                        }
                        else {
                            isUnique = true;
                            lastitem = randitem->itemName;
                        }
                        randitem = NULL;
                        delete randitem;
                    } while (isUnique == false);
                }
            }
        }
        else {
            invSize = random(1, CONTAINER_MAXSIZE);
            for (int cInventory_i = 0; cInventory_i < invSize; cInventory_i++) {
                isEmpty = false;
                do {
                    item* randitem = new item;
                    randitem->randomize();
                    cInventory[cInventory_i] = randitem;
                    inventory_accessKey[cInventory_i] = randitem->itemName;
                    if (randitem->itemName == lastitem) {
                        isUnique = false;
                    }
                    else {
                        isUnique = true;
                        lastitem = randitem->itemName;
                    }
                    randitem = NULL;
                    delete randitem;
                } while (isUnique == false);
            }
        }
    }
    //destructors will eventually be used in place of a bunch of different functions that are hard to keep track of for deletion of containers.
    ~container() {
        deleteContents();
    }
    void checkContents() {
        if (cInventory.size() == 0) {
            isEmpty = true;
        }
        else {
            isEmpty = false;
        }
    }
    void erase_item(int target) {
        cInventory.erase(target);
        inventory_accessKey.erase(target);
        
        checkContents();
    }
    void deleteContents() {
        for (int r = 0; r < invSize; r++) {
            delete cInventory[r];
            erase_item(r);
        }

    }
};
//governs monster
class monster {
public:
    string monsterName;
    short monsterTypes = 5;
    bool doesMind;
    short damageMax_mind;
    short damageMin_mind;
    bool isVisible;
    short damageMax;
    short damageMin;
    short moveSpeed;
    short wakeMin;
    short wakeMax;
    short wakeMoves;
    bool spawned;
    bool awake;
    bool canMove;
    bool alwaysMoves;
    bool moves_useChance;
    short moveChance;
    short moveTick;
    short moveTick_countdown;
    char monsterChar;
    coords pos;
    void glitch() {
        monsterName = "Glitch";
        monsterChar = '#';
        damageMin = 10;
        damageMax = 20;
        wakeMin = MONSTERMOVES_AVGMIN + random(0, 5);
        wakeMax = MONSTERMOVES_AVGMAX + random(-5, 5);
        moveSpeed = random(1, 3);
        isVisible = true;
        doesMind = true;
        damageMax_mind = 30;
        damageMin_mind = 10;
        alwaysMoves = random(0, 1);
        moves_useChance = random(0, 1);
        moveTick = random(1, 3);
        moveChance = random(50, 100);
    }
    void nothing() {
        monsterName = "Nothing";
        monsterChar = 'X';
        damageMin = 30;
        damageMax = 45;
        wakeMin = MONSTERMOVES_AVGMIN;
        wakeMax = MONSTERMOVES_AVGMAX;
        moveSpeed = 1;
        isVisible = true;
        doesMind = false;
        damageMax_mind = 0;
        damageMin_mind = 0;
        alwaysMoves = true;
        moves_useChance = false;
        moveTick = 2;
        moveChance = 80;
    }
    void ghost() {
        monsterName = "Ghost";
        monsterChar = 'X';
        damageMin = 15;
        damageMax = 25;
        wakeMin = MONSTERMOVES_AVGMIN;
        wakeMax = MONSTERMOVES_AVGMAX;
        moveSpeed = 1;
        isVisible = false;
        doesMind = true;
        damageMax_mind = 35;
        damageMin_mind = 20;
        alwaysMoves = true;
        moves_useChance = false;
        moveTick = 2;
        moveChance = 60;
    }
    void flicker() {
        monsterName = "Flicker";
        monsterChar = '*';
        damageMin = 40;
        damageMax = 50;
        wakeMin = MONSTERMOVES_AVGMIN - 3;
        wakeMax = MONSTERMOVES_AVGMAX  + 3;
        moveSpeed = 2;
        isVisible = true;
        doesMind = true;
        damageMax_mind = 50;
        damageMin_mind = 40;
        alwaysMoves = false;
        moves_useChance = false;
        moveTick = 1;
        moveChance = 75;
    }
    void demon() {
        monsterName = "Demon";
        monsterChar = 'Y';
        damageMin = 30;
        damageMax = 50;
        wakeMin = MONSTERMOVES_AVGMIN + 15;
        wakeMax = MONSTERMOVES_AVGMAX + 20;
        moveSpeed = 3;
        isVisible = true;
        doesMind = true;
        damageMax_mind = 25;
        damageMin_mind = 15;
        alwaysMoves = false;
        moves_useChance = true;
        moveTick = 1;
        moveChance = 80;
    }
    void resetMonster() {
        spawned = false;
        awake = false;
        canMove = true;
        pos.x = 1;
        pos.y = 1;
        if (random(1, 100) < NOTHING_CHANCE) {
            nothing();
        }
        else {
            short tempReference = random(2, monsterTypes);
            switch (tempReference) {
            case 2:
                ghost();
                break;
            case 3:
                flicker();
                break;
            case 4:
                demon();
                break;
            case 5:
                glitch();
                break;
            }
        }
        moveTick_countdown = moveTick;
        wakeMoves = random(wakeMin, wakeMax);
    }
    void randomMonster() {
        if (random(1, 100) < NOTHING_CHANCE) {
            nothing();
        }
        else {
            short tempReference = random(2, monsterTypes);
            switch (tempReference) {
            case 2:
                ghost();
                break;
            case 3:
                flicker();
                break;
            case 4:
                demon();
                break;
            case 5:
                glitch();
                break;
            }
        }
    }
    monster() {
        spawned = false;
        awake = false;
        canMove = true;
        pos.x = 1;
        pos.y = 1;
        if (random(1, 100) < NOTHING_CHANCE) {
            nothing();
        }
        else {
            short tempReference = random(2, monsterTypes);
            switch (tempReference) {
            case 2:
                ghost();
                break;
            case 3:
                flicker();
                break;
            case 4:
                demon();
                break;
            case 5:
                glitch();
                break;
            }
        }
        moveTick_countdown = moveTick;
        wakeMoves = random(wakeMin, wakeMax);
    }
};
void checkMonster_state(monster* target) {
    if (target->wakeMoves <= 0) {
        target->wakeMoves = 0;
        target->awake = true;
        if (target->spawned == false) {
        }
    }
    else {
        target->awake = false;
    }
}
//governs player
class player {
private:
    //may be used in future.
    //string fileName;
public:
    enum direction_facing {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };
    coords pos;
    //bool referencing if player is looking at something they can interact with.
    bool player_lookingAt_container;
    bool player_lookingAt_door;
    int x;
    int y;
    direction_facing currentDirection;
    map<direction_facing, char> playerChar_map = { {UP,'^'}, {DOWN, 'v'}, {LEFT, '<'}, {RIGHT, '>'} };
    char playerChar;
    short helditem = -1;
    short health = 100;
    short mind = 100;
    short hunger = 100;
    //inventory with 7 slots. 
    item *inventory[7] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL };
    container* percievedContainer = NULL;
    door* percievedDoor = NULL;
    //these functions govern things that can happen to the player
    //updates player char
    void printHealth() {
        if (mind > 25) {
            cout << "HEALTH: " << health << endl;
        }
        else {
            cout << "KEEP ON RUNNING..." << endl;
        }
    }
    void printMind() {
        if (mind > 25) {
            cout << "MIND: " << mind << endl;
        }
        else {
            cout << "WE KNOW WHERE YOU ARE..." << endl;
        }
    }
    void printHunger() {
        if (mind > 25) {
            cout << "HUNGER: " << hunger << endl;
        }
        else {
            cout << "WE CAN SMELL YOUR REELING MIND..." << endl;
        }
    }
    void printStats() {
        system("cls");
        printHealth();
        printMind();
        printHunger();
        confirm();
        return;
    }
    void updateChar() {
        playerChar = playerChar_map[currentDirection];
        if (health <= hunger && health <= mind) {
            lowest = HEALTH;
        }
        else if (hunger <= health && hunger <= mind) {
            lowest = HUNGER;
        }
        else if (mind <= health && mind <= hunger) {
            lowest = MIND;
        }
        if (neverHurt) {
            health = 100;
        }
        if (alwaysSane) {
            mind = 100;
        }
        if (neverHungry) {
            hunger = 100;
        }
        if (mind < 0) {
            mind = 0;
        }
        if (hunger < 0) {
            hunger = 0;
        }
        //need to find a better way to have the game say the cause of death eventually.
        if (health <= 0 && hunger == 0) {
            EndGame("starvation");
        }
        else if (health <= 0) {
            EndGame("injuries");
        }
        else if (mind <= 0 && health <= 0) {
            EndGame("suicide");
        }
    }
    //when you get hurt (if i implement this.)
    void takedamage(int damage, string  sourceName, bool mindDamage_true, int mindDamage) {
        system("cls");
        health -= damage;
        if (health < 0) {
            health = 0;
        }
        cout << sourceName << " has hurt you..." << endl;
        if (mindDamage_true) {
            mind -= mindDamage;
            if (mind < 0) {
                mind = 0;
            }
        }
        printHealth();
        if (mindDamage_true) {
            printMind();
        }
        updateChar();
        confirm();
    }
    //drops item in specified item slot (deletes it)
    void drop(int invslot) {
        if (inventory[invslot] != NULL) {
            cout << "Discarding " << inventory[invslot]->itemName << "..." << endl;
            delete inventory[invslot];
            inventory[invslot] = NULL;
        }
        else {
            cout << "You can't drop air, idiot..." << endl;
        }
    }
    //takes target item and puts it in specified inventory slot.
    void take(item *grabbedobj, int invslot) {
        if (inventory[invslot] != NULL) {
            cout << "Please drop " << inventory[invslot]->itemName << " before putting an item in that slot." << endl;
            return;
        }
        inventory[invslot] = grabbedobj;
        cout << inventory[invslot]->itemName << " has been picked up. It now takes up slot " << invslot << "." << endl;
    }
    //equips item in specified inventory slot
    void equip(int invslot) {
        if (inventory[invslot] != NULL) {
            cout << "Equipping " << inventory[invslot]->itemName << "... " << endl;
            helditem = invslot;
            return;
        } else {
            cout << "You don't have anything in that slot." << endl;
            return;
        }
    }
    //lists everything in player's inventory
    void checkInv() {
        bool isempty = true;
        short select_pos = 0;
        while (true) {
            system("cls");
            for (int inv_i = 0; inv_i <= 7; inv_i++) {
                if (inv_i < 7) {
                    cout << inv_i << " (" << (inventory[inv_i] == NULL ? "Empty" : inventory[inv_i]->itemName) << ")";
                    if (select_pos == inv_i) {
                        cout << "< | [F]: Equip | [G]: Drop |";
                        if (inventory[inv_i] != NULL) {
                            cout << " " << inventory[inv_i]->tooltip;
                        }
                    }
                    cout << endl;
                }
                else {
                    cout << "Exit";
                    if (select_pos == inv_i) {
                        cout << "<";
                    }
                    cout << endl;
                }
            }
            char inv_input = _getch();
            switch (inv_input) {
            case 'w':
                if (select_pos != 0) {
                    select_pos--;
                }
                break;
            case 's':
                if (select_pos != 7) {
                    select_pos++;
                }
                break;
            case 'f':
                if (select_pos != 7) {
                    system("cls");
                    equip(select_pos);
                    confirm();
                    return;
                }
                break;
            case 'g':
                if (select_pos != 7) {
                    system("cls");
                    if (inventory[select_pos]->isKey == false) {
                        drop(select_pos);
                    }
                    else {
                        cout << "I'd better not drop that..." << endl;
                    }
                    confirm();
                    return;
                }
                break;
            case ' ':
                if (select_pos == 7) {
                    confirm();
                    return;
                }
                break;
            default:
                break;
            }
        }
    }
    //uses equipped item on specified targetable.
    void use_self();
    void open(container* target) {
        if (target->isLocked == false) {
            short select_pos = 0;
            bool itemChosen = false;
            while (true) {
                system("cls");
                cout << "Inside the " << target->containerName << " you find..." << endl;
                //cout << "DEBUG: cInventory size is: " << target->cInventory.size() << endl;
                if (target->isEmpty != true) {
                    for (int i = 0; i <= target->invSize; i++) {
                        if (i < target->invSize) {
                            cout << target->inventory_accessKey[i];
                        }
                        else {
                            cout << "Exit";
                        }
                        if (i == select_pos) {
                            cout << "<";
                            if (i < target->invSize) {
                                cout << " | [SPACE]: Take "; // << "| "<< target->cInventory[i]->tooltip; //This may be used some day.
                            }
                            if (i == target->invSize) {
                                cout << " | [SPACE]: Exit ";
                            }
                        }
                        cout << endl;
                    }
                    char input = _getch();
                    if (input == 'w' && select_pos != 0) {
                        select_pos--;
                    }
                    else if (input == 's' && select_pos != target->invSize) {
                        select_pos++;
                    }
                    else if (input == ' ') {
                        if (select_pos == target->invSize) {
                            confirm();
                            return;
                        }
                        if (target->cInventory.count(select_pos) != 0) {
                            char select_pos_2 = 0;
                            while (true) {
                                system("cls");
                                cout << "What inventory slot do you want to put it in?" << endl;
                                for (int i = 0; i <= 7; i++) {
                                    if (i < 7) {
                                        cout << i << " (" << (inventory[i] == NULL ? "Empty" : inventory[i]->itemName) << ")";
                                    }
                                    else {
                                        cout << "Exit";
                                    }
                                    if (select_pos_2 == i) {
                                        cout << "< | [SPACE]: Select ";
                                    }
                                    cout << endl;
                                }
                                input = _getch();
                                if (input == 'w' && select_pos_2 != 0) {
                                    select_pos_2--;
                                }
                                else if (input == 's' && select_pos_2 != 7) {
                                    select_pos_2++;
                                }
                                else if (input == ' ') {
                                    if (select_pos_2 < 7) {
                                        bool itemChosen = true;
                                        if (inventory[select_pos_2] == NULL) {
                                            take(target->cInventory[select_pos], select_pos_2);
                                            target->erase_item(select_pos);
                                            confirm();
                                            return;
                                        }
                                        else {
                                            cout << "I already have something there." << endl;
                                            confirm();
                                        }
                                    }
                                    else {
                                        confirm();
                                        return;
                                    }
                                }
                            }
                        }
                        else {
                            cout << "Theres nothing there..." << endl;
                            confirm();
                        }
                    }
                }
                else {
                    cout << "The " << target->containerName << " is empty." << endl;
                    confirm();
                    return;
                }
            }
        }
        else {
            target->isLocked = lockpickMinigame();
            confirm();
            return;
        }
    }
    //this function serves only to check if moving with action() is valid;

    //when this function is used the player gets the opportunity to take an action. the Nothing also gets nearer with every actionOpp;
    player() {
        //player (x,y) defaults to (1,1)
        pos.x = 1;
        pos.y = 1;
        currentDirection = DOWN;
        updateChar();
    }
};
//governs rooms
class room {
public:
    short containernum;
    //when player enters new room these are chosen
    string lvlcpy[10];
    door* roomExit;
    ifstream roomfile;
    short roomfile_xLength;
    bool doorLocked_keyHeld;
    const short ROOMCOUNT = 7;
    map<int, string> roomfile_map = { {1, "Hallway"}, {2, "Uturn"}, {3, "2Rooms"}, {4, "Wacky"}, {5, "Siderooms"}, {6, "Bigroom"}, {7, "Maze"} };
    //may eventully be more levels;
    string loadedLevel[10];
    map<int, container*> containers;
    map<int, string> containers_Key;
    room(door* activeDoor) {
        roomfile.open(roomfile_map[random(1, ROOMCOUNT)] + ".txt");
        if (debug_displayProcess) {
            cout << "DEBUG: roomfile opened" << endl;
        }
        for (int r_i = 0; r_i < 10; r_i++) {
            getline(roomfile, loadedLevel[r_i]);
        }
        if (debug_displayProcess) {
            cout << "DEBUG: loadedLevel populated" << endl;
        }
        xlength = loadedLevel[0].length();
        if (debug_displayProcess) {
            cout << "DEBUG: room xlength set" << endl;
        }
        roomfile.close();
        if (debug_displayProcess) {
            cout << "DEBUG: roomfile closed" << endl;
        }
        roomExit = activeDoor;
        if (debug_displayProcess) {
            cout << "DEBUG: roomExit set" << endl;
        }
        if (debug_displayProcess) {
            cout << "DEBUG: roomExit generated" << endl;
        }
        containernum = random(1, 3);
        if (debug_displayProcess) {
            cout << "DEBUG: container count set" << endl;
        }
        roomExit->resetDoor();
        if (debug_displayProcess) {
            cout << "DEBUG: roomExit reset " << endl;
        }
        loadedLevel[roomExit->pos_left.y][roomExit->pos_left.x] = roomExit->doorChars[0];
        loadedLevel[roomExit->pos_right.y][roomExit->pos_right.x] = roomExit->doorChars[0];
        if (debug_displayProcess) {
            cout << "DEBUG: roomExit position determined" << endl;
        }
        for (int i = 0; i < containernum; i++) {
            container* cReference = new container;
            bool container_ypos_illegal = true;
            while (container_ypos_illegal) {
                cReference->cPos.y = random(1, 9);
                for (int j = 0; j < loadedLevel[cReference->cPos.y].length(); j++) {
                    //this if statement is used to ensure player spawnpoint is empty until I have a more advanced spawning mechanic for the player;
                    if (i == 1 && j == 1) {
                        continue;
                    }
                    if (loadedLevel[cReference->cPos.y].at(j) == ' ') {
                        container_ypos_illegal = false;
                        break;
                    }
                }
            }
            bool container_xpos_illegal = true;
            while (container_xpos_illegal) {
                cReference->cPos.x = random(1, loadedLevel[cReference->cPos.y].length() - 1);
                //ditto
                if (cReference->cPos.y == 1 && cReference->cPos.x == 1) {
                    continue;
                }
                if (loadedLevel[cReference->cPos.y].at(cReference->cPos.x) == ' ') {
                    container_xpos_illegal = false;
                    break;
                }
            }
            containers_Key[i] = cReference->containerName;
            containers[i] = cReference;
            loadedLevel[cReference->cPos.y][cReference->cPos.x] = cReference->containerChar;
        }
        if (debug_displayProcess) {
            cout << "DEBUG: containers generated" << endl;
        }
        for (int roomY = 0; roomY < 10; roomY++) {
            for (int roomX = 0; roomX < xlength; roomX++) {
                if (loadedLevel[roomY][roomX] == '#') {
                    loadedLevel[roomY][roomX] = ' ';
                }
            }
        }
        if (debug_displayProcess) {
            cout << "DEBUG: blacklisted spaces removed" << endl;
        }
    }
    ~room() {
        for (short delete_i = 0; delete_i < containernum; delete_i++) {
            delete containers[delete_i];
        }
        if (debug_displayProcess) {
            cout << "DEBUG: containers deleted" << endl;
        }
    }
    void printRoom(player* pReference, monster* mReference) {
        doorLocked_keyHeld = false;
        if (debug_displayProcess) {
            cout << "DEBUG: doorLocked_keyHeld set to false" << endl;
        }
        for (int pr_i = 0; pr_i < 10; pr_i++) {
            lvlcpy[pr_i] = loadedLevel[pr_i];
        }
        if (debug_displayProcess) {
            cout << "DEBUG: lvlcpy populated" << endl;
        }
        if (mReference->spawned == true && mReference->awake == true) {
            if (mReference->isVisible) {
                if (mReference->monsterName != "Glitch") {
                    lvlcpy[mReference->pos.y][mReference->pos.x] = mReference->monsterChar;
                }
                else {
                    lvlcpy[random(mReference->pos.y - 1, mReference->pos.y + 1)][random(mReference->pos.x - 1, mReference->pos.x + 1)] = mReference->monsterChar;
                }
            }
        }
        if (debug_displayProcess) {
            cout << "DEBUG: monster position determined" << endl;
        }
        lvlcpy[pReference->pos.y][pReference->pos.x] = pReference->playerChar;
        if (debug_displayProcess) {
            cout << "DEBUG: player position determined" << endl;
        }
        roomExit->checkState();
        if (debug_displayProcess) {
            cout << "DEBUG: roomexit state checked" << endl;
        }
        lvlcpy[roomExit->pos_left.y][roomExit->pos_left.x] = roomExit->doorChars[0];
        lvlcpy[roomExit->pos_right.y][roomExit->pos_right.x] = roomExit->doorChars[1];
        if (debug_displayProcess) {
            cout << "DEBUG: roomexit position updated" << endl;
        }
        system("cls");
        for (int pr_i = 0; pr_i < 10; pr_i++) {
            cout << lvlcpy[pr_i];
            if (pReference->player_lookingAt_container == true && pr_i == 0) {
                cout << "[SPACE]: Open " << pReference->percievedContainer->containerName << " (" << (pReference->percievedContainer->isLocked ? "Locked" : "Unlocked") << ")";
            }
            if (pReference->player_lookingAt_door == true && pr_i == 0) {
                if (pReference->percievedDoor->isLocked == true) {
                    cout << "Status: Locked";
                }
                else {
                    cout << "[SPACE]: Open Door";
                }
            }
            //this will happen eventually...
            if (pReference->player_lookingAt_door == true && pr_i == 1 && roomExit->isLocked == true) {
                if (pReference->helditem != -1) {
                    if (pReference->inventory[pReference->helditem]->itemName == "Key" && roomExit->isLocked == true) {
                        cout << "[Q]: Unlock Door ";
                        doorLocked_keyHeld = true;
                    }
                    else {
                        cout << "I need a key to unlock this...";
                    }
                }
                else {
                    cout << "I need a key to unlock this...";
                }
            }
            cout << endl;
        }
        checkMonster_state(mReference);
        cout << "Current Room: " << roomNum << endl;
        if (mReference->awake == true) {
            //cout << "DEBUG: " << mReference->monsterName << " is awake!" << endl;
        }
        else {
            //cout << "DEBUG: " << "Moves until " << mReference->monsterName << " wakes up: " << mReference->wakeMoves << endl;
        }
    }
};

/*If I want this to work I need to be able to find out what part of room generation is causing the crashes that sometimes happen.
* clues:
*   -only occasionally happens. (suggests that it has to do with a random number)
*   -return bug takes a second to occur (might have to do with a while? that or game just tries to proccess something it can't)
* My current theories are
*   -container generation via random is ineffeicient and causes the game to stall enough where it just says F it (this would explain when the game just returns a random number);
*   -some random number somewhere steps out of scope (would explain the aborts)
* PROGRESS
*   -Crash has something to do with deleting activeMonster, I think.
*       -I can't tell you why
* 
* */
void newRoom(monster* activeMonster, player* activePlayer, room* activeRoom, door* activeDoor) {
    system("cls");
    cout << "Loading..." << endl;
    delete activeRoom;
    if (debug_displayProcess) {
        cout << "DEBUG: active room deleted" << endl;
    }
    activePlayer->pos.x = 1;
    activePlayer->pos.y = 1;
    activePlayer->currentDirection = player::DOWN;
    if (debug_displayProcess) {
        cout << "DEBUG: player position reset" << endl;
    }
    activeRoom = new room(activeDoor);
    if (debug_displayProcess) {
        cout << "DEBUG: new room loaded" << endl;
    }
    activeMonster->resetMonster();
    if (debug_displayProcess) {
        cout << "DEBUG: active monster reset" << endl;
    }
    return;
}
bool cheats = false;
bool godMode = false;
const int debugQuotes_count = 12;
const int debugOptions_count = 5;
map<int, string> debugQuotes = { {1, "Nice meeting you here..."}, {2, "Hi, how are ya..."}, {3, "Back so soon, are we?"}, {4, "Coding all by yourself, handsome? ;)"}, {5, "OwO, What's this?"}, {6, "IT'S YOU!!!"}, {7, "Speak of the devil..."}, {8, "You just can't live without me, can you?"}, {9, "What is it this time?.."}, {10, "Heyyyyyy ;)"}, {11, ":O"}, {12, "How can I help ya?"} };
map<int, string> debugOptions = { {1, "Generate New Room"}, {2, "End Game"}, {3, "Toggle Process Display"}, {4, "Dev Console"}, {5, "Exit"}};
void debugMenu(monster* activeMonster, player* activePlayer, room* activeRoom, door* activeDoor) {
    string conInput;
    short debugPos = 1;
    string message = debugQuotes[random(1, 12)];
    while (true) {
        system("cls");
        cout << "***** DEBUG MENU *****" << endl;
        cout << message << endl << endl;
        for (int debugList = 1; debugList <= debugOptions_count; debugList++) {
            cout << debugOptions[debugList];
            if (debugPos == debugList) {
                cout << "< | [SPACE]: Select";
            }
            cout << endl;
        }
        char debugInput = _getch();
        switch (debugInput) {
        case 'w':
            if (debugPos != 1) {
                debugPos--;
            }
            break;
        case 's':
            if (debugPos != debugOptions_count) {
                debugPos++;
            }
            break;
        case ' ':
            switch (debugPos) {
            case 1:
                newRoom(activeMonster, activePlayer, activeRoom, activeDoor);
                confirm();
                return;
                break;
            case 2:
                system("cls");
                EndGame("DEBUG: End Game");
                confirm();
                return;
            case 3:
                if (debug_displayProcess) {
                    debug_displayProcess = false;
                    cout << "debug_displayProcess is now false" << endl;
                    confirm();
                }
                else {
                    debug_displayProcess = true;
                    cout << "debug_displayProcess is now true" << endl;
                    confirm();
                }
                break;
            case 4:
                cout << "Type here:" << endl;
                getline(cin, conInput);
                if (conInput == "neverHungry true" || conInput == "neverHungry t" || conInput == "neverHungry 1") {
                    neverHungry = true;
                }
                else if (conInput == "neverHungry false" || conInput == "neverHungry f" || conInput == "neverHungry 0") {
                    neverHungry = false;
                }
                else if (conInput == "neverHurt true" || conInput == "neverHurt t" || conInput == "neverHurt 1") {
                    neverHurt = true;
                }
                else if (conInput == "neverHurt false" || conInput == "neverHurt f" || conInput == "neverHurt 0") {
                    neverHurt = false;
                }
                else if (conInput == "alwaysSane true" || conInput == "alwaysSane t" || conInput == "alwaysSane 1") {
                    alwaysSane = true;
                }
                else if (conInput == "alwaysSane false" || conInput == "alwaysSane f" || conInput == "alwaysSane 0") {
                    alwaysSane = false;
                }
                else if (conInput == "gm") {
                    if (neverHungry == true && neverHurt == true && alwaysSane == true) {
                        neverHungry = false;
                        neverHurt = false;
                        alwaysSane = false;
                    }
                    else {
                        neverHungry = true;
                        neverHurt = true;
                        alwaysSane = true;
                    }
                }
                else if (conInput == "cheats true" || conInput == "cheats t" || conInput == "cheats 1") {
                    cheats = true;
                }
                else if (conInput == "cheats false" || conInput == "cheats f" || conInput == "cheats 0") {
                    cheats = false;
                }
                else if (conInput == "randomMonster") {
                    activeMonster->randomMonster();
                }
                break;
            case 5:
                confirm();
                return;
            default:
                break;
            }
        default:
            break;
        }
    }
}
//these functions reference some information defined above in a way that it isn't convenient to host them in any specific class
bool movementPossible(room* activeRoom, int xpos, int ypos) {
    if (xpos >= 0 && xpos < xlength && ypos >= 0 && ypos < 10) {
        if (activeRoom->loadedLevel[ypos][xpos] == ' ') {
            return true;
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
}
void monster_actionAttempt(monster* activeMonster, player* targetPlayer, room* activeRoom) {
    //this is how Flicker 'flickers'
    if (activeMonster->monsterName == "Flicker" && activeMonster->isVisible == true) {
        activeMonster->isVisible = false;
    }
    else if (activeMonster->monsterName == "Flicker" && activeMonster->isVisible == false) {
        activeMonster->isVisible = true;
    }
    //used for traveling towards targetPlayer's coords
    bool favoredMove_possible;
    bool same_x_y;

    //The way the monster tries to approach player is
    //if values of x or y for monster and player are different (or both), consult action A
    //if monster and target's coords are the same, consult action B
    if (activeMonster->awake == true && activeMonster->spawned == true) {
        if (activeMonster->alwaysMoves == true || (activeMonster->alwaysMoves == false && activeMonster->moves_useChance == true && random(1, 100) < activeMonster->moveChance) || (activeMonster->alwaysMoves == false && activeMonster->moves_useChance == false && activeMonster->moveTick_countdown <= 0)) {
            //action A
            for (int moves_i = 0; moves_i < (activeMonster->monsterName != "Flicker" && activeMonster->monsterName != "Glitch" ? activeMonster->moveSpeed : activeMonster->moveSpeed); moves_i++) {
                if (activeMonster->pos.x != targetPlayer->pos.x || activeMonster->pos.y != targetPlayer->pos.y) {
                    same_x_y = (activeMonster->pos.x == targetPlayer->pos.x || activeMonster->pos.y == targetPlayer->pos.y);
                    if (abs(targetPlayer->pos.x - activeMonster->pos.x) > abs(targetPlayer->pos.y - activeMonster->pos.y)) {
                        favoredMove_possible = (activeMonster->pos.x > targetPlayer->pos.x ? (movementPossible(activeRoom, activeMonster->pos.x - 1, activeMonster->pos.y) ? true : false) : (movementPossible(activeRoom, activeMonster->pos.x + 1, activeMonster->pos.y) ? true : false));
                        if (favoredMove_possible) {
                            activeMonster->pos.x = (activeMonster->pos.x > targetPlayer->pos.x ? activeMonster->pos.x - 1 : activeMonster->pos.x + 1);
                        }
                        else if (same_x_y) {
                            if (random(1, 100) < 70 && activeMonster->monsterName != "Flicker") {
                                if (random(1, 100) < 50) {
                                    activeMonster->pos.y = (movementPossible(activeRoom, activeMonster->pos.x, activeMonster->pos.y + 1) ? activeMonster->pos.y + 1 : (movementPossible(activeRoom, activeMonster->pos.x, activeMonster->pos.y - 1) ? activeMonster->pos.y - 1 : activeMonster->pos.y));
                                }
                                else {
                                    activeMonster->pos.y = (movementPossible(activeRoom, activeMonster->pos.x, activeMonster->pos.y - 1) ? activeMonster->pos.y - 1 : (movementPossible(activeRoom, activeMonster->pos.x, activeMonster->pos.y + 1) ? activeMonster->pos.y + 1 : activeMonster->pos.y));
                                }
                            }
                            else {
                                //if monster can't reach you, it will relocate
                                bool monster_notRelocated = true;
                                while (monster_notRelocated) {
                                    short xRepos = random(2, 4) * (random(1, 100) < 50 ? 1 : -1);
                                    short yRepos = random(2, 4) * (random(1, 100) < 50 ? 1 : -1);
                                    if (movementPossible(activeRoom, targetPlayer->pos.x + xRepos, targetPlayer->pos.y + yRepos) && targetPlayer->pos.x + xRepos > 0 && targetPlayer->pos.x + xRepos < xlength - 1 && targetPlayer->pos.y + yRepos > 0 && targetPlayer->pos.y + yRepos < 9) {
                                        activeMonster->pos.x = targetPlayer->pos.x + xRepos;
                                        activeMonster->pos.y = targetPlayer->pos.y + yRepos;
                                        monster_notRelocated = false;
                                    }
                                }
                                break;
                            }
                        }
                        else {
                            if (activeMonster->monsterName != "Flicker") {
                                if (activeMonster->pos.y > targetPlayer->pos.y) {
                                    if (movementPossible(activeRoom, activeMonster->pos.x, activeMonster->pos.y - 1)) {
                                        activeMonster->pos.y--;
                                    }
                                }
                                else {
                                    if (movementPossible(activeRoom, activeMonster->pos.x, activeMonster->pos.y + 1)) {
                                        activeMonster->pos.y++;
                                    }
                                }
                            }
                            else {
                                bool monster_notRelocated = true;
                                while (monster_notRelocated) {
                                    short xRepos = random(2, 4) * (random(1, 100) < 50 ? 1 : -1);
                                    short yRepos = random(2, 4) * (random(1, 100) < 50 ? 1 : -1);
                                    if (movementPossible(activeRoom, targetPlayer->pos.x + xRepos, targetPlayer->pos.y + yRepos) && targetPlayer->pos.x + xRepos > 0 && targetPlayer->pos.x + xRepos < xlength - 1 && targetPlayer->pos.y + yRepos > 0 && targetPlayer->pos.y + yRepos < 9) {
                                        activeMonster->pos.x = targetPlayer->pos.x + xRepos;
                                        activeMonster->pos.y = targetPlayer->pos.y + yRepos;
                                        monster_notRelocated = false;
                                    }
                                }
                                break;
                            }
                        }
                    }
                    else if (abs(targetPlayer->pos.y - activeMonster->pos.y) > abs(targetPlayer->pos.x - activeMonster->pos.x)) {
                        favoredMove_possible = (activeMonster->pos.y > targetPlayer->pos.y ? (movementPossible(activeRoom, activeMonster->pos.x, activeMonster->pos.y - 1) ? true : false) : (movementPossible(activeRoom, activeMonster->pos.x, activeMonster->pos.y + 1) ? true : false));
                        if (favoredMove_possible) {
                            activeMonster->pos.y = (activeMonster->pos.y > targetPlayer->pos.y ? activeMonster->pos.y - 1 : activeMonster->pos.y + 1);
                        }
                        else if (same_x_y) {
                            if (random(1, 100) < 70 && activeMonster->monsterName != "Flicker") {
                                if (random(1, 100) < 50) {
                                    activeMonster->pos.x = (movementPossible(activeRoom, activeMonster->pos.x + 1, activeMonster->pos.y) ? activeMonster->pos.x + 1 : (movementPossible(activeRoom, activeMonster->pos.x - 1, activeMonster->pos.y) ? activeMonster->pos.x - 1 : activeMonster->pos.x));
                                }
                                else {
                                    activeMonster->pos.x = (movementPossible(activeRoom, activeMonster->pos.x - 1, activeMonster->pos.y) ? activeMonster->pos.x - 1 : (movementPossible(activeRoom, activeMonster->pos.x + 1, activeMonster->pos.y) ? activeMonster->pos.x + 1 : activeMonster->pos.x));
                                }
                            }
                            else {
                                bool monster_notRelocated = true;
                                while (monster_notRelocated) {
                                    short xRepos = random(2, 4) * (random(1, 100) < 50 ? 1 : -1);
                                    short yRepos = random(2, 4) * (random(1, 100) < 50 ? 1 : -1);
                                    if (movementPossible(activeRoom, targetPlayer->pos.x + xRepos, targetPlayer->pos.y + yRepos) && targetPlayer->pos.x + xRepos > 0 && targetPlayer->pos.x + xRepos < xlength - 1 && targetPlayer->pos.y + yRepos > 0 && targetPlayer->pos.y + yRepos < 9) {
                                        activeMonster->pos.x = targetPlayer->pos.x + xRepos;
                                        activeMonster->pos.y = targetPlayer->pos.y + yRepos;
                                        monster_notRelocated = false;
                                    }
                                }
                                break;
                            }
                        }
                        else {
                            if (activeMonster->monsterName != "Flicker") {
                                if (activeMonster->pos.x > targetPlayer->pos.x) {
                                    if (movementPossible(activeRoom, activeMonster->pos.x - 1, activeMonster->pos.x)) {
                                        activeMonster->pos.x--;
                                    }
                                }
                                else {
                                    if (movementPossible(activeRoom, activeMonster->pos.x + 1, activeMonster->pos.x)) {
                                        activeMonster->pos.x++;
                                    }
                                }
                            }
                            else {
                                bool monster_notRelocated = true;
                                while (monster_notRelocated) {
                                    short xRepos = random(2, 4) * (random(1, 100) < 50 ? 1 : -1);
                                    short yRepos = random(2, 4) * (random(1, 100) < 50 ? 1 : -1);
                                    if (movementPossible(activeRoom, targetPlayer->pos.x + xRepos, targetPlayer->pos.y + yRepos) && targetPlayer->pos.x + xRepos > 0 && targetPlayer->pos.x + xRepos < xlength - 1 && targetPlayer->pos.y + yRepos > 0 && targetPlayer->pos.y + yRepos < 9) {
                                        activeMonster->pos.x = targetPlayer->pos.x + xRepos;
                                        activeMonster->pos.y = targetPlayer->pos.y + yRepos;
                                        monster_notRelocated = false;
                                    }
                                }
                                break;
                            }
                        }
                    }
                    else {
                        short xDirection = (activeMonster->pos.x > targetPlayer->pos.x ? -1 : 1);
                        short yDirection = (activeMonster->pos.y > targetPlayer->pos.y ? -1 : 1);
                        if (movementPossible(activeRoom, activeMonster->pos.x + xDirection, activeMonster->pos.y + yDirection)) {
                            activeMonster->pos.x += xDirection;
                            activeMonster->pos.y += yDirection;
                        }
                        else {
                            if (random(1, 100) > 50) {
                                if (movementPossible(activeRoom, activeMonster->pos.x + xDirection, activeMonster->pos.y)) {
                                    activeMonster->pos.x += xDirection;
                                }
                                else if (movementPossible(activeRoom, activeMonster->pos.x, activeMonster->pos.y + yDirection)) {
                                    activeMonster->pos.y += yDirection;
                                }
                            }
                            else {
                                if (movementPossible(activeRoom, activeMonster->pos.x, activeMonster->pos.y + yDirection)) {
                                    activeMonster->pos.y += yDirection;
                                }
                                else if (movementPossible(activeRoom, activeMonster->pos.x + xDirection, activeMonster->pos.y)) {
                                    activeMonster->pos.x += xDirection;
                                }
                            }
                        }
                    }
                }
                if ((abs(targetPlayer->pos.x - activeMonster->pos.x) == 0) && (abs(targetPlayer->pos.y - activeMonster->pos.y) == 0)) {
                    targetPlayer->takedamage(random(activeMonster->damageMin, activeMonster->damageMax), activeMonster->monsterName, activeMonster->doesMind, random(activeMonster->damageMin_mind, activeMonster->damageMax_mind));
                    //DEBUG
                    //activeMonster->pos.x = 1;
                    //activeMonster->pos.y = 1;
                    bool monster_notRelocated = true;
                    while (monster_notRelocated) {
                        short xRepos = random(3, 7) * (random(1, 100) < 50 ? 1 : -1);
                        short yRepos = random(3, 7) * (random(1, 100) < 50 ? 1 : -1);
                        if (movementPossible(activeRoom, activeMonster->pos.x + xRepos, activeMonster->pos.y + yRepos) && activeMonster->pos.x + xRepos > 0 && activeMonster->pos.x + xRepos < xlength - 1 && activeMonster->pos.y + yRepos > 0 && activeMonster->pos.y + yRepos < 9) {
                            activeMonster->pos.x += xRepos;
                            activeMonster->pos.y += yRepos;
                            monster_notRelocated = false;
                        }
                    }
                    break;
                }
            }
            if (activeMonster->alwaysMoves == false && activeMonster->moves_useChance == false) {
                activeMonster->moveTick_countdown = activeMonster->moveTick;
            }
        } else {
            if (activeMonster->moves_useChance == false) {
                activeMonster->moveTick_countdown--;
            }
        }
    }
}
void player_action(monster* activeMonster, player* activePlayer, room* activeRoom, door* activeDoor) {
    char input = _getch();
    switch (input) {
    case 'w':
        if (movementPossible(activeRoom, activePlayer->pos.x, activePlayer->pos.y - 1)) {
            if (activePlayer->hunger > 0) {
                activePlayer->hunger -= MOVE_HUNGERCOST;
            }
            else {
                activePlayer->mind -= MOVE_HUNGERCOST;
                activePlayer->health -= MOVE_HUNGERCOST;
                activeMonster->wakeMoves -= (activePlayer->mind  < 25 ? MONSTERWAKE_MINDPENALTY : 1);
            }
            activePlayer->currentDirection = player::UP;
            activePlayer->pos.y--;
            activePlayer->updateChar();
            activeMonster->wakeMoves -= (activePlayer->mind  < 25 ? MONSTERWAKE_MINDPENALTY : 1);
        }
        else {
            activePlayer->currentDirection = player::UP;
            activePlayer->updateChar();
        }
        break;
    case 'a':
        if (movementPossible(activeRoom, activePlayer->pos.x - 1, activePlayer->pos.y)) {
            if (activePlayer->hunger > 0) {
                activePlayer->hunger -= MOVE_HUNGERCOST;
            }
            else {
                activePlayer->mind -= MOVE_HUNGERCOST;
                activePlayer->health -= MOVE_HUNGERCOST;
                activeMonster->wakeMoves -= (activePlayer->mind  < 25 ? MONSTERWAKE_MINDPENALTY : 1);
            }
            activePlayer->currentDirection = player::LEFT;
            activePlayer->pos.x--;
            activePlayer->updateChar();
            activeMonster->wakeMoves -= (activePlayer->mind  < 25 ? MONSTERWAKE_MINDPENALTY : 1);
        }
        else {
            activePlayer->currentDirection = player::LEFT;
            activePlayer->updateChar();
        }
        break;
    case 's':
        if (movementPossible(activeRoom, activePlayer->pos.x, activePlayer->pos.y + 1)) {
            if (activePlayer->hunger > 0) {
                activePlayer->hunger -= MOVE_HUNGERCOST;
            }
            else {
                activePlayer->mind -= MOVE_HUNGERCOST;
                activePlayer->health -= MOVE_HUNGERCOST;
                activeMonster->wakeMoves -= (activePlayer->mind  < 25 ? MONSTERWAKE_MINDPENALTY : 1);
            }
            activePlayer->currentDirection = player::DOWN;
            activePlayer->pos.y++;
            activePlayer->updateChar();
            activeMonster->wakeMoves -= (activePlayer->mind  < 25 ? MONSTERWAKE_MINDPENALTY : 1);
        }
        else {
            activePlayer->currentDirection = player::DOWN;
            activePlayer->updateChar();
        }
        break;
    case 'd':
        if (movementPossible(activeRoom, activePlayer->pos.x + 1, activePlayer->pos.y)) {
            if (activePlayer->hunger > 0) {
                activePlayer->hunger -= MOVE_HUNGERCOST;
            }
            else {
                activePlayer->mind -= MOVE_HUNGERCOST;
                activePlayer->health -= MOVE_HUNGERCOST;
                activeMonster->wakeMoves -= (activePlayer->mind  < 25 ? MONSTERWAKE_MINDPENALTY : 1);
            }
            activePlayer->currentDirection = player::RIGHT;
            activePlayer->pos.x++;
            activePlayer->updateChar();
            activeMonster->wakeMoves -= (activePlayer->mind  < 25 ? MONSTERWAKE_MINDPENALTY : 1);
        }
        else {
            activePlayer->currentDirection = player::RIGHT;
            activePlayer->updateChar();
        }
        break;
    case 'q':
        if (activePlayer->player_lookingAt_door == true) {
            if (activePlayer->helditem != -1) {
                if (activePlayer->player_lookingAt_door == true && activePlayer->inventory[activePlayer->helditem]->itemName == "Key") {
                    activePlayer->percievedDoor->isLocked = false;
                    delete activePlayer->inventory[activePlayer->helditem];
                    activePlayer->inventory[activePlayer->helditem] = NULL;
                    activePlayer->helditem = -1;
                    system("cls");
                    cout << "The lock on the door clicks, then turns..." << endl;
                    confirm();
                    return;
                }
            }
        }
        else {
            activePlayer->use_self();
        }
        activeMonster->wakeMoves -= (activePlayer->mind  < 25 ? MONSTERWAKE_MINDPENALTY : 1);
        break;
    case '_':
        debugMenu(activeMonster, activePlayer, activeRoom, activeDoor);
        break;
    case '-':
        if (cheats) {
            EndGame("ENDGAME_DEBUG_TEST");
        }
        else {
            activeMonster->canMove = false;
        }
        break;
    case '+':
        if (cheats) {
            newRoom(activeMonster, activePlayer, activeRoom, activeDoor);
        }
        else {
            activeMonster->canMove = false;
        }
        break;/**/
    case 'e':
        activePlayer->printStats();
        activeMonster->wakeMoves -= (activePlayer->mind  < 25 ? MONSTERWAKE_MINDPENALTY : 1);
        break;
    case 'r':
        activePlayer->checkInv();
        activeMonster->wakeMoves -= (activePlayer->mind  < 25 ? MONSTERWAKE_MINDPENALTY : 1);
    case ' ':
        if (activePlayer->player_lookingAt_container == true || activePlayer->player_lookingAt_door == true) {
            if (activePlayer->player_lookingAt_container) {
                activePlayer->open(activePlayer->percievedContainer);
                activeMonster->wakeMoves -= (activePlayer->mind  < 25 ? MONSTERWAKE_MINDPENALTY : 1);
            }
            else if (activePlayer->player_lookingAt_door && activePlayer->percievedDoor->isLocked == false) {
                roomNum++;
                newRoom(activeMonster, activePlayer, activeRoom, activeDoor);
            }
        }
        else {
            activeMonster->canMove = false;
        }
        break;
    default:
        activeMonster->canMove = false;
        break;
    }
}
//this is how we check if a container/door is in an accessible position.
void door_posCheck(player* activePlayer, door* activeDoor) {
    activePlayer->player_lookingAt_door = false;
    switch (activePlayer->currentDirection) {
    case player::UP:
        if ((activeDoor->pos_left.x == activePlayer->pos.x && activeDoor->pos_left.y == activePlayer->pos.y - 1) || (activeDoor->pos_right.x == activePlayer->pos.x && activeDoor->pos_right.y == activePlayer->pos.y - 1) ) {
            activePlayer->player_lookingAt_door = true;
            activePlayer->percievedDoor = activeDoor;
        }
        break;
    case player::DOWN:
        if ((activeDoor->pos_left.x == activePlayer->pos.x && activeDoor->pos_left.y == activePlayer->pos.y + 1) || (activeDoor->pos_right.x == activePlayer->pos.x && activeDoor->pos_right.y == activePlayer->pos.y + 1)) {
            activePlayer->player_lookingAt_door = true;
            activePlayer->percievedDoor = activeDoor;
        }
        break;
    case player::LEFT:
        if ((activeDoor->pos_left.x == activePlayer->pos.x - 1 && activeDoor->pos_left.y == activePlayer->pos.y) || (activeDoor->pos_right.x == activePlayer->pos.x - 1 && activeDoor->pos_right.y == activePlayer->pos.y)) {
            activePlayer->player_lookingAt_door = true;
            activePlayer->percievedDoor = activeDoor;
        }
        break;
    case player::RIGHT:
        if ((activeDoor->pos_left.x == activePlayer->pos.x + 1 && activeDoor->pos_left.y == activePlayer->pos.y) || (activeDoor->pos_right.x == activePlayer->pos.x + 1 && activeDoor->pos_right.y == activePlayer->pos.y)) {
            activePlayer->player_lookingAt_door = true;
            activePlayer->percievedDoor = activeDoor;
        }
        break;
    default:
        break;
    }
}
void container_posCheck(player* activePlayer, room* activeRoom) {
    activePlayer->player_lookingAt_container = false;
    for (int cPos_i = 0; cPos_i < activeRoom->containernum; cPos_i++) {
        switch (activePlayer->currentDirection) {
        case player::UP:
            if (activeRoom->containers[cPos_i]->cPos.x == activePlayer->pos.x && activeRoom->containers[cPos_i]->cPos.y == activePlayer->pos.y - 1) {
                activePlayer->percievedContainer = activeRoom->containers[cPos_i];
                activePlayer->player_lookingAt_container = true;
                return;
            }
            break;
        case player::DOWN:
            if (activeRoom->containers[cPos_i]->cPos.x == activePlayer->pos.x && activeRoom->containers[cPos_i]->cPos.y == activePlayer->pos.y + 1) {
                activePlayer->percievedContainer = activeRoom->containers[cPos_i];
                activePlayer->player_lookingAt_container = true;
                return;
            }
            break;
        case player::LEFT:
            if (activeRoom->containers[cPos_i]->cPos.x == activePlayer->pos.x - 1 && activeRoom->containers[cPos_i]->cPos.y == activePlayer->pos.y) {
                activePlayer->percievedContainer = activeRoom->containers[cPos_i];
                activePlayer->player_lookingAt_container = true;
                return;
            }
            break;
        case player::RIGHT:
            if (activeRoom->containers[cPos_i]->cPos.x == activePlayer->pos.x + 1 && activeRoom->containers[cPos_i]->cPos.y == activePlayer->pos.y) {
                activePlayer->percievedContainer = activeRoom->containers[cPos_i];
                activePlayer->player_lookingAt_container = true;
                return;
            }
            break;
        default:
            break;
        }
    }
}
int main() {
    srand(time(0));
    bool startscreen = true;
    string menu[] = { "***** CONSOLE SURVIVAL GAME *****", "BY: LUCAS REED", "[W]: UP | [S]: DOWN | [SPACE]: SELECT", " ", "START", "CONTROLS", "QUIT" };
    short start_select_pos = 4;
    while (startscreen) {
        system("cls");
        for (int start_i = 0; start_i < sizeof(menu)/sizeof(menu[0]); start_i++) {
            cout << menu[start_i];
            if (start_i == start_select_pos) {
                cout << "<";
            }
            cout << endl;
        }
        char startInput = _getch();
        if (startInput == 'w' || startInput == 'W') {
            if (start_select_pos != 4) {
                start_select_pos--;
            }
        }
        else if (startInput == 's' || startInput == 'S') {
            if (start_select_pos != 6) {
                start_select_pos++;
            }
        }
        else if (startInput == ' ') {
            switch (start_select_pos) {
            default:
                break;
            case 4:
                system("cls");
                startscreen = false;
                break;
            case 5:
                system("cls");
                cout << "CONTROLS:" << endl;
                cout << "[W][A][S][D]: MOVE" << endl;
                cout << "[SPACE]: INTERACT" << endl;
                cout << "[R]: INVENTORY" << endl;
                cout << "[E]: STATS" << endl;
                cout << "[Q]: USE ON SELF | USE ON DOOR" << endl;
                confirm();
                break;
            case 6:
                return 0;
            }
        }
    }

    //cout << "I need to keep running..." << endl;
    confirm();
    cout << "Loading...";
    player* p = new player;
    door* d = new door;
    room* r = new room(d);
    monster* m = new monster;
    while (game_active) {
        container_posCheck(p, r);
        if (debug_displayProcess) {
            cout << "DEBUG: container_posCheck() successful\n";
        }
        door_posCheck(p, d);
        if (debug_displayProcess) {
            cout << "DEBUG: door_posCheck() successful\n";
        }
        if (m->canMove == true) {
            monster_actionAttempt(m, p, r);
        }
        if (m->spawned == false && m->awake == true) {
            m->spawned = true;
        }
        if (game_active == false) {
            break;
        }
        r->printRoom(p, m);
        m->canMove = true;
        player_action(m, p, r, d);
    }
    return 0;
}
//use equipped item on self.
void player::use_self() {
    system("cls");
    if (helditem != -1) {
        if (inventory[helditem]->isConsumable == true) {
            if (inventory[helditem]->isKey == false) {
                int tempReference = helditem;
                helditem = -1;
                if (inventory[tempReference]->health != 0) {
                    health += inventory[tempReference]->health;
                    if (inventory[tempReference]->itemName == "Medical Supplies") {
                        cout << "With Gauze, Antiseptic, and Bandages at your disposal, you've done a good job at patching yourself up..." << endl;
                    }
                    if (inventory[tempReference]->itemName == "Clean Bandages") {
                        cout << "As you tend to your wounds, you find comfort knowing the bandages are clean..." << endl;
                    }
                    if (inventory[tempReference]->itemName == "Dirty Bandages") {
                        cout << "I'm 100% sure this will get infected..." << endl;
                    }
                    if (health > 100) {
                        health = 100;
                    }
                }
                if (inventory[tempReference]->mind != 0) {
                    if (inventory[tempReference]->itemName == "Shard of Glass") {
                        cout << "As the jagged shard of glass rips open your arm, you feel just a little bit more sane..." << endl;
                        //this next bit is for an item that isn't yet implemented
                    }
                    else if (inventory[tempReference]->itemName == "Familiar Doll") {
                        cout << "You look at the doll as it crumbles to ash in your hands. it's presence, however fleeting, brings you comfort..." << endl;
                    }
                    mind += inventory[tempReference]->mind;
                    if (mind > 100) {
                        mind = 100;
                    }
                }
                if (inventory[tempReference]->hunger != 0) {
                    hunger += inventory[tempReference]->hunger;
                    if (inventory[tempReference]->itemName == "Old Sandwich") {
                        cout << "With closed eyes and held breath, you feel fuzzy bits of mold brush along your teeth as you take a bite..." << endl;
                    }
                    if (inventory[tempReference]->itemName == "Granola Bar") {
                        cout << "It's something..." << endl;
                    }
                    if (inventory[tempReference]->itemName == "MRE") {
                        cout << "To be fair, it's supposed to be practical, not delicious..." << endl;
                    }
                    if (hunger < 100) {
                        cout << "You got food in your system, but your stomach growls still... " << endl;
                    }
                    else if (hunger >= 100 && hunger < 115) {
                        cout << "Your hunger has been sated." << endl;
                    }
                    else if (hunger >= 115 && hunger < 135) {
                        cout << "You feel full" << endl;
                    }
                    else if (hunger >= 135 && hunger <= 150) {
                        cout << "You feel nauseatingly full. Eat any more and you risk losing your lunch." << endl;
                    }
                    else if (hunger > 150) {
                        cout << "You pushed your limits and now you pay the consequences. You've thrown up." << endl;
                        hunger -= OVEREAT_PENALTY;
                    }
                    if (random(1, 100) < inventory[tempReference]->poisonchance) {
                        cout << "Bad food means bad time, you feel sick to your stomach." << endl;
                        hunger -= FOODPOISON_PENALTY;
                        mind -= 20;
                        health -= 5;

                    }
                }
                delete inventory[tempReference];
                inventory[tempReference] = NULL;
                confirm();
                return;
            }
            else {
                cout << "What am I supposed to do? Unlock myself?" << endl;
                confirm();
                return;
            }
        }
    }
    else {
        cout << "I have nothing equipped." << endl;;
        confirm();
        return;
    }
}