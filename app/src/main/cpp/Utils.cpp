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

/**
 * 实现一个像素的RB交换。bgr->rgb
 * @param pixel 像素数据
 * @param pixel_data_offset b分量偏移位置
 */
void SwapPixelRB(unsigned char * pixel,int pixel_data_offset){//bgr -> rgb
    unsigned  char b=pixel[pixel_data_offset];
    pixel[pixel_data_offset]=pixel[pixel_data_offset+2];
    pixel[pixel_data_offset+2]=b;
}

/**
 * 解码24位非压缩格式的位图文件
 * @param bmp_fileContent 文件内容
 * @param width 解码后赋值的宽
 * @param height 解码后赋值的高
 * @return 解码后的像素数据
 */
unsigned char* DecodeBMP(unsigned  char* bmp_file_content, int& width, int& height){
    //取出文件的两个字节(short占两个字节，解short指针就可以得出2个字节内容，所以强转short指针再解)，
    //判断这两个字节是不是等于 0x4042，等于的话，说明是位图文件
    if(0x4D42==*((unsigned short*)bmp_file_content)){
        //24位非压缩格式的位图文件开始指针地址偏移10个单位指向的是像素数据的偏移地址。
        //即：解 开始指针地址后10位的这个指针，得出像素真正地址的偏移量
        int pixel_data_offset=*((int*)(bmp_file_content+10));
        //开始指针地址后18位的这个指针存放着图片“宽”的数据
        width=*((int*)(bmp_file_content+18));
        //开始指针地址后22位的这个指针存放着图片“宽”的数据
        height=*((int*)(bmp_file_content+22));
        //用开始指针地址加上数据偏移量，得到的地址就是像素数据的地址，即像素数据的指针。
        unsigned char * pixel=bmp_file_content+pixel_data_offset;
        //图片宽*高得到所有的像素数据大小。其中一个像素占3个字节，即bgr。
        int pixel_data_count=width*height;
        //变量这个数组，每三个为一组，交换每组的RB值，让每组数据变成 rgb。
        for (int i = 0; i < pixel_data_count; ++i) {
                SwapPixelRB(pixel,i*3);//每个像素占三个字节
        }
        return pixel;
    }
    return nullptr;
}

GLuint CreateTexture2D(void*pixel,int width,int height,GLenum gpu_format,GLenum cpu_format){
    GLuint texture;
    glGenTextures(1,&texture);
    glBindTexture(GL_TEXTURE_2D,texture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D,0,gpu_format,width,height,0,cpu_format,GL_UNSIGNED_BYTE,pixel);
    glBindTexture(GL_TEXTURE_2D,0);
    return texture;
}

GLuint CreateTextureFromFile(AAssetManager *sAssetManager, const char *path){
    int file_size=0;
    unsigned char* filecontent=LoadFileContent(sAssetManager, path,file_size);
    int image_width,image_height;
    unsigned char * rgb_pixel=DecodeBMP(filecontent,image_width,image_height);
    GLuint texture=CreateTexture2D(rgb_pixel,image_width,image_height,GL_RGB,GL_RGB);
    delete [] filecontent;
    return texture;
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
