#include "Utils.h"
unsigned  char* LoadFileContent(AAssetManager *sAssetManager, const char* path, int& fileSize){
    unsigned char *fileContent = nullptr;
    fileSize = 0;
    AAsset *asset = AAssetManager_open(sAssetManager, path, AASSET_MODE_UNKNOWN);
    if(asset != nullptr){
        fileSize = AAsset_getLength(asset);
        fileContent = new unsigned char[fileSize + 1];
        AAsset_read(asset, fileContent, fileSize);
        fileContent[fileSize] = 0;
        AAsset_close(asset);
    }
    return fileContent;
}

GLuint CompileShader(GLenum shader_type, const char *source_code){
    //指定类型创建一个 shader
    GLuint shader_to_ret = glCreateShader(shader_type);
    //链接 shader 源码，传入参数分别为：shader、1份代码、代码的内容、代码的长度（能够推断出来，直接传空就行）
    glShaderSource(shader_to_ret, 1, &source_code, nullptr);
    //编译 shader
    glCompileShader(shader_to_ret);
    //查看 shader 编译结果
    GLint compile_result = GL_TRUE;
    glGetShaderiv(shader_to_ret, GL_COMPILE_STATUS, &compile_result);
    if(compile_result == GL_FALSE){
        //打印编译错误日志
        char szLog[1024] = {0};
        GLsizei logLen = 0;
        glGetShaderInfoLog(shader_to_ret, 1024, &logLen, szLog);
        __android_log_print(ANDROID_LOG_ERROR, ALICE_LOG_TAG, "CompileShader %s\ncode:\n%s", szLog, source_code);
        glDeleteShader(shader_to_ret);
        shader_to_ret = 0;
    }
    return shader_to_ret;
}

float GetFrameTime(){
    //无论这个函数执行多少次，下面 static 初始化代码只会执行一次
    static unsigned long long lastTime = 0, currentTime = 0;

    timeval current;
    gettimeofday(&current, nullptr);

    //转换成毫秒单位
    currentTime = current.tv_sec*1000 + current.tv_usec / 1000;
    unsigned long long frameTime = lastTime == 0 ? 0 : currentTime - lastTime;
    lastTime = currentTime;

    //返回秒单位
    return float(frameTime) / 1000.0f;
}
