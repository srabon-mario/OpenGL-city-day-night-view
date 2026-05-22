#include <iostream>
#include <cstdlib>
#include <cmath>
#include <windows.h>
#include <GL/gl.h>
#include <GL/glut.h>
#define PI 3.1416f


int currentScene = 1; // 1=Day, 2=Night
void drawNavButtons(); // forward declaration

//  BASICS
void drawCircleHelper(float cx, float cy, float r, int seg) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= seg; ++i) {
        float a = 2.0f * PI * i / seg;
        glVertex2f(cx + r * cosf(a), cy + r * sinf(a));
    }
    glEnd();
}

void drawRect(float x1, float y1, float x2, float y2) {
    glBegin(GL_QUADS);
    glVertex2f(x1, y1); glVertex2f(x2, y1);
    glVertex2f(x2, y2); glVertex2f(x1, y2);
    glEnd();
}

void drawTriangle(float x1,float y1,float x2,float y2,float x3,float y3){
    glBegin(GL_TRIANGLES);
    glVertex2f(x1,y1); glVertex2f(x2,y2); glVertex2f(x3,y3);
    glEnd();
}


//-------------------------------  SKY ---------------------------
void drawSky(float r, float g, float b) {
    glColor3f(r, g, b);
    drawRect(0, 50, 100, 80);
}

// CLOUDS
float cloudX1 = 10.0f, cloudX2 = 80.0f, cloudX3 = 50.0f;
float balloonY = 62.0f;   // starts low (near field), rises over time
float balloonRise = 0.0f; // phase for gentle bobbing

//------------------------------------  CLOUDS -------------------------------
void drawCloud(float x, float y, float scale, float r, float g, float b) {
    glColor3f(r, g, b);
    drawCircleHelper(x,               y,               4.0f*scale, 40);
    drawCircleHelper(x + 3.0f*scale,  y + 1.0f*scale,  4.5f*scale, 40);
    drawCircleHelper(x + 6.0f*scale,  y,               4.0f*scale, 40);
    drawCircleHelper(x + 3.0f*scale,  y - 1.5f*scale,  4.0f*scale, 40);
}

//----------------------------------- BIRDS ---------------------------
float bird1X = 15.0f, bird1Y = 72.5f;
float bird2X = 40.0f, bird2Y = 69.0f;
float bird3X = 60.0f, bird3Y = 74.0f;
float bird4X = 25.0f, bird4Y = 67.5f;
float bird5X = 75.0f, bird5Y = 71.0f;
float birdWing = 0.0f;
float auroraPhase = 0.0f;

//-----------------------------------  HORIZONTAL ROAD ------------------------------
//------------------------------------ HUMAN FIGURE -----------------------------------
void drawHuman(float x, float y, float r, float g, float b,
               float shirtR, float shirtG, float shirtB,
               float legPhase, float dir) {
    float legSwing = 0.6f * sinf(legPhase);
    float armSwing = 0.5f * sinf(legPhase);

    glColor4f(0,0,0,0.15f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    drawCircleHelper(x, y + 0.1f, 0.8f, 20);
    glDisable(GL_BLEND);

    glColor3f(0.15f, 0.15f, 0.50f);
    glLineWidth(1.5f);
    glBegin(GL_LINES);
    glVertex2f(x, y + 1.2f);
    glVertex2f(x - 0.4f * dir + legSwing * 0.4f, y);
    glVertex2f(x, y + 1.2f);
    glVertex2f(x + 0.4f * dir - legSwing * 0.4f, y);
    glEnd();

    glColor3f(shirtR, shirtG, shirtB);
    glBegin(GL_QUADS);
    glVertex2f(x - 0.5f, y + 1.2f);
    glVertex2f(x + 0.5f, y + 1.2f);
    glVertex2f(x + 0.45f, y + 2.5f);
    glVertex2f(x - 0.45f, y + 2.5f);
    glEnd();

    glColor3f(r, g, b);
    glLineWidth(1.2f);
    glBegin(GL_LINES);
    glVertex2f(x - 0.4f, y + 2.2f);
    glVertex2f(x - 0.7f - armSwing * 0.3f * dir, y + 1.5f);
    glVertex2f(x + 0.4f, y + 2.2f);
    glVertex2f(x + 0.7f + armSwing * 0.3f * dir, y + 1.5f);
    glEnd();
    glLineWidth(1.0f);

    glColor3f(r, g, b);
    drawRect(x - 0.18f, y + 2.5f, x + 0.18f, y + 2.8f);

    glColor3f(r, g, b);
    drawCircleHelper(x, y + 3.1f, 0.55f, 30);

    glColor3f(0.15f, 0.08f, 0.03f);
    drawCircleHelper(x, y + 3.4f, 0.42f, 30);

    // Front-facing eyes
    glColor3f(0.0f, 0.0f, 0.0f);
    drawCircleHelper(x - 0.18f, y + 3.15f, 0.09f, 8);
    drawCircleHelper(x + 0.18f, y + 3.15f, 0.09f, 8);
    // Eye whites
    glColor3f(1.0f, 1.0f, 1.0f);
    drawCircleHelper(x - 0.18f, y + 3.18f, 0.05f, 8);
    drawCircleHelper(x + 0.18f, y + 3.18f, 0.05f, 8);
    // Nose dot
    glColor3f(r * 0.80f, g * 0.70f, b * 0.65f);
    drawCircleHelper(x, y + 3.02f, 0.06f, 8);
    // Smile
    glColor3f(0.55f, 0.15f, 0.10f);
    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glVertex2f(x - 0.15f, y + 2.90f);
    glVertex2f(x,          y + 2.84f);
    glVertex2f(x + 0.15f, y + 2.90f);
    glEnd();
}

void drawChild(float x, float y, float r, float g, float b,
               float shirtR, float shirtG, float shirtB,
               float legPhase, float dir) {
    float legSwing = 0.7f * sinf(legPhase);

    glColor3f(0.20f, 0.20f, 0.55f);
    glLineWidth(1.2f);
    glBegin(GL_LINES);
    glVertex2f(x, y + 0.8f);
    glVertex2f(x - 0.3f * dir + legSwing * 0.3f, y);
    glVertex2f(x, y + 0.8f);
    glVertex2f(x + 0.3f * dir - legSwing * 0.3f, y);
    glEnd();

    glColor3f(shirtR, shirtG, shirtB);
    glBegin(GL_QUADS);
    glVertex2f(x - 0.35f, y + 0.8f);
    glVertex2f(x + 0.35f, y + 0.8f);
    glVertex2f(x + 0.30f, y + 1.7f);
    glVertex2f(x - 0.30f, y + 1.7f);
    glEnd();

    glColor3f(r, g, b);
    drawCircleHelper(x, y + 2.15f, 0.42f, 30);

    glColor3f(0.12f, 0.07f, 0.02f);
    drawCircleHelper(x, y + 2.38f, 0.32f, 30);

    glLineWidth(1.0f);
}

void drawWoman(float x, float y, float r, float g, float b,
               float dressR, float dressG, float dressB,
               float legPhase, float dir) {
    float legSwing = 0.4f * sinf(legPhase);

    glColor3f(r, g, b);
    glLineWidth(1.3f);
    glBegin(GL_LINES);
    glVertex2f(x - 0.2f, y + 0.8f);
    glVertex2f(x - 0.2f + legSwing * 0.2f, y);
    glVertex2f(x + 0.2f, y + 0.8f);
    glVertex2f(x + 0.2f - legSwing * 0.2f, y);
    glEnd();

    glColor3f(dressR, dressG, dressB);
    drawTriangle(x - 0.7f, y + 0.8f,
                 x + 0.7f, y + 0.8f,
                 x,        y + 0.2f);

    glColor3f(dressR * 0.85f, dressG * 0.85f, dressB * 0.85f);
    glBegin(GL_QUADS);
    glVertex2f(x - 0.42f, y + 0.8f);
    glVertex2f(x + 0.42f, y + 0.8f);
    glVertex2f(x + 0.38f, y + 2.1f);
    glVertex2f(x - 0.38f, y + 2.1f);
    glEnd();

    glColor3f(r, g, b);
    glLineWidth(1.1f);
    glBegin(GL_LINES);
    glVertex2f(x - 0.35f, y + 1.9f);
    glVertex2f(x - 0.65f, y + 1.3f);
    glVertex2f(x + 0.35f, y + 1.9f);
    glVertex2f(x + 0.65f, y + 1.3f);
    glEnd();
    glLineWidth(1.0f);

    glColor3f(r, g, b);
    drawRect(x - 0.15f, y + 2.1f, x + 0.15f, y + 2.35f);

    drawCircleHelper(x, y + 2.78f, 0.48f, 30);

    glColor3f(0.55f, 0.30f, 0.05f);
    drawCircleHelper(x, y + 3.02f, 0.40f, 30);
    drawRect(x - 0.48f, y + 2.45f, x - 0.28f, y + 2.9f);
    drawRect(x + 0.28f, y + 2.45f, x + 0.48f, y + 2.9f);

    // Front-facing eyes
    glColor3f(0.0f, 0.0f, 0.0f);
    drawCircleHelper(x - 0.15f, y + 2.82f, 0.08f, 8);
    drawCircleHelper(x + 0.15f, y + 2.82f, 0.08f, 8);
    glColor3f(1.0f, 1.0f, 1.0f);
    drawCircleHelper(x - 0.15f, y + 2.84f, 0.04f, 8);
    drawCircleHelper(x + 0.15f, y + 2.84f, 0.04f, 8);
    // Nose
    glColor3f(0.70f, 0.45f, 0.25f);
    drawCircleHelper(x, y + 2.70f, 0.05f, 8);
    // Smile
    glColor3f(0.75f, 0.20f, 0.20f);
    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glVertex2f(x - 0.12f, y + 2.58f);
    glVertex2f(x,          y + 2.53f);
    glVertex2f(x + 0.12f, y + 2.58f);
    glEnd();
}

float pedTimer = 0.0f;
void drawPedestriansDay() {
    float globalTime = pedTimer;

    // Static y positions and speeds for vertical movers
    static float pedY[8]      = {16.0f, 16.0f, 16.0f, 16.0f, 16.0f, 16.0f, 16.0f, 16.0f};
    static float pedYDir[8]   = { 1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f};
    // 1.0 = this ped moves vertically, 0.0 = moves horizontally (original)

    struct Ped { float x, speed, phase, dir; float skin[3], shirt[3]; bool is_woman, is_child; };
    Ped peds[] = {
        {8.0f,  0.3f,  0.0f,  1.0f, {0.87f,0.65f,0.45f}, {0.70f,0.35f,0.10f}, false, false},
        {22.0f, -0.25f,1.0f, -1.0f, {0.90f,0.70f,0.50f}, {0.85f,0.10f,0.15f}, true,  false},
        {30.0f, 0.35f, 0.5f,  1.0f, {0.85f,0.65f,0.45f}, {0.95f,0.85f,0.10f}, false, true },
        {45.0f, 0.15f, 2.0f,  1.0f, {0.80f,0.60f,0.40f}, {0.10f,0.20f,0.60f}, false, false},
        {58.0f, 0.28f, 1.5f,  1.0f, {0.90f,0.68f,0.48f}, {0.15f,0.65f,0.25f}, true,  false},
        {67.0f, -0.32f,0.8f, -1.0f, {0.85f,0.65f,0.45f}, {0.95f,0.50f,0.05f}, false, true },
        {80.0f, -0.22f,3.0f, -1.0f, {0.88f,0.67f,0.47f}, {0.92f,0.90f,0.88f}, false, false},
        {91.0f, -0.27f,0.3f, -1.0f, {0.87f,0.66f,0.46f}, {0.20f,0.40f,0.85f}, true,  false},
    };

    for (int i = 0; i < 8; i++) {

        float drawX, drawY;
        float legPhase = globalTime * 3.0f + peds[i].phase;

        if (pedYDir[i] == 1.0f) {
            // ---- VERTICAL mover: x is fixed, y moves up/down ----
            drawX = peds[i].x;   // x never changes

            static float ySpeed1 = 0.06f;   // speed for ped 0
            pedY[i] += ySpeed1;
            if (pedY[i] >= 19.5f) { pedY[i] = 19.5f; ySpeed1 = -fabsf(ySpeed1); }
            if (pedY[i] <= 16.0f) { pedY[i] = 16.0f; ySpeed1 =  fabsf(ySpeed1); }

            drawY = pedY[i];
        } else {
            // ---- HORIZONTAL mover: original logic ----
            drawX = peds[i].x + peds[i].speed * globalTime;
            if (drawX > 105.0f) drawX -= 110.0f;
            if (drawX < -5.0f)  drawX += 110.0f;
            drawY = 16.0f;
        }

        if (peds[i].is_child)
            drawChild(drawX, drawY,
                      peds[i].skin[0], peds[i].skin[1], peds[i].skin[2],
                      peds[i].shirt[0], peds[i].shirt[1], peds[i].shirt[2],
                      legPhase, peds[i].dir);
        else if (peds[i].is_woman)
            drawWoman(drawX, drawY,
                      peds[i].skin[0], peds[i].skin[1], peds[i].skin[2],
                      peds[i].shirt[0], peds[i].shirt[1], peds[i].shirt[2],
                      legPhase, peds[i].dir);
        else
            drawHuman(drawX, drawY,
                      peds[i].skin[0], peds[i].skin[1], peds[i].skin[2],
                      peds[i].shirt[0], peds[i].shirt[1], peds[i].shirt[2],
                      legPhase, peds[i].dir);
    }
}

//--------------------------------  Railroad -----------------------------
//---------------------------- TREES ------------------------------
void drawCircle(float cx, float cy, float r, int segments) {
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= segments; ++i) {
        float angle = 2.0f * PI * i / segments;
        glVertex2f(cx + r * cosf(angle), cy + r * sinf(angle));
    }
    glEnd();
}

void drawTree(float tx, float ty, float r, float g, float b) {
    glColor3f(0.55f, 0.27f, 0.07f);
    glBegin(GL_QUADS);
    glVertex2f(tx, ty); glVertex2f(tx+1, ty);
    glVertex2f(tx+1, ty+6); glVertex2f(tx, ty+6);
    glEnd();
    glColor3f(r, g, b);
    drawTriangle(tx-1, ty+3, tx+2, ty+3, tx+0.5f, ty+15);
    drawTriangle(tx-2, ty+4, tx+3, ty+4, tx+0.5f, ty+12);
    drawTriangle(tx-3, ty+5, tx+4, ty+5, tx+0.5f, ty+9);
}

// ===================== BIG PINE TREE (for beside buildings) =====================
void drawBigPineTree(float x, float y, float scale, bool night) {
    // Trunk
    glColor3f(night ? 0.28f : 0.42f, night ? 0.16f : 0.24f, night ? 0.08f : 0.10f);
    drawRect(x - 0.5f*scale, y, x + 0.5f*scale, y + 5.0f*scale);
    // Three layered pine tiers, bottom to top
    float tg1 = night ? 0.18f : 0.12f;
    float tg2 = night ? 0.32f : 0.52f;
    float tg3 = night ? 0.10f : 0.08f;
    // Bottom tier
    glColor3f(tg1, tg2, tg3);
    drawTriangle(x - 4.0f*scale, y + 3.5f*scale, x + 4.0f*scale, y + 3.5f*scale, x, y + 9.5f*scale);
    // Middle tier
    glColor3f(tg1*1.05f, tg2*0.95f, tg3);
    drawTriangle(x - 3.0f*scale, y + 5.8f*scale, x + 3.0f*scale, y + 5.8f*scale, x, y + 11.0f*scale);
    // Top tier
    glColor3f(tg1*1.1f, tg2*0.9f, tg3);
    drawTriangle(x - 2.0f*scale, y + 7.8f*scale, x + 2.0f*scale, y + 7.8f*scale, x, y + 12.5f*scale);
    // Snow cap on tip (small white triangle)
    if (!night) {
        glColor3f(0.96f, 0.97f, 1.0f);
        drawTriangle(x - 0.7f*scale, y + 11.0f*scale, x + 0.7f*scale, y + 11.0f*scale, x, y + 12.5f*scale);
    } else {
        // Night: faint moonlit tip
        glColor3f(0.70f, 0.76f, 0.85f);
        drawTriangle(x - 0.5f*scale, y + 11.4f*scale, x + 0.5f*scale, y + 11.4f*scale, x, y + 12.5f*scale);
    }
}

//----------------------------------------------  BUILDING -------------------------------------------
void drawWindow(double x1, double y1, double maxX, double maxY, double r, double g, double b) {
    for (int x = x1; x < maxX; x += 4) {
        for (int y = y1; y < maxY; y += 4) {
            glColor3f(r, g, b);
            glBegin(GL_QUADS);
            glVertex2f(x, y);
            glVertex2f(x + 2, y);
            glVertex2f(x + 2, y + 2);
            glVertex2f(x, y + 2);
            glEnd();
            glColor3f(r + 0.15, g + 0.20, b - 0.10);
            glBegin(GL_LINE_LOOP);
            glVertex2f(x, y);
            glVertex2f(x + 2, y);
            glVertex2f(x + 2, y + 2);
            glVertex2f(x, y + 2);
            glEnd();
        }
    }
}

//--------------------------------- CARS, BUSES, TRAINS ------------------------------------
float carX1 = -30.0f;
float carSpeed1 = 0.5f;

float carX2 = 120.0f;
float carSpeed2 = -0.4f;

float carX3 = -40.0f;
float carSpeed3 = 0.6f;

bool nightLight = false;

float ambulanceX = -20.0f;
float ambulanceSpeed = 0.7f;

// Tea stall buying animation
float teaBuyerLeg = 0.0f;
float teaBuyerX   = 12.0f;
bool  teaBuyerAtStall = false;
float teaArmSwing = 0.0f;

// Badam wala
float badamArmAngle = 0.0f;
float badamBuyerX   = 54.0f;
int   badamState    = 0;
float badamBuyerLeg = 0.0f;

// Ice cream
float iceCustomerX   = 80.0f;
int   iceState       = 0;
float iceCustomerLeg = 0.0f;
float iceArmSwing    = 0.0f;

void drawAmbulance() {
    glPushMatrix();
    glTranslatef(ambulanceX, 36.0f, 0.0f);
    glScalef(0.9f, 0.9f, 1.0f);
    // Body
    glColor3f(0.96f, 0.96f, 0.96f);
    drawRect(0, 0, 9, 4);
    // Red stripe
    glColor3f(0.90f, 0.10f, 0.10f);
    drawRect(0, 1.6f, 9, 2.4f);
    // Cross symbol
    glColor3f(0.95f, 0.95f, 0.95f);
    drawRect(3.8f, 1.0f, 5.2f, 3.0f);
    drawRect(2.8f, 1.7f, 6.2f, 2.3f);
    // Cab section
    glColor3f(0.88f, 0.88f, 0.88f);
    drawRect(7.0f, 0, 9, 4);
    // Windshield
    glColor3f(0.55f, 0.80f, 0.95f);
    drawRect(7.3f, 1.5f, 8.8f, 3.6f);
    // Wheels
    glColor3f(0.15f, 0.15f, 0.15f);
    drawCircleHelper(1.5f, 0, 0.9f, 16);
    drawCircleHelper(7.0f, 0, 0.9f, 16);
    glColor3f(0.55f, 0.55f, 0.55f);
    drawCircleHelper(1.5f, 0, 0.45f, 12);
    drawCircleHelper(7.0f, 0, 0.45f, 12);
    // Siren light on roof
    glColor3f(0.95f, 0.05f, 0.05f);
    drawRect(3.5f, 4.0f, 5.5f, 4.5f);
    glColor3f(0.10f, 0.45f, 0.95f);
    drawRect(5.5f, 4.0f, 6.5f, 4.5f);
    glPopMatrix();
}

float trainX = 240.0f;
float trainX2 = -20.0f;


float sunAngle = 0.0f;
float billRotX =  22.0f;
float billRotY = -30.0f;
float billZoom =  7.5f;

void drawSun(float cx, float cy, float radius) {
    glColor3f(1.0f, 0.9f, 0.2f);
    for (int i = 0; i < 12; i++) {
        float a = sunAngle + i * (360.0f / 12) * PI / 180.0f;
        float x1 = cx + (radius + 1.0f) * cosf(a);
        float y1 = cy + (radius + 1.0f) * sinf(a);
        float x2 = cx + (radius + 3.5f) * cosf(a);
        float y2 = cy + (radius + 3.5f) * sinf(a);
        glLineWidth(2.0f);
        glBegin(GL_LINES);
        glVertex2f(x1, y1); glVertex2f(x2, y2);
        glEnd();
    }
    glLineWidth(1.0f);
    glColor3f(1.0f, 0.85f, 0.0f);
    drawCircleHelper(cx, cy, radius, 60);
    glColor3f(1.0f, 0.95f, 0.5f);
    drawCircleHelper(cx, cy, radius * 0.6f, 60);
}

void drawSmallBird(float bx, float by, float wingPhase) {
    glColor3f(0.05f, 0.02f, 0.08f);
    glLineWidth(1.2f);
    float flap = 0.8f * sinf(wingPhase);
    glBegin(GL_LINE_STRIP);
    glVertex2f(bx - 2.0f, by + flap);
    glVertex2f(bx,        by + 0.25f);
    glVertex2f(bx + 2.0f, by + flap);
    glEnd();
    drawCircleHelper(bx, by + 0.25f, 0.15f, 8);
    glLineWidth(1.0f);
}

void drawStrokeChar(char ch, float ox, float oy, float ls) {
    glBegin(GL_LINES);
    if (ch=='A'||ch=='a') {
        glVertex2f(ox,oy); glVertex2f(ox+ls*0.5f,oy+ls*1.5f);
        glVertex2f(ox+ls*0.5f,oy+ls*1.5f); glVertex2f(ox+ls,oy);
        glVertex2f(ox+ls*0.15f,oy+ls*0.7f); glVertex2f(ox+ls*0.85f,oy+ls*0.7f);
    } else if (ch=='M'||ch=='m') {
        glVertex2f(ox,oy); glVertex2f(ox,oy+ls*1.5f);
        glVertex2f(ox,oy+ls*1.5f); glVertex2f(ox+ls*0.5f,oy+ls*0.7f);
        glVertex2f(ox+ls*0.5f,oy+ls*0.7f); glVertex2f(ox+ls,oy+ls*1.5f);
        glVertex2f(ox+ls,oy+ls*1.5f); glVertex2f(ox+ls,oy);
    } else if (ch=='R'||ch=='r') {
        glVertex2f(ox,oy); glVertex2f(ox,oy+ls*1.5f);
        glVertex2f(ox,oy+ls*1.5f); glVertex2f(ox+ls,oy+ls*1.5f);
        glVertex2f(ox+ls,oy+ls*1.5f); glVertex2f(ox+ls,oy+ls*0.8f);
        glVertex2f(ox+ls,oy+ls*0.8f); glVertex2f(ox,oy+ls*0.8f);
        glVertex2f(ox,oy+ls*0.8f); glVertex2f(ox+ls,oy);
    } else if (ch=='D'||ch=='d') {
        glVertex2f(ox,oy); glVertex2f(ox,oy+ls*1.5f);
        glVertex2f(ox,oy+ls*1.5f); glVertex2f(ox+ls*0.7f,oy+ls*1.5f);
        glVertex2f(ox+ls*0.7f,oy+ls*1.5f); glVertex2f(ox+ls,oy+ls*0.75f);
        glVertex2f(ox+ls,oy+ls*0.75f); glVertex2f(ox+ls*0.7f,oy);
        glVertex2f(ox+ls*0.7f,oy); glVertex2f(ox,oy);
    } else if (ch=='E'||ch=='e') {
        glVertex2f(ox+ls,oy); glVertex2f(ox,oy);
        glVertex2f(ox,oy); glVertex2f(ox,oy+ls*1.5f);
        glVertex2f(ox,oy+ls*1.5f); glVertex2f(ox+ls,oy+ls*1.5f);
        glVertex2f(ox,oy+ls*0.75f); glVertex2f(ox+ls*0.8f,oy+ls*0.75f);
    } else if (ch=='T'||ch=='t') {
        glVertex2f(ox,oy+ls*1.5f); glVertex2f(ox+ls,oy+ls*1.5f);
        glVertex2f(ox+ls*0.5f,oy+ls*1.5f); glVertex2f(ox+ls*0.5f,oy);
    } else if (ch=='S'||ch=='s') {
        glVertex2f(ox+ls,oy+ls*1.5f); glVertex2f(ox,oy+ls*1.5f);
        glVertex2f(ox,oy+ls*1.5f); glVertex2f(ox,oy+ls*0.8f);
        glVertex2f(ox,oy+ls*0.8f); glVertex2f(ox+ls,oy+ls*0.8f);
        glVertex2f(ox+ls,oy+ls*0.8f); glVertex2f(ox+ls,oy);
        glVertex2f(ox+ls,oy); glVertex2f(ox,oy);
    } else if (ch=='L'||ch=='l') {
        glVertex2f(ox,oy+ls*1.5f); glVertex2f(ox,oy);
        glVertex2f(ox,oy); glVertex2f(ox+ls,oy);
    } else if (ch=='O'||ch=='o') {
        glVertex2f(ox,oy); glVertex2f(ox+ls,oy);
        glVertex2f(ox+ls,oy); glVertex2f(ox+ls,oy+ls*1.5f);
        glVertex2f(ox+ls,oy+ls*1.5f); glVertex2f(ox,oy+ls*1.5f);
        glVertex2f(ox,oy+ls*1.5f); glVertex2f(ox,oy);
    } else if (ch=='W'||ch=='w') {
        glVertex2f(ox,oy+ls*1.5f); glVertex2f(ox+ls*0.25f,oy);
        glVertex2f(ox+ls*0.25f,oy); glVertex2f(ox+ls*0.5f,oy+ls*0.7f);
        glVertex2f(ox+ls*0.5f,oy+ls*0.7f); glVertex2f(ox+ls*0.75f,oy);
        glVertex2f(ox+ls*0.75f,oy); glVertex2f(ox+ls,oy+ls*1.5f);
    } else if (ch=='I'||ch=='i') {
        glVertex2f(ox+ls*0.5f,oy); glVertex2f(ox+ls*0.5f,oy+ls*1.5f);
    } else if (ch=='N'||ch=='n') {
        glVertex2f(ox,oy); glVertex2f(ox,oy+ls*1.5f);
        glVertex2f(ox,oy+ls*1.5f); glVertex2f(ox+ls,oy);
        glVertex2f(ox+ls,oy); glVertex2f(ox+ls,oy+ls*1.5f);
    } else if (ch=='C'||ch=='c') {
        glVertex2f(ox+ls,oy+ls*1.5f); glVertex2f(ox,oy+ls*1.5f);
        glVertex2f(ox,oy+ls*1.5f); glVertex2f(ox,oy);
        glVertex2f(ox,oy); glVertex2f(ox+ls,oy);
    } else if (ch=='H'||ch=='h') {
        glVertex2f(ox,oy); glVertex2f(ox,oy+ls*1.5f);
        glVertex2f(ox+ls,oy); glVertex2f(ox+ls,oy+ls*1.5f);
        glVertex2f(ox,oy+ls*0.75f); glVertex2f(ox+ls,oy+ls*0.75f);
    } else if (ch=='P'||ch=='p') {
        glVertex2f(ox,oy); glVertex2f(ox,oy+ls*1.5f);
        glVertex2f(ox,oy+ls*1.5f); glVertex2f(ox+ls,oy+ls*1.5f);
        glVertex2f(ox+ls,oy+ls*1.5f); glVertex2f(ox+ls,oy+ls*0.8f);
        glVertex2f(ox+ls,oy+ls*0.8f); glVertex2f(ox,oy+ls*0.8f);
    } else if (ch=='K'||ch=='k') {
        glVertex2f(ox,oy); glVertex2f(ox,oy+ls*1.5f);
        glVertex2f(ox,oy+ls*0.75f); glVertex2f(ox+ls,oy+ls*1.5f);
        glVertex2f(ox,oy+ls*0.75f); glVertex2f(ox+ls,oy);
    } else if (ch=='G'||ch=='g') {
        glVertex2f(ox+ls,oy+ls*1.5f); glVertex2f(ox,oy+ls*1.5f);
        glVertex2f(ox,oy+ls*1.5f); glVertex2f(ox,oy);
        glVertex2f(ox,oy); glVertex2f(ox+ls,oy);
        glVertex2f(ox+ls,oy); glVertex2f(ox+ls,oy+ls*0.75f);
        glVertex2f(ox+ls,oy+ls*0.75f); glVertex2f(ox+ls*0.5f,oy+ls*0.75f);
    } else if (ch=='U'||ch=='u') {
        glVertex2f(ox,oy+ls*1.5f); glVertex2f(ox,oy);
        glVertex2f(ox,oy); glVertex2f(ox+ls,oy);
        glVertex2f(ox+ls,oy); glVertex2f(ox+ls,oy+ls*1.5f);
    } else if (ch=='B'||ch=='b') {
        glVertex2f(ox,oy); glVertex2f(ox,oy+ls*1.5f);
        glVertex2f(ox,oy+ls*1.5f); glVertex2f(ox+ls*0.8f,oy+ls*1.5f);
        glVertex2f(ox+ls*0.8f,oy+ls*1.5f); glVertex2f(ox+ls,oy+ls*1.1f);
        glVertex2f(ox+ls,oy+ls*1.1f); glVertex2f(ox+ls*0.8f,oy+ls*0.75f);
        glVertex2f(ox+ls*0.8f,oy+ls*0.75f); glVertex2f(ox,oy+ls*0.75f);
        glVertex2f(ox,oy+ls*0.75f); glVertex2f(ox+ls*0.8f,oy+ls*0.75f);
        glVertex2f(ox+ls*0.8f,oy+ls*0.75f); glVertex2f(ox+ls,oy+ls*0.35f);
        glVertex2f(ox+ls,oy+ls*0.35f); glVertex2f(ox+ls*0.8f,oy);
        glVertex2f(ox+ls*0.8f,oy); glVertex2f(ox,oy);
    } else if (ch=='F'||ch=='f') {
        glVertex2f(ox,oy); glVertex2f(ox,oy+ls*1.5f);
        glVertex2f(ox,oy+ls*1.5f); glVertex2f(ox+ls,oy+ls*1.5f);
        glVertex2f(ox,oy+ls*0.75f); glVertex2f(ox+ls*0.8f,oy+ls*0.75f);
    } else if (ch=='V'||ch=='v') {
        glVertex2f(ox,oy+ls*1.5f); glVertex2f(ox+ls*0.5f,oy);
        glVertex2f(ox+ls*0.5f,oy); glVertex2f(ox+ls,oy+ls*1.5f);
    } else if (ch=='X'||ch=='x') {
        glVertex2f(ox,oy); glVertex2f(ox+ls,oy+ls*1.5f);
        glVertex2f(ox+ls,oy); glVertex2f(ox,oy+ls*1.5f);
    } else if (ch=='Y'||ch=='y') {
        glVertex2f(ox,oy+ls*1.5f); glVertex2f(ox+ls*0.5f,oy+ls*0.75f);
        glVertex2f(ox+ls,oy+ls*1.5f); glVertex2f(ox+ls*0.5f,oy+ls*0.75f);
        glVertex2f(ox+ls*0.5f,oy+ls*0.75f); glVertex2f(ox+ls*0.5f,oy);
    } else if (ch=='Z'||ch=='z') {
        glVertex2f(ox,oy+ls*1.5f); glVertex2f(ox+ls,oy+ls*1.5f);
        glVertex2f(ox+ls,oy+ls*1.5f); glVertex2f(ox,oy);
        glVertex2f(ox,oy); glVertex2f(ox+ls,oy);
    }
    glEnd();
}

void drawSignText(const char* name, float signX1, float signX2, float signY,
                  float signH, float r, float g, float b) {
    int total = 0;
    for (int i = 0; name[i]; i++) total++;

    float avail  = signX2 - signX1 - 0.4f;
    float charW  = 1.7f;
    float ls = avail / (total * charW);
    if (ls > signH * 0.55f) ls = signH * 0.55f;

    float advance = ls * charW;
    float totalW  = total * advance;
    float startX  = signX1 + (avail - totalW) * 0.5f + 0.2f;
    float startY  = signY + (signH - ls * 1.5f) * 0.5f;

    glColor3f(r, g, b);
    glLineWidth(1.3f);
    for (int ci = 0; name[ci]; ci++) {
        float ox = startX + ci * advance;
        if (ox + ls > signX2) break;
        drawStrokeChar(name[ci], ox, startY, ls);
    }
    glLineWidth(1.0f);
}

void drawNamedTeaStall(float cx, float cy,
                       float canR, float canG, float canB,
                       const char* name) {
    float cw = 7.0f, ch2 = 3.0f;
    float cr = 0.6f;
    glColor3f(0.55f, 0.35f, 0.15f);
    glBegin(GL_POLYGON);
    int seg = 8;
    for (int i = seg; i <= 2*seg; i++) {
        float a = PI * i / (2*seg);
        glVertex2f(cx+cr + cr*cosf(a), cy+cr + cr*sinf(a));
    }
    for (int i = 0; i <= seg; i++) {
        float a = -PI*0.5f + PI * i / (2*seg) * (-1);
        glVertex2f(cx+cw-cr + cr*cosf(a+PI*1.5f), cy+cr + cr*sinf(a+PI*1.5f));
    }
    glVertex2f(cx+cw, cy+cr);
    glVertex2f(cx+cw, cy+ch2);
    glVertex2f(cx,    cy+ch2);
    glVertex2f(cx,    cy+cr);
    glEnd();
    glColor3f(0.55f, 0.35f, 0.15f);
    drawRect(cx, cy+cr, cx+cw, cy+ch2);
    drawRect(cx+cr, cy, cx+cw-cr, cy+ch2);

    glColor3f(0.38f, 0.22f, 0.08f);
    glBegin(GL_QUADS);
    glVertex2f(cx,      cy+cr);
    glVertex2f(cx-0.5f, cy+cr-0.5f);
    glVertex2f(cx-0.5f, cy+ch2-0.3f);
    glVertex2f(cx,      cy+ch2);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(cx+cw,      cy+cr);
    glVertex2f(cx+cw+0.5f, cy+cr-0.5f);
    glVertex2f(cx+cw+0.5f, cy+ch2-0.3f);
    glVertex2f(cx+cw,      cy+ch2);
    glEnd();

    glColor3f(0.80f, 0.60f, 0.30f);
    drawRect(cx - 0.2f, cy + ch2, cx + cw + 0.2f, cy + ch2 + 0.55f);
    glColor3f(0.92f, 0.75f, 0.45f);
    drawRect(cx + 0.3f, cy + ch2 + 0.4f, cx + cw - 0.3f, cy + ch2 + 0.55f);

    glColor3f(0.40f, 0.22f, 0.08f);
    drawRect(cx + 0.4f, cy - 1.5f, cx + 1.0f, cy + cr);
    drawRect(cx+cw-1.0f, cy-1.5f, cx+cw-0.4f, cy+cr);
    glColor3f(0.28f, 0.14f, 0.04f);
    drawRect(cx + 0.9f, cy - 1.5f, cx + 1.1f, cy + cr);
    drawRect(cx+cw-1.1f, cy-1.5f, cx+cw-0.9f, cy+cr);

    glColor3f(0.30f, 0.18f, 0.06f);
    drawRect(cx + cw*0.5f - 0.3f, cy + ch2 + 0.55f, cx + cw*0.5f + 0.3f, cy + 7.2f);
    glColor3f(0.55f, 0.35f, 0.15f);
    drawRect(cx + cw*0.5f - 0.1f, cy + ch2 + 0.55f, cx + cw*0.5f, cy + 7.2f);

    float cpx = cx + cw*0.5f, cpy = cy + 7.2f;
    float cpw = 5.2f, cph = 1.2f;
    glColor3f(canR, canG, canB);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cpx, cpy + cph);
    for (int i = 0; i <= 20; i++) {
        float a = PI * i / 20.0f;
        glVertex2f(cpx + cpw * cosf(a), cpy + cph * sinf(a));
    }
    glEnd();
    glColor3f(canR*0.75f, canG*0.75f, canB*0.75f);
    drawRect(cpx - cpw, cpy - 0.15f, cpx + cpw, cpy + 0.15f);
    glColor3f(canR * 0.72f, canG * 0.72f, canB * 0.72f);
    for (float fx = cpx - cpw; fx < cpx + cpw - 0.8f; fx += 1.1f) {
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(fx + 0.55f, cpy - 0.8f);
        for (int i = 0; i <= 10; i++) {
            float a = PI + PI * i / 10.0f;
            glVertex2f(fx + 0.55f + 0.5f*cosf(a), cpy - 0.1f + 0.5f*sinf(a));
        }
        glEnd();
    }

    float sx1 = cx + 0.4f, sx2 = cx + cw - 0.4f;
    float sy1 = cy + ch2 + 0.6f, sy2 = cy + ch2 + 2.2f;
    glColor3f(1.0f, 0.97f, 0.82f);
    drawRect(sx1, sy1, sx2, sy2);
    glColor3f(canR*0.6f, canG*0.3f, 0.08f);
    glLineWidth(1.2f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(sx1, sy1); glVertex2f(sx2, sy1);
    glVertex2f(sx2, sy2); glVertex2f(sx1, sy2);
    glEnd();
    glLineWidth(1.0f);

    glColor3f(canR*0.5f + 0.1f, canG*0.15f + 0.05f, 0.10f);
    drawSignText(name, sx1 + 0.1f, sx2 - 0.1f, sy1 + 0.05f, (sy2-sy1-0.1f),
                 canR*0.5f+0.1f, canG*0.15f+0.05f, 0.10f);

    for (int ci = 0; ci < 3; ci++) {
        float cupx = cx + 1.0f + ci * 1.8f;
        float cupy = cy + ch2 + 0.55f;
        glColor3f(0.88f, 0.84f, 0.78f);
        drawRect(cupx, cupy, cupx + 0.7f, cupy + 0.55f);
        glColor3f(0.75f, 0.70f, 0.65f);
        drawCircleHelper(cupx + 0.35f, cupy, 0.35f, 12);
        glColor3f(0.60f, 0.55f, 0.50f);
        glLineWidth(1.1f);
        glBegin(GL_LINE_STRIP);
        glVertex2f(cupx+0.7f, cupy+0.4f);
        glVertex2f(cupx+1.0f, cupy+0.3f);
        glVertex2f(cupx+0.7f, cupy+0.15f);
        glEnd();
        glLineWidth(1.0f);
    }
    glColor3f(0.88f, 0.88f, 0.88f);
    for (int ci = 0; ci < 3; ci++) {
        float sx0 = cx + 1.35f + ci * 1.8f;
        glBegin(GL_LINE_STRIP);
        glVertex2f(sx0, cy+ch2+1.15f);
        glVertex2f(sx0+0.18f, cy+ch2+1.55f);
        glVertex2f(sx0-0.15f, cy+ch2+1.90f);
        glEnd();
    }

    float selx = cx + cw*0.5f, sely = cy + ch2 + 0.55f;
    glColor3f(0.78f, 0.58f, 0.38f);
    drawCircleHelper(selx, sely + 1.6f, 0.52f, 24);
    glColor3f(0.15f, 0.09f, 0.03f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(selx, sely+1.85f);
    for (int i=0;i<=16;i++){float a=PI*i/16.0f; glVertex2f(selx+0.52f*cosf(a),sely+1.6f+0.52f*sinf(a));}
    glEnd();
    glColor3f(0.15f, 0.45f, 0.75f);
    drawRect(selx-0.42f, sely+0.2f, selx+0.42f, sely+1.1f);
    glColor3f(0.78f, 0.58f, 0.38f);
    glLineWidth(1.4f);
    glBegin(GL_LINES);
    glVertex2f(selx-0.42f, sely+0.9f); glVertex2f(selx-1.8f, sely+0.5f);
    glVertex2f(selx+0.42f, sely+0.9f); glVertex2f(selx+1.8f, sely+0.5f);
    glEnd();
    glLineWidth(1.0f);
}

void drawAnaIceCreamStall(float cx, float cy, float sellerArm = 0.0f) {
    float cw = 6.5f, ch3 = 3.8f, cr2 = 0.9f;

    glColor3f(0.30f, 0.35f, 0.25f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx+cw*0.5f, cy-0.15f);
    for (int i=0;i<=20;i++){
        float a=2*PI*i/20.0f;
        glVertex2f(cx+cw*0.5f+cw*0.52f*cosf(a), cy-0.15f+0.4f*sinf(a));
    }
    glEnd();

    glColor3f(0.60f, 0.85f, 1.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx+cw*0.5f, cy+ch3+0.5f);
    for (int i=0;i<=24;i++){
        float a=PI*i/24.0f;
        glVertex2f(cx+cw*0.5f+cw*0.5f*cosf(a), cy+ch3+0.5f*sinf(a));
    }
    glEnd();

    glColor3f(0.88f, 0.94f, 1.0f);
    drawRect(cx, cy, cx+cw, cy+ch3);
    glColor3f(0.88f, 0.94f, 1.0f);
    drawCircleHelper(cx+cr2, cy+cr2, cr2, 16);
    drawCircleHelper(cx+cw-cr2, cy+cr2, cr2, 16);
    glColor3f(0.45f, 0.60f, 0.25f);
    drawRect(cx, cy, cx+cr2, cy+cr2);
    drawRect(cx+cw-cr2, cy, cx+cw, cy+cr2);

    glColor3f(0.62f, 0.72f, 0.82f);
    glBegin(GL_QUADS);
    glVertex2f(cx+cw,      cy);
    glVertex2f(cx+cw+0.7f, cy+0.5f);
    glVertex2f(cx+cw+0.7f, cy+ch3+0.3f);
    glVertex2f(cx+cw,      cy+ch3);
    glEnd();
    glColor3f(0.50f, 0.65f, 0.80f);
    glBegin(GL_QUADS);
    glVertex2f(cx+cw,      cy+ch3);
    glVertex2f(cx+cw+0.7f, cy+ch3+0.3f);
    glVertex2f(cx+cw*0.5f+cw*0.5f+0.35f, cy+ch3+0.5f+0.15f);
    glVertex2f(cx+cw*0.5f+cw*0.5f, cy+ch3+0.5f);
    glEnd();

    glColor3f(0.45f, 0.65f, 0.88f);
    glLineWidth(1.3f);
    glBegin(GL_LINE_STRIP);
    glVertex2f(cx+0.4f, cy+ch3*0.55f);
    glVertex2f(cx+cw-0.4f, cy+ch3*0.55f);
    glEnd();
    glLineWidth(1.0f);

    float wy = cy + 0.1f;
    for (int wi = 0; wi < 2; wi++) {
        float wx = (wi==0) ? cx+0.9f : cx+cw-0.9f;
        glColor3f(0.18f, 0.18f, 0.18f);
        drawCircleHelper(wx, wy, 0.72f, 20);
        glColor3f(0.65f, 0.65f, 0.68f);
        drawCircleHelper(wx, wy, 0.52f, 20);
        glColor3f(0.90f, 0.90f, 0.92f);
        drawCircleHelper(wx, wy, 0.22f, 16);
        glColor3f(0.50f, 0.50f, 0.52f);
        glLineWidth(1.1f);
        for (int sp=0;sp<4;sp++){
            float sa=sp*PI*0.5f;
            glBegin(GL_LINES);
            glVertex2f(wx+0.22f*cosf(sa), wy+0.22f*sinf(sa));
            glVertex2f(wx+0.52f*cosf(sa), wy+0.52f*sinf(sa));
            glEnd();
        }
        glLineWidth(1.0f);
    }

    glColor3f(0.30f, 0.30f, 0.32f);
    drawRect(cx + cw*0.5f - 0.32f, cy + ch3 + 0.5f, cx + cw*0.5f + 0.32f, cy + 9.2f);
    glColor3f(0.55f, 0.55f, 0.58f);
    drawRect(cx + cw*0.5f - 0.10f, cy + ch3 + 0.5f, cx + cw*0.5f + 0.10f, cy + 9.2f);

    float upx = cx + cw*0.5f, upy = cy + 9.2f, uw = 5.2f, uh = 1.8f;
    int seg2 = 10;
    for (int s=0; s<seg2; s++){
        float a1 = PI + s * PI / seg2;
        float a2 = PI + (s+1) * PI / seg2;
        if (s%2==0) glColor3f(1.0f, 0.50f, 0.72f);
        else        glColor3f(1.0f, 0.95f, 0.97f);
        glBegin(GL_TRIANGLES);
        glVertex2f(upx, upy);
        glVertex2f(upx + uw*cosf(a1), upy + uh*sinf(a1));
        glVertex2f(upx + uw*cosf(a2), upy + uh*sinf(a2));
        glEnd();
    }
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    for (int i=0;i<=20;i++){float a=PI+i*PI/20.0f; glVertex2f(upx+uw*cosf(a),upy+uh*sinf(a));}
    glEnd();
    glColor3f(1.0f, 0.28f, 0.52f);
    for (int s=0;s<=seg2;s++){
        float a = PI + s * PI / seg2;
        float fx = upx + uw * cosf(a);
        float fy = upy + uh * sinf(a);
        drawCircleHelper(fx, fy - 0.28f, 0.22f, 12);
    }

    float sgx1 = cx + 0.4f, sgx2 = cx + cw - 0.4f;
    float sgy1 = cy + ch3*0.58f + 0.05f, sgy2 = cy + ch3 - 0.1f;
    glColor3f(1.0f, 0.97f, 0.94f);
    drawRect(sgx1, sgy1, sgx2, sgy2);
    glColor3f(0.60f, 0.75f, 0.90f);
    glLineWidth(1.2f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(sgx1,sgy1); glVertex2f(sgx2,sgy1);
    glVertex2f(sgx2,sgy2); glVertex2f(sgx1,sgy2);
    glEnd();
    glLineWidth(1.0f);
    drawSignText("ANA ICECREAM", sgx1+0.1f, sgx2-0.1f, sgy1+0.05f, (sgy2-sgy1-0.1f),
                 0.88f, 0.08f, 0.40f);

    float sgx1b = cx+0.5f, sgx2b = cx+cw-0.5f;
    float sgy1b = cy+0.5f, sgy2b = cy+ch3*0.55f;
    glColor3f(1.0f, 0.92f, 0.96f);
    drawRect(sgx1b, sgy1b, sgx2b, sgy2b);
    drawSignText("ANA", sgx1b+0.15f, sgx2b-0.15f, sgy1b+0.1f, (sgy2b-sgy1b-0.2f),
                 1.0f, 0.10f, 0.45f);

    float mx = cx - 1.8f, my = cy + 0.5f;
    glColor3f(0.35f, 0.40f, 0.28f);
    drawCircleHelper(mx, my + 0.05f, 0.65f, 16);
    glColor3f(0.12f, 0.12f, 0.42f);
    glLineWidth(1.6f);
    glBegin(GL_LINES);
    glVertex2f(mx, my+1.2f); glVertex2f(mx-0.35f, my);
    glVertex2f(mx, my+1.2f); glVertex2f(mx+0.35f, my);
    glEnd();
    glLineWidth(1.0f);
    glColor3f(0.15f, 0.52f, 0.22f);
    glBegin(GL_QUADS);
    glVertex2f(mx-0.44f, my+1.2f); glVertex2f(mx+0.44f, my+1.2f);
    glVertex2f(mx+0.44f, my+2.6f); glVertex2f(mx-0.44f, my+2.6f);
    glEnd();
    glColor3f(0.78f, 0.58f, 0.38f);
    glLineWidth(1.4f);
    glBegin(GL_LINES);
    glVertex2f(mx+0.44f, my+2.2f); glVertex2f(mx+1.1f, my+1.8f);
    glEnd();
    float armReach = 0.8f * sinf(sellerArm);
    glBegin(GL_LINES);
    glVertex2f(mx-0.44f, my+2.2f);
    glVertex2f(mx - 1.0f - armReach, my + 1.8f + 0.3f * sinf(sellerArm));
    glEnd();
    glLineWidth(1.0f);
    glColor3f(0.78f, 0.58f, 0.38f);
    drawRect(mx-0.16f, my+2.6f, mx+0.16f, my+2.85f);
    drawCircleHelper(mx, my+3.22f, 0.52f, 24);
    glColor3f(0.12f, 0.08f, 0.03f);
    glBegin(GL_TRIANGLE_FAN); glVertex2f(mx, my+3.45f);
    for(int i=0;i<=16;i++){float a=PI*i/16.0f; glVertex2f(mx+0.52f*cosf(a),my+3.22f+0.52f*sinf(a));}
    glEnd();
    if (sellerArm < PI && sinf(sellerArm) > 0.1f) {
        float hx = mx - 1.0f - armReach, hy = my + 1.8f + 0.3f*sinf(sellerArm);
        glColor3f(1.0f, 0.85f, 0.55f);
        drawTriangle(hx-0.25f, hy+0.35f, hx+0.25f, hy+0.35f, hx, hy-0.2f);
        glColor3f(1.0f, 0.55f, 0.75f);
        drawCircleHelper(hx, hy+0.58f, 0.32f, 18);
    }
}

void drawBadamwala(float cx, float cy) {
    float sx = cx, sy = cy;

    glColor3f(0.35f, 0.58f, 0.20f);
    drawRect(sx-0.8f, sy, sx+0.8f, sy+0.15f);

    glColor3f(0.10f, 0.50f, 0.18f);
    drawRect(sx-0.55f, sy, sx+0.55f, sy+0.50f);
    glColor3f(0.08f, 0.38f, 0.13f);
    for(float lx2=sx-0.5f; lx2<sx+0.5f; lx2+=0.18f) drawRect(lx2, sy, lx2+0.08f, sy+0.50f);
    glColor3f(0.78f, 0.58f, 0.38f);
    glLineWidth(1.5f);
    glBegin(GL_LINES);
    glVertex2f(sx-0.55f, sy+0.20f); glVertex2f(sx-1.0f, sy+0.05f);
    glVertex2f(sx+0.55f, sy+0.20f); glVertex2f(sx+1.0f, sy+0.05f);
    glEnd();
    glLineWidth(1.0f);

    glColor3f(0.95f, 0.92f, 0.85f);
    drawRect(sx-0.45f, sy+0.50f, sx+0.45f, sy+1.80f);

    glColor3f(0.78f, 0.58f, 0.38f);
    glLineWidth(2.5f);
    float scoop = 0.9f * sinf(badamArmAngle);
    float dip   = 0.6f * (sinf(badamArmAngle) - 1.0f);
    glBegin(GL_LINES);
    glVertex2f(sx+0.45f, sy+1.40f);
    glVertex2f(sx+0.30f+scoop*0.8f, sy+0.55f+dip);
    glEnd();
    glBegin(GL_LINES);
    glVertex2f(sx-0.45f, sy+1.40f);
    glVertex2f(sx-1.20f - scoop*0.5f, sy+1.00f + 0.3f*sinf(badamArmAngle+PI));
    glEnd();
    glLineWidth(1.0f);
    glColor3f(0.78f, 0.58f, 0.38f);
    drawCircleHelper(sx+0.30f+scoop*0.8f, sy+0.55f+dip, 0.18f, 8);

    glColor3f(0.78f, 0.58f, 0.38f);
    drawRect(sx-0.15f, sy+1.80f, sx+0.15f, sy+2.05f);
    drawCircleHelper(sx, sy+2.48f, 0.50f, 22);
    glColor3f(0.12f, 0.08f, 0.02f);
    glBegin(GL_TRIANGLE_FAN); glVertex2f(sx, sy+2.70f);
    for(int i=0;i<=16;i++){float a=PI*i/16.0f; glVertex2f(sx+0.50f*cosf(a), sy+2.48f+0.50f*sinf(a));}
    glEnd();
    glColor3f(0.96f, 0.94f, 0.90f);
    drawRect(sx-0.52f, sy+2.70f, sx+0.52f, sy+2.95f);

    float bx = sx, by = cy - 1.8f;
    glColor3f(0.65f, 0.48f, 0.22f);
    glBegin(GL_TRIANGLE_FAN); glVertex2f(bx, by+0.55f);
    for(int i=0;i<=24;i++){float a=2*PI*i/24.0f; glVertex2f(bx+1.4f*cosf(a), by+0.55f+0.62f*sinf(a));}
    glEnd();
    glColor3f(0.50f, 0.35f, 0.14f);
    glLineWidth(0.8f);
    for(int i=0;i<8;i++){
        float a=i*PI/8.0f;
        glBegin(GL_LINES);
        glVertex2f(bx, by+0.55f);
        glVertex2f(bx+1.4f*cosf(a), by+0.55f+0.62f*sinf(a));
        glEnd();
    }
    glLineWidth(1.0f);
    glColor3f(0.80f, 0.60f, 0.28f);
    glLineWidth(1.5f);
    glBegin(GL_LINE_STRIP);
    for(int i=0;i<=24;i++){float a=PI*i/24.0f; glVertex2f(bx+1.4f*cosf(a), by+0.55f+0.62f*sinf(a));}
    glEnd();
    glLineWidth(1.0f);
    glColor3f(0.82f, 0.65f, 0.30f);
    drawCircleHelper(bx-0.7f, by+0.85f, 0.17f, 8);
    drawCircleHelper(bx-0.3f, by+1.00f, 0.19f, 8);
    drawCircleHelper(bx+0.1f, by+0.90f, 0.18f, 8);
    drawCircleHelper(bx+0.5f, by+0.95f, 0.17f, 8);
    drawCircleHelper(bx+0.9f, by+0.80f, 0.16f, 8);
    drawCircleHelper(bx-0.5f, by+0.73f, 0.15f, 8);
    drawCircleHelper(bx+0.3f, by+0.70f, 0.15f, 8);

    float frontY = cy - 3.5f;
    drawChild(cx + 0.5f, frontY, 0.87f,0.65f,0.45f, 0.85f,0.20f,0.20f, 0.0f, 1.0f);
    drawWoman(cx + 2.2f, frontY, 0.88f,0.66f,0.46f, 0.80f,0.10f,0.45f, 0.0f,-1.0f);
    drawChild(cx + 4.0f, frontY, 0.86f,0.64f,0.44f, 0.10f,0.62f,0.28f, 0.0f, 1.0f);

    if (badamState == 0) {
        drawHuman(badamBuyerX, frontY, 0.85f,0.63f,0.42f, 0.15f,0.50f,0.80f, badamBuyerLeg, 1.0f);
    } else if (badamState == 1) {
        float reach = 1.2f * sinf(badamArmAngle * 0.5f);
        drawHuman(cx - 1.5f, frontY, 0.85f,0.63f,0.42f, 0.15f,0.50f,0.80f, reach, 1.0f);
        if (sinf(badamArmAngle * 0.5f) > 0.2f) {
            glColor3f(0.90f, 0.85f, 0.60f);
            drawRect(cx - 0.8f, frontY + 2.8f, cx - 0.3f, frontY + 3.3f);
        }
    } else {
        drawHuman(badamBuyerX, frontY, 0.85f,0.63f,0.42f, 0.15f,0.50f,0.80f, badamBuyerLeg, -1.0f);
    }
}

void drawBox3D(float x1,float y1,float z1,float x2,float y2,float z2) {
    glBegin(GL_QUADS);
    glNormal3f(0,0,1);
    glVertex3f(x1,y1,z2);glVertex3f(x2,y1,z2);glVertex3f(x2,y2,z2);glVertex3f(x1,y2,z2);
    glNormal3f(0,0,-1);
    glVertex3f(x2,y1,z1);glVertex3f(x1,y1,z1);glVertex3f(x1,y2,z1);glVertex3f(x2,y2,z1);
    glNormal3f(-1,0,0);
    glVertex3f(x1,y1,z1);glVertex3f(x1,y1,z2);glVertex3f(x1,y2,z2);glVertex3f(x1,y2,z1);
    glNormal3f(1,0,0);
    glVertex3f(x2,y1,z2);glVertex3f(x2,y1,z1);glVertex3f(x2,y2,z1);glVertex3f(x2,y2,z2);
    glNormal3f(0,1,0);
    glVertex3f(x1,y2,z2);glVertex3f(x2,y2,z2);glVertex3f(x2,y2,z1);glVertex3f(x1,y2,z1);
    glNormal3f(0,-1,0);
    glVertex3f(x1,y1,z1);glVertex3f(x2,y1,z1);glVertex3f(x2,y1,z2);glVertex3f(x1,y1,z2);
    glEnd();
}

void drawCompactRoad() {
    glColor3f(0.26f, 0.27f, 0.29f);
    drawRect(0, 25, 100, 38);

    glColor3f(0.16f, 0.17f, 0.18f);
    drawRect(0, 37.2f, 100, 38.0f);
    drawRect(0, 25.0f, 100, 25.8f);

    glColor3f(1.0f, 0.93f, 0.55f);
    for (int x = 2; x <= 96; x += 13) drawRect((float)x, 31.2f, (float)x + 6.0f, 31.65f);

    glColor3f(0.88f, 0.88f, 0.88f);
    for (int x = 0; x <= 100; x += 8) {
        drawRect((float)x, 24.2f, (float)x + 4.0f, 24.5f);
        drawRect((float)x, 38.3f, (float)x + 4.0f, 38.6f);
    }

    glColor3f(0.72f, 0.70f, 0.65f);
    drawRect(0, 20.5f, 100, 25.0f);
    drawRect(0, 38.0f, 100, 41.0f);
    glColor3f(0.57f, 0.55f, 0.52f);
    for (float x = 0; x < 100; x += 5) {
        glBegin(GL_LINES);
        glVertex2f(x, 20.5f); glVertex2f(x, 25.0f);
        glVertex2f(x, 38.0f); glVertex2f(x, 41.0f);
        glEnd();
    }
}

void drawCompactRailRoad() {
    glColor3f(0.34f, 0.34f, 0.34f);
    drawRect(0, 41.0f, 100, 45.0f);
    glColor3f(0.10f, 0.10f, 0.10f);
    drawRect(0, 42.0f, 100, 42.35f);
    drawRect(0, 44.0f, 100, 44.35f);
    glColor3f(0.48f, 0.30f, 0.16f);
    for (float x = 0; x <= 100; x += 3.0f) drawRect(x, 41.3f, x + 0.8f, 44.8f);
}

void drawBackgroundMountains(bool night) {
    // Mountain bodies  mostly green/forested, just tips are snow
    if (night) glColor3f(0.10f, 0.22f, 0.18f);
    else       glColor3f(0.22f, 0.44f, 0.28f);  // green body
    drawTriangle(-8, 48, 15, 70, 38, 48);
    drawTriangle(20, 48, 48, 76, 77, 48);
    drawTriangle(58, 48, 82, 69, 108, 48);

    // Mid-green layer (darker green mid-section)
    if (night) glColor3f(0.08f, 0.18f, 0.14f);
    else       glColor3f(0.14f, 0.36f, 0.18f);
    drawTriangle(-2, 48, 15, 63, 32, 48);
    drawTriangle(26, 48, 48, 68, 70, 48);
    drawTriangle(62, 48, 82, 62, 102, 48);

    // Very small snow caps only at the very top (short triangle)
    if (night) glColor3f(0.72f, 0.78f, 0.88f);
    else       glColor3f(0.94f, 0.96f, 1.00f);
    drawTriangle(11, 65, 15, 70, 19, 65);   // left peak cap
    drawTriangle(43, 71, 48, 76, 52, 71);   // center peak cap
    drawTriangle(78, 65, 82, 69, 86, 65);   // right peak cap

    // Foreground green base / tree-line
    if (night) glColor3f(0.08f, 0.18f, 0.18f);
    else       glColor3f(0.22f, 0.44f, 0.28f);
    drawRect(0, 45, 100, 50);
}

void drawProTree(float x, float y, float scale, bool night) {
    glColor3f(night ? 0.23f : 0.46f, night ? 0.16f : 0.27f, night ? 0.10f : 0.09f);
    drawRect(x - 0.35f*scale, y, x + 0.35f*scale, y + 4.2f*scale);
    glColor3f(night ? 0.10f : 0.06f, night ? 0.28f : 0.48f, night ? 0.16f : 0.13f);
    drawCircleHelper(x - 1.2f*scale, y + 5.1f*scale, 1.7f*scale, 28);
    drawCircleHelper(x + 1.2f*scale, y + 5.1f*scale, 1.7f*scale, 28);
    drawCircleHelper(x, y + 6.4f*scale, 2.0f*scale, 28);
    drawCircleHelper(x, y + 4.4f*scale, 1.8f*scale, 28);
    if (night) {
        glColor3f(0.86f, 0.90f, 0.96f);
        drawCircleHelper(x - 1.0f*scale, y + 6.2f*scale, 0.35f*scale, 12);
        drawCircleHelper(x + 1.1f*scale, y + 4.9f*scale, 0.30f*scale, 12);
    }
}

void drawWindowGrid(float x1, float y1, float x2, float y2, bool night) {
    for (float x = x1; x < x2; x += 3.2f) {
        for (float y = y1; y < y2; y += 3.0f) {
            glColor3f(night ? 1.0f : 0.55f, night ? 0.78f : 0.82f, night ? 0.25f : 0.95f);
            drawRect(x, y, x + 1.5f, y + 1.5f);
            glColor3f(0.15f, 0.18f, 0.20f);
            glBegin(GL_LINE_LOOP);
            glVertex2f(x, y); glVertex2f(x + 1.5f, y); glVertex2f(x + 1.5f, y + 1.5f); glVertex2f(x, y + 1.5f);
            glEnd();
        }
    }
}

// ---- Small town house: alternates flat (variant=0) and triangular (variant=1) roofs ----
void drawSmallTownHouse(float x, float y, float w, float h, float r, float g, float b, bool night, int roofVariant) {
    glColor3f(r, g, b);
    drawRect(x, y, x + w, y + h);

    if (roofVariant == 0) {
        // Normal gable roof (previously flat - now proper triangular)
        float roofR = night ? r * 0.52f : r * 0.65f;
        float roofG = night ? g * 0.40f : g * 0.50f;
        float roofB = night ? b * 0.36f : b * 0.44f;
        glColor3f(roofR, roofG, roofB);
        drawTriangle(x - 0.5f, y + h, x + w + 0.5f, y + h, x + w * 0.5f, y + h + 3.8f);
        glColor3f(roofR * 0.80f, roofG * 0.80f, roofB * 0.80f);
        glLineWidth(1.5f);
        glBegin(GL_LINE_STRIP);
        glVertex2f(x - 0.5f, y + h);
        glVertex2f(x + w * 0.5f, y + h + 3.8f);
        glVertex2f(x + w + 0.5f, y + h);
        glEnd();
        glLineWidth(1.0f);
    } else {
        // Triangle / gable roof
        float roofR = night ? r * 0.55f : r * 0.68f;
        float roofG = night ? g * 0.42f : g * 0.52f;
        float roofB = night ? b * 0.38f : b * 0.46f;
        glColor3f(roofR, roofG, roofB);
        drawTriangle(x - 0.6f, y + h, x + w + 0.6f, y + h, x + w * 0.5f, y + h + 4.5f);
        // Roof trim
        glColor3f(roofR * 0.82f, roofG * 0.82f, roofB * 0.82f);
        glLineWidth(1.5f);
        glBegin(GL_LINE_STRIP);
        glVertex2f(x - 0.6f, y + h);
        glVertex2f(x + w * 0.5f, y + h + 4.5f);
        glVertex2f(x + w + 0.6f, y + h);
        glEnd();
        glLineWidth(1.0f);
    }

    drawWindowGrid(x + 1.0f, y + 2.2f, x + w - 1.0f, y + h - 1.0f, night);
    glColor3f(0.34f, 0.18f, 0.09f);
    drawRect(x + w*0.43f, y, x + w*0.57f, y + 2.6f);
}

void drawHospital(float x, float y, bool night) {
    // Body
    glColor3f(night ? 0.74f : 0.92f, night ? 0.76f : 0.94f, night ? 0.80f : 0.96f);
    drawRect(x, y, x + 13, y + 12);
    // Gable (triangular) roof
    float roofR = night ? 0.38f : 0.60f;
    float roofG = night ? 0.38f : 0.60f;
    float roofB = night ? 0.42f : 0.65f;
    glColor3f(roofR, roofG, roofB);
    drawTriangle(x - 0.6f, y + 12, x + 13.6f, y + 12, x + 6.5f, y + 17.0f);
    glColor3f(roofR * 0.80f, roofG * 0.80f, roofB * 0.80f);
    glLineWidth(1.5f);
    glBegin(GL_LINE_STRIP);
    glVertex2f(x - 0.6f, y + 12);
    glVertex2f(x + 6.5f, y + 17.0f);
    glVertex2f(x + 13.6f, y + 12);
    glEnd();
    glLineWidth(1.0f);
    drawWindowGrid(x + 1.2f, y + 3.4f, x + 12, y + 10.5f, night);
    // Door
    glColor3f(0.15f, 0.45f, 0.85f); drawRect(x + 5.0f, y, x + 8.0f, y + 3.0f);
    // Cross sign  stroke text style using geometry
    glColor3f(0.90f, 0.05f, 0.05f);
    drawRect(x + 6.0f, y + 10.8f, x + 7.0f, y + 12.5f);
    drawRect(x + 5.3f, y + 11.5f, x + 7.7f, y + 12.0f);
    // "H" sign on front wall
    glColor3f(night ? 0.95f : 0.15f, night ? 0.30f : 0.45f, night ? 0.30f : 0.85f);
    glLineWidth(2.0f);
    float hx = x + 1.8f, hy = y + 6.5f, hs = 1.5f;
    glBegin(GL_LINES);
    glVertex2f(hx, hy); glVertex2f(hx, hy+hs*1.5f);
    glVertex2f(hx+hs, hy); glVertex2f(hx+hs, hy+hs*1.5f);
    glVertex2f(hx, hy+hs*0.75f); glVertex2f(hx+hs, hy+hs*0.75f);
    glEnd();
    glLineWidth(1.0f);
    // Sign board above door: "HOSPITAL" in stroke text
    float sgx1 = x + 1.5f, sgx2 = x + 11.5f, sgy = y + 3.0f, sgh = 1.5f;
    glColor3f(night ? 0.88f : 0.98f, night ? 0.90f : 0.99f, night ? 0.95f : 1.0f);
    drawRect(sgx1, sgy, sgx2, sgy + sgh);
    glColor3f(0.10f, 0.35f, 0.80f);
    drawSignText("HOSPITAL", sgx1+0.1f, sgx2-0.1f, sgy+0.05f, sgh-0.1f, 0.10f, 0.35f, 0.80f);
}

void drawSchool(float x, float y, bool night) {
    // Body
    glColor3f(night ? 0.64f : 0.88f, night ? 0.50f : 0.70f, night ? 0.35f : 0.46f);
    drawRect(x, y, x + 15, y + 11);
    // Triangle (gable) roof for school
    float roofR = night ? 0.32f : 0.55f;
    float roofG = night ? 0.22f : 0.32f;
    float roofB = night ? 0.14f : 0.18f;
    glColor3f(roofR, roofG, roofB);
    drawTriangle(x - 0.6f, y + 11, x + 15.6f, y + 11, x + 7.5f, y + 16.5f);
    glColor3f(roofR * 0.82f, roofG * 0.82f, roofB * 0.82f);
    glLineWidth(1.5f);
    glBegin(GL_LINE_STRIP);
    glVertex2f(x - 0.6f, y + 11);
    glVertex2f(x + 7.5f, y + 16.5f);
    glVertex2f(x + 15.6f, y + 11);
    glEnd();
    glLineWidth(1.0f);
    drawWindowGrid(x + 1.2f, y + 3.0f, x + 14.0f, y + 10.0f, night);
    // Door
    glColor3f(0.35f, 0.16f, 0.06f); drawRect(x + 6.2f, y, x + 8.8f, y + 3.1f);
    // Sign board
    float sgx1 = x + 1.5f, sgx2 = x + 13.5f, sgy = y + 3.0f, sgh = 1.5f;
    glColor3f(night ? 0.85f : 0.98f, night ? 0.78f : 0.95f, night ? 0.62f : 0.88f);
    drawRect(sgx1, sgy, sgx2, sgy + sgh);
    glColor3f(night ? 0.55f : 0.12f, night ? 0.22f : 0.18f, night ? 0.08f : 0.50f);
    drawSignText("SCHOOL", sgx1+0.1f, sgx2-0.1f, sgy+0.05f, sgh-0.1f,
                 night ? 0.55f : 0.12f, night ? 0.22f : 0.18f, night ? 0.08f : 0.50f);
}

void drawChurch(float x, float y, bool night) {
    glColor3f(night ? 0.70f : 0.92f, night ? 0.72f : 0.90f, night ? 0.78f : 0.82f);
    drawRect(x, y, x + 13, y + 13);

    // Decorative stepped gable roof (Gothic style) on main body
    float rR = night ? 0.28f : 0.46f, rG = night ? 0.22f : 0.36f, rB = night ? 0.35f : 0.52f;
    // Main gable
    glColor3f(rR, rG, rB);
    drawTriangle(x - 0.6f, y + 13, x + 13.6f, y + 13, x + 6.5f, y + 18.5f);
    // Decorative stepped side pinnacles
    glColor3f(rR * 0.90f, rG * 0.88f, rB * 0.90f);
    drawTriangle(x - 0.3f, y + 13, x + 2.5f, y + 13, x + 1.1f, y + 16.0f);
    drawTriangle(x + 10.5f, y + 13, x + 13.3f, y + 13, x + 11.9f, y + 16.0f);
    // Decorative ridge ornament (small cross bar at peak)
    glColor3f(rR * 1.1f, rG * 1.05f, rB * 1.1f);
    drawTriangle(x + 4.8f, y + 16.5f, x + 8.2f, y + 16.5f, x + 6.5f, y + 18.5f);
    // Roof outline
    glColor3f(rR * 0.72f, rG * 0.72f, rB * 0.72f);
    glLineWidth(1.5f);
    glBegin(GL_LINE_STRIP);
    glVertex2f(x - 0.6f, y + 13); glVertex2f(x + 6.5f, y + 18.5f); glVertex2f(x + 13.6f, y + 13);
    glEnd();
    glLineWidth(1.0f);

    // Bell tower (kept with spire on top)
    glColor3f(night ? 0.24f : 0.42f, night ? 0.28f : 0.34f, night ? 0.38f : 0.48f);
    drawRect(x + 5.2f, y + 13, x + 7.8f, y + 23.5f);
    // Decorative tower windows (arched look via circles)
    glColor3f(night ? 1.0f : 0.58f, night ? 0.82f : 0.82f, night ? 0.28f : 0.95f);
    drawCircleHelper(x + 6.5f, y + 20.0f, 0.75f, 20);
    drawCircleHelper(x + 6.5f, y + 22.0f, 0.65f, 20);
    // Multi-tier decorative spire: stepped layers
    glColor3f(rR, rG, rB);
    drawTriangle(x + 3.8f, y + 23.5f, x + 9.2f, y + 23.5f, x + 6.5f, y + 27.0f);
    glColor3f(rR * 0.88f, rG * 0.88f, rB * 0.90f);
    drawTriangle(x + 4.8f, y + 26.5f, x + 8.2f, y + 26.5f, x + 6.5f, y + 30.0f);
    // Gold cross at top
    glColor3f(0.95f, 0.78f, 0.18f);
    drawRect(x + 6.25f, y + 29.7f, x + 6.75f, y + 32.0f);
    drawRect(x + 5.5f, y + 30.6f, x + 7.5f, y + 31.1f);
    // Rose window circle
    glColor3f(night ? 1.0f : 0.58f, night ? 0.82f : 0.82f, night ? 0.28f : 0.95f);
    drawCircleHelper(x + 6.5f, y + 16.0f, 1.2f, 24);
    // Decorative inner ring on rose window
    glColor3f(night ? 0.24f : 0.42f, night ? 0.28f : 0.34f, night ? 0.38f : 0.48f);
    drawCircleHelper(x + 6.5f, y + 16.0f, 0.7f, 24);
    glColor3f(night ? 1.0f : 0.58f, night ? 0.82f : 0.82f, night ? 0.28f : 0.95f);
    drawCircleHelper(x + 6.5f, y + 16.0f, 0.35f, 16);
    drawWindowGrid(x + 1.5f, y + 3.0f, x + 12.0f, y + 11.0f, night);
    glColor3f(0.24f, 0.12f, 0.06f); drawRect(x + 5.0f, y, x + 8.0f, y + 4.0f);
    // Arched door top
    glColor3f(0.38f, 0.20f, 0.10f);
    drawCircleHelper(x + 6.5f, y + 4.0f, 1.5f, 20);
}

// ---- Street Floodlight / Lamp Post ----
void drawStreetLamp(float x, float y, bool night) {
    // Pole
    glColor3f(night ? 0.38f : 0.30f, night ? 0.40f : 0.32f, night ? 0.44f : 0.34f);
    drawRect(x - 0.18f, y, x + 0.18f, y + 5.5f);
    // Arm curving out
    glColor3f(night ? 0.38f : 0.30f, night ? 0.40f : 0.32f, night ? 0.44f : 0.34f);
    glLineWidth(2.2f);
    glBegin(GL_LINE_STRIP);
    glVertex2f(x, y + 5.5f);
    glVertex2f(x + 0.5f, y + 5.7f);
    glVertex2f(x + 1.2f, y + 5.5f);
    glEnd();
    glLineWidth(1.0f);
    // Lamp head
    glColor3f(night ? 0.30f : 0.22f, night ? 0.30f : 0.22f, night ? 0.32f : 0.24f);
    drawRect(x + 0.8f, y + 5.1f, x + 1.8f, y + 5.5f);
    // Light glow
    if (night) {
        glColor3f(1.0f, 0.94f, 0.62f);
        drawCircleHelper(x + 1.3f, y + 5.0f, 0.35f, 14);
        // Cone of light pointing downward
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1.0f, 0.92f, 0.50f, 0.18f);
        glBegin(GL_TRIANGLES);
        glVertex2f(x + 0.9f, y + 5.0f);
        glVertex2f(x + 1.7f, y + 5.0f);
        glVertex2f(x + 1.3f, y);
        glEnd();
        glDisable(GL_BLEND);
    } else {
        glColor3f(0.88f, 0.86f, 0.76f);
        drawCircleHelper(x + 1.3f, y + 5.0f, 0.28f, 14);
    }
    // Base
    glColor3f(night ? 0.25f : 0.20f, night ? 0.25f : 0.20f, night ? 0.28f : 0.22f);
    drawRect(x - 0.4f, y, x + 0.4f, y + 0.3f);
}

void drawStreetLamps(bool night) {
    // Place lamps at regular intervals along both sides of the road
    float lampPositions[] = { 5.0f, 18.0f, 32.0f, 46.0f, 60.0f, 74.0f, 88.0f };
    int numLamps = 7;
    for (int i = 0; i < numLamps; i++) {
        drawStreetLamp(lampPositions[i], 38.0f, night);  // top side of road
        drawStreetLamp(lampPositions[i], 20.5f, night);  // bottom side (footpath edge)
    }
}

void drawTownBuildings(bool night) {
    drawHospital(3, 45, night);
    drawSmallTownHouse(18, 45, 8, 10, night ? 0.50f : 0.82f, night ? 0.42f : 0.62f, night ? 0.36f : 0.44f, night, 1); // triangle roof
    drawSchool(29, 45, night);
    drawSmallTownHouse(47, 45, 8, 11, night ? 0.46f : 0.74f, night ? 0.50f : 0.72f, night ? 0.58f : 0.82f, night, 0); // flat roof
    drawChurch(59, 45, night);
    drawSmallTownHouse(76, 45, 8, 10, night ? 0.58f : 0.86f, night ? 0.42f : 0.58f, night ? 0.36f : 0.40f, night, 1); // triangle roof
    drawSmallTownHouse(87, 45, 9, 11, night ? 0.42f : 0.76f, night ? 0.46f : 0.66f, night ? 0.52f : 0.58f, night, 0); // flat roof

    // Big pine trees beside buildings
    float sc = 0.70f;
    drawBigPineTree(17.0f,  45, sc, night);
    drawBigPineTree(28.0f,  45, sc, night);
    drawBigPineTree(45.5f,  45, sc, night);
    drawBigPineTree(58.0f,  45, sc, night);
    drawBigPineTree(74.5f,  45, sc, night);
    drawBigPineTree(86.0f,  45, sc, night);
    drawBigPineTree(97.5f,  45, sc * 0.85f, night);
    drawBigPineTree(2.0f,   45, sc * 0.90f, night);
}

void drawFairTent(float x, float y, float w, float h, float r, float g, float b) {
    // More interesting fair tent: striped roof, flag, door, side shadows and lights
    float roofPeak = h * 0.75f;

    glColor3f(r, g, b);
    drawTriangle(x, y + h, x + w, y + h, x + w * 0.5f, y + h + roofPeak);

    glColor3f(1.0f, 0.96f, 0.84f);
    drawTriangle(x + w*0.05f, y + h, x + w*0.25f, y + h, x + w*0.5f, y + h + roofPeak);
    drawTriangle(x + w*0.38f, y + h, x + w*0.58f, y + h, x + w*0.5f, y + h + roofPeak);
    drawTriangle(x + w*0.72f, y + h, x + w*0.92f, y + h, x + w*0.5f, y + h + roofPeak);

    glColor3f(0.96f, 0.92f, 0.78f);
    drawRect(x + 0.45f, y, x + w - 0.45f, y + h);

    glColor3f(r * 0.75f, g * 0.75f, b * 0.75f);
    drawRect(x + 0.45f, y + h - 0.9f, x + w - 0.45f, y + h);

    glColor3f(r * 0.65f, g * 0.65f, b * 0.65f);
    drawRect(x + w*0.08f, y, x + w*0.16f, y + h - 0.6f);
    drawRect(x + w*0.84f, y, x + w*0.92f, y + h - 0.6f);

    glColor3f(0.32f, 0.16f, 0.07f);
    drawTriangle(x + w*0.39f, y, x + w*0.61f, y, x + w*0.50f, y + h*0.58f);

    glColor3f(1.0f, 0.82f, 0.12f);
    for (float lx = x + 1.0f; lx < x + w - 0.5f; lx += 2.0f) {
        drawCircleHelper(lx, y + h + 0.15f, 0.20f, 10);
    }

    glColor3f(0.25f, 0.25f, 0.28f);
    glLineWidth(1.3f);
    glBegin(GL_LINES);
    glVertex2f(x + w*0.5f, y + h + roofPeak);
    glVertex2f(x + w*0.5f, y + h + roofPeak + 2.2f);
    glEnd();
    glLineWidth(1.0f);

    glColor3f(1.0f, 0.15f, 0.15f);
    drawTriangle(x + w*0.5f, y + h + roofPeak + 2.0f,
                 x + w*0.5f, y + h + roofPeak + 0.9f,
                 x + w*0.5f + 1.8f, y + h + roofPeak + 1.45f);
}

void drawDisneyRoadBillboard(float x, float y) {
    // Billboard beside the road: "WELCOME TO DISNEY LAND"
    float postH = 8.0f;
    float bw = 24.0f;
    float bh = 7.0f;

    glColor3f(0.25f, 0.25f, 0.27f);
    drawRect(x + 3.0f, y, x + 3.7f, y + postH);
    drawRect(x + bw - 3.7f, y, x + bw - 3.0f, y + postH);

    glColor3f(0.48f, 0.48f, 0.52f);
    drawRect(x + 3.12f, y, x + 3.30f, y + postH);
    drawRect(x + bw - 3.58f, y, x + bw - 3.40f, y + postH);

    glColor3f(0.12f, 0.08f, 0.04f);
    drawRect(x, y + postH, x + bw, y + postH + bh);

    glColor3f(0.06f, 0.04f, 0.02f);
    drawRect(x, y + postH + bh, x + bw, y + postH + bh + 0.55f);
    drawRect(x, y + postH - 0.45f, x + bw, y + postH);
    drawRect(x, y + postH, x + 0.55f, y + postH + bh);
    drawRect(x + bw - 0.55f, y + postH, x + bw, y + postH + bh);

    for (float lx = x + 1.2f; lx < x + bw - 0.8f; lx += 1.7f) {
        glColor3f(1.0f, 0.90f, 0.25f);
        drawCircleHelper(lx, y + postH + bh + 0.28f, 0.18f, 10);
    }

    glColor3f(1.0f, 0.96f, 0.80f);
    drawRect(x + 1.0f, y + postH + 0.75f, x + bw - 1.0f, y + postH + bh - 0.75f);

    glColor3f(0.05f, 0.18f, 0.75f);
    glLineWidth(2.5f);
    drawSignText("WELCOME TO", x + 1.5f, x + bw - 1.5f, y + postH + 3.9f, 1.55f,
                 0.05f, 0.20f, 0.75f);
    glColor3f(0.82f, 0.05f, 0.05f);
    glLineWidth(3.0f);
    drawSignText("DISNEY LAND", x + 1.5f, x + bw - 1.5f, y + postH + 1.35f, 1.90f,
                 0.82f, 0.05f, 0.05f);
    glLineWidth(1.0f);
}

void drawHotAirBalloonDay(float x, float y, float scale) {
    // Side-view hot air balloon
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(scale, scale, 1.0f);

    glColor3f(0.95f, 0.18f, 0.15f);
    drawCircleHelper(0.0f, 0.0f, 3.2f, 50);
    glColor3f(1.0f, 0.78f, 0.15f);
    drawCircleHelper(-1.05f, 0.0f, 2.75f, 40);
    glColor3f(0.20f, 0.55f, 0.95f);
    drawCircleHelper(1.15f, 0.0f, 2.55f, 40);

    glColor3f(0.95f, 0.18f, 0.15f);
    drawRect(-0.30f, -3.0f, 0.30f, -0.3f);
    glColor3f(1.0f, 0.96f, 0.84f);
    drawCircleHelper(0.0f, 0.9f, 0.75f, 22);

    glColor3f(0.28f, 0.18f, 0.10f);
    glLineWidth(1.3f);
    glBegin(GL_LINES);
    glVertex2f(-1.7f, -2.5f); glVertex2f(-0.9f, -5.0f);
    glVertex2f( 1.7f, -2.5f); glVertex2f( 0.9f, -5.0f);
    glEnd();
    glLineWidth(1.0f);

    glColor3f(0.55f, 0.32f, 0.12f);
    drawRect(-1.25f, -6.3f, 1.25f, -5.0f);
    glColor3f(0.36f, 0.20f, 0.08f);
    drawRect(-1.35f, -5.2f, 1.35f, -4.9f);
    drawRect(-1.35f, -6.3f, -1.05f, -5.0f);
    drawRect( 1.05f, -6.3f,  1.35f, -5.0f);

    glPopMatrix();
}

// ===================== VARIETY TREES =====================
void drawPalmTree(float x, float y, float scale) {
    glColor3f(0.60f, 0.40f, 0.15f);
    glLineWidth(3.0f * scale);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= 12; i++) {
        float t = i / 12.0f;
        glVertex2f(x + 0.8f * scale * sinf(t * 1.2f), y + t * 8.0f * scale);
    }
    glEnd();
    glLineWidth(1.0f);
    float tx = x + 0.8f * scale * sinf(1.2f), ty = y + 8.0f * scale;
    float frondAngles[] = {0.0f, 0.5f, 1.1f, 1.8f, 2.5f, 3.2f, 3.9f, 4.6f, 5.3f, 5.9f};
    for (int fi = 0; fi < 10; fi++) {
        float a = frondAngles[fi];
        float len = 3.5f * scale;
        glColor3f(0.08f, 0.55f, 0.12f);
        glLineWidth(2.0f);
        glBegin(GL_LINE_STRIP);
        float bx = tx, by = ty;
        for (int j = 0; j <= 8; j++) {
            float t2 = j / 8.0f;
            glVertex2f(bx + len * t2 * cosf(a), by + len * t2 * sinf(a) - 1.5f * scale * t2 * t2);
        }
        glEnd();
        glLineWidth(1.0f);
    }
    glColor3f(0.55f, 0.30f, 0.05f);
    drawCircleHelper(tx - 0.4f * scale, ty - 0.3f * scale, 0.4f * scale, 10);
    drawCircleHelper(tx + 0.3f * scale, ty - 0.5f * scale, 0.35f * scale, 10);
}

void drawCherryTree(float x, float y, float scale) {
    glColor3f(0.38f, 0.20f, 0.08f);
    drawRect(x - 0.3f * scale, y, x + 0.3f * scale, y + 5.0f * scale);
    glColor3f(0.45f, 0.22f, 0.08f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex2f(x, y + 4.5f * scale); glVertex2f(x - 2.5f * scale, y + 6.5f * scale);
    glVertex2f(x, y + 4.5f * scale); glVertex2f(x + 2.5f * scale, y + 6.5f * scale);
    glVertex2f(x, y + 3.5f * scale); glVertex2f(x - 1.5f * scale, y + 5.5f * scale);
    glVertex2f(x, y + 3.5f * scale); glVertex2f(x + 1.5f * scale, y + 5.5f * scale);
    glEnd();
    glLineWidth(1.0f);
    float blossomColors[][3] = {{1.0f,0.65f,0.75f},{1.0f,0.72f,0.80f},{0.98f,0.78f,0.84f}};
    float bpos[][2] = {{0,7},{-2.2f,6.8f},{2.2f,6.8f},{-1.0f,8.2f},{1.0f,8.2f},{0,6.0f},{-3.0f,6.0f},{3.0f,6.0f}};
    for (int bi = 0; bi < 8; bi++) {
        int c = bi % 3;
        glColor3f(blossomColors[c][0], blossomColors[c][1], blossomColors[c][2]);
        drawCircleHelper(x + bpos[bi][0] * scale, y + bpos[bi][1] * scale, 1.3f * scale, 20);
    }
}

void drawAutumnTree(float x, float y, float scale) {
    glColor3f(0.45f, 0.22f, 0.06f);
    drawRect(x - 0.35f * scale, y, x + 0.35f * scale, y + 5.5f * scale);
    float autumnColors[][3] = {{0.88f,0.38f,0.05f},{0.95f,0.62f,0.08f},{0.75f,0.20f,0.08f},{0.92f,0.72f,0.12f}};
    float cpos[][3] = {{0,7,2.2f},{-1.8f,6.5f,1.8f},{1.8f,6.5f,1.8f},{0,5.5f,1.5f},{-2.8f,5.8f,1.5f},{2.8f,5.8f,1.5f}};
    for (int ai = 0; ai < 6; ai++) {
        int c = ai % 4;
        glColor3f(autumnColors[c][0], autumnColors[c][1], autumnColors[c][2]);
        drawCircleHelper(x + cpos[ai][0] * scale, y + cpos[ai][1] * scale, cpos[ai][2] * scale, 20);
    }
}

// ===================== BIG FERRIS WHEEL (radius 15) =====================
float ferrisAngle = 0.0f;

void drawFerrisWheel(float cx, float cy, float radius) {
    int numCabs = 10;
    // Support legs
    glColor3f(0.25f, 0.25f, 0.28f);
    glLineWidth(3.5f);
    glBegin(GL_LINES);
    glVertex2f(cx, cy); glVertex2f(cx - radius * 0.7f, cy - radius * 1.1f);
    glVertex2f(cx, cy); glVertex2f(cx + radius * 0.7f, cy - radius * 1.1f);
    glVertex2f(cx - radius * 0.5f, cy - radius * 0.7f); glVertex2f(cx + radius * 0.5f, cy - radius * 0.7f);
    glEnd();
    glLineWidth(1.0f);

    // Outer rim
    glColor3f(0.20f, 0.20f, 0.22f);
    glLineWidth(4.0f);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 60; i++) {
        float a = 2.0f * PI * i / 60.0f;
        glVertex2f(cx + radius * cosf(a), cy + radius * sinf(a));
    }
    glEnd();
    glLineWidth(1.0f);

    // Inner rim
    glColor3f(0.35f, 0.35f, 0.38f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 60; i++) {
        float a = 2.0f * PI * i / 60.0f;
        glVertex2f(cx + radius * 0.15f * cosf(a), cy + radius * 0.15f * sinf(a));
    }
    glEnd();
    glLineWidth(1.0f);

    // Spokes
    glColor3f(0.40f, 0.40f, 0.42f);
    glLineWidth(1.8f);
    for (int i = 0; i < numCabs; i++) {
        float a = ferrisAngle + 2.0f * PI * i / numCabs;
        glBegin(GL_LINES);
        glVertex2f(cx + radius * 0.15f * cosf(a), cy + radius * 0.15f * sinf(a));
        glVertex2f(cx + radius * cosf(a), cy + radius * sinf(a));
        glEnd();
    }
    glLineWidth(1.0f);

    // Center hub
    glColor3f(0.70f, 0.72f, 0.80f);
    drawCircleHelper(cx, cy, radius * 0.18f, 20);
    glColor3f(0.90f, 0.92f, 1.0f);
    drawCircleHelper(cx, cy, radius * 0.08f, 16);

    // Cabins
    float cabColors[][3] = {
        {0.90f,0.10f,0.10f},{0.10f,0.50f,0.90f},{0.10f,0.80f,0.20f},
        {0.95f,0.80f,0.05f},{0.80f,0.10f,0.80f},{0.95f,0.50f,0.10f},
        {0.10f,0.80f,0.80f},{0.90f,0.90f,0.10f},{0.50f,0.10f,0.90f},{0.90f,0.50f,0.70f}
    };
    float cabW = radius * 0.22f, cabH = radius * 0.26f;
    for (int i = 0; i < numCabs; i++) {
        float a = ferrisAngle + 2.0f * PI * i / numCabs;
        float px = cx + radius * cosf(a);
        float py = cy + radius * sinf(a);
        glColor3f(cabColors[i][0], cabColors[i][1], cabColors[i][2]);
        drawRect(px - cabW * 0.5f, py - cabH, px + cabW * 0.5f, py);
        glColor3f(cabColors[i][0]*0.75f, cabColors[i][1]*0.75f, cabColors[i][2]*0.75f);
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(px, py);
        for (int s = 0; s <= 10; s++) {
            float sa = PI * s / 10.0f;
            glVertex2f(px + cabW * 0.5f * cosf(sa), py + cabH * 0.25f * sinf(sa));
        }
        glEnd();
        glColor3f(0.65f, 0.88f, 0.98f);
        drawRect(px - cabW * 0.28f, py - cabH * 0.75f, px + cabW * 0.28f, py - cabH * 0.25f);
        glColor3f(0.40f, 0.40f, 0.42f);
        glLineWidth(1.2f);
        glBegin(GL_LINES);
        glVertex2f(px, py);
        glVertex2f(cx + radius * cosf(a), cy + radius * sinf(a));
        glEnd();
        glLineWidth(1.0f);
    }

    // Decorative lights on rim
    for (int i = 0; i < 24; i++) {
        float a = ferrisAngle * 2.0f + 2.0f * PI * i / 24.0f;
        float lc = (i % 3);
        if (lc == 0) glColor3f(1.0f, 0.2f, 0.2f);
        else if (lc == 1) glColor3f(0.2f, 0.8f, 1.0f);
        else glColor3f(1.0f, 0.9f, 0.1f);
        drawCircleHelper(cx + (radius + 0.3f) * cosf(a), cy + (radius + 0.3f) * sinf(a), 0.25f, 8);
    }
}



void drawEuropeanBanglaFair() {
    glColor3f(0.22f, 0.50f, 0.20f);
    drawRect(0, 0, 100, 20.5f);
    glColor3f(0.76f, 0.56f, 0.28f);
    drawRect(0, 18.5f, 100, 20.5f);

    for (float x = 4; x < 96; x += 8) {
        glColor3f(0.8f, 0.05f, 0.08f); drawCircleHelper(x, 19.4f, 0.45f, 12);
        glColor3f(0.10f, 0.25f, 0.85f); drawCircleHelper(x + 2.0f, 19.1f, 0.45f, 12);
        glColor3f(1.0f, 0.85f, 0.10f); drawCircleHelper(x + 4.0f, 19.4f, 0.45f, 12);
    }

    // Tents: one taller, one heavier/wider, all more decorative
    drawFairTent(6,  4.2f, 11.0f,  8.8f, 0.90f, 0.08f, 0.08f);  // taller red tent
    drawFairTent(20, 3.8f, 17.0f,  8.0f, 0.10f, 0.35f, 0.90f);  // heavy blue tent
    drawFairTent(63, 5.0f, 11.5f,  7.0f, 0.10f, 0.65f, 0.28f);
    drawFairTent(79, 4.0f, 14.0f,  8.4f, 0.95f, 0.58f, 0.05f);

    // --- BIG FERRIS WHEEL (radius 15) in the centre of the fair ---
    drawFerrisWheel(50.0f, 10.5f, 15.0f);

    // Smaller Disney Land rectangle board
    glColor3f(1.0f, 0.96f, 0.82f);
    drawRect(42.5f, 9.5f, 57.5f, 15.8f);
    glColor3f(0.12f, 0.22f, 0.45f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(42.5f, 9.5f); glVertex2f(57.5f, 9.5f);
    glVertex2f(57.5f,15.8f); glVertex2f(42.5f,15.8f);
    glEnd();
    glLineWidth(1.0f);

    glColor3f(0.05f, 0.20f, 0.75f);
    glLineWidth(2.5f);
    drawSignText("DISNEY", 43.2f, 56.8f, 13.1f, 1.65f, 0.05f, 0.20f, 0.75f);
    glColor3f(0.78f, 0.05f, 0.05f);
    glLineWidth(2.5f);
    drawSignText("LAND", 43.2f, 56.8f, 10.7f, 1.65f, 0.78f, 0.05f, 0.05f);
    glLineWidth(1.0f);

    // Moving people in front of the fair
    struct FairPed { float x0; float speed; float phase; float dir; float skin[3]; float shirt[3]; bool isWoman; bool isChild; };
    FairPed fps[] = {
        { 3.0f,  0.28f, 0.0f,  1.0f, {0.87f,0.65f,0.45f}, {0.85f,0.10f,0.15f}, false, false},
        {12.0f, -0.22f, 0.8f, -1.0f, {0.90f,0.70f,0.50f}, {0.10f,0.45f,0.80f}, true,  false},
        {20.0f,  0.18f, 1.5f,  1.0f, {0.85f,0.62f,0.42f}, {0.95f,0.80f,0.05f}, false, true },
        {30.0f,  0.32f, 0.3f,  1.0f, {0.88f,0.67f,0.47f}, {0.20f,0.65f,0.25f}, false, false},
        {42.0f, -0.25f, 2.0f, -1.0f, {0.86f,0.64f,0.44f}, {0.80f,0.15f,0.50f}, true,  false},
        {54.0f,  0.20f, 0.7f,  1.0f, {0.85f,0.63f,0.43f}, {0.95f,0.50f,0.10f}, false, true },
        {66.0f, -0.30f, 1.2f, -1.0f, {0.90f,0.68f,0.48f}, {0.50f,0.10f,0.85f}, true,  false},
        {78.0f,  0.24f, 2.5f,  1.0f, {0.87f,0.66f,0.46f}, {0.10f,0.55f,0.30f}, false, false},
        {88.0f, -0.18f, 0.5f, -1.0f, {0.89f,0.68f,0.47f}, {0.95f,0.90f,0.10f}, false, true },
        {95.0f,  0.26f, 1.8f,  1.0f, {0.88f,0.65f,0.44f}, {0.60f,0.10f,0.80f}, true,  false},
    };
    for (int i = 0; i < 10; i++) {
        float fx = fps[i].x0 + fps[i].speed * pedTimer * 10.0f;
        if (fx > 105.0f) fx -= 110.0f;
        if (fx < -5.0f)  fx += 110.0f;
        float lp = pedTimer * 3.0f + fps[i].phase;
        float py = 17.8f;
        if (fps[i].isChild)
            drawChild(fx, py, fps[i].skin[0],fps[i].skin[1],fps[i].skin[2], fps[i].shirt[0],fps[i].shirt[1],fps[i].shirt[2], lp, fps[i].dir);
        else if (fps[i].isWoman)
            drawWoman(fx, py, fps[i].skin[0],fps[i].skin[1],fps[i].skin[2], fps[i].shirt[0],fps[i].shirt[1],fps[i].shirt[2], lp, fps[i].dir);
        else
            drawHuman(fx, py, fps[i].skin[0],fps[i].skin[1],fps[i].skin[2], fps[i].shirt[0],fps[i].shirt[1],fps[i].shirt[2], lp, fps[i].dir);
    }
}

void drawModernTrain(float x, bool night) {
    glPushMatrix();
    glTranslatef(x, 42.9f, 0);
    glScalef(1.0f, 1.0f, 1.0f);
    glColor3f(night ? 0.82f : 0.88f, night ? 0.86f : 0.90f, night ? 0.92f : 0.94f);
    glBegin(GL_POLYGON);
    // Front taper on LEFT side (trainX moves left, so leading edge = left = negative x)
    glVertex2f(-24, 1.5f); glVertex2f(-20, 0); glVertex2f(20, 0); glVertex2f(20, 4.5f); glVertex2f(-20, 4.5f); glVertex2f(-24, 1.5f);
    glEnd();
    glColor3f(0.05f, 0.22f, 0.48f); drawRect(-19, 2.5f, 18, 3.8f);
    glColor3f(0.90f, 0.10f, 0.10f); drawRect(-20, 0.7f, 21, 1.2f);
    for (float wx = -16; wx <= 13; wx += 5) {
        glColor3f(night ? 1.0f : 0.45f, night ? 0.78f : 0.80f, night ? 0.32f : 0.95f);
        drawRect(wx, 2.2f, wx + 3.2f, 3.4f);
    }
    glColor3f(0.03f, 0.03f, 0.04f);
    for (float wx = -16; wx <= 16; wx += 8) drawCircleHelper(wx, 0.0f, 0.85f, 18);
    // Headlight on the tapered (left/front) end
    if (night) { glColor3f(1.0f, 0.95f, 0.58f); drawCircleHelper(-23, 2.1f, 0.7f, 18); }
    glPopMatrix();
}

void drawModernTrainRight(float x, bool night) {
    // This train moves RIGHT so front is on the RIGHT side (positive x = leading edge)
    glPushMatrix();
    glTranslatef(x, 42.9f, 0);
    glColor3f(night ? 0.82f : 0.88f, night ? 0.86f : 0.90f, night ? 0.92f : 0.94f);
    glBegin(GL_POLYGON);
    glVertex2f(-20, 0); glVertex2f(20, 0); glVertex2f(24, 1.5f); glVertex2f(20, 4.5f); glVertex2f(-20, 4.5f); glVertex2f(-20, 0);
    glEnd();
    glColor3f(0.05f, 0.22f, 0.48f); drawRect(-19, 2.5f, 18, 3.8f);
    glColor3f(0.90f, 0.10f, 0.10f); drawRect(-20, 0.7f, 21, 1.2f);
    for (float wx = -16; wx <= 13; wx += 5) {
        glColor3f(night ? 1.0f : 0.45f, night ? 0.78f : 0.80f, night ? 0.32f : 0.95f);
        drawRect(wx, 2.2f, wx + 3.2f, 3.4f);
    }
    glColor3f(0.03f, 0.03f, 0.04f);
    for (float wx = -16; wx <= 16; wx += 8) drawCircleHelper(wx, 0.0f, 0.85f, 18);
    if (night) { glColor3f(1.0f, 0.95f, 0.58f); drawCircleHelper(23, 2.1f, 0.7f, 18); }
    glPopMatrix();
}

void drawCompactCar(float x, float y, float scale, float r, float g, float b, bool faceRight, bool night) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(faceRight ? scale : -scale, scale, 1);

    // ---- FULL CAR SILHOUETTE (one polygon, no gaps) ----
    // Going counter-clockwise from bottom-left:
    //   bottom flat → front bottom → front nose → hood top → windshield base →
    //   roof front → roof top → roof rear → rear window base → rear top → rear bottom → close
    glColor3f(r, g, b);
    glBegin(GL_POLYGON);
    glVertex2f(-5.2f,  0.3f);   // rear bottom
    glVertex2f( 5.0f,  0.3f);   // front bottom
    glVertex2f( 5.5f,  0.6f);   // front nose tip
    glVertex2f( 5.0f,  1.4f);   // front nose top (hood front edge)
    glVertex2f( 3.5f,  1.8f);   // hood rear / windshield base-front
    glVertex2f( 2.8f,  3.5f);   // windshield top / roof front
    glVertex2f(-2.8f,  3.5f);   // roof rear
    glVertex2f(-3.5f,  1.8f);   // rear pillar base (matches rear window base)
    glVertex2f(-5.2f,  1.8f);   // rear top
    glEnd();

    // ---- HOOD (slightly lighter shade, front sloped section) ----
    glColor3f(r * 0.93f, g * 0.93f, b * 0.93f);
    glBegin(GL_POLYGON);
    glVertex2f(3.5f, 1.8f);
    glVertex2f(5.0f, 1.4f);
    glVertex2f(5.5f, 0.6f);
    glVertex2f(5.0f, 0.3f);
    glVertex2f(3.0f, 0.3f);
    glVertex2f(3.0f, 1.8f);
    glEnd();

    // ---- ROOF PANEL (darker shade on top of cabin) ----
    glColor3f(r * 0.78f, g * 0.78f, b * 0.78f);
    glBegin(GL_POLYGON);
    glVertex2f(-2.8f, 3.5f);
    glVertex2f( 2.8f, 3.5f);
    glVertex2f( 2.8f, 3.8f);
    glVertex2f(-2.8f, 3.8f);
    glEnd();

    // ---- WINDSHIELD (front) ----
    // Base: x= 0.8 to 3.5  (wide at bottom, at hood level)
    // Top:  x= 0.8 to 2.8  (slopes inward on right = A-pillar lean)
    glColor3f(0.55f, 0.82f, 0.95f);
    glBegin(GL_POLYGON);
    glVertex2f( 0.8f, 1.85f);
    glVertex2f( 3.5f, 1.85f);
    glVertex2f( 2.8f, 3.45f);
    glVertex2f( 0.8f, 3.45f);
    glEnd();

    // ---- REAR WINDOW (mirror of windshield) ----
    // Base: x= -0.8 to -3.5  (wide at bottom)
    // Top:  x= -0.8 to -2.8  (slopes inward on left = C-pillar lean)
    glColor3f(0.50f, 0.76f, 0.92f);
    glBegin(GL_POLYGON);
    glVertex2f(-0.8f, 1.85f);
    glVertex2f(-3.5f, 1.85f);
    glVertex2f(-2.8f, 3.45f);
    glVertex2f(-0.8f, 3.45f);
    glEnd();

    // ---- SIDE WINDOW (middle, between A and C pillars) ----
    glColor3f(0.52f, 0.79f, 0.93f);
    drawRect(-0.75f, 1.85f, 0.75f, 3.45f);

    // ---- DOOR SEAM ----
    glColor3f(r * 0.65f, g * 0.65f, b * 0.65f);
    glLineWidth(1.1f);
    glBegin(GL_LINES);
    glVertex2f(-0.9f, 0.32f); glVertex2f(-0.9f, 1.80f);
    glEnd();
    glLineWidth(1.0f);

    // ---- BUMPERS ----
    glColor3f(r * 0.48f, g * 0.48f, b * 0.48f);
    drawRect( 5.0f, 0.28f,  5.8f, 0.62f);   // front bumper
    drawRect(-5.8f, 0.28f, -5.2f, 0.62f);   // rear bumper

    // ---- WHEELS (drawn last so they sit on top of fender area) ----
    // Fender arch (body color fills over silhouette bottom)
    glColor3f(r, g, b);
    drawCircleHelper(-3.2f, 0.32f, 1.05f, 24);
    drawCircleHelper( 3.2f, 0.32f, 1.05f, 24);
    // Tyre
    glColor3f(0.12f, 0.12f, 0.12f);
    drawCircleHelper(-3.2f, 0.22f, 0.88f, 24);
    drawCircleHelper( 3.2f, 0.22f, 0.88f, 24);
    // Alloy rim
    glColor3f(0.75f, 0.78f, 0.82f);
    drawCircleHelper(-3.2f, 0.22f, 0.48f, 20);
    drawCircleHelper( 3.2f, 0.22f, 0.48f, 20);
    // Centre hub
    glColor3f(0.92f, 0.93f, 0.96f);
    drawCircleHelper(-3.2f, 0.22f, 0.16f, 12);
    drawCircleHelper( 3.2f, 0.22f, 0.16f, 12);
    // Spokes
    glColor3f(0.58f, 0.60f, 0.64f);
    for (int si = 0; si < 5; si++) {
        float sa = si * PI * 2.0f / 5.0f;
        glBegin(GL_LINES);
        glVertex2f(-3.2f + 0.16f*cosf(sa), 0.22f + 0.16f*sinf(sa));
        glVertex2f(-3.2f + 0.46f*cosf(sa), 0.22f + 0.46f*sinf(sa));
        glVertex2f( 3.2f + 0.16f*cosf(sa), 0.22f + 0.16f*sinf(sa));
        glVertex2f( 3.2f + 0.46f*cosf(sa), 0.22f + 0.46f*sinf(sa));
        glEnd();
    }

    // ---- LIGHTS ----
    glColor3f(1.0f, 0.97f, 0.72f);
    drawCircleHelper(5.3f, 0.90f, 0.26f, 12);   // headlight
    glColor3f(0.95f, 0.08f, 0.08f);
    drawRect(-5.7f, 0.65f, -5.15f, 1.10f);      // taillight

    if (night) {
        glColor3f(1.0f, 0.94f, 0.58f);
        drawTriangle(5.8f, 0.55f, 5.8f, 1.35f, 10.5f, 0.95f);
    }
    glPopMatrix();
}

void drawRiverWithBoats() {
    // River width/height matched with fair field size: 20.5f
    glColor3f(0.06f, 0.15f, 0.28f);
    drawRect(0, 0, 100, 20.5f);

    glColor3f(0.18f, 0.38f, 0.55f);
    for (float y = 3; y < 20.0f; y += 3.5f) {
        glBegin(GL_LINE_STRIP);
        for (float x = 0; x <= 100; x += 4)
            glVertex2f(x, y + 0.45f * sinf(x * 0.25f + auroraPhase * 2.0f));
        glEnd();
    }

    glColor3f(0.78f, 0.78f, 0.80f);
    drawRect(0, 20.0f, 100, 20.5f);
}

void drawSpeedBoat(float x, float y, float scale, float r, float g, float b, bool faceRight = false) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(faceRight ? -scale : scale, scale, 1);

    // --- WATER REFLECTION (below boat, blended ripple) ---
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(r * 0.4f, g * 0.4f, b * 0.4f, 0.30f);
    glBegin(GL_POLYGON);
    glVertex2f(-5.5f, -0.2f); glVertex2f( 6.5f, -0.2f);
    glVertex2f( 5.5f, -1.2f); glVertex2f(-4.5f, -1.2f);
    glEnd();
    glDisable(GL_BLEND);

    // --- HULL (deep, pointed bow, flat stern) ---
    glColor3f(r * 0.55f, g * 0.55f, b * 0.55f);  // dark lower hull
    glBegin(GL_POLYGON);
    glVertex2f(-5.5f,  0.0f);   // stern bottom
    glVertex2f( 5.8f,  0.0f);   // bow bottom
    glVertex2f( 7.0f,  0.6f);   // bow tip
    glVertex2f( 5.5f,  1.0f);   // bow top
    glVertex2f(-5.5f,  1.0f);   // stern top
    glEnd();

    // Hull highlight stripe
    glColor3f(r, g, b);
    glBegin(GL_POLYGON);
    glVertex2f(-5.5f,  0.85f);
    glVertex2f( 5.5f,  0.85f);
    glVertex2f( 6.8f,  0.55f);
    glVertex2f( 5.8f,  0.10f);
    glVertex2f(-5.5f,  0.10f);
    glEnd();

    // Waterline stripe (white)
    glColor3f(0.90f, 0.92f, 0.95f);
    glBegin(GL_POLYGON);
    glVertex2f(-5.5f, 0.88f);
    glVertex2f( 5.6f, 0.88f);
    glVertex2f( 6.9f, 0.58f);
    glVertex2f( 6.6f, 0.52f);
    glVertex2f( 5.4f, 0.82f);
    glVertex2f(-5.5f, 0.82f);
    glEnd();

    // --- DECK ---
    glColor3f(0.55f, 0.48f, 0.36f);  // wooden deck
    drawRect(-5.2f, 1.0f, 5.2f, 1.3f);
    // Deck plank lines
    glColor3f(0.42f, 0.36f, 0.26f);
    glLineWidth(0.8f);
    for (float px = -4.5f; px < 5.0f; px += 1.2f) {
        glBegin(GL_LINES);
        glVertex2f(px, 1.0f); glVertex2f(px, 1.3f);
        glEnd();
    }
    glLineWidth(1.0f);

    // --- CABIN (main structure) ---
    glColor3f(0.72f, 0.74f, 0.78f);  // light grey cabin walls
    drawRect(-3.8f, 1.3f, 2.2f, 3.0f);

    // Cabin roof (slightly darker)
    glColor3f(0.38f, 0.40f, 0.44f);
    drawRect(-4.0f, 3.0f, 2.4f, 3.35f);

    // Cabin front slope (windshield angle)
    glColor3f(0.62f, 0.64f, 0.68f);
    glBegin(GL_POLYGON);
    glVertex2f(2.2f, 1.3f);
    glVertex2f(3.5f, 1.3f);
    glVertex2f(2.4f, 3.0f);
    glVertex2f(2.2f, 3.0f);
    glEnd();

    // --- CABIN WINDOWS (lit yellow-warm at night) ---
    // Front windshield glass
    glColor3f(0.95f, 0.88f, 0.55f);
    glBegin(GL_POLYGON);
    glVertex2f(2.25f, 1.45f);
    glVertex2f(3.30f, 1.45f);
    glVertex2f(2.45f, 2.85f);
    glVertex2f(2.25f, 2.85f);
    glEnd();
    // Side cabin windows (warm glow)
    glColor3f(1.00f, 0.90f, 0.50f);
    drawRect(-3.5f, 1.7f, -2.2f, 2.6f);
    drawRect(-1.8f, 1.7f, -0.5f, 2.6f);
    drawRect( 0.0f, 1.7f,  1.3f, 2.6f);
    // Window frames
    glColor3f(0.30f, 0.32f, 0.36f);
    glLineWidth(1.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-3.5f,1.7f); glVertex2f(-2.2f,1.7f); glVertex2f(-2.2f,2.6f); glVertex2f(-3.5f,2.6f);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex2f(-1.8f,1.7f); glVertex2f(-0.5f,1.7f); glVertex2f(-0.5f,2.6f); glVertex2f(-1.8f,2.6f);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex2f( 0.0f,1.7f); glVertex2f( 1.3f,1.7f); glVertex2f( 1.3f,2.6f); glVertex2f( 0.0f,2.6f);
    glEnd();

    // Window glow halos
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0f, 0.85f, 0.30f, 0.18f);
    drawCircleHelper(-2.85f, 2.15f, 0.9f, 16);
    drawCircleHelper(-1.15f, 2.15f, 0.9f, 16);
    drawCircleHelper( 0.65f, 2.15f, 0.9f, 16);
    glDisable(GL_BLEND);

    // --- MAST ---
    glColor3f(0.55f, 0.58f, 0.62f);
    drawRect(-0.12f, 3.35f, 0.12f, 6.2f);

    // Mast cross-arm
    glColor3f(0.50f, 0.52f, 0.56f);
    drawRect(-1.2f, 5.5f, 1.2f, 5.7f);

    // --- NAVIGATION LIGHTS ---
    // Red port light (left)
    glColor3f(0.95f, 0.08f, 0.08f);
    drawCircleHelper(-1.2f, 5.85f, 0.22f, 12);
    // Green starboard light (right)
    glColor3f(0.08f, 0.90f, 0.25f);
    drawCircleHelper( 1.2f, 5.85f, 0.22f, 12);
    // White masthead light
    glColor3f(1.0f, 1.0f, 0.95f);
    drawCircleHelper( 0.0f, 6.25f, 0.28f, 14);

    // Navigation light glows
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.95f, 0.08f, 0.08f, 0.22f);
    drawCircleHelper(-1.2f, 5.85f, 0.7f, 14);
    glColor4f(0.08f, 0.90f, 0.25f, 0.22f);
    drawCircleHelper( 1.2f, 5.85f, 0.7f, 14);
    glColor4f(1.0f, 1.0f, 0.85f, 0.22f);
    drawCircleHelper( 0.0f, 6.25f, 0.8f, 14);
    glDisable(GL_BLEND);

    // --- RADAR/ANTENNA on roof ---
    glColor3f(0.60f, 0.62f, 0.65f);
    drawRect(-0.06f, 3.35f, 0.06f, 4.2f);
    drawRect(-0.8f, 4.1f, 0.8f, 4.25f);

    // --- BOW LIGHT (front facing) ---
    glColor3f(1.0f, 0.96f, 0.70f);
    drawCircleHelper(6.8f, 0.65f, 0.25f, 12);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0f, 0.94f, 0.55f, 0.25f);
    drawTriangle(6.8f, 0.3f, 6.8f, 1.0f, 11.0f, 0.65f);
    glDisable(GL_BLEND);

    // --- ENGINE WAKE (white foam trail at stern) ---
    glColor3f(0.88f, 0.92f, 0.96f);
    for (int wi = 0; wi < 6; wi++) {
        float wx = -5.5f - wi * 1.4f;
        float wy = 0.4f + 0.3f * sinf(auroraPhase * 4.0f + wi * 0.8f);
        float wr = 0.4f - wi * 0.05f;
        drawCircleHelper(wx, wy, wr, 10);
    }
    // Wake V-lines
    glColor3f(0.80f, 0.86f, 0.92f);
    glLineWidth(1.2f);
    glBegin(GL_LINE_STRIP);
    glVertex2f(-5.5f, 0.5f);
    glVertex2f(-8.5f, 1.5f);
    glVertex2f(-12.0f, 1.8f);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2f(-5.5f, 0.5f);
    glVertex2f(-8.5f,-0.5f);
    glVertex2f(-12.0f,-0.8f);
    glEnd();
    glLineWidth(1.0f);

    glPopMatrix();
}

void drawSnowfall() {
    glColor3f(0.95f, 0.98f, 1.0f);
    for (int i = 0; i < 120; i++) {
        float x = fmodf(i * 17.37f + auroraPhase * 12.0f, 100.0f);
        float y = fmodf(i * 9.13f - auroraPhase * 18.0f, 80.0f);
        if (y < 24.0f) y += 24.0f;
        drawCircleHelper(x, y, 0.12f + (i % 3) * 0.06f, 8);
    }
}

//=================================  SCENE 1: DAY  =============================
void drawScene1() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    drawSky(0.55f, 0.82f, 0.96f);
    drawSun(83, 74, 2.7f);
    drawCloud(cloudX1, 73, 0.85f, 1,1,1);
    drawCloud(cloudX2, 69, 1.05f, 0.96f,0.96f,0.96f);
    drawCloud(cloudX3, 76, 0.75f, 0.98f,0.98f,1.0f);
    drawHotAirBalloonDay(28.0f + 0.4f * sinf(balloonRise * 0.5f), balloonY, 0.95f);

    drawBackgroundMountains(false);
    drawTownBuildings(false);

    drawCompactRailRoad();
    drawModernTrain(trainX - 120.0f, false);
    drawModernTrainRight(trainX2 - 70.0f, false);

    drawCompactRoad();
    drawStreetLamps(false);
    drawDisneyRoadBillboard(73.0f, 38.2f);
    drawCompactCar(carX1, 28.0f, 0.75f, 0.90f, 0.05f, 0.05f, true, false);
    drawCompactCar(carX2, 34.0f, 0.85f, 0.10f, 0.25f, 0.90f, false, false);
    drawCompactCar(carX3, 28.5f, 0.65f, 0.95f, 0.75f, 0.12f, true, false);
    drawAmbulance();

    drawProTree(14, 39, 0.80f, false);
    drawProTree(26, 39, 0.75f, false);
    drawProTree(55, 39, 0.85f, false);
    drawProTree(75, 39, 0.80f, false);
    drawProTree(96, 39, 0.70f, false);
    drawProTree(8,  39, 0.72f, false);
    drawProTree(35, 39, 0.78f, false);
    drawProTree(43, 39, 0.82f, false);
    drawProTree(65, 39, 0.76f, false);
    drawProTree(88, 39, 0.74f, false);
    drawPalmTree(38, 39, 0.75f);
    drawPalmTree(62, 39, 0.80f);
    drawPalmTree(20, 39, 0.72f);
    drawCherryTree(5, 39, 0.70f);
    drawCherryTree(85, 39, 0.75f);
    drawCherryTree(50, 39, 0.68f);
    drawAutumnTree(48, 39, 0.78f);
    drawAutumnTree(70, 39, 0.72f);
    drawAutumnTree(30, 39, 0.75f);

    drawEuropeanBanglaFair();

    drawSmallBird(bird1X, bird1Y, birdWing);
    drawSmallBird(bird2X, bird2Y, birdWing + 0.8f);
    drawSmallBird(bird3X, bird3Y, birdWing + 1.6f);
    drawSmallBird(bird4X, bird4Y, birdWing + 2.4f);
    drawSmallBird(bird5X, bird5Y, birdWing + 3.2f);

    drawNavButtons();
    glutSwapBuffers();
}

//=================================  SCENE 2: NIGHT / AURORA  =============================

void drawMoon(float x, float y, float r) {
    glColor3f(1.0f, 1.0f, 0.8f);
    drawCircleHelper(x, y, r, 80);
    glColor3f(0.05f, 0.05f, 0.2f);
    drawCircleHelper(x + r * 0.2f, y + r * 0.1f, r * 0.85f, 80);
}

void drawStars() {
    glColor3f(1.0f, 1.0f, 1.0f);
    srand(42);
    for (int i = 0; i < 100; i++) {
        float x = rand() % 100;
        float y = 55 + rand() % 25;
        drawCircleHelper(x, y, 0.18f, 8);
    }
}

void drawAurora() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    int steps = 100;
    float dx = 100.0f / steps;

    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= steps; i++) {
        float x = i * dx;
        float t = (float)i / steps;
        float bot = 56.0f + 2.8f*sinf(auroraPhase + t*5.5f) + 1.5f*sinf(auroraPhase*1.3f+t*9.0f);
        float top = 68.0f + 3.5f*sinf(auroraPhase*0.9f+t*4.5f+0.8f);
        float bright = 0.55f + 0.25f*sinf(auroraPhase*1.1f+t*6.0f);
        glColor4f(0.05f, bright, 0.35f, bright*0.55f); glVertex2f(x, bot);
        glColor4f(0.05f, bright*0.7f, 0.4f, 0.0f);     glVertex2f(x, top);
    }
    glEnd();
    glDisable(GL_BLEND);
}

void drawScene2() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    drawSky(0.04f,0.05f,0.16f);
    drawStars();
    drawAurora();
    drawMoon(12, 76, 2.5f);
    drawCloud(cloudX1,74,0.85f, 0.26f,0.28f,0.38f);
    drawCloud(cloudX2,69,1.0f, 0.22f,0.24f,0.34f);

    drawBackgroundMountains(true);
    drawTownBuildings(true);

    drawCompactRailRoad();
    drawModernTrain(trainX - 120.0f, true);
    drawModernTrainRight(trainX2 - 70.0f, true);

    drawCompactRoad();
    drawStreetLamps(true);
    drawDisneyRoadBillboard(73.0f, 38.2f);
    drawCompactCar(carX1, 28.0f, 0.72f, 0.70f, 0.08f, 0.10f, true, true);
    drawCompactCar(carX2, 34.0f, 0.82f, 0.08f, 0.16f, 0.52f, false, true);
    drawCompactCar(carX3, 28.5f, 0.65f, 0.90f, 0.62f, 0.08f, true, true);

    drawProTree(14, 39, 0.80f, true);
    drawProTree(26, 39, 0.75f, true);
    drawProTree(55, 39, 0.85f, true);
    drawProTree(75, 39, 0.80f, true);
    drawProTree(96, 39, 0.70f, true);
    drawProTree(8,  39, 0.72f, true);
    drawProTree(35, 39, 0.78f, true);
    drawProTree(43, 39, 0.82f, true);
    drawProTree(65, 39, 0.76f, true);
    drawProTree(88, 39, 0.74f, true);
    drawPalmTree(38, 39, 0.75f);
    drawPalmTree(62, 39, 0.80f);
    drawPalmTree(20, 39, 0.72f);
    drawCherryTree(5, 39, 0.70f);
    drawCherryTree(85, 39, 0.75f);
    drawCherryTree(50, 39, 0.68f);
    drawAutumnTree(48, 39, 0.78f);
    drawAutumnTree(70, 39, 0.72f);
    drawAutumnTree(30, 39, 0.75f);

    drawRiverWithBoats();
    drawSpeedBoat(fmodf(trainX2 * 0.85f, 120.0f) - 10.0f, 15.5f, 0.90f, 0.05f, 0.55f, 0.95f);
    drawSpeedBoat(fmodf(trainX * 0.65f, 130.0f) - 15.0f, 9.0f, 0.78f, 0.90f, 0.05f, 0.10f, true);
    drawSpeedBoat(fmodf(carX1 * 1.10f, 115.0f) - 8.0f, 4.8f, 0.65f, 0.95f, 0.75f, 0.05f);

    drawSnowfall();

    drawNavButtons();
    glutSwapBuffers();
}

//  DISPLAY CALLBACK
void display() {
    switch (currentScene) {
        case 1: drawScene1(); break;
        case 2: drawScene2(); break;
    }
}

//  TIMER CALLBACKS

// Night lights use a slower counter since they blink at ~2000ms intervals
static int nightLightCounter = 0;
const int NIGHT_LIGHT_TICKS = 67; // 67 * 30ms ≈ 2000ms

void update(int value) {
    // --- Cars, ambulance, stall animations ---
    carX1 += carSpeed1;
    if (carX1 > 120.0f) carX1 = -30.0f;

    carX2 += carSpeed2;
    if (carX2 < -30.0f) carX2 = 120.0f;

    carX3 += carSpeed3;
    if (carX3 > 160.0f) carX3 = -40.0f;

    ambulanceX += ambulanceSpeed;
    if (ambulanceX > 130.0f) ambulanceX = -20.0f;

    if (!teaBuyerAtStall) {
        teaBuyerX  += 0.12f;
        teaBuyerLeg += 0.20f;
        if (teaBuyerX >= 8.5f) { teaBuyerAtStall = true; teaBuyerLeg = 0; }
    } else {
        teaArmSwing += 0.15f;
        if (teaArmSwing > 6.28f) { teaBuyerAtStall = false; teaBuyerX = 0.0f; teaArmSwing = 0; }
    }

    badamArmAngle += 0.18f;
    if (badamState == 0) {
        badamBuyerX  += 0.10f;
        badamBuyerLeg += 0.20f;
        if (badamBuyerX >= 56.5f) { badamState = 1; badamBuyerLeg = 0; }
    } else if (badamState == 1) {
        static int badamWait = 0;
        badamWait++;
        if (badamWait >= 60) { badamState = 2; badamBuyerX = 56.5f; badamWait = 0; }
    } else {
        badamBuyerX  -= 0.12f;
        badamBuyerLeg += 0.20f;
        if (badamBuyerX <= 50.0f) { badamState = 0; badamBuyerX = 54.0f; badamBuyerLeg = 0; }
    }

    if (iceState == 0) {
        iceCustomerX  += 0.10f;
        iceCustomerLeg += 0.20f;
        if (iceCustomerX >= 82.5f) { iceState = 1; iceCustomerLeg = 0; iceArmSwing = 0; }
    } else if (iceState == 1) {
        iceArmSwing += 0.14f;
        if (iceArmSwing > 6.28f) { iceState = 2; iceCustomerX = 82.5f; }
    } else {
        iceCustomerX  -= 0.12f;
        iceCustomerLeg += 0.20f;
        if (iceCustomerX <= 72.0f) { iceState = 0; iceCustomerX = 78.0f; iceCustomerLeg = 0; }
    }

    // --- Trains ---
    trainX += -0.9f;
    if (trainX < -40.0f) trainX = 240.0f;
    trainX2 += 0.9f;
    if (trainX2 > 240.0f) trainX2 = -40.0f;

    // --- Sun ---
    sunAngle += 0.01f;
    if (sunAngle > 2*PI) sunAngle -= 2*PI;

    // --- Birds ---
    birdWing += 0.18f;
    if (birdWing > 2.0f * PI) birdWing -= 2.0f * PI;

    bird1X += 0.22f;
    bird1Y += 0.008f * sinf(birdWing * 0.7f);
    if (bird1X > 115.0f) { bird1X = -10.0f; bird1Y = 71.0f + (rand()%5); }

    bird2X += 0.15f;
    bird2Y += 0.010f * sinf(birdWing * 0.5f + 1.0f);
    if (bird2X > 115.0f) { bird2X = -15.0f; bird2Y = 68.0f + (rand()%5); }

    bird3X -= 0.18f;
    bird3Y += 0.007f * sinf(birdWing * 0.6f + 2.0f);
    if (bird3X < -15.0f) { bird3X = 115.0f; bird3Y = 73.0f + (rand()%4); }

    bird4X += 0.28f;
    bird4Y += 0.009f * sinf(birdWing * 0.8f + 0.5f);
    if (bird4X > 115.0f) { bird4X = -20.0f; bird4Y = 67.0f + (rand()%5); }

    bird5X -= 0.12f;
    bird5Y += 0.006f * sinf(birdWing * 0.4f + 1.5f);
    if (bird5X < -15.0f) { bird5X = 115.0f; bird5Y = 70.0f + (rand()%5); }

    // --- Pedestrians ---
    pedTimer += 0.05f;
    if (pedTimer > 2*PI) pedTimer -= 2*PI;

    // --- Aurora ---
    auroraPhase += 0.018f;
    if (auroraPhase > 2*PI) auroraPhase -= 2*PI;

    // --- Clouds & Balloon ---
    cloudX1 += 0.06f; cloudX2 -= 0.12f; cloudX3 += 0.09f;
    if (cloudX1 > 110) cloudX1 = -30;
    if (cloudX2 < -20) cloudX2 = 120;
    if (cloudX3 > 110) cloudX3 = -35;
    balloonRise += 0.025f;
    balloonY += 0.015f;
    if (balloonY > 76.0f) balloonY = 62.0f;

    // --- Ferris wheel ---
    ferrisAngle += 0.008f;
    if (ferrisAngle > 2 * PI) ferrisAngle -= 2 * PI;

    // --- Night lights (slow blink, ~every 2 seconds) ---
    nightLightCounter++;
    if (nightLightCounter >= NIGHT_LIGHT_TICKS) {
        nightLightCounter = 0;
        if (currentScene == 2) nightLight = !nightLight;
    }

    glutPostRedisplay();
    glutTimerFunc(30, update, 0);
}

// NAV BUTTONS (empty - navigation via keyboard/mouse)
void drawNavButtons() {}

//  KEYBOARD
void handleSpecialKey(int key, int x, int y) {
    if (key == GLUT_KEY_UP) {
        if (currentScene < 2) { currentScene++; glutPostRedisplay(); }
    }
    if (key == GLUT_KEY_DOWN) {
        if (currentScene > 1) { currentScene--; glutPostRedisplay(); }
    }
}

void handleMouse(int button, int state, int mx, int my) {
    if (button != GLUT_LEFT_BUTTON || state != GLUT_UP) return;

    int winW = glutGet(GLUT_WINDOW_WIDTH);
    int winH = glutGet(GLUT_WINDOW_HEIGHT);
    float wx = (float)mx / winW * 100.0f;
    float wy = (float)(winH - my) / winH * 80.0f;

    float btnW = 7.0f, btnH = 5.0f, btnY = 0.5f;
    float lx = 1.0f;
    float rx = 100.0f - 1.0f - btnW;

    if (wx >= lx && wx <= lx+btnW && wy >= btnY && wy <= btnY+btnH) {
        if (currentScene > 1) { currentScene--; glutPostRedisplay(); }
    }
    if (wx >= rx && wx <= rx+btnW && wy >= btnY && wy <= btnY+btnH) {
        if (currentScene < 2) { currentScene++; glutPostRedisplay(); }
    }
}

void handleKeypress(unsigned char key, int x, int y) {
    if (key == 0) exit(0);
    if (currentScene == 1) {
        if (key=='w'||key=='W') { billRotX -= 5.0f; if(billRotX<-89) billRotX=-89; glutPostRedisplay(); }
        if (key=='s'||key=='S') { billRotX += 5.0f; if(billRotX> 89) billRotX= 89; glutPostRedisplay(); }
        if (key=='a'||key=='A') { billRotY += 5.0f; glutPostRedisplay(); }
        if (key=='d'||key=='D') { billRotY -= 5.0f; glutPostRedisplay(); }
        if (key=='q'||key=='Q') { billZoom -= 0.5f; if(billZoom<2.0f)  billZoom=2.0f;  glutPostRedisplay(); }
        if (key=='e'||key=='E') { billZoom += 0.5f; if(billZoom>18.0f) billZoom=18.0f; glutPostRedisplay(); }
    }
}

//  RESHAPE
void handleResize(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 100.0, 0.0, 80.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1280, 720);
    glutCreateWindow("City View - Day / Night");
    glClearColor(0,0,0,1);
    glutDisplayFunc(display);
    glutSpecialFunc(handleSpecialKey);
    glutKeyboardFunc(handleKeypress);
    glutMouseFunc(handleMouse);
    glutReshapeFunc(handleResize);

    glutTimerFunc(0, update, 0);

    glutMainLoop();
    return 0;
}
