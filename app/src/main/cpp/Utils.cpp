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

GLuint CreateProgram(GLuint vertex_shader, GLuint fragment_shader){
    GLuint program_to_ret = glCreateProgram();
    glAttachShader(program_to_ret, vertex_shader);
    glAttachShader(program_to_ret, fragment_shader);
    glLinkProgram(program_to_ret);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    GLint link_result = GL_TRUE;
    glGetProgramiv(program_to_ret, GL_LINK_STATUS, &link_result);
    if(GL_FALSE == link_result){
        char szLog[1024] = {0};
        GLsizei logLen = 0;
        glGetShaderInfoLog(program_to_ret, 1024, &logLen, szLog);
        __android_log_print(ANDROID_LOG_ERROR, ALICE_LOG_TAG, "CreateProgram %s\n", szLog);
        glDeleteProgram(program_to_ret);
        program_to_ret = 0;
    }
    return program_to_ret;
}

GLuint CreateStandardProgram(AAssetManager *sAssetManager, const char *vertex_shader_path, const char *fragment_shader_path){
    int fileSize = 0;
    unsigned char *fileContent = LoadFileContent(sAssetManager, vertex_shader_path, fileSize);
    GLuint vsShader = CompileShader(GL_VERTEX_SHADER, (char *)fileContent);
    delete [] fileContent;
    fileContent = LoadFileContent(sAssetManager, fragment_shader_path, fileSize);
    GLuint fsShader = CompileShader(GL_FRAGMENT_SHADER, (char *)fileContent);
    delete [] fileContent;
    __android_log_print(ANDROID_LOG_INFO, ALICE_LOG_TAG, "vs:%u  fs:%u\n", vsShader, fsShader);
    GLuint program = CreateProgram(vsShader, fsShader);
    __android_log_print(ANDROID_LOG_INFO, ALICE_LOG_TAG, "program:%u\n", program);
    glDeleteShader(vsShader);
    glDeleteShader(fsShader);
    return program;
}

GLuint CreateBufferObject(GLenum type, void *data, int size, GLenum usage){
    GLuint object_to_ret;
    glGenBuffers(1, &object_to_ret);
    glBindBuffer(type, object_to_ret);
    glBufferData(type, size, data, usage);
    glBindBuffer(type, 0);
    return object_to_ret;
}

void UpdateBufferObject(GLuint object, GLenum type, void *data, int size, int offset){
    glBindBuffer(type, object);
    glBufferSubData(type, offset, size, data);
    glBindBuffer(type, 0);
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
