#include "ConfigFetcher.h"
#include "log.h"

// --- НАЧАЛО ИЗМЕНЕНИЙ ---
// Говорим библиотеке nlohmann/json НЕ использовать исключения C++
#define JSON_NOEXCEPTION 1
// Говорим библиотеке НЕ использовать специфичные для C++20 типы (char8_t)
#define JSON_HAS_CPP_20 0
// --- КОНЕЦ ИЗМЕНЕНИЙ ---

#include "include/json.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <vector>

using json = nlohmann::json;

DynamicConfig ConfigFetcher::fetch() {
    LOGI("ConfigFetcher: Attempting to fetch dynamic config from %s:%d", SERVER_IP, SERVER_PORT);
    DynamicConfig config;

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        LOGE("ConfigFetcher: Socket creation error");
        return config;
    }

    sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);

    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        LOGE("ConfigFetcher: Invalid address / Address not supported");
        close(sock);
        return config;
    }

    // Устанавливаем таймаут на соединение (5 секунд)
    struct timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout));

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        LOGE("ConfigFetcher: Connection Failed");
        close(sock);
        return config;
    }

    const char* request = "GET /config HTTP/1.1\r\nHost: server\r\nConnection: close\r\n\r\n";
    send(sock, request, strlen(request), 0);

    char buffer[4096] = {0};
    std::string http_response;
    while (read(sock, buffer, sizeof(buffer) - 1) > 0) {
        http_response += buffer;
        memset(buffer, 0, sizeof(buffer));
    }
    close(sock);

    size_t json_start = http_response.find("\r\n\r\n");
    if (json_start == std::string::npos) {
        LOGE("ConfigFetcher: Invalid HTTP response received.");
        return config;
    }

    std::string json_body = http_response.substr(json_start + 4);

    // --- НАЧАЛО ИЗМЕНЕНИЙ: Заменяем try/catch на проверку is_discarded() ---
    json j = json::parse(json_body, nullptr, false); // Третий параметр false отключает исключения
    if (j.is_discarded()) {
        LOGE("ConfigFetcher: JSON parsing failed.");
        return config;
    }

    // Проверяем наличие ключей перед доступом
    if (j.contains("keywords") && j.contains("blacklist") && j.contains("class_blacklist") && j.contains("dll_blacklist")) { // <-- ДОБАВЛЕНО УСЛОВИЕ
        config.keywords = j["keywords"].get<std::vector<std::string>>();
        config.blacklist = j["blacklist"].get<std::vector<std::string>>();
        config.class_blacklist = j["class_blacklist"].get<std::vector<std::string>>();
        config.dll_blacklist = j["dll_blacklist"].get<std::vector<std::string>>(); // <-- ДОБАВЛЕНО
        config.fetch_successful = true;
        LOGI("ConfigFetcher: Successfully fetched and parsed config.");
        LOGI("Keywords: %zu, Method Blacklist: %zu, Class Blacklist: %zu, DLL Blacklist: %zu", // <-- ОБНОВЛЕН ЛОГ
             config.keywords.size(), config.blacklist.size(), config.class_blacklist.size(), config.dll_blacklist.size());
    } else {
        LOGE("ConfigFetcher: JSON does not contain required keys ('keywords', 'blacklist', 'class_blacklist', 'dll_blacklist')."); // <-- ОБНОВЛЕН ЛОГ
    }

    return config;
}