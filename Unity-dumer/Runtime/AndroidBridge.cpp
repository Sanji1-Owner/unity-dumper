#include "AndroidBridge.h"

#ifdef __ANDROID__
#include <android/log.h>

namespace UnityAnalyzer::Runtime {

JavaVM* AndroidBridge::javaVM = nullptr;

void AndroidBridge::Initialize(JavaVM* vm) {
    javaVM = vm;
}

JNIEnv* AndroidBridge::GetEnv() {
    JNIEnv* env = nullptr;
    if (javaVM->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        javaVM->AttachCurrentThread(&env, nullptr);
    }
    return env;
}

} // namespace UnityAnalyzer::Runtime

extern "C" {
    JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
        UnityAnalyzer::Runtime::AndroidBridge::Initialize(vm);
        return JNI_VERSION_1_6;
    }
}

#endif
