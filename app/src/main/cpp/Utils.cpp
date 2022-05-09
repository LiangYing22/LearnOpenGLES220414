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
