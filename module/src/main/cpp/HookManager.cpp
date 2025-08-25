#include "HookManager.h"
#include "log.h"
#include "include/dobby.h"

void HookManager::install_hook(const char* name, uint64_t base_address, uint64_t offset, void* fake_function, void** original_function_ptr) {
    if (base_address == 0) {
        LOGE("Hook [%s]: FAILED. Base address is zero.", name);
        return;
    }

    uint64_t target_address = base_address + offset;
    LOGI("Hook [%s]: Preparing to hook at address: 0x%lx", name, target_address);

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