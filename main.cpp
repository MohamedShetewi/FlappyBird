#include<stdio.h>
#include<GL/glut.h>
#include<math.h>
#include <iostream>
#include <algorithm>
#include <random>
#include </home/mohamedshetewi/CLionProjects/FlappyBird/include/irrKlang.h>


using namespace irrklang;

#pragma comment(lib, "irrKlang-64bit-1.6.0/lib/Winx64-visualStudio/irrKlang.lib")


void playerJump();

void pullPlayerDown();

void moveBarriers();

void drawPlayer(float, float);

void drawBarriers();

void stall();

void handleCollisions();

void drawPowerUps();

std::pair<int, std::pair<int, int>> barriers[9];
bool powerUp[3];
std::pair<int, int> powerUpPositions[3];


float Player_PosX = 400;
float Player_PosY = 400;
int playerRadius = 25;
int collisionsCount = 0;
int curSet = 0;
int distanceByCurSet = 0;
int powerUpRadius = 10;
bool isPowerUp = false;
int lives = 3;
long score = 0;
bool isDead = false;

/**
 * x, y is the Top left coordinates of the barrier
 * Also, we have a width = 100, and the Y always = 800
 * */
void drawBarrierUpward(int x, int h, bool isPowerUp) {

    int w = 100, y = 800;
    glBegin(GL_POLYGON);
    if (isPowerUp)
        glColor3f(0.85, 0.85, 0.85);
    else
        glColor3f(0, 0.7, 0);

    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y - h);
    glVertex2f(x, y - h);

    glEnd();

    glBegin(GL_QUADS);
    if (isPowerUp)
        glColor3f(0.6, 0.6, 0.6);
    else
        glColor3f(0, .4, 0);
    glVertex2f(x, y - h);
    glVertex2f(x + w, y - h);
    glVertex2f(x + w, y - h + 25);
    glVertex2f(x, y - h + 25);

    glEnd();

}

void drawBarrierDownward(int x, int h, bool isPowerUp) {

    int w = 100, y = 0;
    glBegin(GL_POLYGON);
    if (isPowerUp)
        glColor3f(0.85, 0.85, 0.85);
    else
        glColor3f(0, 0.7, 0);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);

    glEnd();

    glBegin(GL_QUADS);
    if (isPowerUp)
        glColor3f(0.6, 0.6, 0.6);
    else
        glColor3f(0, .4, 0);
    glVertex2f(x, y + h);
    glVertex2f(x + w, y + h);
    glVertex2f(x + w, y + h - 25);
    glVertex2f(x, y + h - 25);

    glEnd();

}


bool isCollideWithUpperBarrier(float circleX, float circleY, int circleR, int x, int y, int barrierWidth) {
    float circlePosX = circleX + circleR;
    float circlePosY = circleY + circleR;
    return circlePosX >= x && circlePosX <= x + barrierWidth && circlePosY >= 800 - y && circlePosY <= 800;
}

bool isCollideWithLowerBarrier(int circleX, int circleY, int circleR, int x, int y, int barrierWidth) {
    int circlePosX = circleX + circleR;
    int circlePosY = circleY - circleR;
    return circlePosX >= x && circlePosX <= x + barrierWidth && circlePosY <= y && circlePosY >= 0;
}

bool isPlayerTouchesPowerUp(float x, float y) {

    double dis = sqrt(pow(Player_PosX - x, 2) + pow(Player_PosY - y, 2));

    return dis <= playerRadius + powerUpRadius - 5;
}

void output(int x, int y, float r, float g, float b, std::string string) {
    glColor3f(r, g, b);
    glRasterPos2f(x, y);
    int i;

    for (i = 0; i < string.length(); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
    }
}


void display() {

    glClear(GL_COLOR_BUFFER_BIT);

    if (!isDead && curSet < 3) {
        drawPowerUps();
        drawBarriers();
        drawPlayer(Player_PosX, Player_PosY);
    }
    std::string lifeIndicator;
    lifeIndicator = "Lives: " + std::to_string(lives);
    std::string scoreIndicator = "Score: " + std::to_string(score);
    output(50, 750, 0, 0, 0, lifeIndicator);
    output(400, 750, 0, 0, 0, scoreIndicator);


    if (isDead || curSet == 3) {
        std::string end = "GAME OVER";
        output(400, 400, 0, 0, 0, end);
    }
    glFlush();
}


void Anim() {
    int idx = curSet * 3;
    float x = barriers[idx].first + 50;
    float y = (800 - barriers[idx].second.first - barriers[idx].second.second) / 2 + barriers[idx].second.second;

    if (powerUp[curSet] && isPlayerTouchesPowerUp(x, y)) {
        isPowerUp = true;
    }

    if (!isDead && curSet < 3) {
        score++;

        moveBarriers();
        drawPowerUps();
        pullPlayerDown();
        handleCollisions();
        distanceByCurSet++;

    }

    if (Player_PosY > 800 || Player_PosX > 800 || Player_PosY <= 0 || lives <= 0)
        isDead = true;

    if (distanceByCurSet >= 900) {
        curSet++;
        distanceByCurSet = 0;
        isPowerUp = false;
    }


    glutPostRedisplay();
}

void stall() {
    for (int i = 0; i < 40000000; i++);
}


void Key(unsigned char key, int x, int y) {
    if (key == ' ') {
        playerJump();
    }
    glutPostRedisplay();
}


int main(int argc, char **argr) {

    barriers[0] = std::make_pair(0, std::make_pair(200, 300));
    barriers[1] = std::make_pair(0, std::make_pair(400, 100));
    barriers[2] = std::make_pair(0, std::make_pair(250, 300));
    barriers[3] = std::make_pair(0, std::make_pair(400, 170));
    barriers[4] = std::make_pair(0, std::make_pair(300, 170));
    barriers[5] = std::make_pair(0, std::make_pair(250, 250));
    barriers[6] = std::make_pair(0, std::make_pair(200, 400));
    barriers[7] = std::make_pair(0, std::make_pair(400, 150));
    barriers[8] = std::make_pair(0, std::make_pair(350, 170));
    std::shuffle(std::begin(barriers), std::end(barriers), std::mt19937(std::random_device()()));


    for (int i = 0; i < 9; i++) {
        barriers[i].first = 610 + 300 * i;
    }

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(1, 6);

    powerUp[0] = dist6(rng) > 3;
    powerUp[1] = dist6(rng) > 3;
    powerUp[2] = dist6(rng) > 3;

    glutInit(&argc, argr);

    glutInitWindowSize(800, 800);
    glutInitWindowPosition(150, 150);

    glutCreateWindow("Robotic Arm");
    glutDisplayFunc(display);
    glutIdleFunc(Anim);
    glutKeyboardFunc(Key);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glClearColor(1, 1, 0.8, 0);
    gluOrtho2D(0, 800, 0, 800);

    glutMainLoop();
}


void pullPlayerDown() {
    for (int i = 0; i < 4000000; i++);
    Player_PosY--;
    for (int i = 0; i < 400000; i++);

}

void moveBarriers() {
    for (auto &barrier: barriers)
        barrier.first = barrier.first - 1;

}

void drawPowerUps() {

    for (int i = 0; i < 3; i++) {
        int idx = i * 3;
        if (powerUp[i]) {


            float x = barriers[idx].first + 50;
            float y =
                    (800 - barriers[idx].second.first - barriers[idx].second.second) / 2 + barriers[idx].second.second;


            if (isPowerUp && i == curSet) {
                x = 1000;
                y = 1000;
            }

            y -= 25;
            glPushMatrix();
            glColor3f(0, 0.0, 0.7);
            glTranslatef(x, y, 1);
            GLUquadric *quadObj = gluNewQuadric();
            gluDisk(quadObj, 0, powerUpRadius, 60, 50);
            glPopMatrix();

            glPushMatrix();
            glBegin(GL_LINES);
            glColor3f(1, 1, 1);
            glVertex2f(x - (powerUpRadius / 2 + 10), y);
            glVertex2f(x + (powerUpRadius / 2 + 10), y);
            glEnd();
            glPopMatrix();
        }
    }

}

void playerJump() {
    int jump = 20;
    int stall = 800000;

    for (int i = jump; i > 0; i /= 2) {
        Player_PosY += floor(i);
        for (int j = 0; j < stall; ++j);
    }

}

void drawPlayer(float x, float y) {

    if (!isDead && curSet < 3) {
        glPushMatrix();
        glColor3f(0.5, 0.0, 0);
        glTranslatef(x, y, 1);
        GLUquadric *quadObj = gluNewQuadric();
        gluDisk(quadObj, 0, playerRadius, 30, 50);
        glPopMatrix();

        glPushMatrix();
        glColor3f(1, 1, 1);

        glBegin(GL_TRIANGLES);

        glVertex2f(x + 18 + 4, y + 5);
        glVertex2f(x + 12 + 4, y + 15);
        glVertex2f(x + 7, y + 7);
        glEnd();
        glPopMatrix();

        glPushMatrix();
        glBegin(GL_LINES);
        glColor3f(1, 1, 1);
        glVertex3d(x + 23, y - 5, 0);
        glVertex3d(x + 5, y - 5, 0);
        glEnd();
        glPopMatrix();
    }
}

void drawBarriers() {

    for (int i = 0; i < 9; i++) {

        bool greyBarrier = false;
        if (isPowerUp && floor(i / 3) == curSet)
            greyBarrier = true;

        drawBarrierUpward(barriers[i].first, barriers[i].second.first, greyBarrier);
        drawBarrierDownward(barriers[i].first, barriers[i].second.second, greyBarrier);
    }
}


void handleCollisions() {

    if (!isPowerUp)
        for (auto &barrier: barriers) {
            int x_pos = barrier.first;
            int upper_y_pos = barrier.second.first;
            int lower_y_pos = barrier.second.second;

            if (isCollideWithUpperBarrier(Player_PosX, Player_PosY, playerRadius, x_pos, upper_y_pos, 100)) {
                Player_PosY = lower_y_pos + 100;
                collisionsCount++;
                stall();

                lives--;
                break;
            }

            if (isCollideWithLowerBarrier(Player_PosX, Player_PosY, playerRadius, x_pos, lower_y_pos, 100)) {
                Player_PosY = lower_y_pos + 100;
                collisionsCount++;
                stall();
                lives--;
                break;
            }
        }

}