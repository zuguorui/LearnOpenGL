

#include<iostream>

#include "DrawPractice.h"
#include "TexturePractice.h"
#include "Transform.h"
#include "CoordSystemTest.h"

using namespace std;

// https://learnopengl-cn.readthedocs.io/zh/latest

int main()
{
    //drawTriangle();
    //drawRectangle();
    //moreProperties();
    //drawTexture();

    // Transform transform;
    // transform.draw();

    CoordSystemTest coordSystemTest;
    coordSystemTest.draw();
    return 0;
}
