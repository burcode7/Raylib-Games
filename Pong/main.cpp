#include <iostream>
#include <raylib.h>

using namespace std;

Color Green = Color{38, 200, 154, 255};
Color Dark_Green = Color{20, 190, 133, 255};
Color Light_Green = Color{129, 224, 184, 255};
Color Yellow = Color{243, 213, 91, 255};

int player_score = 0;
int cpu_score = 0;

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

        DrawCircle(x, y, radius, Yellow);

    }

    // Function to update the ball
    void Update()
    {
        // Updates the balls position
        x += speed_x;
        y += speed_y;

        

        // Checks if the ball Y is touching the top or bottom of the screen

        if(y + radius >= GetScreenHeight() || y - radius <= 0)
        {
            speed_y *= -1;
        }


        if (x + radius >= GetScreenWidth()) // Cpu wins a point
        {
            cpu_score++;
            ResetBall();
        }
        if(x - radius <= 0)
        {
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
    int rectangle_height = 120;
    // Initializes the window with the arguments (width, height, title)
    InitWindow(screen_width, screen_height, "Pong");
    // Sets the FPS the game wants to run at
    SetTargetFPS(60);

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

    
    /*
        WindowShouldClose() checks if the close button was clicked.
        This loop runs as long as it has not been touched.
    */
    while(WindowShouldClose() == false)
    {
        // ******DRAWS THE OBJECTS******

        // Creates a blank canvas we can draw the game on
        BeginDrawing();

        // Updating the objects
        ball.Update();
        player.Update();
        // Fills the parameter ball_y with ball.y to give it a value to follow
        cpu.Update(ball.y);

        

        // Clears the screen
        ClearBackground(Dark_Green);
        DrawRectangle(screen_width/2, 0, screen_width/2, screen_height, Green);
        DrawCircle(screen_width/2, screen_height/2, 150, Light_Green);

        // Draws a text with the parameters: (text, xpos, ypos, fontsize, color)
        // TextFormat("%i" converts something to text)
        DrawText(TextFormat("%i",cpu_score), screen_width/4, 20, 80, WHITE);
        DrawText(TextFormat("%i",player_score), 3 * screen_width/4, 20, 80, WHITE);

        // Checking for collisions with both paddles
        if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player.x, player.y, player.width, player.height}))
        {
            ball.speed_x *= -1;
        }
        if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{cpu.x, cpu.y, cpu.width, cpu.height}))
        {
            ball.speed_x *= -1;
        }

        // Draws a line from the top of the screen to the bottom
        DrawLine(screen_width/2, 0, screen_width/2, screen_height, WHITE);

        // Draws the ball and player objects
        ball.Draw();

        player.Draw();

        cpu.Draw();

        

        // Ends drawing the canvas
        EndDrawing();
    }

    // Closes the window
    CloseWindow();

    return 0;
}