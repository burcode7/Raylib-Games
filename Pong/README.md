## Steps to develop Pong
1. Create a blank screen & Game Loop
1. Draw the paddles and the ball
1. Move the ball around
1. Check for a collision with all edges
1. Move the player's paddle
1. Move the CPU paddle with AI
1. Check for a collision with the paddles
1. Add scoring

# Game structure
## Definition:
- Defining the variables needed
- Creating the game objects
## Game Loop
- Updating the positions of the game objects
- Checking for collisions

- Steps for the game loop:
    1. Check for events
    1. Update the positions of the game objects
    1. Draw the game objects in their new position

## How to draw things

**Basic Shapes:**
```c++
void DrawRectangle(int posX, int posY, int width, int height, Color color);

void DrawCircle(int centerX, int centerY, float radius, Color color);

void DrawLine(int startPosX, int startPosY, int endPosX, int endPosY, Color, color);

void DrawPoly(Vector2 center, int sides, float radius, float rotation, Color color);
```

## Collision Detection
**Rectangle collision:**
```c++
CheckCollisionCircleRec(Vector2 center, float radius, Rectangle rec);