#include "Sence.h"
#include "Utils.h"

static AAssetManager *sAssetManager = nullptr;

//vertex buffer object：放顶点缓冲区的一个对象。对硬件来说，数字就是一个明确的东西，所以使用 GLuint 类型。
GLuint vbo;
//shader编译后是GPU的一个程序
GLuint program;
//矩阵索引
GLint modelMatrixLocation, viewMatrixLocation, projectionMatrixLocation;
//属性索引
GLint attrPositionLocation, attrColorLocation;
glm::mat4 modelMatrix, viewMatrix, projectionMatrix;


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
    vertices[0].mColor[0] = 1.0f;
    vertices[0].mColor[1] = 1.0f;
    vertices[0].mColor[2] = 0.0;
    vertices[0].mColor[3] = 1.0f;
    //设置第二个顶点数据
    vertices[1].mPosition[0] = 0.5f;
    vertices[1].mPosition[1] = -0.5f;
    vertices[1].mPosition[2] = -2.0f;
    vertices[1].mPosition[3] = 1.0f;
    vertices[1].mColor[0] = 1.0f;
    vertices[1].mColor[1] = 0.0f;
    vertices[1].mColor[2] = 1.0;
    vertices[1].mColor[3] = 1.0f;
    //设置第三个顶点数据
    vertices[2].mPosition[0] = 0.0f;
    vertices[2].mPosition[1] = 0.5f;
    vertices[2].mPosition[2] = -2.0f;
    vertices[2].mPosition[3] = 1.0f;
    vertices[2].mColor[0] = 0.0f;
    vertices[2].mColor[1] = 1.0f;
    vertices[2].mColor[2] = 1.0;
    vertices[2].mColor[3] = 1.0f;

    glGenBuffers(1, &vbo);//向显卡申请一个 vbo，即在显卡上创建了一个vbo，并把vbo编号赋值到了我们转入的GLuint中。
    glBindBuffer(GL_ARRAY_BUFFER, vbo);//把 vbo 设置到 GL_ARRAY_BUFFER 卡槽上去。
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)*3, vertices, GL_STATIC_DRAW);//这里就会把CPU中的数据vertices，传到了GPU的vbo中去。因为此时OpenGL的GL_ARRAY_BUFFER卡槽指向的是刚刚申请的vbo。
    glBindBuffer(GL_ARRAY_BUFFER, 0);//GL_ARRAY_BUFFER 卡槽重新设置到 0 上去，避免后面在 GL_ARRAY_BUFFER 卡槽上的操作污染 我们上面申请的 vbo。

    program = CreateStandardProgram(sAssetManager, "test.vs", "test.fs");
    //这些索引应该不为-1。没有使用的变量，拿到的索引会为-1。
    attrPositionLocation = glGetAttribLocation(program, "position");
    attrColorLocation = glGetAttribLocation(program, "color");
    modelMatrixLocation = glGetUniformLocation(program, "U_ModelMatrix");
    viewMatrixLocation = glGetUniformLocation(program, "U_ViewMatrix");
    projectionMatrixLocation = glGetUniformLocation(program, "U_ProjectionMatrix");
    __android_log_print(ANDROID_LOG_INFO, ALICE_LOG_TAG, "location %d, %d, %d, %d, %d",
            attrPositionLocation, attrColorLocation, modelMatrixLocation, viewMatrixLocation, projectionMatrixLocation);

//    int fileSize = 0;
//    unsigned char * fileContent = LoadFileContent(sAssetManager, "test.txt", fileSize);
//    if(fileContent != nullptr){
//        __android_log_print(ANDROID_LOG_INFO, ALICE_LOG_TAG, "[%s]", (char *)fileContent);
//        delete [] fileContent;
//    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_innup_learnopengles220414_MainActivity_onSurfaceChanged(JNIEnv *env, jobject thiz,
                                                                 jint width, jint height) {
    __android_log_print(ANDROID_LOG_INFO, ALICE_LOG_TAG, "onSurfaceChanged width:%d,height:%d", width, height);
    glViewport(0, 0, width, height);
    viewMatrix=glm::lookAt(glm::vec3(0.0f,0.0f,0.0f),
                           glm::vec3(0.0,0.0,-1.0f),
                           glm::vec3(0.0,1.0f,0.0f));
    projectionMatrix=glm::perspective(45.0f,float(width)/float(height),0.1f,1000.0f);
}
extern "C" JNIEXPORT void JNICALL
Java_com_innup_learnopengles220414_MainActivity_onDrawFrame(JNIEnv *env, jobject thiz) {
//    __android_log_print(ANDROID_LOG_INFO, ALICE_LOG_TAG, "onDrawFrame %f", GetFrameTime());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glUseProgram(program);
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    //设置属性
    glEnableVertexAttribArray(attrPositionLocation);
    //参数：属性索引、一个点有多少组成部分（x,y,z,w）、每个组成部分是什么类型、是否需要转置、每个顶点的大小、在第一个顶点数据偏移量多少。
    //设置这个后，GPU会去vbo取相应数据了
    glVertexAttribPointer(attrPositionLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertice), 0);
    glEnableVertexAttribArray(attrColorLocation);
    glVertexAttribPointer(attrColorLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertice), (void *)(sizeof(float )*4));
    //从0号点开始画，画3个点
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUseProgram(0);
}