#include <iostream>  
#include <string>
#include <fstream>
#include <random>  
#include <cstdlib>  // rand() - exit ()
#include <ctime>   // time() - srand()
#include <windows.h>  // Sleep() - color
#include <conio.h>  // getch()
#include <chrono>   // start-stop tracking time - duration
#include <mmsystem.h> // play sound
#include <filesystem>

using namespace std;
using namespace chrono;

// Global Items //////////////////////////////////////////////////////////////////////////////

// Main Menu /////////////////////////////////////////////////////////////////////////////////
const string ADMINISTRATORPASSWORD = "SPACEINVADERS.clear";

const string pauseMenuOption[] = {
    "NEW GAME",
    "LOAD GAME",
    "PVP",
    "CREATE AN ACCOUNT",
    "GUIDE",
    "LEADER BOARD",
    "CLEAR DATA",
    "EXIT"
};

const int numOption = sizeof(pauseMenuOption) / sizeof(pauseMenuOption[0]);
int currentOption = 0; // Keeps track of the current selected option

//////////////////////////////////////////////////////////////////////////////////////////////
const string pauseMenuOptions[] = {
    "RESUME",
    "SAVE & RETURN TO THE MAIN MENU",
    "SAVE & EXIT",
};
const int numPauseOption = sizeof(pauseMenuOptions) / sizeof(pauseMenuOptions[0]);
int currentPauseOption = 0; // Keeps track of the current selected option

struct user
{
    string username;
    string password;
};

struct player
{
    string username;
    int score;
    double timeTaken;
    int level;
};

user userList[100];
int currentUsers = 0;

player currentPlayers[100];
string currentPlayersName[100];
int currentPlayersCount = 0;

const int width = 50; 
const int height = 25; 
string screen[height][width];

int x, y; // ship position

const int maxMissiles = 1; 

int missileX[maxMissiles], missileY[maxMissiles]; // The location of the missiles
bool missileActive[maxMissiles]; // Active or inactive status of missiles

steady_clock::time_point lastFrameTime; // The time of the last frame
const double FPS = 60.0; // Number of frames per second

string playerName;
int playerScore = 0;
double timePlayed = 0;

steady_clock::time_point gameStartTime; // Game start time


bool gameOver = false;

steady_clock::time_point pauseStartTime; // Pause start time
steady_clock::time_point pauseEndTime;   // pause end time
double totalPauseDuration = 0.0;         // Total pause time

int playerLives = 3;

int currentLevel = 1;


const int numShields = 4; 
const int shieldwidth = 4; 
const int shieldY = height - 5; 
const int shieldMaxHealth = 3; // The number of lives of each shield house
int shields[numShields][shieldwidth];


// Phases /////////////////////////////////////////////////////////////////////////////////////

string enemy = u8"\033[32m⩔\033[0m";

string enemiesPhase1[5][15] = {{enemy," ",enemy," ",enemy," ",enemy," ",enemy," ",enemy," ",enemy," ",enemy},
                               {" "," "," "," "," "," "," "," "," "," "," "," "," "," "," "},
                              {enemy," ",enemy," ",enemy," ",enemy," ",enemy," ",enemy," ",enemy," ",enemy},
                              {" "," "," "," "," "," "," "," "," "," "," "," "," "," "," "},
                              {enemy," ",enemy," ",enemy," ",enemy," ",enemy," ",enemy," ",enemy," ",enemy}};

const int enemiesRows = 5;
const int enemiesColumns = 15;
int enemyX = 1; // enemies location
int enemyY = 2; // enemies location
bool moveRight = true; // enemies movement
double enemySpeed = 0.5; // enemies speed (s)
steady_clock::time_point lastEnemyMoveTime;

// specialShip
bool specialShipActive = false;
int specialShipX = 0;           // X position of special ship
int specialShipY = 1;           // Y position of special ship
const double specialShipSpeed = 0.1; // Special ship movement speed (seconds)
steady_clock::time_point lastSpecialShipMoveTime; // The time of the last departure of the special ship

steady_clock::time_point lastSpecialShipExitTime; // The time of the last departure of the special ship
const double specialShipCooldownTime = 10.0; // Cooldown time between two special ships (seconds)

int maxEnemyMissiles = 1; // Maximum number of enemy arrows
int enemyMissileX[100], enemyMissileY[100]; // Position of enemy arrows
bool enemyMissileActive[100]; // Active or inactive status of enemy arrows

steady_clock::time_point lastEnemyShootTime; // The time of the enemy's last shot
double enemyShootCooldown = 2.0; // Delay time between enemy shots (seconds)

// PVP //

// Structure for Barrier
struct Barrier {
    int x, y;
    int health; // Health of the barrier (initially 2)
    bool active; // Active state of the barrier
};

Barrier barriers_p1[5]; // 5 barriers for player 1
Barrier barriers_p2[5]; // 5 barriers for player 2

const char EMPTY = ' ';
const string PLAYER1 = "\033[31m⩔\033[0m";
const string PLAYER2 = "\033[36m⩓\033[0m";
const string BULLETP1 = "\033[31m|\033[0m";
const string BULLETP2 = "\033[36m|\033[0m";
const string BARRIER1 = "\033[32m=\033[0m";
const string BARRIER2 = "\033[33m-\033[0m";

// Game variables
const int PVPwidth = 40;
const int PVPheight = 25;
string board[PVPheight][PVPwidth];
int p1_x = PVPwidth / 2, p1_y = 1;
int p2_x = PVPwidth / 2, p2_y = PVPheight - 2;
int p1_lives = 3;
int p2_lives = 3;
int bullets_p1[10][2]; // Max 10 bullets for Player 1
int bullets_p2[10][2]; // Max 10 bullets for Player 2
int bullet_count_p1 = 0;
int bullet_count_p2 = 0;
int active_bullets_p1 = 0; // Active bullets for Player 1
int active_bullets_p2 = 0; // Active bullets for Player 2




// Functions Prototypes //////////////////////////////////////////////////////////////////////

// main game
void setup();
void draw();
void setCursorPosition(int x, int y);
void updateScreen();
void playerInput();
void rocketLogic();
void enemyLogic();
void specialShipLogic();
void activateSpecialShip();
void moveSpecialShip();
void drawSpecialShip();
void checkSpecialShipHit();
void checkMissilestrike();
void sleepUntilNextFrame();
void gameLoop();
void pauseGame();
void drawPauseMenu();
void printPauseMenuOption(int option, int color, int yOffset);
void setColor(int textColor, int bgColor);
void allienGifPart1();
void allienGifPart2();
void sleepyPrint(string str);
void saveGame(string username);
void loadGame(string username);
void showTime();
void checkPlayerHit();
void enemyMissileLogic();
void enemyShoot();
bool areAllEnemiesDestroyed();
void nextLevel();
void calculateLevel();
void reSetup();
void setupShield();

// PVP //

void PVPGame();

void reSetupPVP();

void clearScreenPVP();
void drawBoardPVP();
void initBoardPVP();
void updateBoardPVP();
void handleInputPVP();
void updateBulletsPVP();
void gameLoopPVP();
void placeBarriersPVP();


// menu //
void allienLogo1txt();
void mainShipLogotxt();
void allienLogo2txt();
void MainMenu();
void printMenuOption(int option, int color, int yOffset);
void drawMenu();
void createAnAccount();
void saveUsersData();
void loadUsersData();
void deleteData();
void clearData();
void checkAccount();
void guide();
void createFolder(string folderPath);
void removeFolder(string folderPath);
bool isSaveFileExist(string username);
void displayRankedPlayers();
void loadCurrentPlayersName();
void loadAllPlayersData();
void merge(player arr[], int left, int mid, int right);
void mergeSort(player arr[], int left, int right);
bool comparePlayers(const player& p1, const player& p2);
bool playerExists(const string& playerName);
void savePlayerName(const string& playerName);
bool loadCheckPlayerData(const string& playerName, player& player);
void updatePlayerRecord(const string& playerName);

// main //////////////////////////////////////////////////////////////////////////////////////

int main(){
    
    // Hide the console cursor
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = false; // Set cursor to hidden mode
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    while(true){

        loadUsersData();
        loadCurrentPlayersName();
        loadAllPlayersData();

        MainMenu();
        system("cls");
        reSetup();
        gameLoop();
    }

}


// Functions /////////////////////////////////////////////////////////////////////////////////

void setColor(int textColor, int bgColor) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (textColor + (bgColor * 16)));
}

void sleepyPrint(string str){
	for(int i=0 ; i < str.length() ; i++){
		Sleep(50);
		cout << str[i];
	}
}


void setup() {

    currentLevel = 1; // Start from the first step
    maxEnemyMissiles = 1; // The number of enemy arrows in the first stage
    enemyShootCooldown = 2.0; // Delay time between enemy shots in the first stage

    x = width / 2; // Set the initial position of the ship in the center of the width
    y = height - 2; // Set the initial position of the ship at the bottom of the screen

    for (int i = 0; i < maxMissiles; i++) {
        missileX[i] = -1; // Adjusting the initial position of the missiles
        missileY[i] = -1; // Adjusting the initial position of the missiles
        missileActive[i] = false; // Set initial state of missiles to inactive
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            screen[i][j] = " "; 
        }
    }

    setupShield();



    // Set console buffer size
    setCursorPosition(width, height);

    lastFrameTime = steady_clock::now(); // Record the current time as the time of the last frame
}

void draw() {
    setCursorPosition(0, 0);     // Move the cursor to the upper-left corner

    // draw board
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            cout << screen[i][j];
        }
        cout << endl;
    }
    setCursorPosition(width + 8, (height / 2) - 5);
    cout << "\033[35m╔\033[0m";
    for (int i = 0; i < 18; i++){cout << "\033[35m═\033[0m";}
    setCursorPosition(width + 8 + 19 , (height / 2) - 5);
    cout << "\033[35m╗\033[0m";

    setCursorPosition(width + 10 , (height / 2) - 3);
    cout << "    LEVEL: " << "\033[33m" << currentLevel << "\033[0m";
    setCursorPosition(width + 10 , (height / 2) - 1);
    cout << "    SCORE: " << "\033[36m" << playerScore << "\033[0m";
    setCursorPosition(width + 10 , (height / 2) + 1);
    cout << "    LIVES: " << "\033[31m" << playerLives << "\033[0m";
    setCursorPosition(width + 10 , (height / 2) + 3);
    cout << " TIME: " << timePlayed;

    setCursorPosition(width + 8, (height / 2) - 5 + 10);
    cout << "\033[35m╚\033[0m";
    for (int i = 0; i < 18; i++){cout << "\033[35m═\033[0m";}
    cout << "\033[35m╝\033[0m";
}

void setCursorPosition(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void updateScreen() {
    // Delete previous positions
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            screen[i][j] = " "; // Clear board
        }
    }

    // Draw a box around the page
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (i == 0 && j == 0) {
                screen[i][j] = "\033[35m╔\033[0m"; // Upper-left corner
            } else if (i == 0 && j == width - 1) {
                screen[i][j] = "\033[35m╗\033[0m"; // upper-right corner
            } else if (i == height - 1 && j == 0) {
                screen[i][j] = "\033[35m╚\033[0m"; // Lower-left corner
            } else if (i == height - 1 && j == width - 1) {
                screen[i][j] = "\033[35m╝\033[0m"; // lower-right corner
            } else if (i == 0 || i == height - 1) {
                screen[i][j] = "\033[35m═\033[0m"; // Top and bottom horizontal lines
            } else if (j == 0 || j == width - 1) {
                screen[i][j] = "\033[35m║\033[0m"; // Left and right vertical lines
            }
        }
    }

    // Drawing the player's ship (inside the box)
    if (x >= 1 && x < width - 1 && y >= 1 && y < height - 1) {
        screen[y][x] = "\033[36m⩓\033[0m";
    }

    // Drawing the player's rockets (inside the box)
    for (int i = 0; i < maxMissiles; i++) {
        if (missileActive[i] && missileY[i] >= 1 && missileY[i] < height - 1 && missileX[i] >= 1 && missileX[i] < width - 1) {
            screen[missileY[i]][missileX[i]] = "\033[32m|\033[0m";
        }
    }

    // drawing enemy arrows (inside the box)
    for (int i = 0; i < maxEnemyMissiles; i++) {
        if (enemyMissileActive[i] && enemyMissileY[i] >= 1 && enemyMissileY[i] < height - 1 && enemyMissileX[i] >= 1 && enemyMissileX[i] < width - 1) {
            screen[enemyMissileY[i]][enemyMissileX[i]] = "\033[31m|\033[0m";
        }
    }

    // drawing enemy arrows (inside the box)
    for (int i = 0; i < enemiesRows; i++) {
        for (int j = 0; j < enemiesColumns; j++) {
            if (enemiesPhase1[i][j] == enemy) {
                int posX = enemyX + j;
                int posY = enemyY + i;
                if (posX >= 1 && posX < width - 1 && posY >= 1 && posY < height - 1) {
                    screen[posY][posX] = enemy;
                }
            }
        }
    }

    // Special ship drawing (inside the box)
    if (specialShipActive && specialShipX >= 1 && specialShipX < width - 1 && specialShipY >= 1 && specialShipY < height - 1) {
        screen[specialShipY][specialShipX] = "\033[31m$\033[0m";
    }


    for (int i = 0; i < numShields; i++) {
        for (int j = 0; j < shieldwidth; j++) {
            int shieldX = (width / (numShields + 1)) * (i + 1) - shieldwidth / 2 + j;
            if (shieldX >= 0 && shieldX < width) {
                if (shields[i][j] == 3) screen[shieldY][shieldX] = "\033[35m≡\033[0m";       // Shield 3
                else if (shields[i][j] == 2) screen[shieldY][shieldX] = "\033[36m=\033[0m"; // Shield 2
                else if (shields[i][j] == 1) screen[shieldY][shieldX] = "\033[33m-\033[0m"; // Shield 1
            }
        }
    }
}

void playerInput() {
    if (_kbhit()) { // Check input from the keyboard
        switch (_getch()) {
            case 'a':
                if (x > 1) x--; // move left (left border)
                break;
            case 'd':
                if (x < width - 2) x++; // move right (right border)
                break;
            case ' ':
                for (int i = 0; i < maxMissiles; i++) {
                    if (!missileActive[i]) { // Find the inactive missile
                        missileActive[i] = true; // Activate the missile
                        missileX[i] = x; // Adjusting the initial position of the missile
                        missileY[i] = y - 1; // Adjusting the initial position of the missile
                        break;
                    }
                }
                break;
            case 27: // pause
                pauseGame();
                break;
        }
    }
}

void rocketLogic() {

    for (int i = 0; i < maxMissiles; i++) {
        if (missileActive[i]) {
            missileY[i]--; // Move up
            if (missileY[i] < 0) { // Check if the rocket has left the screen
                missileActive[i] = false; // Disable the missile
            }
        }
    }
}

void sleepUntilNextFrame() {
    steady_clock::time_point now = steady_clock::now();
    duration<double> timeSpan = duration_cast<duration<double>>(now - lastFrameTime); // Calculation of elapsed time
    double sleepTime = (1.0 / FPS) - timeSpan.count(); // Calculation of rest time until the next frame
    if (sleepTime > 0) {
        Sleep(sleepTime * 1000); // Rest for the calculated period
    }
    lastFrameTime = steady_clock::now(); // Record the current time as the time of the last frame
}

void gameLoop() {
    gameStartTime = steady_clock::now(); // Game start time
    while (!gameOver) {
        playerInput(); // Receive input from the keyboard
        rocketLogic(); // Game logic processing
        enemyLogic();
        enemyShoot();
        enemyMissileLogic();
        specialShipLogic();
        checkMissilestrike();
        checkPlayerHit();
        updateScreen();
        draw();
        sleepUntilNextFrame(); // Rest until next frame
        showTime();

        // Checking the destruction of all enemies
        if (areAllEnemiesDestroyed()) {
            system("cls");
            setColor(2, 0);
            allienGifPart2();
            setCursorPosition(13, 6);
            cout << "CONGRATULATIONS! ALL ENEMIES DESTROYED!";
            setCursorPosition(13 , 8);
            cout << "WELCOME TO THE NEXT LEVEL!";
            setColor(7, 0);
            Sleep(4000);
            nextLevel(); // Go to the next step
        }
    }
}

void enemyLogic() {
    steady_clock::time_point now = steady_clock::now();
    duration<double> timeSpan = duration_cast<duration<double>>(now - lastEnemyMoveTime);

    // Enemy movement
    if (timeSpan.count() >= enemySpeed) {
        if (moveRight) {
            if (enemyX + enemiesColumns < width - 1) { // Checking the right side
                enemyX++;
            } else {
                moveRight = false;
                enemyY++;
            }
        } else {
            if (enemyX > 1) { // Checking the left side
                enemyX--;
            } else {
                moveRight = true;
                enemyY++;
            }
        }

        // Check the lower side
        if (enemyY + enemiesRows >= height - 2) {
            cout << "Game Over!" << endl;
            Sleep(5000);
            exit(0);
        }

        // Updated enemy movement times
        lastEnemyMoveTime = steady_clock::now();
    }
}


void activateSpecialShip() {
    steady_clock::time_point now = steady_clock::now();
    duration<double> timeSpan = duration_cast<duration<double>>(now - lastSpecialShipExitTime);

    // Only when the ship is inactive and the cooldown period is over will the special ship be activated
    if (!specialShipActive && timeSpan.count() >= specialShipCooldownTime) {
        srand(static_cast<unsigned int>(time(nullptr)));
        int randomNum = rand() % 100 + 1;
        if (randomNum % 100 < 10) { // 10% chance to activate special ship
            specialShipActive = true;
            specialShipX = 0; // Start from the left side of the page
            specialShipY = 1; // Always in the first line
            lastSpecialShipMoveTime = now; // record the start time of movement
        }
    }
}


void moveSpecialShip() {
    if (specialShipActive) {
        steady_clock::time_point now = steady_clock::now();
        duration<double> timeSpan = duration_cast<duration<double>>(now - lastSpecialShipMoveTime);

        if (timeSpan.count() >= specialShipSpeed) {
            specialShipX++; // Move to the right
            if (specialShipX >= width - 1) { // Checking the right border
                specialShipActive = false; // Disable if page exits
                lastSpecialShipExitTime = now; // Record the departure time of the special ship from the page
            }
            lastSpecialShipMoveTime = now;
        }
    }
}


void drawSpecialShip() {
    if (specialShipActive && specialShipX >= 0 && specialShipX < width) {
        screen[specialShipY][specialShipX] = "\033[31m$\033[0m";
    }
}


void checkMissilestrike() {

    for (int i = 0; i < maxEnemyMissiles; i++) {
        if (enemyMissileActive[i]) {
            for (int shieldIndex = 0; shieldIndex < numShields; shieldIndex++) {
                for (int j = 0; j < shieldwidth; j++) {
                    int shieldX = (width / (numShields + 1)) * (shieldIndex + 1) - shieldwidth / 2 + j;
                    if (enemyMissileX[i] == shieldX && enemyMissileY[i] == shieldY && shields[shieldIndex][j] > 0) {
                        shields[shieldIndex][j]--; // Decreased shield life
                        enemyMissileActive[i] = false; // Disable the enemy's arrow
                        break;
                    }
                }
            }
        }
    }

    for (int i = 0; i < maxMissiles; i++) {
        if (missileActive[i]) {
            for (int shieldIndex = 0; shieldIndex < numShields; shieldIndex++) {
                for (int j = 0; j < shieldwidth; j++) {
                    int shieldX = (width / (numShields + 1)) * (shieldIndex + 1) - shieldwidth / 2 + j;
                    if (missileX[i] == shieldX && missileY[i] == shieldY && shields[shieldIndex][j] > 0) {
                        shields[shieldIndex][j]--;
                        missileActive[i] = false;
                        break;
                    }
                }
            }
        }
    }


    for (int i = 0; i < maxMissiles; i++) {
        if (missileActive[i]) {
            for (int row = 0; row < enemiesRows; row++) {
                for (int col = 0; col < enemiesColumns; col++) {
                    if (enemiesPhase1[row][col] == enemy) {
                        int posX = enemyX + col;
                        int posY = enemyY + row;
                        if (missileX[i] == posX && missileY[i] == posY) {
                            enemiesPhase1[row][col] = " ";
                            missileActive[i] = false;
                            playerScore += 10;
                            break;
                        }
                    }
                }
            }
        }
    }
    checkSpecialShipHit();
}

void specialShipLogic(){
    activateSpecialShip();
    moveSpecialShip();
}

void checkSpecialShipHit() {
    if (specialShipActive) {
        for (int i = 0; i < maxMissiles; i++) {
            if (missileActive[i] && missileY[i] == specialShipY && missileX[i] == specialShipX) {
                specialShipActive = false;
                missileActive[i] = false;
                playerScore += 100;
                lastSpecialShipExitTime = steady_clock::now(); // Record the time of exit from the page after collision
                break;
            }
        }
    }
}



void pauseGame() {
    pauseStartTime = steady_clock::now(); // Record the start time of the pause

    int i = 0;

    while (true) {
        system("cls");
        if (i % 2 == 0) allienGifPart1();
        else allienGifPart2();
        i++;

        // Draw the initial menu without clearing the screen every time
        drawPauseMenu();

        char inputChar = getch();

        if (inputChar == 'w' || inputChar == 'W') {
            // Move up in the menu
            currentPauseOption = (currentPauseOption - 1 + numPauseOption) % numPauseOption;
            drawPauseMenu(); // Redraw only the updated menu section
        }
        else if (inputChar == 's' || inputChar == 'S') {
            // Move down in the menu
            currentPauseOption = (currentPauseOption + 1) % numPauseOption;
            drawPauseMenu(); // Redraw only the updated menu section
        }
        else if (inputChar == '\r') { // Enter key to select option
            switch (currentPauseOption) {
                case 0: {
                    // resume
                    pauseEndTime = steady_clock::now(); // Record the pause end time
                    duration<double> pauseDuration = duration_cast<duration<double>>(pauseEndTime - pauseStartTime);
                    totalPauseDuration += pauseDuration.count(); // Add pause time to total pause time
                    system("cls");
                    setColor(7, 0);
                    return;
                    break;
                }
                case 1:
                    gameOver = true;
                    return;
                case 2:
                    updatePlayerRecord(playerName);
                    savePlayerName(playerName);
                    system("cls");
                    setColor(2, 0);
                    sleepyPrint(" < DONE > ");
                    Sleep(2000);
                    exit(0);
                    break;

            }
        }
    }
}


void drawPauseMenu(){
    // Draw the static parts of the menu (title, developer info, etc.)
    setCursorPosition(10, 3);
    setColor(2, 0);
    cout << "PASUE MENU";
    setCursorPosition(10, 5);
    setColor(7, 0);
    cout << "DEVELOPED BY \033[35mARYAN GHASEMI & SEYED SADRA SEYED TABAI\033[0m";


    // Draw all the menu options with increased space between them
    for (int i = 0; i < numPauseOption; ++i) {
        // Adjust spacing for each menu option (2 spaces between options)
        printPauseMenuOption(i, 2, 9 + (i * 2)); // Increased spacing between options (2)
    }
}

void printPauseMenuOption(int option, int color, int yOffset) {
    setCursorPosition(10, yOffset); // Set cursor position before printing each option
    setColor(color, 0);

    // Check if the current option should be highlighted
    if (option == currentPauseOption) {
        // Print a box around the selected option
        cout << "\033[35m[\033[0m " << "\033[35m" << pauseMenuOptions[option]
        << "\033[0" << "\033[35m ]\033[0m";
    } else {
        // Otherwise, just print the option without the box
        cout << "  "<< pauseMenuOptions[option] << "  ";
    }

    cout << "\n";
}

void saveGame(string username) {
    ofstream outFile("DATA/playersData/" + username + "_save.txt");

    if (outFile.is_open()) {
        // Save player information
        outFile << "Score: " << playerScore << endl;
        outFile << "Level: " << currentLevel << endl;
        steady_clock::time_point now = steady_clock::now();
        duration<double> elapsedTime = duration_cast<duration<double>>(now - gameStartTime);
        outFile << "Time: " << elapsedTime.count() << endl;
        outFile << "Lives: " << playerLives << endl;
        outFile.close();
    } else {
        cout << "Error saving the game!" << endl;
    }
}

void loadGame(string username) {
    ifstream inFile("DATA/PlayersData/" + username + "_save.txt");

    if (inFile.is_open()) {
        string line;
        while (getline(inFile, line)) {
            if (line.find("Score:") != string::npos) {
                playerScore = stoi(line.substr(7));
            } else if (line.find("Level:") != string::npos) {
                currentLevel = stoi(line.substr(7));
            } else if (line.find("Time:") != string::npos) {
                double savedTime = stod(line.substr(6));
                gameStartTime = steady_clock::now() - seconds(static_cast<int>(savedTime)); // Restore game time
            } else if (line.find("Lives:") != string::npos) {
                playerLives = stoi(line.substr(7));
            }
        }
        inFile.close();
    } else {
        cout << "Error loading the game!" << endl;
    }
}



void allienGifPart1() {
    ifstream file("DATA/Logo/allienGifPart1.txt");
    string line;

    setColor(2, 0);

    if (file.is_open()) {
        while (getline(file, line)) {
            cout << line << endl;
        }
        file.close();
    }
}

void allienGifPart2() {
    ifstream file("DATA/Logo/allienGifPart2.txt");
    string line;

    setColor(2, 0);

    if (file.is_open()) {
        while (getline(file, line)) {
            cout << line << endl;
        }
        file.close();
    }
}

void showTime() {
    steady_clock::time_point now = steady_clock::now();
    duration<double> elapsedTime = duration_cast<duration<double>>(now - gameStartTime);
    double seconds = elapsedTime.count() - totalPauseDuration; // Subtract the pause time from the total time

    timePlayed = seconds;
}


void checkPlayerHit() {
    for (int i = 0; i < maxEnemyMissiles; i++) {
        if (enemyMissileActive[i] && enemyMissileY[i] == y && enemyMissileX[i] == x) {
            playerLives--;
            enemyMissileActive[i] = false;

            if (playerLives <= 0) {
                saveGame(playerName);
                gameOver = true;
                setColor(4,0);
                setCursorPosition(53, 13);
                cout << "GAME OVER ! YOU RAN OUT OF LIVES ";
                updatePlayerRecord(playerName);
                Sleep(5000);
                exit(0);
            }
        }
    }
}

void enemyMissileLogic() {
    for (int i = 0; i < maxEnemyMissiles; i++) {
        if (enemyMissileActive[i]) {
            enemyMissileY[i]++; // The enemy's arrow moves downwards
            if (enemyMissileY[i] >= height) {
                enemyMissileActive[i] = false; // If the arrow goes off the screen, it will be disabled
            }
        }
    }
}

void enemyShoot() {
    steady_clock::time_point now = steady_clock::now();
    duration<double> timeSpan = duration_cast<duration<double>>(now - lastEnemyShootTime);

    if (timeSpan.count() >= enemyShootCooldown) {
        // Count the number of active enemies in the enemiesPhase1 matrix
        int enemyCount = 0;
        for (int row = 0; row < enemiesRows; row++) {
            for (int col = 0; col < enemiesColumns; col++) {
                if (enemiesPhase1[row][col] == enemy) {
                    enemyCount++;
                }
            }
        }

        // If there are no active enemies, the function exits
        if (enemyCount == 0) {
            return;
        }

        // Choose a random enemy from among active enemies
        int randomEnemyIndex = rand() % enemyCount;
        int currentIndex = 0;
        int selectedRow = -1, selectedCol = -1;
        for (int row = 0; row < enemiesRows; row++) {
            for (int col = 0; col < enemiesColumns; col++) {
                if (enemiesPhase1[row][col] == enemy) {
                    if (currentIndex == randomEnemyIndex) {
                        selectedRow = row;
                        selectedCol = col;
                        break;
                    }
                    currentIndex++;
                }
            }
            if (selectedRow != -1) {  // The desired enemy has been found
                break;
            }
        }

        // Find the first inactive enemy missile to fire
        for (int i = 0; i < maxEnemyMissiles; i++) {
            if (!enemyMissileActive[i]) {
                enemyMissileActive[i] = true;
                // Setting the position of the arrow based on the coordinates of the selected enemy
                enemyMissileX[i] = enemyX + selectedCol;
                enemyMissileY[i] = enemyY + selectedRow + 1; // Shoot from below the enemy
                lastEnemyShootTime = now; // Update time of last shot
                break;
            }
        }
    }
}


bool areAllEnemiesDestroyed() {
    for (int i = 0; i < enemiesRows; i++) {
        for (int j = 0; j < enemiesColumns; j++) {
            if (enemiesPhase1[i][j] == enemy) {
                return false; // At least one enemy remains
            }
        }
    }
    return true; // All enemies are destroyed
}

void nextLevel() {

    currentLevel++;

    for (int i = 0; i < enemiesRows ; i++) {
        for (int j = 0; j < enemiesColumns; j++) {
            if(i % 2 == 0 && j % 2 == 0){enemiesPhase1[i][j] = enemy;}
            else {enemiesPhase1[i][j] = " ";}
        }
    }


    enemySpeed = enemySpeed * 0.8;

    // Increasing the speed of enemy arrows
    // Reduce latency (faster)
    enemyShootCooldown *= 0.5;

    // Set the initial position of the enemies
    enemyX = 1;
    enemyY = 2;

    playerLives = 3;

    setupShield();

    system("cls");

}

void calculateLevel(){
    for(int i = 0; i < currentLevel; i++){
        enemySpeed = enemySpeed * 0.8;
        enemyShootCooldown *= 0.5;
    }
    playerLives = 3;

    setupShield();
}

void reSetup(){

    playerScore = 0;
    timePlayed = 0;

    gameOver = false;

    totalPauseDuration = 0.0;         // Total pause time

    playerLives = 3;

    currentLevel = 1;


    enemyX = 1; // enemies location
    enemyY = 2; // enemies location

    for (int i = 0; i < enemiesRows ; i++) {
        for (int j = 0; j < enemiesColumns; j++) {
            if(i % 2 == 0 && j % 2 == 0){enemiesPhase1[i][j] = enemy;}
            else {enemiesPhase1[i][j] = " ";}
        }
    }

    moveRight = true; // enemies movement

    enemySpeed = 0.5; // enemies speed (s)

// specialShip
    specialShipActive = false;
    specialShipX = 0;           // X position of special ship
    specialShipY = 1;           // Y position of special ship


    maxEnemyMissiles = 1; // Maximum number of enemy arrows
    enemyMissileX[100], enemyMissileY[100]; // Position of enemy arrows
    enemyMissileActive[100]; // Active or inactive status of enemy arrows


    enemyShootCooldown = 2.0; // Delay time between enemy shots (seconds)


    setup();
}


void setupShield(){
    for (int i = 0; i < numShields; i++) {
        for (int j = 0; j < shieldwidth; j++) {
            shields[i][j] = shieldMaxHealth; // Initialization with maximum life
        }
    }
}



// Main Menu Functions /////////////////////////////////////////////////////////////////////
void printMenuOption(int option, int color, int yOffset) {
    setCursorPosition(10, yOffset); // Set cursor position before printing each option
    setColor(color, 0);

    // Check if the current option should be highlighted
    if (option == currentOption) {
        // Print a box around the selected option
        cout << "\033[35m[\033[0m " << "\033[35m" << pauseMenuOption[option]
        << "\033[0" << "\033[35m ]\033[0m";
    } else {
        // Otherwise, just print the option without the box
        cout << "  "<< pauseMenuOption[option] << "  ";
    }

    cout << "\n";
}

void drawMenu() {
    setCursorPosition(10, 3);
    setColor(2, 0);
    cout << "SPACE INVADERS";
    setCursorPosition(10, 5);
    setColor(7, 0);
    cout << "DEVELOPED BY \033[35mARYAN GHASEMI & SEYED SADRA SEYED TABAI\033[0m";


    // Draw all the menu options with increased space between them
    for (int i = 0; i < numOption; ++i) {
        // 2 spaces between options
        printMenuOption(i, 2, 9 + (i * 2));
    }
}

void MainMenu() {
    system("cls");

    allienLogo1txt();
    
    // Draw the initial menu without clearing the screen every time
    drawMenu();

    while (true) {
        char inputChar = getch();

        if (inputChar == 'w' || inputChar == 'W') {
            // Move up in the menu
            currentOption = (currentOption - 1 + numOption) % numOption;
            drawMenu(); // Redraw only the updated menu section
        }
        else if (inputChar == 's' || inputChar == 'S') {
            // Move down in the menu
            currentOption = (currentOption + 1) % numOption;
            drawMenu(); // Redraw only the updated menu section
        }
        else if (inputChar == '\r') { // Enter key to select option
            switch (currentOption) {
                case 0:
                    checkAccount();
                    return;
                case 1:
                    // Load game
                    checkAccount();
                    if (!isSaveFileExist(playerName)) {
                        setColor(4, 0);
                        sleepyPrint("\nBUT THERE IS NO SAVE DATA FILE");
                        setColor(7, 0);
                        cout << endl;
                        cout << "\nPress \033[31mESC\033[0m to return to the main menu...";
                        while (true) {
                            char inputChar = getch();
                            if (inputChar == 27) {
                                MainMenu();
                                return;
                            } else if (inputChar == 'R' || inputChar == 'r') {
                                checkAccount();
                                return;
                            }
                        }                   
                    }
                    loadGame(playerName);
                    calculateLevel();
                    system("cls");
                    setColor(2, 0);
                    sleepyPrint(" < GAME LOADED > ");
                    Sleep(2000);
                    x = width / 2;
                    y = height - 2;                  
                    gameLoop(); // Start the game with loaded state
                    return;
                case 2:
                    PVPGame();
                    MainMenu();
                    return;
                case 3:
                    createAnAccount();
                    MainMenu();
                    return;
                case 4:
                    guide();
                    MainMenu();
                    return;
                case 5:
                    displayRankedPlayers();
                    return;
                case 6:
                    clearData();
                    break;
                case 7:
                    // Exit the game
                    exit(0);
                    break;
            }
        }
    }
}

void allienLogo1txt() {
    ifstream file("DATA/Logo/allienLogo1.txt");
    string line;

    setColor(2, 0);

    if (file.is_open()) {
        while (getline(file, line)) {
            Sleep(150);
            cout << line << endl;
        }
        file.close();
    }
    Sleep(2000);
}


void mainShipLogotxt() {

    ifstream file("DATA/Logo/mainShipLogo.txt");
    string line;

    setColor(2, 0);

    if (file.is_open()) {
        while (getline(file, line)) {
            Sleep(150);
            cout << line << endl;
        }
        file.close();
    }

    Sleep(2000);
    setCursorPosition(0, 0);

}


void createAnAccount() {
    system("cls");

    mainShipLogotxt();

    setColor(5, 0);
    sleepyPrint("👤 Create a New Account 👤\n\n");

    setColor(7, 0);
    string inputUserName, inputPassword;


    cout << "Enter your username: ";
    setColor(11,0);
    cin >> inputUserName;
    setColor(7,0);

    // Check if the username is already taken
    for (int i = 0; i < currentUsers; i++) {
        if (userList[i].username == inputUserName) {
            setColor(14, 0);
            cout << "\nThis username is already taken! Please choose another one.\n";
            setColor(7, 0);
            Sleep(3000);
            createAnAccount();  // Return and let the user try again with a new username
            return;
        }
    }

    while (true) {
        cout << "\nEnter your password (at least 8 characters): ";
        setColor(4,0);
        cin >> inputPassword;
        setColor(7,0);

        // Check if the password is at least 8 characters long
        if (inputPassword.length() >= 8) {
            break;
        } else {
            setColor(14, 0);
            cout << "The password must be at least 8 characters long. Please try again.\n";
            setColor(7, 0);
            Sleep(3000);
            createAnAccount();  // Return and let the user try again with a new password
            return;
        }
    }

    // Save the new user account information
    userList[currentUsers].username = inputUserName;
    userList[currentUsers].password = inputPassword;

    currentUsers++;

    saveUsersData();

    setColor(14, 0);
    cout << "\nYour account has been successfully created!\n\n";
    setColor(7, 0);


    cout << "Press \033[31mESC\033[0m to return to the main menu...";
    while (true)
    {
        char inputChar2 = getch();
        if(inputChar2 == 27)
        {
            break;
        }
    }
}

void saveUsersData(){
    ofstream usersDataFile("DATA/UDF.txt" , ios::trunc);
    if (usersDataFile.is_open()){
        for(int i = 0; i < currentUsers; i++){
            usersDataFile << userList[i].username << " " <<  userList[i].password << "\n";
        }
    }
    usersDataFile.close();
}

void loadUsersData(){
    ifstream userDataFile("DATA/UDF.txt");
    if (userDataFile.is_open()){
        user intializeUsers;
        while (userDataFile >> intializeUsers.username && userDataFile >> intializeUsers.password){
            userList[currentUsers] = intializeUsers;
            currentUsers++;
        }
    }
    userDataFile.close();
}

void deleteData (){
    ofstream targetFile("DATA/UDF.txt" , ios::trunc);
    targetFile.close();
    removeFolder("DATA/PlayersData");
    createFolder("DATA/PlayersData");
}

void clearData(){

    system("cls");

    setColor(4, 0);
    cout << "CLEAR DATA SECTION :\n";
    setColor(14, 0);

    ifstream CDSfile("DATA/clearDataSection.txt");
    string CDSline;

    if (CDSfile.is_open()){
        while(getline(CDSfile , CDSline)){
            cout << CDSline << endl;
        }
        CDSfile.close();
    }
    cout << "\n" << endl;
    string inputPass;
    while (true){
        setColor(4, 0);
        sleepyPrint("ENTER ADMINISTRATOR PASSWORD : ");
        setColor(7, 0);
        cin >> inputPass;
        setColor(7, 0);
        if(inputPass == ADMINISTRATORPASSWORD){deleteData();break;}
        else{
            setColor(14, 0);
            cout << "\n\nINVALID PASSWORD - TRY AGAIN\n\n";
        }
    }
    system("cls");
    setColor(4, 0);
    Sleep(1000);
    sleepyPrint("DELETING");
    Sleep(1000);
    cout << ".";
    Sleep(1000);
    cout << ".";
    Sleep(1000);
    cout << ".";
    setColor(2, 0);
    cout << "\n\n";
    sleepyPrint("DONE!");
    cout << "\n\n";
    sleepyPrint("DATA HAS BEEN SUCCESSFULLY DELETED");
    cout << "\n\n";
    sleepyPrint("APPLICATION WILL BE CLOSED");
    Sleep(3000);
    exit(0);
}

void checkAccount() {
    system("cls");
    allienLogo2txt();

    setColor(6, 0);
    sleepyPrint("🔑 LOG IN TO YOUR ACCOUNT 🔑\n\n");
    setColor(7, 0);
    cout << "USERNAME : ";
    setColor(11, 0);
    string inputUsername;
    cin >> inputUsername;
    cout << "\n";
    setColor(7, 0);
    cout << "PASSWORD : ";
    setColor(11, 0);
    string inputPassword;
    cin >> inputPassword;
    cout << "\n";

    bool accountFound = false;

    if (currentUsers == 0) {
        setColor(4, 0);
        sleepyPrint("No accounts found! Please create an account first.\n\n");
        setColor(7, 0);
        cout << "\nPress \033[31mESC\033[0m to return to the main menu...";
        while (true) {
            char inputChar = getch();
            if (inputChar == 27) {
                MainMenu();
                return;
            }
        }
    }

    for (int i = 0; i < currentUsers; i++) {
        if (userList[i].username == inputUsername && userList[i].password == inputPassword) {
            playerName = userList[i].username;
            setColor(2, 0);
            sleepyPrint("✅ YOU HAVE SUCCESSFULLY LOGGED IN TO YOUR ACCOUNT!\n");
            Sleep(1000);
            setColor(7, 0);
            accountFound = true;
        }
    }

    if (!accountFound) {
        setColor(4, 0);
        sleepyPrint("❌ INVALID USERNAME OR PASSWORD!\n");
        setColor(7, 0);
        cout << "\nPress \033[31mESC\033[0m to return to the main menu...\n";
        cout << "Press \033[31mR\033[0m to try again...\n";
        while (true) {
            char inputChar = getch();
            if (inputChar == 27) {
                MainMenu();
                return;
            } else if (inputChar == 'R' || inputChar == 'r') {
                checkAccount();
                return;
            }
        }
    }
}

void allienLogo2txt() {

    ifstream file("DATA/Logo/allienLogo2.txt");
    string line;

    setColor(2, 0);

    if (file.is_open()) {
        while (getline(file, line)) {
            Sleep(150);
            cout << line << endl;
        }
        file.close();
    }

    Sleep(2000);
    setCursorPosition(0, 0);

}

void guide (){
    system("cls");
    setColor(7, 0);

    ifstream file1("DATA/GUIDE/GUIDE1.txt");
    string line1;

    if (file1.is_open()) {
        while (getline(file1, line1)) {
            cout << line1 << endl;
        }
        file1.close();
    }
    cout << "\nPress \033[31mEnter\033[0m to go to the next page...";
    
    while (true)
    {
        char inputChar3 = getch();
        if(inputChar3 == 13 || inputChar3 == 10)
        {
            break;
        }
    }

    system("cls");

    ifstream file2("DATA/GUIDE/GUIDE2.txt");
    string line2;

    if (file2.is_open()) {
        while (getline(file2, line2)) {
            cout << line2 << endl;
        }
        file2.close();
    }

    cout << "\nPress \033[31mEnter\033[0m to go to the next page...";
    
    while (true)
    {
        char inputChar3 = getch();
        if(inputChar3 == 13 || inputChar3 == 10)
        {
            break;
        }
    }


    system("cls");

    ifstream file3("DATA/GUIDE/GUIDE3.txt");
    string line3;

    if (file3.is_open()) {
        while (getline(file3, line3)) {
            cout << line3 << endl;
        }
        file3.close();
    }

    cout << "\nPress \033[31mESC\033[0m to return to the main menu...";
    
    while (true) {
        char inputChar3 = getch();
        if (inputChar3 == 27) {
            break;
        }
    }
}

void removeFolder(string folderPath) {
    try {
        if (std::filesystem::exists(folderPath)) {  // Checking that the folder exists
            std::filesystem::remove_all(folderPath);  // Delete the folder and all its contents
            cout << "DELETED " << folderPath << std::endl;
        } else {
            std::cout << "NOT FOUND " << folderPath << std::endl;
        }
    } catch (const std::filesystem::filesystem_error& e) {
        cout << "ERROR : " << e.what() << endl;
        Sleep(5000);
        exit(0);
    }
}

void createFolder(string folderPath) {
    try {
        if (!std::filesystem::exists(folderPath)) {  // Checking if the folder already exists
            std::filesystem::create_directory(folderPath);  // Create new folder
            cout << "DONE " << folderPath << endl;
        } else {
            cout << "ALREADY EXIST " << folderPath << endl;
        }
    } catch (const std::filesystem::filesystem_error& e) {
        cout << "ERROR : " << e.what() << endl;
        Sleep(5000);
        exit(0);
    }
}

bool isSaveFileExist(string username) {
    ifstream saveFile("DATA/PlayersData/" + username + "_save.txt");
    return saveFile.good();  // We check if the file exists or not
}

void displayRankedPlayers() {
    system("cls");

    mergeSort(currentPlayers, 0, currentPlayersCount - 1);

    setColor(14, 0);
    cout << "🏆 LEADERBOARD 🏆\n\n";
    setColor(7, 0);

    cout << "RANK\tPLAYER-ID\tLEVEL\tTIME TAKEN\tSCORE\n";
    cout << "---------------------------------------------------------\n";

    for (int i = 0; i < currentPlayersCount; i++) {
        if (!currentPlayers[i].username.empty()) {
            cout << " " << i + 1 << "\t";
            cout << " " << currentPlayers[i].username << "\t\t";
            cout << " " << currentPlayers[i].level << "\t";
            cout << " " << currentPlayers[i].timeTaken << "\t";
            cout << " " << currentPlayers[i].score << "\n";
        }
    }

    cout << "\n\nPress \033[31mESC\033[0m to return to the main menu...";
    while (true) {
        char inputChar = getch();
        if (inputChar == 27) {
            MainMenu();
            return;
        }
    } 
    
}

void loadCurrentPlayersName(){
    ifstream file("DATA/currentPlayersName.txt");
    string name;
    
    // Read each line of the file that contains the names of the players
    while (getline(file, name)) {
        if (currentPlayersCount < 100) {  // Checking that the capacity of the array is not full
            currentPlayersName[currentPlayersCount] = name;
            currentPlayersCount++;
        }
    }    
}

void loadAllPlayersData(){

    ifstream file1("DATA/currentPlayersName.txt");
    string name;

    int i = 0;

    while (getline(file1, name)) {
        ifstream file2("DATA/PlayersData/" + name + "_save.txt");
        string line;
        currentPlayers[i].username = name;
        while (getline(file2, line)) {
            if (line.find("Score:") != string::npos) {
                currentPlayers[i].score = stoi(line.substr(7));
            } else if (line.find("Level:") != string::npos) {
                currentPlayers[i].level = stoi(line.substr(7));
            } else if (line.find("Time:") != string::npos) {
                currentPlayers[i].timeTaken = stod(line.substr(6));
            }
        }
    i++; 
    }
}

// A function to merge two parts of an array
void merge(player arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    player* leftArr = new player[n1];
    player* rightArr = new player[n2];

    for (int i = 0; i < n1; i++) {
        leftArr[i] = arr[left + i];
    }
    for (int j = 0; j < n2; j++) {
        rightArr[j] = arr[mid + 1 + j];
    }

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (comparePlayers(leftArr[i], rightArr[j])) {
            arr[k++] = leftArr[i++];
        } else {
            arr[k++] = rightArr[j++];
        }
    }

    while (i < n1) {
        arr[k++] = leftArr[i++];
    }
    while (j < n2) {
        arr[k++] = rightArr[j++];
    }

    delete[] leftArr;
    delete[] rightArr;
}

// Merge Sort function for sorting
void mergeSort(player arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

bool comparePlayers(const player& p1, const player& p2) {
    // First priority: more level
    if (p1.level != p2.level) {
        return p1.level > p2.level;  // The highest level is the winner
    }
    // Second priority: less time
    if (p1.timeTaken != p2.timeTaken) {
        return p1.timeTaken < p2.timeTaken;  // Less time wins
    }
    // Third priority: more points
    return p1.score > p2.score;  // The highest score is the winner
}

bool playerExists(const string& playerName) {
    ifstream file("DATA/currentPlayersName.txt");
    string name;
    
    // Checking if the player's name exists in the file
    while (getline(file, name)) {
        if (name == playerName) {
            return true;
        }
    }
    
    return false;
}

void savePlayerName(const string& playerName) {
    // Check if the player already exists in the file
    if (playerExists(playerName)) {
        return;
    }

    // Open the file to write new information
    ofstream file("DATA/currentPlayersName.txt", ios::app); // With 'app' which means information is added to the end of the file

    // Write the name of the player in the file
    file << endl << playerName ;

    file.close();

}


bool loadCheckPlayerData(const string& playerName, player& player) {
    ifstream file("DATA/PlayersData/" + playerName + "_save.txt");
    if (!file) {
        return false;
    }

    string line;
    while (getline(file, line)) {
        if (line.find("Score:") != string::npos) {
            player.score = stoi(line.substr(7)); // Score get
        } else if (line.find("Level:") != string::npos) {
            player.level = stoi(line.substr(7)); // Level get
        } else if (line.find("Time:") != string::npos) {
            player.timeTaken = stod(line.substr(6)); // Time get
        }
    }

    file.close();
    return true; // If the information is read correctly
}


// A function to update player information if the new record was better
void updatePlayerRecord(const string& playerName) {
    player currentPlayerX;
    
    steady_clock::time_point now = steady_clock::now();
    duration<double> elapsedTime = duration_cast<duration<double>>(now - gameStartTime);

    // First, we read the player's information from the file
    if (loadCheckPlayerData(playerName, currentPlayerX)) {
        // Compare new records with previous records
        bool shouldUpdate = false;

        // Check if the new record is better
        if (currentLevel > currentPlayerX.level) {
            shouldUpdate = true;
        } else if (currentLevel == currentPlayerX.level) {
            if (elapsedTime.count() < currentPlayerX.timeTaken) {
                shouldUpdate = true;
            } else if (elapsedTime.count() == currentPlayerX.timeTaken && playerScore > currentPlayerX.score) {
                shouldUpdate = true;
            }
        }

        // If the new record is better, the new information is saved
        if (shouldUpdate) {
            currentPlayerX.score = playerScore;
            currentPlayerX.level = currentLevel;
            currentPlayerX.timeTaken = elapsedTime.count();

            // Save the updated information to the file
            saveGame(playerName);
        }
    } else {
        saveGame(playerName);
    }
}

// PVP ////////////////////////////////////////////////////////////////////////////////////

void PVPGame(){
    system("cls");
    initBoardPVP();
    gameLoopPVP();
    return ;
}

// Clears the console
void clearScreenPVP() {
    // Hide console cursor for smoother game experience
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE;  // Set cursor visibility to false
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    COORD cursorPosition;
    cursorPosition.X = 0;
    cursorPosition.Y = 0;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
}

// Draw the game board
void drawBoardPVP() {
    clearScreenPVP();

    for (int i = 0; i < PVPheight; i++) {
        for (int j = 0; j < PVPwidth; j++) {
            if (i == 0 && j == 0) {
                board[i][j] = "\033[35m╔\033[0m"; // Upper-left corner
            } else if (i == 0 && j == PVPwidth - 1) {
                board[i][j] = "\033[35m╗\033[0m"; // upper-right corner
            } else if (i == PVPheight - 1 && j == 0) {
                board[i][j] = "\033[35m╚\033[0m"; // Lower-left corner
            } else if (i == PVPheight - 1 && j == PVPwidth - 1) {
                board[i][j] = "\033[35m╝\033[0m"; // lower-right corner
            } else if (i == 0 || i == PVPheight - 1) {
                board[i][j] = "\033[35m═\033[0m"; // Top and bottom horizontal lines
            } else if (j == 0 || j == PVPwidth - 1) {
                board[i][j] = "\033[35m║\033[0m"; // Left and right vertical lines
            }
        }
    }

    for (int i = 0; i < PVPheight; ++i) {
        for (int j = 0; j < PVPwidth; ++j) {
            cout << board[i][j];
        }
        cout << endl;
    }
    cout << "\nPLAYER \033[31m1\033[0m LIVES: " << "\033[32m" << p1_lives << "\033[0m"
    << "\tPLAYER \033[36m2\033[0m LIVES: " << "\033[32m" << p2_lives << "\033[0m" << endl;
}

// Initialize the game board
void initBoardPVP() {
    for (int i = 0; i < PVPheight; ++i) {
        for (int j = 0; j < PVPwidth; ++j) {
            board[i][j] = EMPTY;
        }
    }
    // Place players
    board[p1_y][p1_x] = PLAYER1;
    board[p2_y][p2_x] = PLAYER2;

    // Initialize bullets
    for (int i = 0; i < 10; ++i) {
        bullets_p1[i][0] = -1;
        bullets_p1[i][1] = -1;
        bullets_p2[i][0] = -1;
        bullets_p2[i][1] = -1;
    }

    // Place barriers for both players
    placeBarriersPVP();
}

// Place barriers for both players
void placeBarriersPVP() {
    // Player 1 barriers (one row, not aligned with player 2 barriers)
    for (int i = 0; i < 5; ++i) {
        barriers_p1[i].x = 1 + i * PVPwidth / 5; // Spaced out barriers (shifted to the left)
        barriers_p1[i].y = 3;
        barriers_p1[i].health = 2; // Initial health
        barriers_p1[i].active = true;
        board[barriers_p1[i].y][barriers_p1[i].x] = BARRIER1; // Place initial barrier
    }

    // Player 2 barriers (one row, shifted to avoid alignment with player 1 barriers)
    for (int i = 0; i < 5; ++i) {
        barriers_p2[i].x = 6 + i * PVPwidth / 5; // Spaced out barriers (shifted to the right)
        barriers_p2[i].y = PVPheight - 4;
        barriers_p2[i].health = 2;
        barriers_p2[i].active = true;
        board[barriers_p2[i].y][barriers_p2[i].x] = BARRIER1; // Place initial barrier
    }
}

// Update the board
void updateBoardPVP() {
    
    for (int i = 0; i < PVPheight; ++i) {
        for (int j = 0; j < PVPwidth; ++j) {
            board[i][j] = EMPTY;
        }
    }
    // Update players
    board[p1_y][p1_x] = PLAYER1;
    board[p2_y][p2_x] = PLAYER2;

    // Update bullets for Player 1
    for (int i = 0; i < 10; ++i) {
        if (bullets_p1[i][1] >= 0 && bullets_p1[i][1] < PVPheight) {
            board[bullets_p1[i][1]][bullets_p1[i][0]] = BULLETP1;
        }
    }

    // Update bullets for Player 2
    for (int i = 0; i < 10; ++i) {
        if (bullets_p2[i][1] >= 0 && bullets_p2[i][1] < PVPheight) {
            board[bullets_p2[i][1]][bullets_p2[i][0]] = BULLETP2;
        }
    }

    // Update barriers
    for (int i = 0; i < 5; ++i) {
        if (barriers_p1[i].active) {
            if (barriers_p1[i].health == 1) {
                board[barriers_p1[i].y][barriers_p1[i].x] = BARRIER2; // Change shape after hit
            } else {
                board[barriers_p1[i].y][barriers_p1[i].x] = BARRIER1;
            }
        }
        if (barriers_p2[i].active) {
            if (barriers_p2[i].health == 1) {
                board[barriers_p2[i].y][barriers_p2[i].x] = BARRIER2; // Change shape after hit
            } else {
                board[barriers_p2[i].y][barriers_p2[i].x] = BARRIER1;
            }
        }
    }
}

// Handle player input
void handleInputPVP() {
    if (_kbhit()) {
        char key = _getch();
        // Player 1 controls (A, D, W, S)
        if (key == 'a' && p1_x > 0) p1_x--;
        if (key == 'd' && p1_x < PVPwidth - 1) p1_x++;
        if (key == 's') {  // Player 1 shooting with 's'
            if (active_bullets_p1 < 2) { // Limit to 2 active bullets
                for (int i = 0; i < 10; ++i) {
                    if (bullets_p1[i][1] == -1) {
                        bullets_p1[i][0] = p1_x;
                        bullets_p1[i][1] = p1_y + 1;
                        active_bullets_p1++;
                        break;
                    }
                }
            }
        }

        // Player 2 controls (Arrow keys)
        if (key == 75 && p2_x > 0) p2_x--;  // Left arrow
        if (key == 77 && p2_x < PVPwidth - 1) p2_x++;  // Right arrow
        if (key == 80) {  // Player 2 shooting with Down Arrow
            if (active_bullets_p2 < 2) { // Limit to 2 active bullets
                for (int i = 0; i < 10; ++i) {
                    if (bullets_p2[i][1] == -1) {
                        bullets_p2[i][0] = p2_x;
                        bullets_p2[i][1] = p2_y - 1;
                        active_bullets_p2++;
                        break;
                    }
                }
            }
        }
    }
}

// Update bullets
void updateBulletsPVP() {
    // Update Player 1 bullets
    for (int i = 0; i < 10; ++i) {
        if (bullets_p1[i][1] >= 0) {
            // Check collision with Player 2
            if (bullets_p1[i][1] == p2_y && bullets_p1[i][0] == p2_x) {
                p2_lives--;
                bullets_p1[i][1] = -1;
                active_bullets_p1--;
                continue;
            }

            // Check collision with barriers of Player 1 (Neutralizing the missle)
            for (int j = 0; j < 5; ++j) {
                if (barriers_p1[j].active && bullets_p1[i][0] == barriers_p1[j].x && bullets_p1[i][1] == barriers_p1[j].y) {
                    bullets_p1[i][1] = -1;
                    active_bullets_p1--;
                    break;
                }
            }

            // Check collision with barriers of Player 2 
            for (int j = 0; j < 5; ++j) {
                if (barriers_p2[j].active && bullets_p1[i][0] == barriers_p2[j].x && bullets_p1[i][1] == barriers_p2[j].y) {
                    barriers_p2[j].health--;
                    if (barriers_p2[j].health == 0) {
                        barriers_p2[j].active = false;
                    }
                    bullets_p1[i][1] = -1;
                    active_bullets_p1--;
                    break;
                }
            }

            // Move bullet if no collision
            if (bullets_p1[i][1] >= 0) {
                bullets_p1[i][1]++;
                if (bullets_p1[i][1] >= PVPheight) {
                    bullets_p1[i][1] = -1;
                    active_bullets_p1--;
                }
            }
        }
    }

    // Update Player 2 bullets
    for (int i = 0; i < 10; ++i) {
        if (bullets_p2[i][1] >= 0) {
            // Check collision with Player 1
            if (bullets_p2[i][1] == p1_y && bullets_p2[i][0] == p1_x) {
                p1_lives--;
                bullets_p2[i][1] = -1;
                active_bullets_p2--;
                continue;
            }

            // Check collision with barriers of Player 2 (Neutralizing the missle)
            for (int j = 0; j < 5; ++j) {
                if (barriers_p2[j].active && bullets_p2[i][0] == barriers_p2[j].x && bullets_p2[i][1] == barriers_p2[j].y) {
                    bullets_p2[i][1] = -1; // The missle is neutralized
                    active_bullets_p2--;
                    break;
                }
            }

            // Check collision with barriers of Player 1
            for (int j = 0; j < 5; ++j) {
                if (barriers_p1[j].active && bullets_p2[i][0] == barriers_p1[j].x && bullets_p2[i][1] == barriers_p1[j].y) {
                    barriers_p1[j].health--;
                    if (barriers_p1[j].health == 0) {
                        barriers_p1[j].active = false;
                    }
                    bullets_p2[i][1] = -1;
                    active_bullets_p2--;
                    break;
                }
            }

            // Move bullet if no collision
            if (bullets_p2[i][1] >= 0) {
                bullets_p2[i][1]--;
                if (bullets_p2[i][1] < 0) {
                    bullets_p2[i][1] = -1;
                    active_bullets_p2--;
                }
            }
        }
    }
}

// Main game loop
void gameLoopPVP() {
    while (p1_lives > 0 && p2_lives > 0) {
        handleInputPVP();
        updateBulletsPVP();
        updateBoardPVP();
        drawBoardPVP();
        Sleep(10); // Reduced sleep time to make the game smoother
    }
    clearScreenPVP();
    if (p1_lives > 0) {       
        setCursorPosition( PVPwidth + 10 ,PVPheight / 2 - 1);
        cout << "PLAYER \033[31m1\033[0m WINS!";
        setCursorPosition( PVPwidth + 10 ,PVPheight / 2 + 1);
        cout << "Press \033[31mESC\033[0m to return to the main menu...";
        while (true)
        {
            char inputChar2 = getch();
            if(inputChar2 == 27)
            {
                break;
            }
        }
        reSetupPVP();
        MainMenu();
        return;
    } else {       
        setCursorPosition( PVPwidth + 10 ,PVPheight / 2 - 1);
        cout << "PLAYER \033[36m2\033[0m WINS!";
        setCursorPosition( PVPwidth + 10 ,PVPheight / 2 + 1);
        cout << "Press \033[31mESC\033[0m to return to the main menu...";
        while (true)
        {
            char inputChar2 = getch();
            if(inputChar2 == 27)
            {
                break;
            }
        }
        reSetupPVP();
        return;
    }
}

void reSetupPVP(){
    p1_x = PVPwidth / 2, p1_y = 1;
    p2_x = PVPwidth / 2, p2_y = PVPheight - 2;
    p1_lives = 3;
    p2_lives = 3;
    bullets_p1[10][2]; // Max 10 bullets for Player 1
    bullets_p2[10][2]; // Max 10 bullets for Player 2
    bullet_count_p1 = 0;
    bullet_count_p2 = 0;
    active_bullets_p1 = 0; // Active bullets for Player 1
    active_bullets_p2 = 0; // Active bullets for Player 2
}