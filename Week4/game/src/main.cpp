#include "raylib.h"
#include "raymath.h"
#include <vector>
using namespace std;

const unsigned int TARGET_FPS = 50; //fps
float dt; // Delta time
int origin[2] = { 100, 500 };
int launchAngle = 0;
int launchSpeed = 1;
int endVectorX;
int endVectorY;
Vector2 gravityAcceleration = { 0, 100 }; // Gravity acceleration vector


struct Ball { // Ball structure
    Vector2 position;
    Vector2 velocity;
	bool active; // is the ball in motion
    vector<Vector2> trail; // stores past positions
    Color color;
    int randomRadius = GetRandomValue(0, 75);
};
vector<Ball> balls; // stores all balls

void launchAngleCalc() //Keeps launch angle between 0 and 360 degrees
{
    launchAngle = launchAngle % 360;
    if (launchAngle < 0)
        launchAngle += 360;
}

void endVector() //Converts to radians for cos and sin functions
{
    endVectorX = origin[0] + (launchSpeed * cosf(launchAngle * (PI / 180)));
    endVectorY = origin[1] - (launchSpeed * sinf(launchAngle * (PI / 180)));
}

void PrintStats() //Prints stats and instructions to screen
{
    DrawText(TextFormat("Angle: %d", launchAngle), GetScreenWidth() - 300, 10, 30, WHITE);
    DrawText(TextFormat("Speed: %d", launchSpeed), GetScreenWidth() - 300, 50, 30, WHITE);

    DrawText("Use Up/Down arrows to change angle", 10, 10, 20, WHITE);
    DrawText("Use Left/Right arrows to change speed", 10, 40, 20, WHITE);
    DrawText("Use R-Key to reset both line and all ball/trails", 10, 70, 20, WHITE);
}

void Controls() {
    if (IsKeyPressed(KEY_ESCAPE)) CloseWindow();

    if (IsKeyPressed(KEY_R)) { // Reset launch angle and speed with R key
        launchAngle = 0;
        launchSpeed = 1;
		balls.clear(); // Clear all balls and trails
    }

    if (IsKeyDown(KEY_UP)) launchAngle++;
    else if (IsKeyDown(KEY_DOWN)) launchAngle--;

    if (IsKeyDown(KEY_RIGHT)) {
        launchSpeed += 2;
        if (launchSpeed > 800) launchSpeed = 800;  // Cap maximum speed
    }
    else if (IsKeyDown(KEY_LEFT)) {
        launchSpeed -= 2;
        if (launchSpeed < 1) launchSpeed = 1;  // Prevent negative/zero speed
    }
}

void update()
{
    dt = GetFrameTime();
    Controls(); // moved here so reset happens first.

    if (IsKeyPressed(KEY_SPACE))
    {
        Ball newBall; // new ball object/instance
        newBall.position = { (float)origin[0], (float)origin[1] };
		newBall.velocity = { cosf(launchAngle * DEG2RAD) * launchSpeed, -sinf(launchAngle * DEG2RAD) * launchSpeed }; //velocity based on angle and speed for position
        newBall.active = true;
        balls.push_back(newBall); //pushes into vector
    }

    // Update all balls
    for (auto& ball : balls) { //auto simplifies the declared type (good in this case for templates, pointers to fucntions &or members).
        if (ball.active) {
            ball.velocity.y += gravityAcceleration.y * dt;
            ball.position += ball.velocity * dt;
			ball.trail.push_back(ball.position); // store current position in trail
        }
    }

    launchAngleCalc();
    endVector();
}

void draw()
{
    Vector2 startpos = { (float)origin[0], (float)origin[1] };
    Vector2 endpos = { (float)endVectorX, (float)endVectorY };

    BeginDrawing();
    ClearBackground(DARKBLUE);

    DrawText("Anthony Laylor 101547506", 10, float(GetScreenHeight() - 30), 20, WHITE);
    DrawLineEx(startpos, endpos, 10, GRAY); // aim line

	// Draw all balls and their trails
    for (auto& ball : balls) {
        if (!ball.trail.empty()) {
            for (size_t i = 1; i < ball.trail.size(); i++) {
                DrawLineV(ball.trail[i - 1], ball.trail[i], YELLOW); // draws trail within the image/ball loop
            }
        }
        if (ball.active) {
            DrawCircleV(ball.position, ball.randomRadius, RED);
        }
    }

    PrintStats();
    EndDrawing();
}

int main()
{
    InitWindow(800, 600, "Physics Labs: Anthony Laylor 101547506");
    SetTargetFPS(TARGET_FPS);

    while (!WindowShouldClose())
    {
        update();
        draw();
    }

    CloseWindow();
    return 0;
}