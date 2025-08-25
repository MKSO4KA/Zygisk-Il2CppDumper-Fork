#include "HookManager.h"
#include "log.h"
#include "include/dobby.h"
#include <cinttypes> // <-- ДОБАВЬТЕ ЭТУ СТРОКУ

void HookManager::install_hook(const char* name, uint64_t base_address, uint64_t offset, void* fake_function, void** original_function_ptr) {
    if (base_address == 0) {
        LOGE("Hook [%s]: FAILED. Base address is zero.", name);
        return;
    }

    uint64_t target_address = base_address + offset;

    // ===================================================================
    // ===== ИСПРАВЛЕНИЕ ЗДЕСЬ ===========================================
    // ===================================================================
    // Заменяем %lx на %" PRIx64
    LOGI("Hook [%s]: Preparing to hook at address: 0x%" PRIx64, name, target_address);
    // ===================================================================

    int result = DobbyHook(
            (void*)target_address,
            fake_function,
            original_function_ptr
    );

    if (result == 0) {
        LOGI("Hook [%s]: Installed successfully!", name);
    } else {
        LOGE("Hook [%s]: FAILED with Dobby code: %d", name, result);
    }
}