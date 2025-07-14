#include <iostream>
#include <windows.h>
#include <conio.h>

void clearScreen() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD count;
    COORD home = {0, 0};

    if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) return;
    DWORD cellCount = csbi.dwSize.X * csbi.dwSize.Y;

    // Fill with spaces
    FillConsoleOutputCharacter(hConsole, ' ', cellCount, home, &count);
    // Reset attributes
    FillConsoleOutputAttribute(hConsole, csbi.wAttributes, cellCount, home, &count);
    // Move cursor home
    SetConsoleCursorPosition(hConsole, home);
}

void draw(int WIDTH, int HEIGHT, int ballX, int ballY,
          int paddleLeftY, int paddleRightY, int PADDLE_SIZE,
          int scoreLeft, int scoreRight)
{
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            if (x == 0 || x == WIDTH - 1) {
                std::cout << '|';
            }
            else if (y == 0 || y == HEIGHT - 1) {
                std::cout << '-';
            }
            else if (x == ballX && y == ballY) {
                std::cout << 'O';
            }
            else if (x == 1 && y >= paddleLeftY && y < paddleLeftY + PADDLE_SIZE) {
                std::cout << '#';
            }
            else if (x == WIDTH - 2 &&
                     y >= paddleRightY &&
                     y < paddleRightY + PADDLE_SIZE) {
                std::cout << '#';
            }
            else {
                std::cout << ' ';
            }
        }
        std::cout << '\n';
    }
    std::cout << "Sinistra: " << scoreLeft
              << "   Destra: " << scoreRight << std::endl;
}

int update(int &ballX, int &ballY, int &ballDirX, int &ballDirY,
           int WIDTH, int HEIGHT,
           int paddleLeftY, int paddleRightY, int PADDLE_SIZE)
{
    ballX += ballDirX;
    ballY += ballDirY;

    // Bounce off top/bottom
    if (ballY <= 1 || ballY >= HEIGHT - 2) {
        ballDirY = -ballDirY;
    }

    // Left paddle
    if (ballX == 2 &&
        ballY >= paddleLeftY &&
        ballY < paddleLeftY + PADDLE_SIZE)
    {
        ballDirX = -ballDirX;
    }
    // Right paddle
    if (ballX == WIDTH - 3 &&
        ballY >= paddleRightY &&
        ballY < paddleRightY + PADDLE_SIZE)
    {
        ballDirX = -ballDirX;
    }

    // Scoring
    if (ballX <= 0) {
        ballX = WIDTH / 2;
        ballY = HEIGHT / 2;
        return -1;
    }
    if (ballX >= WIDTH - 1) {
        ballX = WIDTH / 2;
        ballY = HEIGHT / 2;
        return 1;
    }
    return 0;
}

int main() {
    const int WIDTH = 40;
    const int HEIGHT = 20;
    const int PADDLE_SIZE = 4;

    int ballX = WIDTH / 2;
    int ballY = HEIGHT / 2;
    int ballDirX = -1;
    int ballDirY = -1;

    int paddleLeftY  = HEIGHT / 2 - PADDLE_SIZE / 2;
    int paddleRightY = HEIGHT / 2 - PADDLE_SIZE / 2;

    int scoreLeft  = 0;
    int scoreRight = 0;

    bool running = true;
    while (running) {
        clearScreen();

        // Nonblocking input
        if (_kbhit()) {
            char ch = _getch();
            if (ch == 'q') {
                running = false;
            }
            if (ch == 'w' && paddleLeftY > 1) {
                paddleLeftY--;
            }
            if (ch == 's' && paddleLeftY + PADDLE_SIZE < HEIGHT - 1) {
                paddleLeftY++;
            }
            if (ch == 'r') {
                // Reset everything
                scoreLeft  = scoreRight = 0;
                ballX = WIDTH / 2;
                ballY = HEIGHT / 2;
                ballDirX = -1;
                ballDirY = -1;
                paddleLeftY  = paddleRightY = HEIGHT / 2 - PADDLE_SIZE / 2;
            }
        }

        // Simple AI for right paddle
        if (ballY < paddleRightY + PADDLE_SIZE/2 && paddleRightY > 1) {
            paddleRightY--;
        }
        else if (ballY > paddleRightY + PADDLE_SIZE/2 &&
                 paddleRightY + PADDLE_SIZE < HEIGHT - 1)
        {
            paddleRightY++;
        }

        int goal = update(ballX, ballY, ballDirX, ballDirY,
                          WIDTH, HEIGHT,
                          paddleLeftY, paddleRightY, PADDLE_SIZE);
        if (goal == 1)  scoreLeft++;
        if (goal == -1) scoreRight++;

        draw(WIDTH, HEIGHT, ballX, ballY,
             paddleLeftY, paddleRightY, PADDLE_SIZE,
             scoreLeft, scoreRight);

        Sleep(50); // 50 ms delay
    }

    return 0;
}
