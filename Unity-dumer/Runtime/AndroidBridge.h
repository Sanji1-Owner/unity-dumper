#pragma once

#ifdef __ANDROID__
#include <jni.h>

namespace UnityAnalyzer::Runtime {

class AndroidBridge {
public:
    static void Initialize(JavaVM* vm);
    static JNIEnv* GetEnv();
    
private:
    static JavaVM* javaVM;
};

} // namespace UnityAnalyzer::Runtime

extern "C" {
    JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved);
}

#endif
