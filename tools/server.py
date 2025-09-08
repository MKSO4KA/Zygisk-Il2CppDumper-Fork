import http.server
import socketserver
import json
import os
import socket

# --- Настройки ---
PORT = 8888
JSON_FILE = "config.json"

# --- Логика сервера ---

class ConfigHandler(http.server.SimpleHTTPRequestHandler):
    """
    Обработчик запросов, который отдает JSON только по пути /config.
    """
    def do_GET(self):
        if self.path == '/config':
            try:
                # Проверяем, существует ли файл
                if not os.path.exists(JSON_FILE):
                    self.send_error(404, f"File Not Found: {JSON_FILE}")
                    print(f"Ошибка: Файл '{JSON_FILE}' не найден в текущей директории.")
                    return

                # Читаем и отправляем файл
                with open(JSON_FILE, 'r', encoding='utf-8') as f:
                    content = f.read()
                    # Проверяем, что JSON валидный
                    json.loads(content)

                self.send_response(200)
                self.send_header('Content-type', 'application/json')
                self.end_headers()
                self.wfile.write(content.encode('utf-8'))
                print(f"Конфигурация '{JSON_FILE}' успешно отправлена клиенту {self.client_address[0]}")

            except json.JSONDecodeError:
                error_msg = f"Invalid JSON in {JSON_FILE}"
                self.send_error(500, error_msg)
                print(f"Ошибка: Невалидный JSON в файле '{JSON_FILE}'.")
            except Exception as e:
                self.send_error(500, f"Server Error: {e}")
                print(f"Произошла внутренняя ошибка сервера: {e}")
        else:
            self.send_error(404, "Not Found: Use /config endpoint")

def get_local_ip():
    """
    Находит локальный IP-адрес компьютера.
    """
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    try:
        # Не обязательно должен быть доступен
        s.connect(('10.255.255.255', 1))
        IP = s.getsockname()[0]
    except Exception:
        IP = '127.0.0.1'
    finally:
        s.close()
    return IP

# --- Запуск сервера ---

if __name__ == "__main__":
    # Создаем TCPServer, который будет слушать на всех интерфейсах
    with socketserver.TCPServer(("", PORT), ConfigHandler) as httpd:
        local_ip = get_local_ip()
        print("="*50)
        print("Сервер для Zygisk-Il2CppDumper-Fork запущен.")
        print(f"Он слушает порт: {PORT}")
        print(f"Ваш локальный IP-адрес: {local_ip}")
        print("\nВАЖНО:")
        print(f"1. Убедитесь, что в файле 'module/src/main/cpp/ConfigFetcher.h'")
        print(f"   установлен IP-адрес: const char* SERVER_IP = \"{local_ip}\";")
        print("2. После изменения IP пересоберите Magisk-модуль.")
        print("3. Ваш телефон и компьютер должны быть в одной Wi-Fi сети.")
        print("="*50)

        try:
            httpd.serve_forever()
        except KeyboardInterrupt:
            print("\nСервер остановлен.")
            pass