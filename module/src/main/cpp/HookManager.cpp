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

// --- Глобальная переменная для включения/выключения логов вызовов ---
static bool g_enable_hook_call_logging = false;


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
// ВАЖНО: Определения функций теперь находятся ВНУТРИ макросов

#define DEFINE_BUTTON_HOOK_STUB_0() \
void* HookManager::button_hook_stub_0(void* instance) { \
    if (g_enable_hook_call_logging) { \
        void* current_func_addr = (void*)button_hook_stub_0; \
        MethodInfo* method = g_stub_to_method_map[current_func_addr]; \
        if (method) { \
            Il2CppClass* klass = il2cpp_method_get_class(method); \
            LOGI(">>>> HOOK CALL: %s::%s", il2cpp_class_get_name(klass), il2cpp_method_get_name(method)); \
        } \
    } \
    auto original_func = (void* (*)(void*))g_original_func_map[(void*)button_hook_stub_0]; \
    if (original_func) return original_func(instance); \
    return nullptr; \
}

#define DEFINE_BUTTON_HOOK_STUB_1() \
void* HookManager::button_hook_stub_1(void* instance, void* p1) { \
    if (g_enable_hook_call_logging) { \
        void* current_func_addr = (void*)button_hook_stub_1; \
        MethodInfo* method = g_stub_to_method_map[current_func_addr]; \
        if (method) { \
            Il2CppClass* klass = il2cpp_method_get_class(method); \
            LOGI(">>>> HOOK CALL: %s::%s", il2cpp_class_get_name(klass), il2cpp_method_get_name(method)); \
        } \
    } \
    auto original_func = (void* (*)(void*, void*))g_original_func_map[(void*)button_hook_stub_1]; \
    if (original_func) return original_func(instance, p1); \
    return nullptr; \
}

#define DEFINE_BUTTON_HOOK_STUB_2() \
void* HookManager::button_hook_stub_2(void* instance, void* p1, void* p2) { \
    if (g_enable_hook_call_logging) { \
        void* current_func_addr = (void*)button_hook_stub_2; \
        MethodInfo* method = g_stub_to_method_map[current_func_addr]; \
        if (method) { \
            Il2CppClass* klass = il2cpp_method_get_class(method); \
            LOGI(">>>> HOOK CALL: %s::%s", il2cpp_class_get_name(klass), il2cpp_method_get_name(method)); \
        } \
    } \
    auto original_func = (void* (*)(void*, void*, void*))g_original_func_map[(void*)button_hook_stub_2]; \
    if (original_func) return original_func(instance, p1, p2); \
    return nullptr; \
}

#define DEFINE_BUTTON_HOOK_STUB_3() \
void* HookManager::button_hook_stub_3(void* instance, void* p1, void* p2, void* p3) { \
    if (g_enable_hook_call_logging) { \
        void* current_func_addr = (void*)button_hook_stub_3; \
        MethodInfo* method = g_stub_to_method_map[current_func_addr]; \
        if (method) { \
            Il2CppClass* klass = il2cpp_method_get_class(method); \
            LOGI(">>>> HOOK CALL: %s::%s", il2cpp_class_get_name(klass), il2cpp_method_get_name(method)); \
        } \
    } \
    auto original_func = (void* (*)(void*, void*, void*, void*))g_original_func_map[(void*)button_hook_stub_3]; \
    if (original_func) return original_func(instance, p1, p2, p3); \
    return nullptr; \
}

#define DEFINE_BUTTON_HOOK_STUB_4() \
void* HookManager::button_hook_stub_4(void* instance, void* p1, void* p2, void* p3, void* p4) { \
    if (g_enable_hook_call_logging) { \
        void* current_func_addr = (void*)button_hook_stub_4; \
        MethodInfo* method = g_stub_to_method_map[current_func_addr]; \
        if (method) { \
            Il2CppClass* klass = il2cpp_method_get_class(method); \
            LOGI(">>>> HOOK CALL: %s::%s", il2cpp_class_get_name(klass), il2cpp_method_get_name(method)); \
        } \
    } \
    auto original_func = (void* (*)(void*, void*, void*, void*, void*))g_original_func_map[(void*)button_hook_stub_4]; \
    if (original_func) return original_func(instance, p1, p2, p3, p4); \
    return nullptr; \
}

#define DEFINE_BUTTON_HOOK_STUB_5() \
void* HookManager::button_hook_stub_5(void* instance, void* p1, void* p2, void* p3, void* p4, void* p5) { \
    if (g_enable_hook_call_logging) { \
        void* current_func_addr = (void*)button_hook_stub_5; \
        MethodInfo* method = g_stub_to_method_map[current_func_addr]; \
        if (method) { \
            Il2CppClass* klass = il2cpp_method_get_class(method); \
            LOGI(">>>> HOOK CALL: %s::%s", il2cpp_class_get_name(klass), il2cpp_method_get_name(method)); \
        } \
    } \
    auto original_func = (void* (*)(void*, void*, void*, void*, void*, void*))g_original_func_map[(void*)button_hook_stub_5]; \
    if (original_func) return original_func(instance, p1, p2, p3, p4, p5); \
    return nullptr; \
}

#define DEFINE_BUTTON_HOOK_STUB_6() \
void* HookManager::button_hook_stub_6(void* instance, void* p1, void* p2, void* p3, void* p4, void* p5, void* p6) { \
    if (g_enable_hook_call_logging) { \
        void* current_func_addr = (void*)button_hook_stub_6; \
        MethodInfo* method = g_stub_to_method_map[current_func_addr]; \
        if (method) { \
            Il2CppClass* klass = il2cpp_method_get_class(method); \
            LOGI(">>>> HOOK CALL: %s::%s", il2cpp_class_get_name(klass), il2cpp_method_get_name(method)); \
        } \
    } \
    auto original_func = (void* (*)(void*, void*, void*, void*, void*, void*, void*))g_original_func_map[(void*)button_hook_stub_6]; \
    if (original_func) return original_func(instance, p1, p2, p3, p4, p5, p6); \
    return nullptr; \
}

#define DEFINE_BUTTON_HOOK_STUB_7() \
void* HookManager::button_hook_stub_7(void* instance, void* p1, void* p2, void* p3, void* p4, void* p5, void* p6, void* p7) { \
    if (g_enable_hook_call_logging) { \
        void* current_func_addr = (void*)button_hook_stub_7; \
        MethodInfo* method = g_stub_to_method_map[current_func_addr]; \
        if (method) { \
            Il2CppClass* klass = il2cpp_method_get_class(method); \
            LOGI(">>>> HOOK CALL: %s::%s", il2cpp_class_get_name(klass), il2cpp_method_get_name(method)); \
        } \
    } \
    auto original_func = (void* (*)(void*, void*, void*, void*, void*, void*, void*, void*))g_original_func_map[(void*)button_hook_stub_7]; \
    if (original_func) return original_func(instance, p1, p2, p3, p4, p5, p6, p7); \
    return nullptr; \
}

#define DEFINE_BUTTON_HOOK_STUB_8() \
void* HookManager::button_hook_stub_8(void* instance, void* p1, void* p2, void* p3, void* p4, void* p5, void* p6, void* p7, void* p8) { \
    if (g_enable_hook_call_logging) { \
        void* current_func_addr = (void*)button_hook_stub_8; \
        MethodInfo* method = g_stub_to_method_map[current_func_addr]; \
        if (method) { \
            Il2CppClass* klass = il2cpp_method_get_class(method); \
            LOGI(">>>> HOOK CALL: %s::%s", il2cpp_class_get_name(klass), il2cpp_method_get_name(method)); \
        } \
    } \
    auto original_func = (void* (*)(void*, void*, void*, void*, void*, void*, void*, void*, void*))g_original_func_map[(void*)button_hook_stub_8]; \
    if (original_func) return original_func(instance, p1, p2, p3, p4, p5, p6, p7, p8); \
    return nullptr; \
}

#define DEFINE_BUTTON_HOOK_STUB_9() \
void* HookManager::button_hook_stub_9(void* instance, void* p1, void* p2, void* p3, void* p4, void* p5, void* p6, void* p7, void* p8, void* p9) { \
    if (g_enable_hook_call_logging) { \
        void* current_func_addr = (void*)button_hook_stub_9; \
        MethodInfo* method = g_stub_to_method_map[current_func_addr]; \
        if (method) { \
            Il2CppClass* klass = il2cpp_method_get_class(method); \
            LOGI(">>>> HOOK CALL: %s::%s", il2cpp_class_get_name(klass), il2cpp_method_get_name(method)); \
        } \
    } \
    auto original_func = (void* (*)(void*, void*, void*, void*, void*, void*, void*, void*, void*, void*))g_original_func_map[(void*)button_hook_stub_9]; \
    if (original_func) return original_func(instance, p1, p2, p3, p4, p5, p6, p7, p8, p9); \
    return nullptr; \
}

#define DEFINE_BUTTON_HOOK_STUB_10() \
void* HookManager::button_hook_stub_10(void* instance, void* p1, void* p2, void* p3, void* p4, void* p5, void* p6, void* p7, void* p8, void* p9, void* p10) { \
    if (g_enable_hook_call_logging) { \
        void* current_func_addr = (void*)button_hook_stub_10; \
        MethodInfo* method = g_stub_to_method_map[current_func_addr]; \
        if (method) { \
            Il2CppClass* klass = il2cpp_method_get_class(method); \
            LOGI(">>>> HOOK CALL: %s::%s", il2cpp_class_get_name(klass), il2cpp_method_get_name(method)); \
        } \
    } \
    auto original_func = (void* (*)(void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*))g_original_func_map[(void*)button_hook_stub_10]; \
    if (original_func) return original_func(instance, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10); \
    return nullptr; \
}

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
void HookManager::install_hooks(const DynamicConfig& config) {
    // Устанавливаем глобальный флаг
    g_enable_hook_call_logging = config.enable_hook_call_logging;

    LOGI("HookManager: Installing dynamic hooks...");
    LOGI("HookManager: Using %zu keywords, %zu method blacklist, %zu class blacklist, %zu DLL blacklist.",
         config.keywords.size(), config.blacklist.size(), config.class_blacklist.size(), config.dll_blacklist.size());

    if (config.keywords.empty()) {
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

        const char* image_name = il2cpp_image_get_name(image);
        if (!image_name) continue;

        bool is_dll_blacklisted = false;
        for (const auto& blacklisted_dll_name : config.dll_blacklist) {
            if (strcmp(image_name, blacklisted_dll_name.c_str()) == 0) {
                is_dll_blacklisted = true;
                break;
            }
        }
        if (is_dll_blacklisted) {
            continue;
        }

        auto class_count = il2cpp_image_get_class_count(image);

        for (size_t j = 0; j < class_count; ++j) {
            Il2CppClass* klass = (Il2CppClass*)il2cpp_image_get_class(image, j);

            const char* class_name_cstr = il2cpp_class_get_name(klass);
            if (!class_name_cstr) continue;

            bool is_class_blacklisted = false;
            for (const auto& blacklisted_class_name : config.class_blacklist) {
                if (strcmp(class_name_cstr, blacklisted_class_name.c_str()) == 0) {
                    is_class_blacklisted = true;
                    break;
                }
            }
            if (is_class_blacklisted) {
                continue;
            }

            void* iter = nullptr;
            while (auto method_const = il2cpp_class_get_methods(klass, &iter)) {
                MethodInfo* method = const_cast<MethodInfo*>(method_const);
                const char* method_name_cstr = il2cpp_method_get_name(method);
                if (!method->methodPointer || !method_name_cstr) continue;

                std::string method_name(method_name_cstr);

                bool match = false;
                for (const auto& keyword : config.keywords) {
                    if (find_case_insensitive(method_name, keyword)) {
                        match = true;
                        break;
                    }
                }

                if (match) {
                    std::string full_method_name = std::string(class_name_cstr) + "::" + method_name;
                    bool is_method_blacklisted = false;
                    for (const auto& blacklisted_name : config.blacklist) {
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