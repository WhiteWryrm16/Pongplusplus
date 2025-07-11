#include <iostream>
#include <cstdlib> // per system("clear")
#include <fcntl.h>
#include <unistd.h> // per sleep()
#include <termios.h>

void draw(int WIDTH, int HEIGHT, int ballX, int ballY, int paddleLeftY, int paddleRightY, int PADDLE_SIZE) {
    // Disegna il campo di gioco
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            if (x == 0 || x == WIDTH - 1) {
                std::cout << '|'; // muri laterali
            } else if (y == 0 || y == HEIGHT - 1) {
                std::cout << '-'; // muri superiore e inferiore
            } else if (x == ballX && y == ballY) {
                std::cout << 'O'; // palla
            } else if (x == 1 && y >= paddleLeftY && y < paddleLeftY + PADDLE_SIZE) {
                std::cout << '#'; // racchetta sinistra
            } else if (x == WIDTH - 2 && y >= paddleRightY && y < paddleRightY + PADDLE_SIZE) {
                std::cout << '#'; // racchetta destra
            } else {
                std::cout << ' '; // spazio vuoto
            }
        }
        std::cout << '\n';
    }
}


void setupTerminal() {
    termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ICANON | ECHO); // disabilita input canonico e echo
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK); // imposta stdin in modalità non bloccante
}

void resetTerminal() {
    termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

char getInput() {
    char ch = 0;
    read(STDIN_FILENO, &ch, 1);
    return ch;
}
int main() {
    const int WIDTH = 40;      // larghezza campo
    const int HEIGHT = 20;     // altezza campo
    const int PADDLE_SIZE = 4; // altezza racchetta

    int ballX = WIDTH / 2;     // posizione X della palla (orizzontale)
    int ballY = HEIGHT / 2;    // posizione Y della palla (verticale)

    int ballDirX = -1;         // direzione orizzontale della palla (-1 = sinistra, 1 = destra)
    int ballDirY = -1;         // direzione verticale della palla (-1 = su, 1 = giù)

    int paddleLeftY = HEIGHT / 2 - PADDLE_SIZE / 2;  // posizione verticale racchetta sinistra
    int paddleRightY = HEIGHT / 2 - PADDLE_SIZE / 2; // posizione verticale racchetta destra

    system("clear");

    draw(WIDTH, HEIGHT, ballDirY, ballDirX, paddleLeftY, paddleRightY, PADDLE_SIZE);

    setupTerminal();

	atexit(0, resetTerminal);

    char ch = getInput();
	if (ch == 'w' && paddleLeftY > 1) paddleLeftY--;
	if (ch == 's' && paddleLeftY + PADDLE_SIZE < HEIGHT - 1) paddleLeftY++;

    return 0;
}