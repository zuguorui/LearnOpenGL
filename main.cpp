

#include<iostream>

#include "DrawPractice.h"
#include "TexturePractice.h"
#include "Transform.h"

using namespace std;

// https://learnopengl-cn.readthedocs.io/zh/latest

int main()
{
    //drawTriangle();
    //drawRectangle();
    //moreProperties();
    //drawTexture();

    Transform transform;
    transform.draw();
    return 0;
}
