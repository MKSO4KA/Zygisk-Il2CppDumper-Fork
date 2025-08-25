#ifndef ZYGISK_IL2CPPDUMPER_CONFIGFETCHER_H
#define ZYGISK_IL2CPPDUMPER_CONFIGFETCHER_H

#include <vector> // <-- ДОБАВЛЕНО
#include <string> // <-- ДОБАВЛЕНО

struct DynamicConfig {
    std::vector<std::string> keywords;
    std::vector<std::string> blacklist;
    std::vector<std::string> class_blacklist;
    std::vector<std::string> dll_blacklist; // <-- ДОБАВЛЕНО
    bool fetch_successful = false;
};

class ConfigFetcher {
public:
    // ЗАМЕНИТЕ НА IP-АДРЕС ВАШЕГО ПК В ЛОКАЛЬНОЙ СЕТИ!
    static constexpr const char* SERVER_IP = "192.168.0.104";
    static constexpr int SERVER_PORT = 8888;

    static DynamicConfig fetch();
};

#endif //ZYGISK_IL2CPPDUMPER_CONFIGFETCHER_H