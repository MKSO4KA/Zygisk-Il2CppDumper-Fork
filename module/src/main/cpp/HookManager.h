#ifndef ZYGISK_IL2CPPDUMPER_HOOKMANAGER_H
#define ZYGISK_IL2CPPDUMPER_HOOKMANAGER_H

#include "il2cpp-class.h"
#include <vector>
#include <string>
#include "ConfigFetcher.h"

class HookManager {
public:
    static void install_hooks(const DynamicConfig& config);

private:
    // --- ИЗМЕНЕНИЕ ЗДЕСЬ: void -> void* ---
    static void* button_hook_stub_0(void* instance);
    static void* button_hook_stub_1(void* instance, void* p1);
    static void* button_hook_stub_2(void* instance, void* p1, void* p2);
    static void* button_hook_stub_3(void* instance, void* p1, void* p2, void* p3);
    static void* button_hook_stub_4(void* instance, void* p1, void* p2, void* p3, void* p4);
    static void* button_hook_stub_5(void* instance, void* p1, void* p2, void* p3, void* p4, void* p5);
    static void* button_hook_stub_6(void* instance, void* p1, void* p2, void* p3, void* p4, void* p5, void* p6);
    static void* button_hook_stub_7(void* instance, void* p1, void* p2, void* p3, void* p4, void* p5, void* p6, void* p7);
    static void* button_hook_stub_8(void* instance, void* p1, void* p2, void* p3, void* p4, void* p5, void* p6, void* p7, void* p8);
    static void* button_hook_stub_9(void* instance, void* p1, void* p2, void* p3, void* p4, void* p5, void* p6, void* p7, void* p8, void* p9);
    static void* button_hook_stub_10(void* instance, void* p1, void* p2, void* p3, void* p4, void* p5, void* p6, void* p7, void* p8, void* p9, void* p10);
};

#endif //ZYGISK_IL2CPPDUMPER_HOOKMANAGER_H