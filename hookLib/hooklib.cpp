#include <stdio.h>
#include <signal.h>

#include <android/log.h>

// 日志
#define LOG_TAG "KILLHOOK"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

#include "../dobby/dobby.h"

extern "C"{
    __attribute__((visibility ("default"))) int not_kill(pid_t pid, int sig) {
        LOGD("NOT KILL");
        return 114514;
    }
    // 如果功能需要被dlsym调用出来，那么传入的应该是函数的symbpls
    // 因此 如果编译选项开启了-fvisibility=hidden
    // 那么就需要用__attribute__((visibility ("default")))修饰被调用的功能 以确保功能可以被外部调用
    // 同时用extern "C"保证以c风格编译(c风格编译，symbols是函数名本身)
    // 而不是c++风格编译(c++风格编译后，symbaps为__ZXXX_XX格式)
    // 或者自行ida查看symbols
    __attribute__((visibility ("default"))) void hook_lmkd() {
        printf("Not kill");
        void *sym1 = DobbySymbolResolver(nullptr, "kill");
        void *sym2 = DobbySymbolResolver(nullptr, "kill@LIBC");
        if(nullptr != sym1) {
            DobbyHook(sym1, (void*)not_kill, nullptr);
            LOGD("LMKD HOOKED AT SYM kill");
        }
        
        if(nullptr != sym2) {
            DobbyHook(sym2, (void*)not_kill, nullptr);
            LOGD("LMKD HOOKED AT SYM kill@LIBC");
        }
    }
}