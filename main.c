//---------------------------------------------------------
// file:	main.c
// author:	[NAME]
// email:	[DIGIPEN EMAIL ADDRESS]
//
// brief:	Main entry point for the sample project
//			of the CProcessing library
//
// documentation link:
// https://github.com/DigiPen-Faculty/CProcessing/wiki
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#include "cprocessing.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define CUBE_POINTS_ARRAY_SIZE 8
#define CUBES_ARRAY_SIZE 10

CP_Vector position;
CP_Matrix translate;
CP_Matrix scale;
CP_Matrix rotate;

float scalarValue;
float thetaX, thetaY, thetaZ;
float thetaIncrement;

typedef struct {
    float xPos;
    float yPos;
    float zPos;
    float width;
    float height;
    float depth;
    CP_Matrix points3D[CUBE_POINTS_ARRAY_SIZE];
    CP_Vector points2D[CUBE_POINTS_ARRAY_SIZE];
} Cube;

CP_Matrix projMatrix;

Cube cubes[CUBES_ARRAY_SIZE] = { 0 };

void game_exit(void);
Cube projectPoints(Cube);

CP_Matrix newVec3(float x, float y, float z) {
    return CP_Matrix_Set(
        x, 0, 0,
        y, 0, 0,
        z, 0, 0
    );
}

Cube newCube(float x, float y, float z, float w, float h, float d) {
    Cube c = { 0 };
    c.points3D[0] = newVec3(x - w / 2, y - h / 2, z - d / 2);
    c.points3D[1] = newVec3(x + w / 2, y - h / 2, z - d / 2);
    c.points3D[2] = newVec3(x + w / 2, y - h / 2, z + d / 2);
    c.points3D[3] = newVec3(x - w / 2, y - h / 2, z + d / 2);
    c.points3D[4] = newVec3(x - w / 2, y + h / 2, z - d / 2);
    c.points3D[5] = newVec3(x + w / 2, y + h / 2, z - d / 2);
    c.points3D[6] = newVec3(x + w / 2, y + h / 2, z + d / 2);
    c.points3D[7] = newVec3(x - w / 2, y + h / 2, z + d / 2);

    c.xPos = x;
    c.yPos = y;
    c.zPos = z;
    c.depth = d;
    c.height = h;
    c.width = w;

    c = projectPoints(c);

    return c;
}


CP_Matrix rotationX() {
    return CP_Matrix_Set(
        1, 0, 0, 
        0, cos(thetaX), -sin(thetaX), 
        0, sin(thetaX), cos(thetaX));
}

CP_Matrix rotationY() {
    return CP_Matrix_Set(
        cos(thetaY), 0, sin(thetaY), 
        0, 1, 0, 
        -sin(thetaY), 0, cos(thetaY));
}

CP_Matrix rotationZ() {
    return CP_Matrix_Set(
        cos(thetaZ), -sin(thetaZ), 0,
        sin(thetaZ), cos(thetaZ), 0,
        0, 0, 1);
}

Cube projectPoints(Cube c) {
    for (int i = 0; i < CUBE_POINTS_ARRAY_SIZE; i++) {
        CP_Matrix projectionMultipled = CP_Matrix_Multiply(
            projMatrix,
            CP_Matrix_Multiply(
                rotationX(),
                CP_Matrix_Multiply(
                    rotationY(),
                    CP_Matrix_Multiply(
                        rotationZ(),
                        c.points3D[i]
                    )
                )
            )
        );
        c.points2D[i] = CP_Vector_Set(projectionMultipled.m00, projectionMultipled.m10);
    }
    return c;
}

void game_init(void)
{
    //CP_System_Fullscreen();
    CP_System_SetWindowSize(800, 800);

    position = CP_Vector_Set(CP_System_GetWindowWidth() * 0.5f, CP_System_GetWindowHeight() * 0.5f);
    // Create a transform matrix
    scalarValue = 1.0f;
    rotate = CP_Matrix_Rotate(0.0f);
    thetaX = 0;
    thetaY = 0;
    thetaZ = 0;
    thetaIncrement = 0.02f;

    projMatrix = CP_Matrix_Set(1, 0, 0, 0, 1, 0, 0, 0, 0);

    cubes[0] = newCube(0, 0, 0, 100, 100, 100);

	CP_Settings_RectMode(CP_POSITION_CENTER);
	CP_Settings_NoFill();
    CP_Settings_Stroke(CP_Color_Create(255, 255, 255, 255));
    CP_Settings_StrokeWeight(1.0f);
    CP_Settings_TextSize(25.0f);
    CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
}

void drawCubeLines(Cube c) {
    CP_Graphics_DrawLine(c.points2D[0].x, c.points2D[0].y, c.points2D[1].x, c.points2D[1].y);
    CP_Graphics_DrawLine(c.points2D[0].x, c.points2D[0].y, c.points2D[3].x, c.points2D[3].y);
    CP_Graphics_DrawLine(c.points2D[0].x, c.points2D[0].y, c.points2D[4].x, c.points2D[4].y);
    CP_Graphics_DrawLine(c.points2D[1].x, c.points2D[1].y, c.points2D[2].x, c.points2D[2].y);
    CP_Graphics_DrawLine(c.points2D[1].x, c.points2D[1].y, c.points2D[5].x, c.points2D[5].y);
    CP_Graphics_DrawLine(c.points2D[2].x, c.points2D[2].y, c.points2D[3].x, c.points2D[3].y);
    CP_Graphics_DrawLine(c.points2D[2].x, c.points2D[2].y, c.points2D[6].x, c.points2D[6].y);
    CP_Graphics_DrawLine(c.points2D[7].x, c.points2D[7].y, c.points2D[3].x, c.points2D[3].y);
    CP_Graphics_DrawLine(c.points2D[7].x, c.points2D[7].y, c.points2D[4].x, c.points2D[4].y);
    CP_Graphics_DrawLine(c.points2D[7].x, c.points2D[7].y, c.points2D[6].x, c.points2D[6].y);
    CP_Graphics_DrawLine(c.points2D[5].x, c.points2D[5].y, c.points2D[4].x, c.points2D[4].y);
    CP_Graphics_DrawLine(c.points2D[5].x, c.points2D[5].y, c.points2D[6].x, c.points2D[6].y);
}

int getCloesestVertexIndex(Cube c) {
    /*
        if COS(ThetaY) is positive, RED FACE    (index 0154)
        if COS(ThetaY) is negative, MAGETA FACE (index 2367)

        if SIN(ThetaY) is positive, YELLOW FACE (index 1256)
        if SIN(ThetaY) is negative, CYAN FACE   (index 3047)

        if SIN(ThetaX) is positive, GREEN FACE  (index 4567)
        if SIN(ThetaX) is negative, BLUE FACE   (index 0123)

        We should have 6 inputs, 3 outputs, then we'll compare them and should get one vertex.

        Examples:
            pos pos pos = R Y G = 0154, 1256, 4567 | Compare Numbers: 0154 15 5 | 5 is the only one all three share, that's our vertex.

            neg pos neg = M Y B = 2367, 1256, 0123 | Compare Numbers: 2367 26 2 | 2 is our vertex.


        Actually, I realized this is all global math and shouldn't *need* a cube passed in.
        I'm going to list all possibilities and see what happens

        ppp = 5
        ppn = 1
        pnp = 4
        pnn = 0
        npp = 6
        npn = 2
        nnp = 7
        nnn = 3

        Start with the end and work our way backwards to figure out the algorithm:
            If the LAST condition is Positive, add 4:
            PPN = 1, PPP = 5 | PNN = 0, PNP = 4 | etc.

        Now we have the following sequence:
            PP = 1 | PN = 0 | NP = 2 | NN = 3
        
        Interesting. The difference from PP to PN is -1, but from NP to NN is +1. 
        Realistically, I think this might be because of how I coded it - this math issue is my own fault.
        But what I do see is that if the first output is negative and the value is *actually negative*, 
            then I can always subtract one (getting 1-1 = 0 or -2-1 = -3) then take the ABS.

            If the SECOND condition is negative, subtract one.
            Then, either sign, take ABS:
            P =  1  ->  ABS( 1 - 1) = ABS( 0) = 0
            N = -2  ->  ABS(-2 - 1) = ABS(-3) = 3
        
        Lastly, just a "set" statement. I am disappointed that the math isn't super easy starting at 0. 
        Like, if I had positive : +1; negative : -1; but since negative is -2, we're just in force-declaration teritory. 
        I'm also not going to spend too much time figuring this out for now. 
        I feel like I'll need to come back to it later when my camera starts moving.

    */
    int index = (cos(thetaY) >= 0) ? 1 : -2;
    index = (sin(thetaY) >= 0) ? abs(index) : abs(index - 1);
    index = (sin(thetaX) >= 0) ? index : index + 4;
    return index;
}

void drawCubeFaces(Cube c) {

    int index = getCloesestVertexIndex(c);

    char buffer[50] = {0};

    sprintf_s(buffer, _countof(buffer), "Winning Vertex: %d", index);
    CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
    CP_Font_DrawText(buffer, 0, -150);

    if (index == 0 || index == 1 || index == 5 || index == 4) {
        CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
        CP_Graphics_DrawQuad(
            c.points2D[0].x, c.points2D[0].y,
            c.points2D[1].x, c.points2D[1].y,
            c.points2D[5].x, c.points2D[5].y,
            c.points2D[4].x, c.points2D[4].y);
    }

    if (index == 2 || index == 1 || index == 5 || index == 6) {
        CP_Settings_Fill(CP_Color_Create(255, 255, 0, 255));
        CP_Graphics_DrawQuad(
            c.points2D[1].x, c.points2D[1].y,
            c.points2D[2].x, c.points2D[2].y,
            c.points2D[6].x, c.points2D[6].y,
            c.points2D[5].x, c.points2D[5].y);
    }

    if (index == 2 || index == 3 || index == 7 || index == 6) {
        CP_Settings_Fill(CP_Color_Create(255, 0, 255, 255));
        CP_Graphics_DrawQuad(
            c.points2D[2].x, c.points2D[2].y,
            c.points2D[3].x, c.points2D[3].y,
            c.points2D[7].x, c.points2D[7].y,
            c.points2D[6].x, c.points2D[6].y);
    }
    
    if (index == 0 || index == 3 || index == 7 || index == 4) {
        CP_Settings_Fill(CP_Color_Create(0, 255, 255, 255));
        CP_Graphics_DrawQuad(
            c.points2D[3].x, c.points2D[3].y,
            c.points2D[0].x, c.points2D[0].y,
            c.points2D[4].x, c.points2D[4].y,
            c.points2D[7].x, c.points2D[7].y);
    }
    
    if (index == 0 || index == 1 || index == 2 || index == 3) {
        CP_Settings_Fill(CP_Color_Create(0, 0, 255, 255));
        CP_Graphics_DrawQuad(
            c.points2D[0].x, c.points2D[0].y,
            c.points2D[1].x, c.points2D[1].y,
            c.points2D[2].x, c.points2D[2].y,
            c.points2D[3].x, c.points2D[3].y);
    }
    
    if (index == 7 || index == 6 || index == 5 || index == 4) {
        CP_Settings_Fill(CP_Color_Create(0, 255, 0, 255));
        CP_Graphics_DrawQuad(
            c.points2D[4].x, c.points2D[4].y,
            c.points2D[5].x, c.points2D[5].y,
            c.points2D[6].x, c.points2D[6].y,
            c.points2D[7].x, c.points2D[7].y);
    }
        
}

void game_update(void)
{
    CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));

    scale = CP_Matrix_Scale(CP_Vector_Set(scalarValue, scalarValue));
    translate = CP_Matrix_Translate(position);

    CP_Matrix transform = CP_Matrix_Multiply(translate, CP_Matrix_Multiply(rotate, scale));

    CP_Settings_ApplyMatrix(transform);
    //drawRectAsQuad(0, 0, 100, 100);

    char buffer[50] = { 0 };
    

    for (int i = 0; i < CUBE_POINTS_ARRAY_SIZE; i++) {
        CP_Vector curP = cubes[0].points2D[i];
        CP_Graphics_DrawCircle(curP.x, curP.y, 7);

        //sprintf_s(buffer, _countof(buffer), "Cube Point %d: %.1f, %.1f", i, curP.x, curP.y);
        //CP_Font_DrawText(buffer, 50, 70+40*(i+1));
    }
    drawCubeLines(cubes[0]);
    //drawCubeFaces(cubes[0]);

    cubes[0] = projectPoints(cubes[0]);

    /*********\
    | CONTROL |
    \*********/
    if (CP_Input_KeyReleased(KEY_ESCAPE)) CP_Engine_Terminate();
    if (CP_Input_KeyReleased(KEY_R)) CP_Engine_SetNextGameStateForced(game_init, game_update, game_exit);

    if (CP_Input_KeyDown(KEY_UP)) { 
        thetaX += thetaIncrement * cos(thetaY);
        thetaZ += thetaIncrement * sin(thetaY);
    }
    if (CP_Input_KeyDown(KEY_DOWN)) {
        thetaX -= thetaIncrement * cos(thetaY);
        thetaZ -= thetaIncrement * sin(thetaY);
    }
    if (CP_Input_KeyDown(KEY_W)) {
        thetaX += 0.02f;
    }
    if (CP_Input_KeyDown(KEY_S)) {
        thetaZ += 0.02f;
    }

    if (CP_Input_KeyDown(KEY_A) || CP_Input_KeyDown(KEY_LEFT))  {
        thetaY -= thetaIncrement * cos(thetaX);
        thetaZ -= thetaIncrement * sin(thetaX);
    }
    if (CP_Input_KeyDown(KEY_D) || CP_Input_KeyDown(KEY_RIGHT)) {
        thetaY += thetaIncrement * cos(thetaX);
        thetaZ += thetaIncrement * sin(thetaX);
    }



    sprintf_s(buffer, _countof(buffer), "cos(tY): %.2f", cos(thetaY));
    CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
    CP_Font_DrawText(buffer, 0, 110);


    sprintf_s(buffer, _countof(buffer), "sin(tY): %.2f", sin(thetaY));
    CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
    CP_Font_DrawText(buffer, 0, 150);

    sprintf_s(buffer, _countof(buffer), "sin(tX): %.2f", sin(thetaX));
    CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
    CP_Font_DrawText(buffer, 0, 230);

    sprintf_s(buffer, _countof(buffer), "cos(tX): %.2f", cos(thetaX));
    CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
    CP_Font_DrawText(buffer, 0, 190);

    sprintf_s(buffer, _countof(buffer), "sin(tZ): %.2f", sin(thetaZ));
    CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
    CP_Font_DrawText(buffer, 0, 270);

    sprintf_s(buffer, _countof(buffer), "cos(tZ): %.2f", cos(thetaZ));
    CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
    CP_Font_DrawText(buffer, 0, 310);
}

void game_exit(void) {
}

int main(void)
{
	CP_Engine_SetNextGameState(game_init, game_update, game_exit);
	CP_Engine_Run();
	return 0;
}
