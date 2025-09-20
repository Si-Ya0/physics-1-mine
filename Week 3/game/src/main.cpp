#include "raylib.h"
#include "raymath.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "game.h"

const unsigned int TARGET_FPS = 50; // frames/second
float dt; // Delta time (time between frames) used for physics calculations
int origin[2] = { 200, 500 };
int launchAngle = 0;
int launchSpeed = 1;
int endVectorX;
int endVectorY;
float drag = 0.99f; // keep it 0.99 for now;
float mass; //Not needed atm
float time; //Time in seconds
Vector2 velocity;
Vector2 position;
Vector2 gravityAcceleration = { 0, 100 }; // Gravity acceleration vector

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

//void velocityCalc() //Calculates velocity based on speed and angle
//{
//	velocity = launchSpeed * (cosf(launchAngle * DEG2RAD));
//}

void PrintStats() //Prints stats and instructions to screen
{
	DrawText(TextFormat("Angle: %d", launchAngle), GetScreenWidth() - 300, 10, 30, WHITE);
	DrawText(TextFormat("Speed: %d", launchSpeed), GetScreenWidth() - 300, 50, 30, WHITE);

	DrawText("Use Up/Down arrows to change angle", 10, 10, 20, WHITE);
	DrawText("Use Left/Right arrows to change speed", 10, 40, 20, WHITE);
}


void Controls() {

	if (IsKeyPressed(KEY_ESCAPE)) // Exit program with escape key
	{
		CloseWindow();
	}

	if (IsKeyDown(KEY_R)) // Reset launch angle and speed with R key
	{
		launchAngle = 0;
		launchSpeed = 1;
	}

	if (IsKeyDown(KEY_UP)) // Adjust launch angle with up and down arrow keys
	{
		launchAngle++;
	}
	else if (IsKeyDown(KEY_DOWN))
	{
		launchAngle--;
	}

	if (IsKeyDown(KEY_RIGHT))// Adjust launch speed with left and right arrow keys
	{
		launchSpeed++;
	}

	else if (IsKeyDown(KEY_LEFT))
	{
		launchSpeed--;
	}

	if (IsKeyDown(KEY_RIGHT))// Adjust launch speed with left and right arrow keys
	{
		launchSpeed++;
		if (launchSpeed > 800) launchSpeed = 800;  // Cap maximum speed
	}
	else if (IsKeyDown(KEY_LEFT))
	{
		launchSpeed--;
		if (launchSpeed < 1) launchSpeed = 1;  // Prevent negative/zero speed
	}
}

void update() 
{
	dt = GetFrameTime();
	time += dt;

	if (IsKeyPressed(KEY_SPACE)) 
	{
		position = { (float)origin[0], (float)origin[1] };
		velocity = { (float)cos(launchAngle * DEG2RAD) * launchSpeed, (float) - sin(launchAngle * DEG2RAD) * launchSpeed};
	}

	position += velocity * dt;
	Controls();
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
	

	DrawLineEx(startpos, endpos, 10, GRAY); // Draw line from origin to projectile
	Vector2 velocity = { cos(launchAngle * DEG2RAD) * launchSpeed, -sin(launchAngle * DEG2RAD) * launchSpeed };
	DrawCircle(position.x, position.y, 25, RED); // Draw birb at origin

	PrintStats();
	EndDrawing();
}

int main()
{
    InitWindow(InitialWidth, InitialHeight, "Physics Labs: Anthony Laylor 101547506");
    SetTargetFPS(TARGET_FPS);

	while (!WindowShouldClose())
	{
		update();
		draw();
	}

    CloseWindow();
    return 0;
}
