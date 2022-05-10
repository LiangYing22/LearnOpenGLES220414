#include "Sence.h"
#include "Utils.h"

static AAssetManager *sAssetManager = nullptr;


extern "C" JNIEXPORT jstring JNICALL
Java_com_innup_learnopengles220414_MainActivity_testScence(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT void JNICALL
Java_com_innup_learnopengles220414_MainActivity_onSurfaceCreated(JNIEnv *env, jobject thiz, jobject am) {
    __android_log_print(ANDROID_LOG_INFO, ALICE_LOG_TAG, "onSurfaceCreated");
    sAssetManager = AAssetManager_fromJava(env, am);
    glClearColor(0.1f, 0.4f, 0.6f, 1.0f);
    //定义三个顶点（每个顶点有四个坐标），此时定位的数据还在 CPU 上。
    Vertice vertices[3];
    //设置第一个顶点数据
    vertices[0].mPosition[0] = -0.5f;
    vertices[0].mPosition[1] = -0.5f;
    vertices[0].mPosition[2] = -2.0f;
    vertices[0].mPosition[3] = 1.0f;
    //设置第二个顶点数据
    vertices[1].mPosition[0] = 0.5f;
    vertices[1].mPosition[1] = -0.5f;
    vertices[1].mPosition[2] = -2.0f;
    vertices[1].mPosition[3] = 1.0f;
    //设置第三个顶点数据
    vertices[2].mPosition[0] = 0.0f;
    vertices[2].mPosition[1] = 0.5f;
    vertices[2].mPosition[2] = -2.0f;
    vertices[2].mPosition[3] = 1.0f;


    int fileSize = 0;
    unsigned char * fileContent = LoadFileContent(sAssetManager, "test.txt", fileSize);
    if(fileContent != nullptr){
        __android_log_print(ANDROID_LOG_INFO, ALICE_LOG_TAG, "[%s]", (char *)fileContent);
        delete [] fileContent;
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_innup_learnopengles220414_MainActivity_onSurfaceChanged(JNIEnv *env, jobject thiz,
                                                                 jint width, jint height) {
    __android_log_print(ANDROID_LOG_INFO, ALICE_LOG_TAG, "onSurfaceChanged width:%d,height:%d", width, height);
    glViewport(0, 0, width, height);
}
extern "C" JNIEXPORT void JNICALL
Java_com_innup_learnopengles220414_MainActivity_onDrawFrame(JNIEnv *env, jobject thiz) {
    __android_log_print(ANDROID_LOG_INFO, ALICE_LOG_TAG, "onDrawFrame %f", GetFrameTime());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}