#include <iostream>
#include <raylib.h>


using namespace std;

Color Green = Color{38, 200, 154, 255};
Color Dark_Green = Color{20, 190, 133, 255};
Color Light_Green = Color{129, 224, 184, 255};
Color Yellow = Color{243, 213, 91, 255};
Color Menu_Color = Color{100, 255, 154, 255};

bool gamePaused = false;
Vector2 mousePoint = { 0.0f, 0.0f };



int player_score = 0;
int cpu_score = 0;


typedef enum GameScreen { TITLE = 0, GAMEPLAY, PAUSED, ENDING, SETTINGS } GameScreen;

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

        if(y + radius >= GetScreenHeight() || y - radius <= 0)
        {
            speed_y *= -1;
            PlaySound(ballFX);
        }


        if (x + radius >= GetScreenWidth()) // Cpu wins a point
        {
            cpu_score++;
            PlaySound(enemyPoint);
            ResetBall();
        } 
        if(x - radius <= 0) // Player point
        {
            PlaySound(playerPoint);
            player_score++;
            ResetBall();
        }

        
    }

    // Sets the ball back to the center if someone gets a point
        void ResetBall()
        {
            x = GetScreenWidth()/2;
            y = GetScreenHeight()/2;

            int speed_choices[2] = {-1,1};
            speed_x *= speed_choices[GetRandomValue(0,1)];
            speed_y *= speed_choices[GetRandomValue(0,1)];
        }
        
};

class Paddle
{
    // Allows this code to be initialized and run whenever it is declared
    protected: 

    void LimitMovement()
    {
        // Checks for collisions with the top and bottom part of the screen
        if(y <= 0)
        {
            y = 0;
        }
        
        if (y + height >= GetScreenHeight())
        {
            y = GetScreenHeight() - height;
        }
    }
    public:
        float x,y;
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
        if(IsKeyDown(KEY_UP))
        {
            y = y - speed;
        }
        if(IsKeyDown(KEY_DOWN))
        {
            y = y + speed;
        }

        LimitMovement();
        
    }
    

};

// A class that inherits the same methods as the specified class
class CpuPaddle : public Paddle
{
    public:
    // Overrites the update function and declares ball y that must be declared when updating
        void Update(int ball_y)
        {
            if (y + height / 2 > ball_y)
            {
                y = y - speed;
            }
            if (y + height/2 <= ball_y)
            {
                y = y + speed;
            }
            LimitMovement();
            
        }
        
};

// Initializes a object (you can initialize more than one with different names)
Ball ball;
Paddle player;
CpuPaddle cpu;


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
    Texture2D settingsImage = LoadTexture("assets/settings.png");
    Vector2 settingsCenter = { float(settingsImage.width / 2), float(settingsImage.height / 2) };

    // Initialize the play button
    Texture2D pause = LoadTexture("assets/playRect.png");
    float pauseHeight = (float)pause.height;
    Rectangle pauseRec = { 0, 0, (float)pause.width, pauseHeight };
    Rectangle pauseBounds = { 380, 490, (float)pause.width, (float)pause.height };

    int pauseState = 0;               // Button state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
    bool pauseAction = false;

    // Initialize the settings button
    Texture2D settings = LoadTexture("assets/settingsRect.png");
    float settingsHeight = (float)settings.height;
    Rectangle settingsRect = { 0, 0, (float)settings.width, settingsHeight };
    Rectangle settingsBounds = { 400, 670, (float)settings.width, (float)settings.height };

    int settingsState = 0;               // Button state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
    bool settingsAction = false;

    // Initialize Quit button
    Texture2D quit = LoadTexture("assets/settingsRect.png");
    float quitHeight = (float)quit.height;
    Rectangle quitRect = { 0, 0, (float)quit.width, quitHeight };
    Rectangle quitBounds = { 650, 670, (float)quit.width, (float)quit.height };

    int quitState = 0;               // Button state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
    bool quitAction = false;

    int rectangle_height = 120;
    

    ball.radius = 20;
    ball.x = screen_width / 2;
    ball.y = screen_height / 2;
    ball.speed_x = 7;
    ball.speed_y = 7;

    player.width = 25;
    player.height = rectangle_height;
    player.x = screen_width - player.width - 10;
    player.y = screen_height / 2 - player.height / 2;
    player.speed = 6;

    cpu.height = rectangle_height;
    cpu.width = 25;
    cpu.x = 10;
    cpu.y = screen_height / 2 - cpu.height / 2;
    cpu.speed = 6;

    Texture exitBack = LoadTexture("assets/exit.png");
    Rectangle exitRectangle = {0, 0, float(exitBack.width), float(exitBack.height) };
    Rectangle exitHitbox = {0, 0, float(exitBack.width/6), float(exitBack.height/6) };
    Vector2 exitCenter = { 0, 0 };
    int exitState = 0;               // Button state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
    bool exitAction = false;

    

    

    
    /*
        WindowShouldClose() checks if the close button was clicked.
        This loop runs as long as it has not been touched.
    */

    Texture background = LoadTexture("assets/Gameplay.png");
    Texture titleScreen = LoadTexture("assets/titlescreen.png");
    Texture pauseImage = LoadTexture("assets/pause.png");

    Sound buttonSFX = LoadSound("sfx/button.wav");
    Sound ballFX = LoadSound("sfx/ball.wav");

    // Intiialize pause button

    Rectangle pauseButtonRec = {0, 0, (float)pauseImage.width, pauseImage.height };
    Rectangle pauseButtonBounds = { screen_width/2 - pauseImage.width/2, screen_height/2 - pauseImage.height/1.9 };

    int pauseButtonState = 0;
    bool pauseButtonAction = false;

    float PauseButtonX = screen_width/2 - pauseImage.width/2;
    float PauseButtonY = screen_height/2 - pauseImage.height/1.9;
    
    currentScreen = TITLE;
    
    Rectangle pauseButtonHitbox = {PauseButtonX, PauseButtonY, pauseImage.width, pauseImage.height};

    while(!WindowShouldClose())
    {
        // Creates a blank canvas we can draw the game on
        BeginDrawing();
        
        mousePoint = GetMousePosition();
        pauseAction = false;

        
        switch(currentScreen)
        {
            case TITLE:

                ClearBackground(RAYWHITE);
                DrawTexture(titleScreen, 0, 0, WHITE);

                // Check play button state
                if (CheckCollisionPointRec(mousePoint, pauseBounds))
                {
                    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    {
                        pauseState = 2;
                    } 
                    else 
                    {
                        pauseState = 1;
                    }
                    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) 
                    {
                        pauseAction = true;
                    }
                }
                else 
                {
                    pauseState = 0;
                }
                if (pauseAction)
                {
                    PlaySound(buttonSFX);
                    currentScreen = GAMEPLAY;
                    pauseAction = false;
                }

                // Check settings button state
                if (CheckCollisionPointRec(mousePoint, settingsBounds))
                {
                    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    {
                        settingsState = 2;
                    }
                    else
                    {
                        settingsState = 1;
                    } 

                    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) 
                    {
                        settingsAction = true;
                    }
                }
                else
                {
                    settingsState = 0;  
                } 

                if (settingsAction)
                {
                    PlaySound(buttonSFX);
                    currentScreen = SETTINGS;
                    settingsAction = false;
                }

                // Check quit button state
                if (CheckCollisionPointRec(mousePoint, quitBounds))
                {
                    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) 
                    {
                        quitState = 2;
                    }

                    else
                    {
                         quitState = 1;
                    }

                    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) 
                    {
                        quitAction = true;
                    }
                }
                else 
                {
                    quitState = 0;
                }
                if (quitAction)
                {
                    PlaySound(buttonSFX);
                    WindowShouldClose();
                    exit(0);
                }


            break;
            case GAMEPLAY:
                    
                    pauseAction = false;

                    if (IsKeyPressed(KEY_ESCAPE))
                    {
                        PlaySound(buttonSFX);
                        gamePaused = !gamePaused;
                        
                    }

                    if (!gamePaused)
                    {
                        ball.Update();
                        player.Update();
                        // Fills the parameter ball_y with ball.y to give it a value to follow
                        cpu.Update(ball.y);

                        // Checking for collisions with both paddles
                        if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player.x, player.y, player.width, player.height}))
                        {
                            ball.speed_x *= -1;
                            PlaySound(ballFX);
                        }
                        if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{cpu.x, cpu.y, cpu.width, cpu.height}))
                        {
                            ball.speed_x *= -1;
                            PlaySound(ballFX);
                        }
                    }
                // Clears the gameplay screen
                ClearBackground(RAYWHITE);
                DrawTexture(background, 0, 0, WHITE);
                        

                // Draws a text with the parameters: (text, xpos, ypos, fontsize, color)
                // TextFormat("%i" converts something to text)
                DrawText(TextFormat("%i",cpu_score), screen_width/4, 20, 80, WHITE);
                DrawText(TextFormat("%i",player_score), 3 * screen_width/4, 20, 80, WHITE);

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
                            PlaySound(buttonSFX);
                            gamePaused = false;
                            
                        }
  
                    }


                    DrawTexturePro(exitBack,
                    exitRectangle,
                    (Rectangle) { 0, 0, exitRectangle.width/6, exitRectangle.height/6},
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
                            PlaySound(buttonSFX);
                            gamePaused = false;
                            currentScreen = TITLE;
                        }
  
                    }
                }
                
                break;
            
            case SETTINGS:
                ClearBackground(WHITE);
                DrawTexture(settingsImage, 0, 0, WHITE);
                if (IsKeyPressed(KEY_ESCAPE))
                {
                    PlaySound(buttonSFX);
                    currentScreen = TITLE;
                }
                DrawTexturePro(exitBack,
                exitRectangle,
                (Rectangle) { 0, 0, exitRectangle.width/6, exitRectangle.height/6},
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
                        PlaySound(buttonSFX);
                        gamePaused = false;
                        currentScreen = TITLE;
                    }

                }
            break;    

        }
        
        // Ends drawing the canvas
        
        EndDrawing();
    }

    // Closes the window
    UnloadTexture(titleScreen);
    CloseWindow();

    return 0;
    }
        