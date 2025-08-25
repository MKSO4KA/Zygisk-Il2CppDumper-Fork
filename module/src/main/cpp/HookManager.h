#ifndef ZYGISK_IL2CPPDUMPER_HOOKMANAGER_H
#define ZYGISK_IL2CPPDUMPER_HOOKMANAGER_H

#include <cstdint>

class HookManager {
public:
    /**
     * Устанавливает хук на указанный адрес в памяти.
     * @param name Название хука для логирования.
     * @param base_address Базовый адрес библиотеки (il2cpp_base).
     * @param offset Смещение функции от базового адреса.
     * @param fake_function Указатель на вашу функцию-замену.
     * @param original_function_ptr Указатель для сохранения адреса оригинальной функции.
     */
    static void install_hook(const char* name, uint64_t base_address, uint64_t offset, void* fake_function, void** original_function_ptr);
};

#endif // ZYGISK_IL2CPPDUMPER_HOOKMANAGER_H