#include <GL/glut.h>
#include <ctime> 
#include <cstdlib> 
#include <iostream> 
#include <vector> 
#include <string> 
#include <fstream> 
#include <unordered_set>
using namespace std; 

// Global variables 
const int WIDTH = 930; 
const int HEIGHT = 660; 
const int TILE_SIZE = 30; 
const int ROWS = HEIGHT / TILE_SIZE; 
const int COLS = WIDTH / TILE_SIZE; 

char board[ROWS][COLS]; // Game board 
char shooterChar;       // Current alphabet in the shooter 
int shooterX = WIDTH / 2; // Shooter position 
int shooterY = TILE_SIZE; 
int score = 0;          // Player's score 
int timeLeft = 120;     // Time remaining (in seconds) 
unordered_set<string> dictionary; // Dictionary to store valid words 

// Function prototypes 
void timer(int);
void initializeBoard();
void drawBoard();
void drawShooter();
void drawString(int x, int y, const string& str, void* font, float r, float g, float b);
void display();
void mouse(int button, int state, int x, int y);
bool isWord(const string& word);
void checkAndRemoveWords();

// Initialize the board with random alphabets 
void initializeBoard() {
    srand(time(0)); // Seed for randomness 
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            board[i][j] = 'A' + (rand() % 26); // Random alphabet 
        }
    }
}

// Draw a string on the canvas 
void drawString(int x, int y, const string& str, void* font, float r, float g, float b) {
    glColor3f(r, g, b);
    glRasterPos2f(x, y);
    for (char c : str) {
        glutBitmapCharacter(font, c);
    }
}

// Draw the game board 
void drawBoard() {
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            glColor3f(0.5 + (i % 2) * 0.2, 0.7, 0.9); // Alternating colors 
            glRectf(j * TILE_SIZE, i * TILE_SIZE, (j + 1) * TILE_SIZE, (i + 1) * TILE_SIZE);
            drawString(j * TILE_SIZE + 10, i * TILE_SIZE + 10, string(1, board[i][j]), GLUT_BITMAP_HELVETICA_18, 0, 0, 0);
        }
    }
}

// Draw the shooter 
void drawShooter() {
    glColor3f(1.0, 1.0, 0.0);
    glRectf(shooterX - TILE_SIZE / 2, shooterY, shooterX + TILE_SIZE / 2, shooterY + TILE_SIZE);
    drawString(shooterX - 5, shooterY + 10, string(1, shooterChar), GLUT_BITMAP_HELVETICA_18, 0, 0, 0);
}

// Check if a given string is a valid word 
bool isWord(const string& word) {
    return dictionary.find(word) != dictionary.end();
}

// Check the board for valid words and remove them 
void checkAndRemoveWords() {
    vector<pair<int, int>> toClear;
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            string word = "";

            // Check horizontally 
            for (int k = 0; k < 5 && j + k < COLS; ++k) {
                word += board[i][j + k];
                if (isWord(word)) {
                    for (int l = 0; l <= k; ++l)
                        toClear.push_back({i, j + l});
                }
            }

            // Check vertically 
            word = "";
            for (int k = 0; k < 5 && i + k < ROWS; ++k) {
                word += board[i + k][j];
                if (isWord(word)) {
                    for (int l = 0; l <= k; ++l)
                        toClear.push_back({i + l, j});
                }
            }
        }
    }

    // Clear the tiles 
    for (auto& cell : toClear) {
        board[cell.first][cell.second] = ' ';
        score++; // Increment score for each cleared tile 
    }
}

// Display callback 
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawBoard();
    drawShooter();
    drawString(10, HEIGHT - 20, "Score: " + to_string(score), GLUT_BITMAP_HELVETICA_18, 1, 0, 0);
    drawString(WIDTH - 150, HEIGHT - 20, "Time Left: " + to_string(timeLeft) + " secs", GLUT_BITMAP_HELVETICA_18, 1, 0, 0);
    glFlush();
}

// Mouse click handler 
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        int targetX = x / TILE_SIZE;
        int targetY = (HEIGHT - y) / TILE_SIZE;

        // Place the shooter alphabet on the board 
        board[targetY][targetX] = shooterChar;

        // Generate a new shooter alphabet 
        shooterChar = 'A' + (rand() % 26);

        checkAndRemoveWords();
        glutPostRedisplay();
    }
}

// Timer callback to decrement the timer 
void timer(int) {
    if (timeLeft > 0) {
        timeLeft--;
        glutTimerFunc(1000, timer, 0);
    }
    glutPostRedisplay();
}

// Initialize the game 
void initializeGame() {
    shooterChar = 'A' + (rand() % 26); // Random alphabet for shooter 
    initializeBoard();

    // Load the dictionary (assuming a "dictionary.txt" file exists) 
    ifstream dictFile("dictionary.txt");
    string word;
    while (dictFile >> word) {
        dictionary.insert(word);
    }
    dictFile.close();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Word Shooter");
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WIDTH, 0, HEIGHT);

    initializeGame();

    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutTimerFunc(1000, timer, 0);
    glutMainLoop();

    return 0;
}

