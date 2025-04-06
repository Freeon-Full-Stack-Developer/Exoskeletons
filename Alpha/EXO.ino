#include <WiFi.h>
#include <HX711.h>
#include <Wire.h>

const char* ssid = "WIFI";      // Имя вашей сети Wi-Fi
const char* password = "192837465"; // Пароль от сети Wi-Fi

const char* server_ip = "192.168.0.109"; // IP-адрес вашего сервера
const int server_port = 8080; // Порт вашего сервера


// HX711 circuit wiring
const int LOADCELL_DOUT_PIN1 = 13;
const int LOADCELL_SCK_PIN1 = 12;

const int LOADCELL_DOUT_PIN2 = 14;
const int LOADCELL_SCK_PIN2 = 27;

const int K1 =  23;
const int K2 =  22;
const int D = 2;
// 2 и 3 это выводы реле

int KS1 = HIGH; 
int KS2 = HIGH; 

HX711 scale1;
HX711 scale2;

WiFiClient client;

void setup() {
  pinMode(D, OUTPUT);
  
  // Открываем серийный порт для вывода данных
  Serial.begin(115200);
  scale1.begin(LOADCELL_DOUT_PIN1, LOADCELL_SCK_PIN1);
  scale2.begin(LOADCELL_DOUT_PIN2, LOADCELL_SCK_PIN2);
  pinMode(K1, OUTPUT);
  pinMode(K2, OUTPUT);

  // Подключаемся к Wi-Fi
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  digitalWrite(D, HIGH);
  delay(100);
  digitalWrite(D, LOW);
  delay(100);
  }
  
  Serial.println("Connected to WiFi!");
  Serial.println("Setup complete");


}

void loop() {
  long reading1 = 0;
  long reading2 = 0;

  // Получаем данные с тензодатчика
  if (scale1.is_ready() && scale2.is_ready()) {
    reading1 = scale1.read() / 10000 - 634;
    reading2 = scale2.read() / 10000 - 148;

    Serial.print("1 reading: ");
    Serial.println(-reading1);
    Serial.print("2 reading: ");
    Serial.println(-reading2);

    // Управление реле на основе значений с тензодатчиков
    KS1 = (-reading1 > 5) ? LOW : HIGH;
    KS2 = (-reading2 > 5) ? LOW : HIGH;
    digitalWrite(K1, KS1);
    digitalWrite(K2, KS2);
  }

  // Проверяем, можем ли мы подключиться к серверу
  if (client.connect(server_ip, server_port)) {
    // Отправляем данные на сервер
    String data = "weight1=" + String(-reading1/4) + "&weight2=" + String(-reading2/4);
    client.print(String("GET /update?") + data + " HTTP/1.1\r\n" +
                 "Host: " + server_ip + "\r\n" + 
                 "Connection: close\r\n\r\n");
    Serial.println("Data sent to server");
    client.stop();
  } else {
    Serial.println("Failed to connect to server");
      digitalWrite(D, HIGH);
      delay(100);
      digitalWrite(D, LOW);
      delay(100);
  }

  // Ждем 1 секунду перед следующей отправкой данных
  delay(500);
}
