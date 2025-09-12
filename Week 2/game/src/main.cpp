#include "raylib.h"
#include "raymath.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "game.h"

const unsigned int TARGET_FPS = 50; // frames/second
float time = 0;
float dt;
float x = 500;
float y = 500;
float frequency = 1;
float amplitude = 100;
int origin[2] = { 500, 500 };
int launchAngle = 0;
int launchSpeed;

void update()
{
	dt = 1.0f / TARGET_FPS;
    time += dt;
	
    x = x + (-sin(time * frequency)) * frequency * amplitude * dt;
    y = y + (cos(time * frequency)) * frequency * amplitude * dt;
}

void launchAngleCalc() 
{
	launchAngle = launchAngle % 360;
}

void launchSpeedCalc()
{
	//launchSpeed = sqrt((2 * 9.81 * (y - origin[1])));
}

void draw()
{
	switch (launchAngle) // Adjust launch angle with up and down arrow keys
	{
		case KEY_UP:
		{
			launchAngle++;
		}
		case KEY_DOWN:
		{
			launchAngle--;
		}
	}

	switch (launchSpeed) // Adjust launch speed with left and right arrow keys
	{
		case KEY_RIGHT:
		{
			launchSpeed++;
		}
		case KEY_LEFT:
		{
			launchSpeed--;
		}
	}
	launchAngleCalc();
	launchSpeedCalc();

    BeginDrawing();
    ClearBackground(DARKBLUE);
	DrawText("Anthony Laylor 101547506", 10, float(GetScreenHeight() - 30), 20, WHITE);

	GuiSliderBar(Rectangle{ 10, 15, 1000, 20 }, "", TextFormat("%.2f", time), &time, 0, 360);
	DrawText(TextFormat("T: %6.2f", time), GetScreenWidth() - 140, 10, 10, BLACK);

    DrawCircle(x, y, 70, RED);
	DrawCircle(500 + cos(time * frequency) * amplitude, 500 + sin(time * frequency) * amplitude, 70, GREEN);

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
