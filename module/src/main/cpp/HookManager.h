#ifndef ZYGISK_IL2CPPDUMPER_HOOKMANAGER_H
#define ZYGISK_IL2CPPDUMPER_HOOKMANAGER_H

#include "il2cpp-class.h"

/**
 * Класс для установки хуков на методы действий игрока.
 */
class HookManager {
public:
    /**
     * Находит классы RGController и NormalMechaController и устанавливает хуки.
     */
    static void install_player_action_hooks();

private:
    // --- Хуки для RGController (обычный игрок) ---
    static void (*original_BtnAtkClick)(void* instance, bool value);
    static void (*original_BtnSkillDown)(void* instance);
    static void hook_BtnAtkClick(void* instance, bool value);
    static void hook_BtnSkillDown(void* instance);

    // --- Хуки для NormalMechaController (мех) ---
    static void (*original_MannualAttackButtonDown)(void* instance, bool value);
    static void (*original_SkillButtonDown)(void* instance);
    static void hook_MannualAttackButtonDown(void* instance, bool value);
    static void hook_SkillButtonDown(void* instance);
};

#endif //ZYGISK_IL2CPPDUMPER_HOOKMANAGER_H