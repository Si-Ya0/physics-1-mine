#include "raylib.h"
#include "raymath.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
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

enum Shapes {
    SPHERE, HALFSPACE
};

struct Ball { // Ball structure
    Vector2 position;
    Vector2 velocity;
	bool active; // is the ball in motion
    vector<Vector2> trail; // stores past positions
    Color color;
    int randomRadius = GetRandomValue(10, 75);
};
vector<Ball> balls; // stores all balls

struct HalfSpace {
    Vector2 position;
    Vector2 normal;
    float angle;
};

HalfSpace halfspace = { {400, 550}, {0, -1}, 0.0f };
HalfSpace halfspace2 = { {750, 700}, {0, -1}, 60.0f };
float halfspaceXSlider = halfspace.position.x;
float halfspaceYSlider = halfspace.position.y;
float halfspaceAngleSlider = halfspace.angle;

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

    if (IsKeyDown(KEY_T)) {
        halfspace2.angle += 1.0f;
    }
    else if (IsKeyDown(KEY_G)) {
        halfspace2.angle -= 1.0f;
    }

    // keep halfspace2.angle in a reasonable range (-180..180)
    if (halfspace2.angle > 180.0f) halfspace2.angle -= 360.0f;
    if (halfspace2.angle < -180.0f) halfspace2.angle += 360.0f;
}

bool CheckSphereHalfSpaceOverlap(Vector2 sphereCenter, float radius, HalfSpace halfSpace) {
    float distance = Vector2DotProduct(halfSpace.normal, Vector2Subtract(sphereCenter, halfSpace.position));
    return distance <= radius;
}

bool BallCollisionResponse(Ball& ballA, Ball& ballB) { //similar to overlap function.
    Vector2 displacement = ballB.position - ballA.position;
    float distance = Vector2Length(displacement);
	float sumOfRadii = ballA.randomRadius + ballB.randomRadius;
	float overlap = sumOfRadii - distance;
	Vector2 normal = displacement/ distance; // normalized displacement
    Vector2 mtv = normal * overlap;

    if (overlap > 0) {
        Vector2 normal = displacement / distance;
		Vector2 mtv = normal * overlap;
        ballA.position -= mtv * 0.5f;
        ballB.position += mtv * 0.5f;
		return true;
    }

    else {
		return false;
    }
    // if overlap > 0, return true, else, false
	// for overlap > 0, circleA->position += displacementFromAtoB * overlap * 0.5, and make circleB position -= displacementFromAtoB * overlap *0.5
}

bool HalfSpaceCollisionResponse(Ball& ball, HalfSpace halfSpace) {
	Vector2 displacement = ball.position - halfSpace.position;
    float distance = Vector2DotProduct(halfSpace.normal, displacement);
	float overlap = ball.randomRadius - distance;

    if (overlap > 0) {
        ball.position += halfSpace.normal * overlap;
        return true;
    }
    else {
        return false;
	}
}

void update()
{
    dt = GetFrameTime();
    Controls(); // moved here so reset happens first.

    GuiSliderBar({ 150, 100, 200, 20 }, "Halfspace X", NULL, &halfspaceXSlider, 0, GetScreenWidth());
    GuiSliderBar({ 150, 130, 200, 20 }, "Halfspace Y", NULL, &halfspaceYSlider, 0, GetScreenHeight());
    GuiSliderBar({ 150, 160, 200, 20 }, "Halfspace Angle", NULL, &halfspaceAngleSlider, -90, 90);

    halfspace.position = { halfspaceXSlider, halfspaceYSlider };
    halfspace.angle = halfspaceAngleSlider;

    float radians = halfspace.angle * DEG2RAD;
    halfspace.normal.x = sinf(radians);
    halfspace.normal.y = -cosf(radians);


    // Update ground plane from sliders

    if (IsKeyPressed(KEY_SPACE))
    {
        Ball newBall; // new ball object/instance
        newBall.position = { (float)origin[0], (float)origin[1] };
		newBall.velocity = { cosf(launchAngle * DEG2RAD) * launchSpeed, -sinf(launchAngle * DEG2RAD) * launchSpeed }; //velocity based on angle and speed for position
        newBall.active = true;
        newBall.color = GREEN;
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

    for (auto& ball : balls) {
        ball.color = GREEN; //default color

        if (CheckSphereHalfSpaceOverlap(ball.position, ball.randomRadius, halfspace) || 
            CheckSphereHalfSpaceOverlap(ball.position, ball.randomRadius, halfspace2)) {
            ball.color = RED;
        }
    }

    for (size_t i = 0; i < balls.size(); i++) {
        for (size_t j = i + 1; j < balls.size(); j++) {
            float distance = Vector2Distance(balls[i].position, balls[j].position);

            float radius = balls[i].randomRadius + balls[j].randomRadius;

            if (distance < radius) {
                balls[i].color = RED;
                balls[j].color = RED;
            }
        }
    }


    for (size_t i = 0; i < balls.size(); i++) {
        for (size_t j = i + 1; j < balls.size(); j++) {
            BallCollisionResponse(balls[i], balls[j]);
		}
        HalfSpaceCollisionResponse(balls[i], halfspace);
        HalfSpaceCollisionResponse(balls[i], halfspace2);
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


    float radians2 = halfspace2.angle * DEG2RAD;
    halfspace2.normal.x = sinf(radians2);
    halfspace2.normal.y = -cosf(radians2);

	// Draw all balls and their trails
    for (auto& ball : balls) {
        if (!ball.trail.empty()) {
            for (size_t i = 1; i < ball.trail.size(); i++) {
                DrawLineV(ball.trail[i - 1], ball.trail[i], YELLOW); // draws trail within the image/ball loop
            }
        }
        if (ball.active) {
            DrawCircleV(ball.position, ball.randomRadius, ball.color);
        }
    }

    Vector2 dir = { -halfspace.normal.y, halfspace.normal.x }; // direction along the plane
    Vector2 p1 = Vector2Add(halfspace.position, Vector2Scale(dir, 1000));
    Vector2 p2 = Vector2Add(halfspace.position, Vector2Scale(dir, -1000));
    DrawLineV(p1, p2, WHITE);



    // Draw normal (red arrow)
    Vector2 normalEnd = Vector2Add(halfspace.position, Vector2Scale(halfspace.normal, 50));
    DrawLineEx(halfspace.position, normalEnd, 3, RED);


    Vector2 dir2 = { -halfspace2.normal.y, halfspace2.normal.x };
    Vector2 p1_2 = Vector2Add(halfspace2.position, Vector2Scale(dir2, 1000));
    Vector2 p2_2 = Vector2Add(halfspace2.position, Vector2Scale(dir2, -1000));
    DrawLineV(p1_2, p2_2, WHITE);

    Vector2 normalEnd2 = Vector2Add(halfspace2.position, Vector2Scale(halfspace2.normal, 50));
    DrawLineEx(halfspace2.position, normalEnd2, 3, RED);


    PrintStats();
    EndDrawing();
}
// dot product - radius, in half space
//remember to add halfspace 

int main()
{
    InitWindow(1000, 800, "Physics Labs: Anthony Laylor 101547506");
    SetTargetFPS(TARGET_FPS);

    while (!WindowShouldClose())
    {
        update();
        draw();
    }

    CloseWindow();
    return 0;
}