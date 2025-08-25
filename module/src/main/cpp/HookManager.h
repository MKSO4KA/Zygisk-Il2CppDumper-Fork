#ifndef ZYGISK_IL2CPPDUMPER_HOOKMANAGER_H
#define ZYGISK_IL2CPPDUMPER_HOOKMANAGER_H

#include "il2cpp-class.h"
#include <vector> // <-- ДОБАВЛЕНО
#include <string> // <-- ДОБАВЛЕНО

/**
 * Класс для установки универсальных логирующих хуков на методы.
 */
class HookManager {
public:
    /**
     * Ищет во всех сборках методы и устанавливает на них хуки.
     * @param keywords Список ключевых слов для поиска в именах методов.
     * @param method_blacklist Список полных имен методов для исключения.
     * @param class_blacklist Список имен классов для полного исключения.
     * @param dll_blacklist Список имен DLL (образов) для полного исключения.
     */
    static void install_hooks(const std::vector<std::string>& keywords,
                              const std::vector<std::string>& method_blacklist,
                              const std::vector<std::string>& class_blacklist,
                              const std::vector<std::string>& dll_blacklist); // <-- ДОБАВЛЕНО

private:
    // --- Универсальные заглушки-логгеры ---
    static void button_hook_stub_0(void* instance);
    static void button_hook_stub_1(void* instance, void* p1);
    static void button_hook_stub_2(void* instance, void* p1, void* p2);
    static void button_hook_stub_3(void* instance, void* p1, void* p2, void* p3);
    static void button_hook_stub_4(void* instance, void* p1, void* p2, void* p3, void* p4);
    static void button_hook_stub_5(void* instance, void* p1, void* p2, void* p3, void* p4, void* p5);
    static void button_hook_stub_6(void* instance, void* p1, void* p2, void* p3, void* p4, void* p5, void* p6);
    static void button_hook_stub_7(void* instance, void* p1, void* p2, void* p3, void* p4, void* p5, void* p6, void* p7);
    static void button_hook_stub_8(void* instance, void* p1, void* p2, void* p3, void* p4, void* p5, void* p6, void* p7, void* p8);
    static void button_hook_stub_9(void* instance, void* p1, void* p2, void* p3, void* p4, void* p5, void* p6, void* p7, void* p8, void* p9);
    static void button_hook_stub_10(void* instance, void* p1, void* p2, void* p3, void* p4, void* p5, void* p6, void* p7, void* p8, void* p9, void* p10);
};

#endif //ZYGISK_IL2CPPDUMPER_HOOKMANAGER_H