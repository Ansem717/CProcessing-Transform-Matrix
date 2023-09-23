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
#include <math.h>

#define CUBE_POINTS_ARRAY_SIZE 8

CP_Vector position;
CP_Matrix translate;
CP_Matrix scale;
CP_Matrix rotate;

float scalarValue;
float theta;

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
Cube cubes[10] = { 0 };

void game_exit(void);
void projectPoints(Cube);

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

    projectPoints(c);

    return c;
}

void projectPoints(Cube c) {
    for (int i = 0; i < CUBE_POINTS_ARRAY_SIZE; i++) {
        CP_Matrix projectionMultipled = CP_Matrix_Multiply(projMatrix, c.points3D[i]);
        c.points2D[i] = CP_Vector_Set(projectionMultipled.m00, projectionMultipled.m10);
    }
}

void game_init(void)
{
    CP_System_Fullscreen();

    position = CP_Vector_Set(CP_System_GetWindowWidth() * 0.5f, CP_System_GetWindowHeight() * 0.5f);
    // Create a transform matrix
    scalarValue = 1.0f;
    rotate = CP_Matrix_Rotate(0.0f);
    theta = 0;

    projMatrix = CP_Matrix_Set(1, 0, 0, 0, 1, 0, 0, 0, 0);

    cubes[0] = newCube(0, 0, 0, 100, 100, 100);

	CP_Settings_RectMode(CP_POSITION_CENTER);
	CP_Settings_NoFill();
    CP_Settings_Stroke(CP_Color_Create(255, 255, 255, 255));
    CP_Settings_StrokeWeight(1.0f);
}

void drawRectAsQuad(float x, float y, float w, float h) {
    float w2 = w / 2;
    float h2 = h / 2;
    CP_Graphics_DrawQuad(x - w2, y - h2 - tan(theta)*w, x - w2, y + h2 + tan(theta)*w, x + h2, y + h2 + tan(theta) * w, x + h2, y - h2 - tan(theta) * w);
}

void game_update(void)
{
    CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));

    scale = CP_Matrix_Scale(CP_Vector_Set(scalarValue, scalarValue));
    translate = CP_Matrix_Translate(position);

    CP_Matrix transform = CP_Matrix_Multiply(translate, CP_Matrix_Multiply(rotate, scale));

    //CP_Settings_ApplyMatrix(transform);
    //drawRectAsQuad(0, 0, 100, 100);



    /*********\
    | CONTROL |
    \*********/
    if (CP_Input_KeyReleased(KEY_ESCAPE)) CP_Engine_Terminate();
    if (CP_Input_KeyReleased(KEY_R)) CP_Engine_SetNextGameStateForced(game_init, game_update, game_exit);

    if (CP_Input_KeyDown(KEY_W) || CP_Input_KeyDown(KEY_UP))    scalarValue += 0.05f;
    if (CP_Input_KeyDown(KEY_S) || CP_Input_KeyDown(KEY_DOWN))  scalarValue -= 0.05f;
    if (CP_Input_KeyDown(KEY_A) || CP_Input_KeyDown(KEY_LEFT))  {
        position.x -= 15.0f;
        theta += CP_Math_Radians(1.0f);
    }
    if (CP_Input_KeyDown(KEY_D) || CP_Input_KeyDown(KEY_RIGHT)) {
        position.x += 15.0f;
        theta -= CP_Math_Radians(1.0f);
    }


}

void game_exit(void)
{
}

int main(void)
{
	CP_Engine_SetNextGameState(game_init, game_update, game_exit);
	CP_Engine_Run();
	return 0;
}
