#include <iostream>

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

    return 0;
}