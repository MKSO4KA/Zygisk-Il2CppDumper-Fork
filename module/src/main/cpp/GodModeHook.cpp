#include "GodModeHook.h"
#include "HookManager.h" // <-- Подключаем наш менеджер

// Определения TAG и LOG* можно убрать, так как они теперь в HookManager
// Но оставим для fake_function, если там нужен лог
#include "log.h"
#define TAG "GodModeHook"

void* GodModeHook::original_function = nullptr;

bool GodModeHook::fake_function() {
    // LOGI("HOOK TRIGGERED: Damage blocked by GodMode!");
    return false;
}

void GodModeHook::install(uint64_t base_address) {
    // Смещение для IsHitPlayer, специфичное для этого хука
    const uint64_t offset = 0x60bad88;

    // Используем наш универсальный установщик
    HookManager::install_hook(
            "GodMode(IsHitPlayer)", // Имя для логов
            base_address,
            offset,
            (void*)fake_function,
            &original_function
    );
}