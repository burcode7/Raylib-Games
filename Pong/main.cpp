#include <iostream>
#include <raylib.h>
#include <algorithm>
#include <chrono>

using namespace std;
const int screen_width = 1280;
const int screen_height = 800;
Color Green = Color{38, 200, 154, 255};
Color Dark_Green = Color{20, 190, 133, 255};
Color Light_Green = Color{129, 224, 184, 255};
Color Yellow = Color{243, 213, 91, 255};
Color Menu_Color = Color{100, 255, 154, 255};
Color image_color = Color{176, 176, 176, 255};
Color BACKGROUNDCOLOR = Color{102, 102, 102, 255};

int highScoreEasy = 0;
int highScoreMedium = 0;
int highScoreHard = 0;
int finalScore = 0;

bool soundMuted;
bool musicMuted;

bool hardMode = false;
bool resumeGame = false;

bool gameHasEnded = false;
int playerLives = 3;

bool gamePaused = false;
Vector2 mousePoint = {0.0f, 0.0f};

bool JustScored = false;

int player_score = 0;
int cpu_score = 0;

float musicAudioX = 350;
float sfxAudioX = 710;
float audioButtonsY = 290;

int player_score2nd = 0;
int player_score2 = 0;

typedef enum GameScreen
{
    TITLE = 0,
    GAMEPLAYSINGLE,
    PAUSED,
    ENDING,
    SETTINGS,
    GAMEMODE,
    GAMEPLAYDOUBLE,
    DIFFICULTY,
    GAMEOVER
} GameScreen;

GameScreen currentScreen;

// Initializes the ball object
class Ball
{
    // Public variables for the ball
public:
    float x, y;
    int speed_x, speed_y;
    int radius;

    // Function to draw the ball
    void Draw()
    {

        DrawCircle(x, y, radius, WHITE);
    }

    // Function to update the ball
    void Update()
    {
        // Updates the balls position
        x += speed_x;
        y += speed_y;

        Sound ballFX = LoadSound("sfx/ball.wav");
        Sound playerPoint = LoadSound("sfx/playerpoint.wav");
        Sound enemyPoint = LoadSound("sfx/enemypoint.wav");

        // Checks if the ball Y is touching the top or bottom of the screen

        if (y + radius >= GetScreenHeight() || y - radius <= 0)
        {
            speed_y *= -1;
            PlaySound(ballFX);
        }

        if (x + radius >= GetScreenWidth()) // Cpu wins a point
        {
            cpu_score++;
            JustScored = true;
            PlaySound(enemyPoint);
            playerLives--;
            ResetBall();
        }
        if (x - radius <= 0) // Player point
        {
            PlaySound(playerPoint);
            player_score++;
            JustScored = true;
            ResetBall();
        }
        if (JustScored)
        {
            speed_x = 7;
            speed_y = 7;
            JustScored = false;
        }
    }

    // Sets the ball back to the center if someone gets a point
    void ResetBall()
    {

        x = GetScreenWidth() / 2;
        y = GetScreenHeight() / 2;

        int speed_choices[2] = {-1, 1};
        speed_x *= speed_choices[GetRandomValue(0, 1)];
        speed_y *= speed_choices[GetRandomValue(0, 1)];
    }
};

class Ball2
{
    // Public variables for the ball
public:
    float x, y;
    int speed_x, speed_y;
    int radius;

    // Function to draw the ball
    void Draw()
    {

        DrawCircle(x, y, radius, WHITE);
    }

    // Function to update the ball
    void Update()
    {
        // Updates the balls position
        x += speed_x;
        y += speed_y;

        Sound ballFX = LoadSound("sfx/ball.wav");
        Sound playerPoint = LoadSound("sfx/playerpoint.wav");
        Sound player2point = LoadSound("sfx/player2point.wav");

        // Checks if the ball Y is touching the top or bottom of the screen

        if (y + radius >= GetScreenHeight() || y - radius <= 0)
        {
            speed_y *= -1;
            PlaySound(ballFX);
        }

        if (x + radius >= GetScreenWidth()) // Player2 wins a point
        {
            player_score2nd++;
            JustScored = true;
            PlaySound(player2point);
            ResetBall();
        }
        if (x - radius <= 0) // Player point
        {
            PlaySound(playerPoint);
            player_score2++;
            JustScored = true;
            ResetBall();
        }
        if (JustScored)
        {
            speed_x = 7;
            speed_y = 7;
            JustScored = false;
        }
    }

    // Sets the ball back to the center if someone gets a point
    void ResetBall()
    {
        x = GetScreenWidth() / 2;
        y = GetScreenHeight() / 2;

        int speed_choices[2] = {-1, 1};
        speed_x *= speed_choices[GetRandomValue(0, 1)];
        speed_y *= speed_choices[GetRandomValue(0, 1)];
    }
};

class Paddle
{
    // Allows this code to be initialized and run whenever it is declared
protected:
    void LimitMovement()
    {
        // Checks for collisions with the top and bottom part of the screen
        if (y <= 0)
        {
            y = 0;
        }

        if (y + height >= GetScreenHeight())
        {
            y = GetScreenHeight() - height;
        }
    }

public:
    float x, y;
    float width, height;
    int speed;

    void Draw()
    {
        // Draws a rounded rectangle with these parameters: (rec, roundness, segment, color
        DrawRectangleRounded(Rectangle{x, y, width, height}, 0.8, 0, WHITE);
        // DrawRectangle(x, y, width, height, WHITE);
    }

    void Update()
    {

        // Checks if the player pushes a key down and moves the paddle
        if (IsKeyDown(KEY_UP))
        {
            y = y - speed;
        }
        if (IsKeyDown(KEY_DOWN))
        {
            y = y + speed;
        }

        LimitMovement();
    }
};

class Paddle2
{
    // Allows this code to be initialized and run whenever it is declared
protected:
    void LimitMovement()
    {
        // Checks for collisions with the top and bottom part of the screen
        if (y <= 0)
        {
            y = 0;
        }

        if (y + height >= GetScreenHeight())
        {
            y = GetScreenHeight() - height;
        }
    }

public:
    float x, y;
    float width, height;
    int speed;

    void Draw()
    {
        // Draws a rounded rectangle with these parameters: (rec, roundness, segment, color
        DrawRectangleRounded(Rectangle{x, y, width, height}, 0.8, 0, WHITE);
        // DrawRectangle(x, y, width, height, WHITE);
    }

    void Update()
    {

        // Checks if the player pushes a key down and moves the paddle
        if (IsKeyDown(KEY_W))
        {
            y = y - speed;
        }
        if (IsKeyDown(KEY_S))
        {
            y = y + speed;
        }

        LimitMovement();
    }
};

class CpuPaddle : public Paddle
{
private:
    int prev_predicted_ball_y = 0;   // Track previous predicted ball position
    const float delay_factor = 1.0f; // Adjust this value to control the delay

public:
    // Overwrites the update function and declares ball y that must be declared when updating
    void Update(int ball_x, int ball_y, int ball_speed_x, int ball_speed_y)
    {
        // Predict the future position of the ball considering direction of the ball's movement
        int predicted_ball_y = ball_y + (ball_speed_y * ((x - ball_x) / static_cast<float>(ball_speed_x)));

        // Adjust the predicted position based on the ball's vertical speed
        predicted_ball_y += ball_speed_y * std::abs((x - ball_x) / static_cast<float>(ball_speed_x));

        // Ensure the predicted ball position is within the screen bounds
        predicted_ball_y = std::max(predicted_ball_y, 0);
        predicted_ball_y = std::min(predicted_ball_y, 1280 - 120);

        // Introduce a small delay by considering the previous predicted position
        predicted_ball_y = (1.0f - delay_factor) * prev_predicted_ball_y + delay_factor * predicted_ball_y;

        // Move towards the predicted position
        if (y + height / 2 > predicted_ball_y)
        {
            y -= speed;
        }
        else if (y + height / 2 < predicted_ball_y)
        {
            y += speed;
        }

        prev_predicted_ball_y = predicted_ball_y; // Update previous predicted position

        LimitMovement();
    }
};

// Initializes a object (you can initialize more than one with different names)
Ball ball;
Ball2 ball2;
Paddle player;
CpuPaddle cpu;
Paddle2 player2;

int numHits = 0;
int numHits2 = 0;

int main()
{
    cout << "Starting the game" << endl;
    const int screen_width = 1280;
    const int screen_height = 800;

    // Initializes the window with the arguments (width, height, title)
    InitWindow(screen_width, screen_height, "Pong");
    InitAudioDevice();

    SetExitKey(KEY_NULL);
    // Sets the FPS the game wants to run at
    SetTargetFPS(60);

    // Loads settings screen

    // Initialize the play button
    Texture2D pause = LoadTexture("rects/playRect.png");
    float pauseHeight = (float)pause.height;
    Rectangle pauseRec = {380, 520, (float)pause.width, pauseHeight};
    Rectangle pauseBounds = {380, 520, (float)pause.width, (float)pause.height};
    Rectangle twoPlayerRect = {400, 670, (float)pause.width, pauseHeight};

    int pauseState = 0; // Button state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
    bool pauseAction = false;

    // Initialize the settings button
    Texture2D settings = LoadTexture("rects/settingsRect.png");
    float settingsHeight = (float)settings.height;
    Rectangle settingsRect = {0, 0, (float)settings.width, settingsHeight};
    Rectangle settingsBounds = {400, 670, (float)settings.width, (float)settings.height};

    int settingsState = 0; // Button state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
    bool settingsAction = false;

    // Initialize Quit button
    Texture2D quit = LoadTexture("rects/settingsRect.png");
    float quitHeight = (float)quit.height;
    Rectangle quitRect = {0, 0, (float)quit.width, quitHeight};
    Rectangle quitBounds = {650, 670, (float)quit.width, (float)quit.height};

    int quitState = 0; // Button state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
    bool quitAction = false;

    int rectangle_height = 120;

    ball.radius = 20;
    ball.x = screen_width / 2;
    ball.y = screen_height / 2;
    ball.speed_x = 7;
    ball.speed_y = 7;

    ball2.radius = 20;
    ball2.x = screen_width / 2;
    ball2.y = screen_height / 2;
    ball2.speed_x = 7;
    ball2.speed_y = 7;

    player.width = 25;
    player.height = rectangle_height;
    player.x = screen_width - player.width - 10;
    player.y = screen_height / 2 - player.height / 2;
    player.speed = 6;

    cpu.height = rectangle_height;
    cpu.width = 25;
    cpu.x = 10;
    cpu.y = screen_height / 2 - cpu.height / 2;
    cpu.speed;

    player2.height = rectangle_height;
    player2.width = 25;
    player2.x = 10;
    player2.y = screen_height / 2 - cpu.height / 2;
    player2.speed = 6;

    Texture exitBack = LoadTexture("assets/exit.png");
    Rectangle exitRectangle = {0, 0, float(exitBack.width), float(exitBack.height)};
    Rectangle exitHitbox = {0, 0, float(exitBack.width / 6), float(exitBack.height / 6)};
    Vector2 exitCenter = {0, 0};
    int exitState = 0; // Button state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
    bool exitAction = false;

    /*
        WindowShouldClose() checks if the close button was clicked.
        This loop runs as long as it has not been touched.
    */
    // initializes sound and music images

    Texture musicAudio = LoadTexture("assets/music.png");
    Texture musicAudioMuted = LoadTexture("assets/musicMuted.png");

    float musicHeight = (float)musicAudio.height;
    Rectangle musicRect = {musicAudioX, audioButtonsY, (float)musicAudio.width, musicHeight};
    Rectangle musicBounds = {musicAudioX, audioButtonsY, (float)musicAudio.width, (float)musicAudio.height};

    int musicState = 0; // Button state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
    bool musicAction = false;

    Texture sfxAudio = LoadTexture("assets/full.png");
    Texture sfxAudioMuted = LoadTexture("assets/mute.png");

    float sfxHeight = (float)sfxAudio.height;
    Rectangle sfxRect = {sfxAudioX, audioButtonsY, (float)sfxAudio.width, musicHeight};
    Rectangle sfxBounds = {sfxAudioX, audioButtonsY, (float)sfxAudio.width, (float)sfxAudio.height};

    int sfxState = 0; // Button state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
    bool sfxAction = false;

    // initialize images

    Texture settingsScreen = LoadTexture("assets/settingsScreen.png");
    Texture continueGame = LoadTexture("assets/continue.png");
    Texture player1victory = LoadTexture("assets/player1victory.png");
    Texture player2victory = LoadTexture("assets/player2victory.png");
    Texture backgroundMedium = LoadTexture("assets/backgroundMedium.png");
    Texture backgroundHard = LoadTexture("assets/backgroundHard.png");
    Texture background = LoadTexture("assets/Gameplay.png");
    Texture titleScreen = LoadTexture("assets/titleScreen.png");
    Texture pauseImage = LoadTexture("assets/pause.png");
    Texture playImage = LoadTexture("buttons/play.png");
    Texture playGlow = LoadTexture("buttons/playGlow.png");
    Texture settingsImage = LoadTexture("buttons/settings.png");
    Texture settingsGlow = LoadTexture("buttons/settingsGlow.png");
    Texture quitImage = LoadTexture("buttons/quit.png");
    Texture quitGlow = LoadTexture("buttons/quitGlow.png");
    Texture singleplayerImage = LoadTexture("buttons/singleplayer.png");
    Texture singleplayerImageGlow = LoadTexture("buttons/singleplayerGlow.png");
    Texture twoPlayerImage = LoadTexture("buttons/twoPlayer.png");
    Texture twoPlayerImageGlow = LoadTexture("buttons/twoPlayerGlow.png");
    Texture gameOver = LoadTexture("assets/gameOver.png");

    Texture playAgainImage = LoadTexture("buttons/playAgain.png");
    Texture playAgainGlow = LoadTexture("buttons/playAgainGlow.png");

    int playAgainX = 373;
    int playAgainY = 536;

    float againHeight = (float)playAgainImage.height;
    Rectangle againRect = {0, 0, (float)playAgainImage.width, againHeight};
    Rectangle againBounds = {playAgainX, playAgainY, (float)playAgainImage.width, (float)playAgainImage.height};

    int againState = 0;
    bool againAction = false;

    // MAIN MENU RECT AND PNG
    Texture mainMenuImage = LoadTexture("buttons/mainMenu.png");
    Texture mainMenuImageGlow = LoadTexture("buttons/mainMenuGlow.png");

    int mainMenuX = 373;
    int mainMenuY = 650;

    float mainHeight = (float)mainMenuImage.height;
    Rectangle mainMenuRect = {0, 0, (float)mainMenuImage.width, mainHeight};
    Rectangle mainMenuBounds = {mainMenuX, mainMenuY, (float)mainMenuImage.width, (float)mainMenuImage.height};

    int mainState = 0; // Button state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
    bool mainAction = false;

    // EASY IMAGE RECT AND PNG

    Texture easyImage = LoadTexture("buttons/easy.png");
    Texture easyGlow = LoadTexture("buttons/easyGlow.png");

    int easyImageX = 111;
    int easyImageY = 460;

    float easyHeight = (float)easyImage.height;
    Rectangle easyRect = {0, 0, (float)easyImage.width, easyHeight};
    Rectangle easyBounds = {easyImageX, easyImageY, (float)easyImage.width, (float)easyImage.height};

    int easyState = 0; // Button state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
    bool easyAction = false;

    // MEDIUM IMAGE RECT AND PNG
    Texture mediumRed = LoadTexture("buttons/mediumRed.png");
    Texture mediumImage = LoadTexture("buttons/medium.png");
    Texture mediumGlow = LoadTexture("buttons/mediumGlow.png");

    int mediumImageX = 639;
    int mediumImageY = 460;

    float mediumHeight = (float)mediumImage.height;
    Rectangle mediumRect = {0, 0, (float)mediumImage.width, mediumHeight};
    Rectangle mediumBounds = {mediumImageX, mediumImageY, (float)mediumImage.width, (float)mediumImage.height};

    int mediumState = 0; // Button state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
    bool mediumAction = false;

    // HARD IMAGE RECT AND PNG

    Texture hardRed = LoadTexture("buttons/hardRed.png");
    Texture hardImage = LoadTexture("buttons/hard.png");
    Texture hardGlow = LoadTexture("buttons/hardGlow.png");

    int hardImageX = 384;
    int hardImageY = 590;

    float hardHeight = (float)hardImage.height;
    Rectangle hardRect = {0, 0, (float)hardImage.width, hardHeight};
    Rectangle hardBounds = {hardImageX, hardImageY, (float)hardImage.width, (float)hardImage.height};

    int hardState = 0; // Button state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
    bool hardAction = false;

    // CONTINUE BUTTON RECT AND PNG
    Texture continueImage = LoadTexture("buttons/continueButton.png");
    Texture continueImageGlow = LoadTexture("buttons/continueButtonGlow.png");

    int continueImageX = 373;
    int continueImageY = 530;

    float continueHeight = (float)continueImage.height;
    Rectangle continueImageRect = {0, 0, (float)continueImage.width, continueHeight};
    Rectangle continueImageBounds = {continueImageX, continueImageY, (float)continueImage.width, (float)continueImage.height};

    int continueState = 0; // Button state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
    bool continueAction = false;

    // Initialize sound effects

    Sound buttonSFX = LoadSound("sfx/button.wav");
    Sound ballFX = LoadSound("sfx/ball.wav");
    Sound player2point = LoadSound("sfx/player2point.wav");
    Sound cantSelect = LoadSound("sfx/cantSelect.wav");
    Sound youLose = LoadSound("sfx/gameOver.wav");
    Sound winSFX = LoadSound("sfx/winSFX.mp3");

    // Initialize music

    Music menuMusic = LoadMusicStream("music/menuMusic.mp3");
    Music gameplayMusic = LoadMusicStream("music/fastMenuMusic.mp3");
    Music bossMusic = LoadMusicStream("music/gameplayMusic.mp3");
    Music gameOverMusic = LoadMusicStream("music/gameOverSong.mp3");
    Music victorySong = LoadMusicStream("music/victoryMusic.mp3");

    int playImageX = 380;
    int playImageY = 520;
    // Intiialize pause button

    int pauseButtonState = 0;
    bool pauseButtonAction = false;

    float PauseButtonX = screen_width / 2 - pauseImage.width / 2;
    float PauseButtonY = screen_height / 2 - pauseImage.height / 1.9;

    currentScreen = TITLE;

    Rectangle pauseButtonHitbox = {PauseButtonX, PauseButtonY, pauseImage.width, pauseImage.height};

    int settingsImageX = 380;
    int settingsImageY = 646;

    while (!WindowShouldClose())
    {
        // Creates a blank canvas we can draw the game on
        BeginDrawing();

        mousePoint = GetMousePosition();
        pauseAction = false;

        switch (currentScreen)
        {
        /*
        ******************************************
           TITLE  SCREEN
        ******************************************
        */
        case TITLE:

            ClearBackground(RAYWHITE);
            DrawTexture(titleScreen, 0, 0, WHITE);
            if (!musicMuted)
            {
                PlayMusicStream(menuMusic);
                UpdateMusicStream(menuMusic);
            }

            // Check play button state
            if (CheckCollisionPointRec(mousePoint, pauseBounds))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                {
                    pauseState = 2;
                    DrawTexture(playImage, playImageX, playImageY, WHITE);
                }
                else
                {
                    pauseState = 1;
                    DrawTexture(playGlow, playImageX, playImageY, WHITE);
                }
                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                {
                    pauseAction = true;
                }
            }
            else
            {
                pauseState = 0;
                DrawTexture(playImage, playImageX, playImageY, WHITE);
            }
            if (pauseAction)
            {
                if (!soundMuted)
                {
                    PlaySound(buttonSFX);
                }
                currentScreen = GAMEMODE;

                pauseAction = false;
            }

            // Check settings button state
            if (CheckCollisionPointRec(mousePoint, settingsBounds))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                {
                    settingsState = 2;
                    DrawTexture(settingsImage, settingsImageX, settingsImageY, WHITE);
                }
                else
                {
                    settingsState = 1;
                    DrawTexture(settingsGlow, settingsImageX, settingsImageY, WHITE);
                }

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                {
                    settingsAction = true;
                }
            }
            else
            {
                settingsState = 0;
                DrawTexture(settingsImage, settingsImageX, settingsImageY, WHITE);
            }

            if (settingsAction)
            {
                if (!soundMuted)
                {
                    PlaySound(buttonSFX);
                }
                currentScreen = SETTINGS;
                settingsAction = false;
            }

            // Check quit button state
            if (CheckCollisionPointRec(mousePoint, quitBounds))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                {
                    quitState = 2;
                    DrawTexture(quitImage, settingsImageX + 257, settingsImageY, WHITE);
                }

                else
                {
                    quitState = 1;
                    DrawTexture(quitGlow, settingsImageX + 257, settingsImageY, WHITE);
                }

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                {
                    quitAction = true;
                }
            }
            else
            {
                quitState = 0;
                DrawTexture(quitImage, settingsImageX + 257, settingsImageY, WHITE);
            }
            if (quitAction)
            {
                if (!soundMuted)
                {
                    PlaySound(buttonSFX);
                }
                WindowShouldClose();
                exit(0);
            }

            break;

        /*
        ******************************************
           GAME SELECT SCREEN
        ******************************************
        */
        case GAMEMODE:
            ClearBackground(WHITE);
            DrawTexture(titleScreen, 0, 0, WHITE);
            if (!musicMuted)
            {
                UpdateMusicStream(menuMusic);
            }
            if (CheckCollisionPointRec(mousePoint, pauseBounds))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                {
                    pauseState = 2;
                    DrawTexture(singleplayerImage, playImageX, playImageY, WHITE);
                }
                else
                {
                    pauseState = 1;
                    DrawTexture(singleplayerImageGlow, playImageX, playImageY, WHITE);
                }
                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                {
                    pauseAction = true;
                }
            }
            else
            {
                pauseState = 0;
                DrawTexture(singleplayerImage, playImageX, playImageY, WHITE);
            }
            if (pauseAction)
            {
                if (!soundMuted)
                {
                    PlaySound(buttonSFX);
                }
                currentScreen = DIFFICULTY;
                pauseAction = false;
            }
            DrawTexturePro(exitBack,
                           exitRectangle,
                           (Rectangle){0, 0, exitRectangle.width / 6, exitRectangle.height / 6},
                           exitCenter,
                           0,
                           WHITE);

            if (CheckCollisionPointRec(mousePoint, exitHitbox))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                {
                    exitState = 2;
                }
                else
                {
                    exitState = 1;
                }
                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                {
                    if (!soundMuted)
                    {
                        PlaySound(buttonSFX);
                    }
                    gamePaused = false;
                    currentScreen = TITLE;
                }
            }
            if (CheckCollisionPointRec(mousePoint, twoPlayerRect))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                {
                    settingsState = 2;
                    DrawTexture(twoPlayerImage, settingsImageX, settingsImageY, WHITE);
                }
                else
                {
                    settingsState = 1;
                    DrawTexture(twoPlayerImageGlow, settingsImageX, settingsImageY, WHITE);
                }

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                {
                    settingsAction = true;
                    currentScreen = GAMEPLAYDOUBLE;
                    if (!soundMuted)
                    {
                        PlaySound(buttonSFX);
                    }
                }
            }
            else
            {
                settingsState = 0;
                DrawTexture(twoPlayerImage, settingsImageX, settingsImageY, WHITE);
            }

            if (settingsAction)
            {
                if (!soundMuted)
                {
                    PlaySound(buttonSFX);
                }
                currentScreen = GAMEPLAYDOUBLE;
                settingsAction = false;
            }

            if (IsKeyPressed(KEY_ESCAPE))
            {
                if (!soundMuted)
                {
                    PlaySound(buttonSFX);
                }
                currentScreen = TITLE;
            }

            break;
        /*
        *******************************************
         SELECT DIFFICULT
         *********************************************
        */
        case DIFFICULTY:
            ClearBackground(WHITE);
            DrawTexture(titleScreen, 0, 0, WHITE);
            if (!musicMuted)
            {
                UpdateMusicStream(menuMusic);
            }

            // Check easy button state
            if (CheckCollisionPointRec(mousePoint, easyBounds))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                {
                    easyState = 2;
                    DrawTexture(easyImage, easyImageX, easyImageY, WHITE);
                }
                else
                {
                    easyState = 1;
                    DrawTexture(easyGlow, easyImageX, easyImageY, WHITE);
                }
                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                {
                    easyAction = true;
                }
            }
            else
            {
                easyState = 0;
                DrawTexture(easyImage, easyImageX, easyImageY, WHITE);
            }
            if (easyAction)
            {
                if (!soundMuted)
                {
                    PlaySound(buttonSFX);
                }
                currentScreen = GAMEPLAYSINGLE;
                cpu_score = 0;
                player_score = 0;
                playerLives = 9;
                cpu.speed = 3;
                easyAction = false;
                hardMode = false;
            }

            // Check medium button state
            if (CheckCollisionPointRec(mousePoint, mediumBounds))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                {
                    mediumState = 2;
                    DrawTexture(mediumImage, mediumImageX, mediumImageY, WHITE);
                }
                else if (highScoreEasy >= 5)
                {
                    mediumState = 1;
                    DrawTexture(mediumGlow, mediumImageX, mediumImageY, WHITE);
                }
                else
                {
                    mediumState = 1;
                    DrawTexture(mediumRed, mediumImageX, mediumImageY, WHITE);
                }
                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                {

                    mediumAction = true;
                }
            }
            else
            {
                mediumState = 0;

                DrawTexture(mediumImage, mediumImageX, mediumImageY, WHITE);
            }
            if (mediumAction && highScoreEasy >= 5)
            {
                if (!soundMuted)
                {
                    PlaySound(buttonSFX);
                }
                currentScreen = GAMEPLAYSINGLE;
                cpu_score = 0;
                player_score = 0;
                playerLives = 5;
                cpu.speed = 4;
                mediumAction = false;
                hardMode = false;
            }
            if (mediumAction && !(highScoreEasy >= 5))
            {
                if (!soundMuted)
                {
                    PlaySound(cantSelect);
                }
                mediumAction = false;
            }

            // check hard button state
            if (CheckCollisionPointRec(mousePoint, hardBounds))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                {
                    hardState = 2;
                    DrawTexture(hardImage, hardImageX, hardImageY, WHITE);
                }
                else if (highScoreMedium >= 3)
                {
                    hardState = 1;
                    DrawTexture(hardGlow, hardImageX, hardImageY, WHITE);
                }
                else
                {
                    hardState = 1;
                    DrawTexture(hardRed, hardImageX, hardImageY, WHITE);
                }
                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                {
                    hardAction = true;
                }
            }
            else
            {
                hardState = 0;
                DrawTexture(hardImage, hardImageX, hardImageY, WHITE);
            }
            if (hardAction && highScoreMedium >= 5)
            {
                if (!soundMuted)
                {
                    PlaySound(buttonSFX);
                }
                currentScreen = GAMEPLAYSINGLE;
                cpu_score = 0;
                player_score = 0;
                cpu.speed = 4;
                playerLives = 1;
                hardAction = false;
                hardMode = true;
            }
            if (hardAction && !(highScoreMedium >= 5))
            {
                if (!soundMuted)
                {
                    PlaySound(cantSelect);
                }
                hardAction = false;
            }

            if (IsKeyPressed(KEY_ESCAPE))
            {
                if (!soundMuted)
                {
                    PlaySound(buttonSFX);
                }
                currentScreen = TITLE;
            }
            DrawTexturePro(exitBack,
                           exitRectangle,
                           (Rectangle){0, 0, exitRectangle.width / 6, exitRectangle.height / 6},
                           exitCenter,
                           0,
                           WHITE);

            if (CheckCollisionPointRec(mousePoint, exitHitbox))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                {
                    exitState = 2;
                }
                else
                {
                    exitState = 1;
                }
                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                {
                    if (!soundMuted)
                    {
                        PlaySound(buttonSFX);
                    }
                    gamePaused = false;
                    currentScreen = GAMEMODE;
                }
            }

            if (player_score >= highScoreEasy && cpu.speed == 3)
            {
                highScoreEasy = player_score;
            }
            if (player_score >= highScoreMedium && cpu.speed == 4 && !hardMode)
            {
                highScoreMedium = player_score;
            }
            if (player_score >= highScoreHard && cpu.speed == 4 && hardMode)
            {
                highScoreHard = player_score;
            }
            break;
            /*
        ******************************************
           SINGLE PLAYER
        ******************************************
        */
        case GAMEPLAYSINGLE:
            pauseAction = false;
            if (cpu.speed == 3)
            {
                if (!musicMuted)
                {
                    UpdateMusicStream(menuMusic);
                }
                if (resumeGame && !musicMuted)
                {
                    PlayMusicStream(menuMusic);
                }
                finalScore = player_score;

                if (finalScore >= highScoreEasy)
                {
                    highScoreEasy = finalScore;
                }
            }
            else if (cpu.speed == 4 && !hardMode)
            {
                if (!musicMuted)
                {
                    StopMusicStream(menuMusic);
                    PlayMusicStream(gameplayMusic);
                    UpdateMusicStream(gameplayMusic);
                }
                if (resumeGame && !musicMuted)
                {
                    PlayMusicStream(gameplayMusic);
                }
                finalScore = player_score;

                if (finalScore >= highScoreMedium)
                {
                    highScoreMedium = finalScore;
                }
            }
            else if (cpu.speed == 4 && hardMode)
            {
                if (!musicMuted)
                {
                    StopMusicStream(menuMusic);
                    PlayMusicStream(bossMusic);
                    UpdateMusicStream(bossMusic);
                }
                if (resumeGame && !musicMuted)
                {
                    PlayMusicStream(bossMusic);
                }
                finalScore = player_score;

                if (finalScore >= highScoreHard)
                {
                    highScoreHard = finalScore;
                }
            }

            if (IsKeyPressed(KEY_ESCAPE))
            {
                if (!soundMuted)
                {
                    PlaySound(buttonSFX);
                }
                gamePaused = !gamePaused;
            }

            if (!gamePaused)
            {
                ball.Update();
                player.Update();
                // Fills the parameter ball_y with ball.y to give it a value to follow
                cpu.Update(ball.x, ball.y, ball.speed_x, ball.speed_y);

                // Checking for collisions with both paddles
                if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player.x, player.y, player.width, player.height}))
                {
                    // Move the ball outside the paddle's bounds
                    if (ball.x < player.x)
                    {
                        ball.x = player.x - ball.radius - 1; // Move left of the paddle
                    }
                    else
                    {
                        ball.x = player.x + player.width + ball.radius + 1; // Move right of the paddle
                    }

                    // Reverse ball's x direction
                    ball.speed_x *= -1;

                    // Increment hit count and adjust speed if necessary
                    numHits++;
                    if (numHits >= 3)
                    {
                        ball.speed_x *= 1.15;
                        ball.speed_y *= 1.15;
                        numHits = 0;
                    }
                    if (JustScored)
                    {
                        ball.speed_x = 7;
                        ball.speed_y = 7;
                        JustScored = false;
                    }
                    if (!soundMuted)
                    {
                        PlaySound(ballFX);
                    }
                }

                // cpu collision
                if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{cpu.x, cpu.y, cpu.width, cpu.height}))
                {
                    // Move the ball outside the paddle's bounds
                    if (ball.x < cpu.x)
                    {
                        ball.x = cpu.x + ball.radius + 1; // Move left of the paddle
                    }
                    else
                    {
                        ball.x = cpu.x + cpu.width + ball.radius + 1; // Move right of the paddle
                    }

                    // Reverse ball's x direction
                    ball.speed_x *= -1;

                    // Increment hit count and adjust speed if necessary
                    numHits++;
                    if (numHits >= 3)
                    {
                        ball.speed_x *= 1.15;
                        ball.speed_y *= 1.15;
                        numHits = 0;
                    }
                    if (JustScored)
                    {
                        ball.speed_x = 7;
                        ball.speed_y = 7;
                        JustScored = false;
                    }
                    if (!soundMuted)
                    {
                        PlaySound(ballFX);
                    }
                }
            }

            // Clears the gameplay screen
            ClearBackground(RAYWHITE);
            DrawTexture(background, 0, 0, WHITE);

            // Draws a text with the parameters: (text, xpos, ypos, fontsize, color)
            // TextFormat("%i" converts something to text)
            DrawText(TextFormat("%i", cpu_score), screen_width / 4, 20, 80, WHITE);
            DrawText(TextFormat("%i", player_score), 3 * screen_width / 4, 20, 80, WHITE);

            DrawText(TextFormat("%i", playerLives), 618, 360, 80, image_color);

            ball.Draw();

            player.Draw();

            cpu.Draw();

            if (gamePaused)
            {
                DrawTexture(pauseImage, PauseButtonX, PauseButtonY, WHITE);
                pauseButtonAction = false;

                if (CheckCollisionPointRec(mousePoint, pauseButtonHitbox))
                {
                    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    {
                        pauseButtonState = 2;
                    }
                    else
                    {
                        pauseButtonState = 1;
                    }
                    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    {
                        if (!soundMuted)
                        {
                            PlaySound(buttonSFX);
                        }
                        gamePaused = false;
                    }
                }

                DrawTexturePro(exitBack,
                               exitRectangle,
                               (Rectangle){0, 0, exitRectangle.width / 6, exitRectangle.height / 6},
                               exitCenter,
                               0,
                               WHITE);

                if (CheckCollisionPointRec(mousePoint, exitHitbox))
                {
                    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    {
                        exitState = 2;
                    }
                    else
                    {
                        exitState = 1;
                    }
                    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    {
                        exitAction = true;
                    }
                }
                if (exitAction)
                {
                    player_score = 0;
                    gamePaused = false;
                    StopMusicStream(gameplayMusic);
                    currentScreen = TITLE;
                    if (!soundMuted)
                    {
                        PlaySound(buttonSFX);
                    }
                    hardMode = false;
                    exitAction = false;
                    resumeGame = false;
                }
            }
            if (cpu.speed == 4 && !hardMode)
            {
                DrawTexture(backgroundMedium, 0, 0, WHITE);
            }
            if (cpu.speed == 4 && hardMode)
            {
                DrawTexture(backgroundHard, 0, 0, WHITE);
            }

            // if (IsKeyPressed(KEY_SPACE))
            // {
            //     player_score++;
            // }

            if (playerLives <= 0 && !hardMode)
            {
                gameHasEnded = true;
                if (!soundMuted)
                {
                    PlaySound(youLose);
                }
                currentScreen = GAMEOVER;
            }
            else if (playerLives <= 0 && hardMode)
            {
                gameHasEnded = true;
                if (!soundMuted)
                {
                    PlaySound(youLose);
                }
                currentScreen = GAMEOVER;
            }
            if (player_score == 5 && !resumeGame)
            {
                if (!soundMuted)
                {
                    PlaySound(winSFX);
                }
                currentScreen = ENDING;
            }
            break;
        /*
        ********************************
         TWO PLAYER
        ********************************
        */
        case GAMEPLAYDOUBLE:
            pauseAction = false;
            if (!musicMuted)
            {
                UpdateMusicStream(menuMusic);
            }
            if (IsKeyPressed(KEY_ESCAPE))
            {
                if (!soundMuted)
                {
                    PlaySound(buttonSFX);
                }
                gamePaused = !gamePaused;
            }

            if (!gamePaused)
            {
                ball2.Update();
                player.Update();
                // Fills the parameter ball_y with ball.y to give it a value to follow
                player2.Update();

                // Checking for collisions with both paddles
                if (CheckCollisionCircleRec(Vector2{ball2.x, ball2.y}, ball.radius, Rectangle{player.x, player.y, player.width, player.height}))
                {
                    if (ball2.x < player.x)
                    {
                        ball2.x = player.x - ball2.radius - 1; // Move left of the paddle
                    }
                    else
                    {
                        ball2.x = player.x + player.width + ball2.radius + 1; // Move right of the paddle
                    }
                    numHits2++;
                    ball2.speed_x *= -1;

                    if (numHits2 >= 3)
                    {
                        ball2.speed_x *= 1.15;
                        ball2.speed_y *= 1.15;
                        numHits2 = 0;
                    }
                    if (JustScored)
                    {
                        ball2.speed_x = 7;
                        ball2.speed_y = 7;
                        JustScored = false;
                    }

                    ball2.x += ball2.speed_x * 0.1; // Stops the ball from getting inside the paddle
                    if (!soundMuted)
                    {
                        PlaySound(ballFX);
                    }
                }
                if (CheckCollisionCircleRec(Vector2{ball2.x, ball2.y}, ball.radius, Rectangle{player2.x, player2.y, player2.width, player2.height}))
                {
                    if (ball2.x < player2.x)
                    {
                        ball2.x = player2.x + ball2.radius + 1; // Move left of the paddle
                    }
                    else
                    {
                        ball2.x = player2.x + player2.width + ball2.radius + 1; // Move right of the paddle
                    }
                    numHits2++;
                    ball2.speed_x *= -1;

                    if (numHits2 >= 3)
                    {
                        ball2.speed_x *= 1.15;
                        ball2.speed_y *= 1.15;
                        numHits = 0;
                    }

                    if (JustScored)
                    {
                        ball2.speed_x = 7;
                        ball2.speed_y = 7;
                        JustScored = false;
                    }

                    ball2.x += ball2.speed_x * 0.1; // Stops the ball from getting inside the paddle
                    if (!soundMuted)
                    {
                        PlaySound(ballFX);
                    }
                }
            }
            // Clears the gameplay screen
            ClearBackground(RAYWHITE);
            DrawTexture(background, 0, 0, WHITE);

            // Draws a text with the parameters: (text, xpos, ypos, fontsize, color)
            // TextFormat("%i" converts something to text)
            DrawText(TextFormat("%i", player_score2nd), screen_width / 4, 20, 80, WHITE);
            DrawText(TextFormat("%i", player_score2), 3 * screen_width / 4, 20, 80, WHITE);

            ball2.Draw();

            player.Draw();

            player2.Draw();

            if (gamePaused)
            {
                DrawTexture(pauseImage, PauseButtonX, PauseButtonY, WHITE);
                pauseButtonAction = false;

                if (CheckCollisionPointRec(mousePoint, pauseButtonHitbox))
                {
                    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    {
                        pauseButtonState = 2;
                    }
                    else
                    {
                        pauseButtonState = 1;
                    }
                    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    {
                        if (!soundMuted)
                        {
                            PlaySound(buttonSFX);
                        }
                        gamePaused = false;
                    }
                }

                DrawTexturePro(exitBack,
                               exitRectangle,
                               (Rectangle){0, 0, exitRectangle.width / 6, exitRectangle.height / 6},
                               exitCenter,
                               0,
                               WHITE);

                if (CheckCollisionPointRec(mousePoint, exitHitbox))
                {
                    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    {
                        exitState = 2;
                    }
                    else
                    {
                        exitState = 1;
                    }
                    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    {
                        if (!soundMuted)
                        {
                            PlaySound(buttonSFX);
                        }
                        gamePaused = false;
                        if (!musicMuted)
                        {
                            StopMusicStream(gameplayMusic);
                        }
                        currentScreen = TITLE;
                    }
                }
            }

            if (player_score2 == 7 || player_score2nd == 7)
            {
                currentScreen = ENDING;
            }
            break;
            /*
            *****************
            SETTINGS
            ********************
            */
        case SETTINGS:
            ClearBackground(WHITE);
            DrawTexture(settingsScreen, 0, 0, WHITE);
            if (!musicMuted)
            {
                UpdateMusicStream(menuMusic);
            }

            //
            //
            // MUSIC SOUND
            //
            //
            if (CheckCollisionPointRec(mousePoint, musicRect))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                {
                    musicState = 2;
                    if (!musicMuted)
                    {
                        DrawTexture(musicAudio, musicAudioX, audioButtonsY, WHITE);
                    }
                    else
                    {
                        DrawTexture(musicAudioMuted, musicAudioX, audioButtonsY, WHITE);
                    }
                }
                else
                {
                    musicState = 1;
                    if (!musicMuted)
                    {
                        DrawTexture(musicAudio, musicAudioX, audioButtonsY, WHITE);
                    }
                    else
                    {
                        DrawTexture(musicAudioMuted, musicAudioX, audioButtonsY, WHITE);
                    }
                }
                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                {
                    musicAction = true;
                }
            }
            else
            {
                musicState = 0;
                if (!musicMuted)
                {
                    DrawTexture(musicAudio, musicAudioX, audioButtonsY, WHITE);
                }
                else
                {
                    DrawTexture(musicAudioMuted, musicAudioX, audioButtonsY, WHITE);
                }
            }
            if (musicAction && !musicMuted)
            {
                if (!soundMuted)
                {
                    PlaySound(buttonSFX);
                }
                musicMuted = true;
                musicAction = false;
            }
            if (musicAction && musicMuted)
            {
                if (!soundMuted)
                {
                    PlaySound(buttonSFX);
                }
                musicMuted = false;
                musicAction = false;
            }
            //
            //
            // sfx sound
            //
            //
            if (CheckCollisionPointRec(mousePoint, sfxRect))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                {
                    sfxState = 2;
                    if (!soundMuted)
                    {
                        DrawTexture(sfxAudio, sfxAudioX, audioButtonsY, WHITE);
                    }
                    else
                    {
                        DrawTexture(sfxAudioMuted, sfxAudioX, audioButtonsY, WHITE);
                    }
                }
                else
                {
                    sfxState = 1;
                    if (!soundMuted)
                    {
                        DrawTexture(sfxAudio, sfxAudioX, audioButtonsY, WHITE);
                    }
                    else
                    {
                        DrawTexture(sfxAudioMuted, sfxAudioX, audioButtonsY, WHITE);
                    }
                }
                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                {
                    sfxAction = true;
                }
            }
            else
            {
                sfxState = 0;
                if (!soundMuted)
                {
                    DrawTexture(sfxAudio, sfxAudioX, audioButtonsY, WHITE);
                }
                else
                {
                    DrawTexture(sfxAudioMuted, sfxAudioX, audioButtonsY, WHITE);
                }
            }
            if (sfxAction && !soundMuted)
            {

                PlaySound(buttonSFX);

                soundMuted = true;
                sfxAction = false;
            }
            if (sfxAction && soundMuted)
            {

                PlaySound(buttonSFX);

                soundMuted = false;
                sfxAction = false;
            }
            // return to title

            if (IsKeyPressed(KEY_ESCAPE))
            {
                if (!soundMuted)
                {
                    PlaySound(buttonSFX);
                }
                currentScreen = TITLE;
            }
            DrawTexturePro(exitBack,
                           exitRectangle,
                           (Rectangle){0, 0, exitRectangle.width / 6, exitRectangle.height / 6},
                           exitCenter,
                           0,
                           WHITE);

            if (CheckCollisionPointRec(mousePoint, exitHitbox))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                {
                    exitState = 2;
                }
                else
                {
                    exitState = 1;
                }
                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                {
                    if (!soundMuted)
                    {
                        PlaySound(buttonSFX);
                    }
                    gamePaused = false;
                    currentScreen = TITLE;
                }
            }
            break;
            /*
            *********************************************************
            GAME OVER
            ************************************************************
            */
        case GAMEOVER:
            ClearBackground(WHITE);
            DrawTexture(gameOver, 0, 0, WHITE);
            if (!musicMuted)
            {
                PlayMusicStream(gameOverMusic);
                UpdateMusicStream(gameOverMusic);
            }
            resumeGame = false;
            if (cpu.speed == 3)
            {
                if (!musicMuted)
                {
                    StopMusicStream(menuMusic);
                }
                DrawText(TextFormat("%i", player_score), 290, 300, 80, WHITE);
                DrawText(TextFormat("%i", highScoreEasy), 895, 300, 80, WHITE);
            }
            if (cpu.speed == 4 && !hardMode)
            {
                if (!musicMuted)
                {
                    StopMusicStream(gameplayMusic);
                }
                DrawText(TextFormat("%i", player_score), 290, 300, 80, WHITE);
                DrawText(TextFormat("%i", highScoreMedium), 895, 300, 80, WHITE);
            }
            if (cpu.speed == 4 && hardMode)
            {
                if (!musicMuted)
                {
                    StopMusicStream(bossMusic);
                }
                DrawText(TextFormat("%i", player_score), 290, 300, 80, WHITE);
                DrawText(TextFormat("%i", highScoreHard), 895, 300, 80, WHITE);
            }

            // Check main menu button state
            if (CheckCollisionPointRec(mousePoint, mainMenuBounds))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                {
                    mainState = 2;
                    DrawTexture(mainMenuImage, mainMenuX, mainMenuY, WHITE);
                }

                else
                {
                    mainState = 1;
                    DrawTexture(mainMenuImageGlow, mainMenuX, mainMenuY, WHITE);
                }

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                {
                    mainAction = true;
                }
            }
            else
            {
                mainState = 0;
                DrawTexture(mainMenuImage, mainMenuX, mainMenuY, WHITE);
            }
            if (mainAction)
            {
                if (!soundMuted)
                {
                    PlaySound(buttonSFX);
                }
                currentScreen = TITLE;
                mainAction = false;
                playerLives = 3;
                hardMode = false;
                if (!musicMuted)
                {
                    StopMusicStream(gameOverMusic);
                }
            }

            // PLAY AGAIN
            if (CheckCollisionPointRec(mousePoint, againBounds))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                {
                    againState = 2;
                    DrawTexture(playAgainImage, playAgainX, playAgainY, WHITE);
                }

                else
                {
                    againState = 1;
                    DrawTexture(playAgainGlow, playAgainX, playAgainY, WHITE);
                }

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                {
                    againAction = true;
                }
            }
            else
            {
                againState = 0;
                DrawTexture(playAgainImage, playAgainX, playAgainY, WHITE);
            }
            if (againAction)
            {
                if (!soundMuted)
                {
                    PlaySound(buttonSFX);
                }
                currentScreen = GAMEPLAYSINGLE;
                if (!musicMuted)
                {
                    StopMusicStream(gameOverMusic);
                }
                if (cpu.speed == 3)
                {

                    playerLives = 9;
                    player_score = 0;
                    cpu_score = 0;
                }

                if (cpu.speed == 4 && !hardMode)
                {

                    playerLives = 5;
                    player_score = 0;
                    cpu_score = 0;
                }

                if (cpu.speed == 4 && hardMode)
                {

                    playerLives = 1;
                    player_score = 0;
                    cpu_score = 0;
                }

                againAction = false;
            }
            if (cpu.speed == 4 && !hardMode)
            {
                DrawTexture(backgroundMedium, 0, 0, WHITE);
            }
            if (cpu.speed == 4 && hardMode)
            {
                DrawTexture(backgroundHard, 0, 0, WHITE);
            }
            finalScore = 0;
            break;

        /*
        *************************
        GAME ENDING
        *********************
        */
        case ENDING:
            if (!musicMuted)
            {
                PlayMusicStream(victorySong);
                UpdateMusicStream(victorySong);
            }
            ClearBackground(WHITE);
            if (player_score == 5)
            {

                if (player_score == 5 && cpu.speed == 3)
                {
                    DrawTexture(continueGame, 0, 0, WHITE);
                    DrawTexture(continueImage, continueImageX, continueImageY, WHITE);
                    if (!musicMuted)
                    {
                        StopMusicStream(menuMusic);
                    }
                }
                if (player_score == 5 && cpu.speed == 4 && !hardMode)
                {
                    if (!musicMuted)
                    {
                        StopMusicStream(gameplayMusic);
                    }
                    DrawTexture(continueGame, 0, 0, WHITE);
                }
                if (player_score == 5 && cpu.speed == 4 && hardMode)
                {
                    if (!musicMuted)
                    {
                        StopMusicStream(bossMusic);
                    }
                    DrawTexture(continueGame, 0, 0, WHITE);
                }

                // CONTINUE BUTTON
                if (CheckCollisionPointRec(mousePoint, continueImageBounds))
                {
                    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    {
                        continueState = 2;
                        DrawTexture(continueImage, continueImageX, continueImageY, WHITE);
                    }

                    else
                    {
                        continueState = 1;
                        DrawTexture(continueImageGlow, continueImageX, continueImageY, WHITE);
                    }

                    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    {
                        continueAction = true;
                    }
                }
                else
                {
                    continueState = 0;
                    DrawTexture(continueImage, continueImageX, continueImageY, WHITE);
                }
                if (continueAction && !resumeGame)
                {
                    if (!soundMuted)
                    {
                        PlaySound(buttonSFX);
                    }
                    currentScreen = GAMEPLAYSINGLE;
                    continueAction = false;
                    resumeGame = true;
                    StopMusicStream(victorySong);
                }
            }
            if (player_score2 == 7 || player_score2nd == 7)
            {

                if (player_score2 == 7)
                {
                    DrawTexture(player1victory, 0, 0, WHITE);
                    DrawText(TextFormat("%i", player_score2), 962, 353, 104, BACKGROUNDCOLOR);
                    DrawText(TextFormat("%i", player_score2), 960, 351, 104, BLACK);

                    DrawText("to", 802, 353, 104, BACKGROUNDCOLOR);
                    DrawText("to", 800, 351, 104, BLACK);

                    DrawText(TextFormat("%i", player_score2nd), 702, 353, 104, BACKGROUNDCOLOR);
                    DrawText(TextFormat("%i", player_score2nd), 700, 351, 104, BLACK);
                }
                else if (player_score2nd == 7)
                {
                    DrawTexture(player2victory, 0, 0, WHITE);
                    DrawText(TextFormat("%i", player_score2), 962, 353, 104, BACKGROUNDCOLOR);
                    DrawText(TextFormat("%i", player_score2), 960, 351, 104, BLACK);

                    DrawText("to", 802, 353, 104, BACKGROUNDCOLOR);
                    DrawText("to", 800, 351, 104, BLACK);

                    DrawText(TextFormat("%i", player_score2nd), 702, 353, 104, BACKGROUNDCOLOR);
                    DrawText(TextFormat("%i", player_score2nd), 700, 351, 104, BLACK);
                }

                // PLAY AGAIN
                if (CheckCollisionPointRec(mousePoint, againBounds))
                {
                    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    {
                        againState = 2;
                        DrawTexture(playAgainImage, playAgainX, playAgainY, WHITE);
                    }

                    else
                    {
                        againState = 1;
                        DrawTexture(playAgainGlow, playAgainX, playAgainY, WHITE);
                    }

                    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    {
                        againAction = true;
                    }
                }
                else
                {
                    againState = 0;
                    DrawTexture(playAgainImage, playAgainX, playAgainY, WHITE);
                }
                if (againAction)
                {
                    if (!soundMuted)
                    {
                        PlaySound(buttonSFX);
                    }
                    currentScreen = GAMEPLAYDOUBLE;
                    player_score2 = 0;
                    player_score2nd = 0;
                    againAction = false;
                    StopMusicStream(victorySong);
                }
            }

            if (IsKeyPressed(KEY_ESCAPE))
            {
                currentScreen = TITLE;
                player_score2 = 0;
                player_score2nd = 0;
                if (!soundMuted)
                {
                    PlaySound(buttonSFX);
                }
            }
            // Check main menu button state
            if (CheckCollisionPointRec(mousePoint, mainMenuBounds))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                {
                    mainState = 2;
                    DrawTexture(mainMenuImage, mainMenuX, mainMenuY, WHITE);
                }

                else
                {
                    mainState = 1;
                    DrawTexture(mainMenuImageGlow, mainMenuX, mainMenuY, WHITE);
                }

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                {
                    mainAction = true;
                }
            }
            else
            {
                mainState = 0;
                DrawTexture(mainMenuImage, mainMenuX, mainMenuY, WHITE);
            }
            if (mainAction)
            {
                if (!soundMuted)
                {
                    PlaySound(buttonSFX);
                }
                currentScreen = TITLE;
                mainAction = false;
                playerLives = 3;
                if (!musicMuted)
                {
                    StopMusicStream(gameOverMusic);
                }
                player_score2 = 0;
                player_score2nd = 0;
                StopMusicStream(victorySong);
            }
            if (player_score == 5 && cpu.speed == 4 && !hardMode)
            {
                DrawTexture(backgroundMedium, 0, 0, WHITE);
            }
            if (player_score == 5 && cpu.speed == 4 && hardMode)
            {
                DrawTexture(backgroundHard, 0, 0, WHITE);
            }
            break;
        }

        // Ends drawing the canvas

        EndDrawing();
    }

    // Closes the window
    CloseAudioDevice();
    CloseWindow();

    return 0;
}
