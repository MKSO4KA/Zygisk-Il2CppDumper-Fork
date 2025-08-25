#ifndef ZYGISK_IL2CPPDUMPER_GODMODEHOOK_H
#define ZYGISK_IL2CPPDUMPER_GODMODEHOOK_H

#include <cstdint>

class GodModeHook {
public:
    // Теперь install принимает базовый адрес
    static void install(uint64_t base_address);

private:
    static void* original_function;
    static bool fake_function();
};

#endif //ZYGISK_IL2CPPDUMPER_GODMODEHOOK_H