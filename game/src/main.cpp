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

void update()
{
	dt = 1.0f / TARGET_FPS;
    time += dt;

    x = x + (-sin(time * frequency)) * frequency * amplitude * dt;
    y = y + (cos(time * frequency)) * frequency * amplitude * dt;
}

void draw()
{
    BeginDrawing();
    ClearBackground(BLACK);
	DrawText("Anthony Laylor 101547506", 10, float(GetScreenHeight() - 30), 20, BLACK);

	GuiSliderBar(Rectangle{ 10, 15, 1000, 20 }, "", TextFormat("%.2f", time), &time, 0, 240);
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
    /*while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(DARKGREEN);
            DrawText("Anthony Laylor 101547506", 10, float(GetScreenHeight() - 10), 20, BLACK);


            time += 1;
            GuiSliderBar(Rectangle{ 60, 5, 1000, 10 }, "Time", TextFormat("%.2f", time), &time, 0, 240);


        EndDrawing();
    }*/



    CloseWindow();
    return 0;
}
