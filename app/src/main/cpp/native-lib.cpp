#include "AlicePrefix.h"

extern "C" JNIEXPORT jstring JNICALL
Java_com_innup_learnopengles220414_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}