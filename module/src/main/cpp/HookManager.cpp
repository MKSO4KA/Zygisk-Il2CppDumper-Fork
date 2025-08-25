#include "HookManager.h"
#include "log.h"
#include <cstring>

#define DO_API(r, n, p) extern r (*n) p
#include "il2cpp-api-functions.h"
#undef DO_API

// --- Определения указателей на оригиналы ---
// Для RGController
void (*HookManager::original_BtnAtkClick)(void* instance, bool value);
void (*HookManager::original_BtnSkillDown)(void* instance);
// Для NormalMechaController
void (*HookManager::original_MannualAttackButtonDown)(void* instance, bool value);
void (*HookManager::original_SkillButtonDown)(void* instance);


// --- Реализация хуков для RGController ---

void HookManager::hook_BtnAtkClick(void* instance, bool value) {
    LOGI(">>>> HOOKED: RGController::BtnAtkClick(instance: %p, isDown: %d)", instance, value);
    original_BtnAtkClick(instance, value);
}

void HookManager::hook_BtnSkillDown(void* instance) {
    LOGI(">>>> HOOKED: RGController::BtnSkillDown(instance: %p)", instance);
    original_BtnSkillDown(instance);
}

// --- Реализация хуков для NormalMechaController ---

void HookManager::hook_MannualAttackButtonDown(void* instance, bool value) {
    LOGI(">>>> HOOKED: NormalMechaController::MannualAttackButtonDown(instance: %p, isDown: %d)", instance, value);
    original_MannualAttackButtonDown(instance, value);
}

void HookManager::hook_SkillButtonDown(void* instance) {
    LOGI(">>>> HOOKED: NormalMechaController::SkillButtonDown(instance: %p)", instance);
    original_SkillButtonDown(instance);
}


// --- Установщик хуков ---

void HookManager::install_player_action_hooks() {
    LOGI("HookManager: Searching for player controller classes...");

    auto domain = il2cpp_domain_get();
    if (!domain) return;
    size_t n_assemblies;
    auto assemblies = il2cpp_domain_get_assemblies(domain, &n_assemblies);
    if (!assemblies) return;

    Il2CppClass* rg_controller_class = nullptr;
    Il2CppClass* mecha_controller_class = nullptr;

    // Ищем оба класса в сборке Assembly-CSharp.dll
    for (size_t i = 0; i < n_assemblies; ++i) {
        const Il2CppImage* image = il2cpp_assembly_get_image(assemblies[i]);
        if (strcmp(il2cpp_image_get_name(image), "Assembly-CSharp.dll") == 0) {
            LOGI("Searching in Assembly-CSharp.dll...");
            rg_controller_class = il2cpp_class_from_name(image, "", "RGController");
            mecha_controller_class = il2cpp_class_from_name(image, "", "NormalMechaController");
            // Если нашли оба, можно выходить
            if (rg_controller_class && mecha_controller_class) {
                break;
            }
        }
    }

    // Макрос для установки хука
#define INSTALL_HOOK(TargetClass, MethodName, ArgCount) \
    { \
        MethodInfo* method = (MethodInfo*)il2cpp_class_get_method_from_name(TargetClass, #MethodName, ArgCount); \
        if (method && method->methodPointer) { \
            original_##MethodName = (decltype(original_##MethodName))method->methodPointer; \
            method->methodPointer = (Il2CppMethodPointer)hook_##MethodName; \
            LOGI("Hooked %s::%s", il2cpp_class_get_name(TargetClass), #MethodName); \
        } else { \
            LOGW("Failed to find %s::%s", il2cpp_class_get_name(TargetClass), #MethodName); \
        } \
    }

    // Устанавливаем хуки для RGController
    if (rg_controller_class) {
        LOGI("Found RGController, installing hooks...");
        INSTALL_HOOK(rg_controller_class, BtnAtkClick, 1);
        INSTALL_HOOK(rg_controller_class, BtnSkillDown, 0);
    } else {
        LOGE("HookManager: FAILED to find RGController class.");
    }

    // Устанавливаем хуки для NormalMechaController
    if (mecha_controller_class) {
        LOGI("Found NormalMechaController, installing hooks...");
        INSTALL_HOOK(mecha_controller_class, MannualAttackButtonDown, 1);
        INSTALL_HOOK(mecha_controller_class, SkillButtonDown, 0);
    } else {
        LOGE("HookManager: FAILED to find NormalMechaController class.");
    }

    LOGI("HookManager: Finished installing player action hooks.");
}