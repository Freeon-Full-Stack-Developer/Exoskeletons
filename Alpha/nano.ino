const int relayPins[8] = {2, 3, 4, 5, 6, 7, 8, 9}; // Пины для реле

void setup() {
  Serial.begin(115200); // Инициализация последовательного порта
  for (int i = 0; i < 8; i++) {
    pinMode(relayPins[i], OUTPUT); // Устанавливаем пины реле как выходы
    digitalWrite(relayPins[i], HIGH); // Изначально выключаем реле
  }
}

void loop() {
  if (Serial.available() > 0) {
    String relayStates = Serial.readStringUntil('\n'); // Читаем данные до новой строки

    // Устанавливаем состояние реле в зависимости от полученных данных
    for (int i = 0; i < 8; i++) {
      if (i < relayStates.length()) {
        if (relayStates.charAt(i) == '1') {
          digitalWrite(relayPins[i], LOW); // Включаем реле
        } else {
          digitalWrite(relayPins[i], HIGH); // Выключаем реле
        }
      }
    }
  }
}
