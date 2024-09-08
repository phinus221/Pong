#include <iostream>
#include <raylib.h>
#include <time.h>
#include <raygui.h>

using namespace std;

const int screenWidth = 800;
const int screenHeight = 600;
int score_left = 0;
int score_right = 0;
bool gameStart = false;

class Pong
{
public:
    Rectangle rectangle;

    Pong(int x, int y)
    {
        rectangle.width = 10;
        rectangle.height = 100;
        rectangle.x = x;
        rectangle.y = y;
    }

    void Draw()
    {
        DrawRectangleRec(rectangle, WHITE);
    }
};

class Ball
{
public:
    Vector2 position = {screenWidth / 2, screenHeight / 2};
    int speed = 5;
    int collision_count = 0;
    int direction = 1;
    float direction_y = 0;

    void Draw(Rectangle paddle1, Rectangle paddle2)
    {
        DrawCircle(position.x, position.y, 20, WHITE);
        position.x += (speed + collision_count) * direction;
        position.y += (speed + collision_count) * direction_y;
        CheckCollision(paddle1);
        CheckCollision(paddle2);
    }

    void CheckCollision(Rectangle paddle)
    {
        if (CheckCollisionCircleRec(position, 20, paddle) == true)
        {
            direction = direction * -1;
            int randomDirecton = GetRandomValue(-1, 1);
            float randomValue = GetRandomValue(-45, 45) / 100.0f;

            direction_y = randomValue * randomDirecton;

            if (collision_count < 13)
            {
                collision_count++;
            }
        }
        if (position.y > screenHeight - 20)
        {
            direction_y -= 0.15f;
        }
        if (position.y < 20)
        {
            direction_y += 0.15f;
        }
        if (position.x < -20)
        {
            score_right++;
            ResetBall();
        }
        else if (position.x > screenWidth + 20)
        {
            score_left++;
            ResetBall();
        }
    }

    void ResetBall()
    {
        position = {screenWidth / 2, screenHeight / 2};
        speed = 5;
        collision_count = 0;
        direction = 1;
        direction_y = 0;
    }
};

class Game
{
public:
    Pong pong1 = Pong(20, 250);
    Pong pong2 = Pong(775, 250);
    Ball ball;

    void Draw()
    {
        ClearBackground(BLACK);
        pong1.Draw();
        pong2.Draw();
        ball.Draw(pong1.rectangle, pong2.rectangle);
        DrawText(TextFormat("%i", score_left), 30, 30, 20, WHITE);
        DrawText(TextFormat("%i", score_right), 770, 30, 20, WHITE);
    }
};

void MainMenu()
{

    ClearBackground(BLACK);
    Rectangle buttonStart;
    Rectangle buttonQuit;

    DrawText("PONG!", screenWidth / 2 - 75, screenHeight / 2 - 75, 50, WHITE);

    buttonStart.x = 100;
    buttonStart.y = 400;
    buttonStart.width = 150;
    buttonStart.height = 80;
    DrawRectangleRec(buttonStart, WHITE);
    DrawText("Start", 140, 430, 25, BLACK);

    buttonQuit.x = screenWidth - 250;
    buttonQuit.y = 400;
    buttonQuit.width = 150;
    buttonQuit.height = 80;
    DrawRectangleRec(buttonQuit, WHITE);
    DrawText("Exit", 600, 430, 25, BLACK);

    if (GetMouseX() >= buttonStart.x && GetMouseX() <= buttonStart.x + buttonStart.width && GetMouseY() >= buttonStart.y && GetMouseY() <= buttonStart.y + buttonStart.height)
    {
        DrawRectangleRec(buttonStart, GRAY);
        DrawText("Start", 140, 430, 25, BLACK);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) == true)
        {
            gameStart = true;
            ClearBackground(BLACK);
        }
    }
    if (GetMouseX() >= buttonQuit.x && GetMouseX() <= buttonQuit.x + buttonQuit.width && GetMouseY() >= buttonQuit.y && GetMouseY() <= buttonQuit.y + buttonQuit.height)
    {
        DrawRectangleRec(buttonQuit, GRAY);
        DrawText("Exit", 600, 430, 25, BLACK);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) == true)
        {
            EndDrawing();
            CloseWindow();
            exit(0);
        }
    }
}

int main()
{
    InitWindow(screenWidth, screenHeight, "Pong!!!");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);

    Game game = Game();
    bool gamePaused = false;
    int ball_speed = 0;
    int ball_counter = 0;

    while (WindowShouldClose() == false)
    {
        BeginDrawing();
        if (gameStart == true)
        {
            game.Draw();
        }
        else
        {
            MainMenu();
        }

        if (gamePaused)
        {
            DrawText("PAUSED", screenWidth / 2 - 100, screenHeight / 2 - 100, 50, WHITE);

            Rectangle buttonResume;
            buttonResume.x = 100;
            buttonResume.y = 400;
            buttonResume.width = 150;
            buttonResume.height = 80;
            
            DrawRectangleRec(buttonResume, WHITE);
            DrawText("Resume", 140, 430, 25, BLACK);

            Rectangle buttonQuit;
            buttonQuit.x = screenWidth - 250;
            buttonQuit.y = 400;
            buttonQuit.width = 150;
            buttonQuit.height = 80;
            DrawRectangleRec(buttonQuit, WHITE);
            DrawText("Exit", 600, 430, 25, BLACK);

            if (GetMouseX() >= buttonResume.x && GetMouseX() <= buttonResume.x + buttonResume.width && GetMouseY() >= buttonResume.y && GetMouseY() <= buttonResume.y + buttonResume.height)
            {
                DrawRectangleRec(buttonResume, GRAY);
                DrawText("Resume", 140, 430, 25, BLACK);
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) == true)
                {
                    game.ball.speed = ball_speed;
                    game.ball.collision_count = ball_counter;
                    gamePaused = false;
                }
            }
            if (GetMouseX() >= buttonQuit.x && GetMouseX() <= buttonQuit.x + buttonQuit.width && GetMouseY() >= buttonQuit.y && GetMouseY() <= buttonQuit.y + buttonQuit.height)
            {
                DrawRectangleRec(buttonQuit, GRAY);
                DrawText("Exit", 600, 430, 25, BLACK);
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) == true)
                {
                    EndDrawing();
                    CloseWindow();
                    exit(0);
                }
            }

            if (IsKeyPressed(KEY_ESCAPE) == true)
            {
                game.ball.speed = ball_speed;
                game.ball.collision_count = ball_counter;
                gamePaused = false;
            }
        }
        else
        {
            if (IsKeyDown(KEY_S) == true && game.pong1.rectangle.y < screenHeight - 100)
            {
                game.pong1.rectangle.y += 10;
            }
            if (IsKeyDown(KEY_W) == true && game.pong1.rectangle.y > 0)
            {
                game.pong1.rectangle.y += -10;
            }
            if (IsKeyDown(KEY_DOWN) == true && game.pong2.rectangle.y < screenHeight - 100)
            {
                game.pong2.rectangle.y += 10;
            }
            if (IsKeyDown(KEY_UP) == true && game.pong2.rectangle.y > 0)
            {
                game.pong2.rectangle.y += -10;
            }

            if (IsKeyPressed(KEY_ESCAPE) == true)
            {
                ball_speed = game.ball.speed;
                ball_counter = game.ball.collision_count;
                game.ball.collision_count = 0;
                game.ball.speed = 0;
                gamePaused = true;
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}