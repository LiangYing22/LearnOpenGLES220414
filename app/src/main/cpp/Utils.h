#pragma once
#include "AlicePrefix.h"

//定义顶点
struct Vertice{
    float mPosition[4]; //x, y, z, w
};

unsigned  char* LoadFileContent(AAssetManager *sAssetManager, const char* path, int& fileSize);

float GetFrameTime();

