#pragma once
#include "AlicePrefix.h"

//定义顶点
struct Vertice{
    float mPosition[4]; //x, y, z, w
//    float mColor[4];//r, g, b, a
    float mTexcoord[4];//u,v,r,q纹理坐标
};

unsigned  char* LoadFileContent(AAssetManager *sAssetManager, const char* path, int& fileSize);

GLuint CompileShader(GLenum shader_type, const char *source_code);

GLuint CreateProgram(GLuint vertex_shader, GLuint fragment_shader);

GLuint CreateStandardProgram(AAssetManager *sAssetManager, const char *vertex_shader_path, const char *fragment_shader_path);

GLuint CreateBufferObject(GLenum type, void *data, int size, GLenum usage);

unsigned char* DecodeBMP(unsigned  char* bmp_fileContent, int& width, int& height);

GLuint CreateTextureFromFile(AAssetManager *sAssetManager, const char *path);

void UpdateBufferObject(GLuint object, GLenum type, void *data, int size, int offset = 0);

float GetFrameTime();

