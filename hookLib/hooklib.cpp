#include <stdio.h>
#include <signal.h>
#include <linux/signal.h>
#include <sys/syscall.h>
#include <unistd.h>

#include <android/log.h>

#define LOG_TAG "KILLHOOK"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

#include "../dobby/dobby.h"

// 功能需要被dlsym调用出来，传入的应该是函数的symbols
// 开启了-fvisibility=hidden
// 需要用__attribute__((visibility ("default")))修饰被调用的功能 以确保功能可以被外部调用
// 同时用extern "C"保证以c风格编译(c风格编译，symbols是函数名本身)9

extern "C" {
    __attribute__((visibility ("default"))) int hooked_kill(pid_t pid, int sig) {
        return 0;
    }
    
    __attribute__((visibility ("default"))) int hooked_pidfd_send_signal(int pidfd, int sig, siginfo_t *info, unsigned int flags) {
        return 0;
    }
}

static void hook_sym_kill() {
    void *sym1 = DobbySymbolResolver(nullptr, "kill");
    void *sym2 = DobbySymbolResolver(nullptr, "kill@LIBC");
    if(nullptr != sym1) {
        DobbyHook(sym1, (void*)hooked_kill, nullptr);
        LOGD("LMKD HOOKED AT SYM kill");
    }
    
    if(nullptr != sym2) {
        DobbyHook(sym2, (void*)hooked_kill, nullptr);
        LOGD("LMKD HOOKED AT SYM kill@LIBC");
    }
}

static void hook_sym_pidfd_send_signal() {
    void *sym1 = DobbySymbolResolver(nullptr, "pidfd_send_signal");
    void *sym2 = DobbySymbolResolver(nullptr, "pidfd_send_signal@LIBC");
    if(nullptr != sym1) {
        DobbyHook(sym1, (void*)hooked_kill, nullptr);
        LOGD("LMKD HOOKED AT SYM pidfd_send_signal");
    }
    
    if(nullptr != sym2) {
        DobbyHook(sym2, (void*)hooked_kill, nullptr);
        LOGD("LMKD HOOKED AT SYM pidfd_send_signal@LIBC");
    }
}

extern "C" {
    __attribute__((visibility ("default"))) void hook_lmkd() {
        hook_sym_kill();
        hook_sym_pidfd_send_signal();
    }
}