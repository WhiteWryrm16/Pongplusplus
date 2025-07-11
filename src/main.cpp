#include <iostream>
#include <cstdlib> // per system("clear")
#include <fcntl.h>
#include <unistd.h> // per sleep()
#include <termios.h>

void draw(int WIDTH, int HEIGHT, int ballX, int ballY, int paddleLeftY, int paddleRightY, int PADDLE_SIZE, int scoreLeft, int scoreRight ) {
    // Disegna il campo di gioco
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            if (x == 0 || x == WIDTH - 1) {
                std::cout << '|'; // muri laterali
            } else if (y == 0 || y == HEIGHT - 1) {
                std::cout << '-'; // muri superiore e inferiori
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
    std::cout << "Sinistra: " << scoreLeft << "   Destra: " << scoreRight << std::endl;
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

// Aggiorna la posizione della palla e gestisce le collisioni
int update(int &ballX, int &ballY, int &ballDirX, int &ballDirY,
            const int WIDTH, const int HEIGHT,
            const int paddleLeftY, const int paddleRightY, const int PADDLE_SIZE) {
    // Aggiorna la posizione della palla
    ballX += ballDirX;
    ballY += ballDirY;

    // Collisione con il muro superiore o inferiore
    if (ballY <= 1 || ballY >= HEIGHT - 2) {
        ballDirY = -ballDirY;
    }

    // Collisione con la racchetta sinistra (muro vicino alla x==1)
    if (ballX == 2) {
        if (ballY >= paddleLeftY && ballY < paddleLeftY + PADDLE_SIZE) {
            ballDirX = -ballDirX;
        }
    }
    // Collisione con la racchetta destra (muro vicino a WIDTH-2)
    if (ballX == WIDTH - 3) {
        if (ballY >= paddleRightY && ballY < paddleRightY + PADDLE_SIZE) {
            ballDirX = -ballDirX;
        }
    }

    // Se la palla esce dai bordi laterali, resettala al centro
    if (ballX <= 0) {
        ballX = WIDTH / 2;
        ballY = HEIGHT / 2;
        return -1; // Punto al giocatore destro
    }
    if (ballX >= WIDTH - 1) {
        ballX = WIDTH / 2;
        ballY = HEIGHT / 2;
        return 1; // Punto al giocatore sinistro
    }
    return 0;
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

    int scoreLeft = 0;  // Punteggio giocatore sinistro
    int scoreRight = 0; // Punteggio giocatore destro

    setupTerminal();
    atexit(resetTerminal);

    bool running = true;
    while (running) {
        system("clear");

        // Input utente per la racchetta sinistra
        char ch = getInput();
        if (ch == 'q') running = false; // Esci con 'q'
        if (ch == 'w' && paddleLeftY > 1) paddleLeftY--;
        if (ch == 's' && paddleLeftY + PADDLE_SIZE < HEIGHT - 1) paddleLeftY++;

        // Reset punteggio e posizione con 'r'
        if (ch == 'r') {
            scoreLeft = 0;
            scoreRight = 0;
            ballX = WIDTH / 2;
            ballY = HEIGHT / 2;
            ballDirX = -1;
            ballDirY = -1;
            paddleLeftY = HEIGHT / 2 - PADDLE_SIZE / 2;
            paddleRightY = HEIGHT / 2 - PADDLE_SIZE / 2;
        }

        // Semplice IA per la racchetta destra
        if (ballY < paddleRightY + PADDLE_SIZE/2 && paddleRightY > 1) paddleRightY--;
        else if (ballY > paddleRightY + PADDLE_SIZE/2 && paddleRightY + PADDLE_SIZE < HEIGHT - 1) paddleRightY++;

        // Aggiorna la posizione della palla e gestisci le collisioni
        int goal = update(ballX, ballY, ballDirX, ballDirY, WIDTH, HEIGHT, paddleLeftY, paddleRightY, PADDLE_SIZE);
        if (goal == 1) scoreLeft++;
        if (goal == -1) scoreRight++;

        // Ridisegna il campo
        draw(WIDTH, HEIGHT, ballX, ballY, paddleLeftY, paddleRightY, PADDLE_SIZE, scoreLeft, scoreRight);

        usleep(50000); // Attendi 50ms (velocità del gioco)
    }

    return 0;
}