#include "HookManager.h"
#include "log.h"
#include <cstring>
#include <map>
#include <string>
#include <vector>
#include <algorithm>

#define DO_API(r, n, p) extern r (*n) p
#include "il2cpp-api-functions.h"
#undef DO_API

// --- Глобальные хранилища для информации о хуках ---
static std::map<void*, MethodInfo*> g_stub_to_method_map;
static std::map<void*, void*> g_original_func_map;


// ==================================================================
// ===== НОВОВВЕДЕНИЕ: Черный список методов для хуков ==============
// ==================================================================
// Формат: "ИмяКласса::ИмяМетода"


// --- Вспомогательная функция ---
bool find_case_insensitive(const std::string& haystack, const std::string& needle) {
    if (needle.empty()) return true;
    auto it = std::search(
            haystack.begin(), haystack.end(),
            needle.begin(), needle.end(),
            [](char ch1, char ch2) { return std::toupper(ch1) == std::toupper(ch2); }
    );
    return (it != haystack.end());
}


// --- Макросы для создания универсальных заглушек ---
#define DEFINE_BUTTON_HOOK_STUB_0() \
void HookManager::button_hook_stub_0(void* instance) { \
    void* current_func_addr = (void*)button_hook_stub_0; \
    MethodInfo* method = g_stub_to_method_map[current_func_addr]; \
    if (method) { \
        Il2CppClass* klass = il2cpp_method_get_class(method); \
        LOGI(">>>> HOOK CALL: %s::%s", il2cpp_class_get_name(klass), il2cpp_method_get_name(method)); \
    } \
    auto original_func = (void (*)(void*))g_original_func_map[current_func_addr]; \
    if (original_func) { \
        original_func(instance); \
    } \
}

#define DEFINE_BUTTON_HOOK_STUB_1() \
void HookManager::button_hook_stub_1(void* instance, void* p1) { \
    void* current_func_addr = (void*)button_hook_stub_1; \
    MethodInfo* method = g_stub_to_method_map[current_func_addr]; \
    if (method) { \
        Il2CppClass* klass = il2cpp_method_get_class(method); \
        LOGI(">>>> HOOK CALL: %s::%s", il2cpp_class_get_name(klass), il2cpp_method_get_name(method)); \
    } \
    auto original_func = (void (*)(void*, void*))g_original_func_map[current_func_addr]; \
    if (original_func) { \
        original_func(instance, p1); \
    } \
}

// ... (остальные макросы до 10)
#define DEFINE_BUTTON_HOOK_STUB_2() void HookManager::button_hook_stub_2(void* i, void* p1, void* p2){ auto o=(void(*)(void*,void*,void*))g_original_func_map[(void*)button_hook_stub_2]; if(o) o(i,p1,p2); }
#define DEFINE_BUTTON_HOOK_STUB_3() void HookManager::button_hook_stub_3(void* i, void* p1, void* p2, void* p3){ auto o=(void(*)(void*,void*,void*,void*))g_original_func_map[(void*)button_hook_stub_3]; if(o) o(i,p1,p2,p3); }
#define DEFINE_BUTTON_HOOK_STUB_4() void HookManager::button_hook_stub_4(void* i, void* p1, void* p2, void* p3, void* p4){ auto o=(void(*)(void*,void*,void*,void*,void*))g_original_func_map[(void*)button_hook_stub_4]; if(o) o(i,p1,p2,p3,p4); }
#define DEFINE_BUTTON_HOOK_STUB_5() void HookManager::button_hook_stub_5(void* i, void* p1, void* p2, void* p3, void* p4, void* p5){ auto o=(void(*)(void*,void*,void*,void*,void*,void*))g_original_func_map[(void*)button_hook_stub_5]; if(o) o(i,p1,p2,p3,p4,p5); }
#define DEFINE_BUTTON_HOOK_STUB_6() void HookManager::button_hook_stub_6(void* i, void* p1, void* p2, void* p3, void* p4, void* p5, void* p6){ auto o=(void(*)(void*,void*,void*,void*,void*,void*,void*))g_original_func_map[(void*)button_hook_stub_6]; if(o) o(i,p1,p2,p3,p4,p5,p6); }
#define DEFINE_BUTTON_HOOK_STUB_7() void HookManager::button_hook_stub_7(void* i, void* p1, void* p2, void* p3, void* p4, void* p5, void* p6, void* p7){ auto o=(void(*)(void*,void*,void*,void*,void*,void*,void*,void*))g_original_func_map[(void*)button_hook_stub_7]; if(o) o(i,p1,p2,p3,p4,p5,p6,p7); }
#define DEFINE_BUTTON_HOOK_STUB_8() void HookManager::button_hook_stub_8(void* i, void* p1, void* p2, void* p3, void* p4, void* p5, void* p6, void* p7, void* p8){ auto o=(void(*)(void*,void*,void*,void*,void*,void*,void*,void*,void*))g_original_func_map[(void*)button_hook_stub_8]; if(o) o(i,p1,p2,p3,p4,p5,p6,p7,p8); }
#define DEFINE_BUTTON_HOOK_STUB_9() void HookManager::button_hook_stub_9(void* i, void* p1, void* p2, void* p3, void* p4, void* p5, void* p6, void* p7, void* p8, void* p9){ auto o=(void(*)(void*,void*,void*,void*,void*,void*,void*,void*,void*,void*))g_original_func_map[(void*)button_hook_stub_9]; if(o) o(i,p1,p2,p3,p4,p5,p6,p7,p8,p9); }
#define DEFINE_BUTTON_HOOK_STUB_10() void HookManager::button_hook_stub_10(void* i, void* p1, void* p2, void* p3, void* p4, void* p5, void* p6, void* p7, void* p8, void* p9, void* p10){ auto o=(void(*)(void*,void*,void*,void*,void*,void*,void*,void*,void*,void*,void*))g_original_func_map[(void*)button_hook_stub_10]; if(o) o(i,p1,p2,p3,p4,p5,p6,p7,p8,p9,p10); }


// Вызываем макросы для создания тел функций
DEFINE_BUTTON_HOOK_STUB_0()
DEFINE_BUTTON_HOOK_STUB_1()
DEFINE_BUTTON_HOOK_STUB_2()
DEFINE_BUTTON_HOOK_STUB_3()
DEFINE_BUTTON_HOOK_STUB_4()
DEFINE_BUTTON_HOOK_STUB_5()
DEFINE_BUTTON_HOOK_STUB_6()
DEFINE_BUTTON_HOOK_STUB_7()
DEFINE_BUTTON_HOOK_STUB_8()
DEFINE_BUTTON_HOOK_STUB_9()
DEFINE_BUTTON_HOOK_STUB_10()


// --- Установщик хуков ---

void HookManager::install_hooks(const std::vector<std::string>& keywords,
                                const std::vector<std::string>& method_blacklist,
                                const std::vector<std::string>& class_blacklist,
                                const std::vector<std::string>& dll_blacklist) { // <-- ДОБАВЛЕН ПАРАМЕТР
    LOGI("HookManager: Installing dynamic hooks...");
    LOGI("HookManager: Using %zu keywords, %zu method blacklist, %zu class blacklist, %zu DLL blacklist.", // <-- ОБНОВЛЕН ЛОГ
         keywords.size(), method_blacklist.size(), class_blacklist.size(), dll_blacklist.size());

    if (keywords.empty()) {
        LOGW("HookManager: Keyword list is empty, no hooks will be installed.");
        return;
    }

    int hooks_installed = 0;

    auto domain = il2cpp_domain_get();
    if (!domain) return;
    size_t n_assemblies;
    auto assemblies = il2cpp_domain_get_assemblies(domain, &n_assemblies);
    if (!assemblies) return;

    for (size_t i = 0; i < n_assemblies; ++i) {
        const Il2CppImage* image = il2cpp_assembly_get_image(assemblies[i]);

        // --- НАЧАЛО НОВОЙ ЛОГИКИ (ЧЕРНЫЙ СПИСОК DLL) ---
        const char* image_name = il2cpp_image_get_name(image);
        if (!image_name) continue;

        bool is_dll_blacklisted = false;
        for (const auto& blacklisted_dll_name : dll_blacklist) {
            if (strcmp(image_name, blacklisted_dll_name.c_str()) == 0) {
                is_dll_blacklisted = true;
                break;
            }
        }
        if (is_dll_blacklisted) {
            continue; // Пропускаем всю DLL
        }
        // --- КОНЕЦ НОВОЙ ЛОГИКИ ---

        auto class_count = il2cpp_image_get_class_count(image);

        for (size_t j = 0; j < class_count; ++j)for (size_t j = 0; j < class_count; ++j) {
            Il2CppClass* klass = (Il2CppClass*)il2cpp_image_get_class(image, j);
            void* iter = nullptr;

            // --- НАЧАЛО НОВОЙ ЛОГИКИ ---
            // Проверяем, не находится ли сам класс в черном списке
            const char* class_name_cstr = il2cpp_class_get_name(klass);
            if (!class_name_cstr) continue;

            bool is_class_blacklisted = false;
            for (const auto& blacklisted_class_name : class_blacklist) {
                if (strcmp(class_name_cstr, blacklisted_class_name.c_str()) == 0) {
                    is_class_blacklisted = true;
                    break;
                }
            }
            if (is_class_blacklisted) {
                continue; // Пропускаем все методы этого класса
            }
            // --- КОНЕЦ НОВОЙ ЛОГИКИ ---

            while (auto method_const = il2cpp_class_get_methods(klass, &iter)) {
                MethodInfo* method = const_cast<MethodInfo*>(method_const);
                const char* method_name_cstr = il2cpp_method_get_name(method);
                // class_name_cstr уже получен выше
                if (!method->methodPointer || !method_name_cstr) continue;

                std::string method_name(method_name_cstr);

                bool match = false;
                for (const auto& keyword : keywords) {
                    if (find_case_insensitive(method_name, keyword)) {
                        match = true;
                        break;
                    }
                }
                if (match) {
                    std::string full_method_name = std::string(class_name_cstr) + "::" + method_name;
                    bool is_method_blacklisted = false;
                    // --- ИЗМЕНЕНИЕ ЗДЕСЬ (имя переменной) ---
                    for (const auto& blacklisted_name : method_blacklist) {
                        if (full_method_name == blacklisted_name) {
                            is_method_blacklisted = true;
                            break;
                        }
                    }
                    if (is_method_blacklisted) {
                        continue;
                    }

                    uint32_t param_count = il2cpp_method_get_param_count(method);
                    void* target_stub = nullptr;

                    switch (param_count) {
                        case 0: target_stub = (void*)button_hook_stub_0; break;
                        case 1: target_stub = (void*)button_hook_stub_1; break;
                        case 2: target_stub = (void*)button_hook_stub_2; break;
                        case 3: target_stub = (void*)button_hook_stub_3; break;
                        case 4: target_stub = (void*)button_hook_stub_4; break;
                        case 5: target_stub = (void*)button_hook_stub_5; break;
                        case 6: target_stub = (void*)button_hook_stub_6; break;
                        case 7: target_stub = (void*)button_hook_stub_7; break;
                        case 8: target_stub = (void*)button_hook_stub_8; break;
                        case 9: target_stub = (void*)button_hook_stub_9; break;
                        case 10: target_stub = (void*)button_hook_stub_10; break;
                        default: continue;
                    }

                    g_stub_to_method_map[target_stub] = method;
                    g_original_func_map[target_stub] = reinterpret_cast<void*>(method->methodPointer);

                    method->methodPointer = (Il2CppMethodPointer)target_stub;

                    LOGI("Hooked: [%s] %s", il2cpp_image_get_name(image), full_method_name.c_str());
                    hooks_installed++;
                }
            }
        }
    }
    LOGI("HookManager: Finished. Total hooks installed: %d", hooks_installed);
}