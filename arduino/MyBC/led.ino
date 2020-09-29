// led操作
void handleLed() {
  ticker.detach();
  String type = webServer.arg("type");
  Serial.println("led: " + type);
  replyServerCode(200);

  if (type.length() != 3) {
    isLightLed1 = false;
    isLightLed2 = false;
    isWLLed = false;
  } else {
    isLightLed1 = type.charAt(0) == '1';
    isLightLed2 = type.charAt(1) == '1';
    isWLLed = type.charAt(2) == '1';
  }

  tickerIndex = 0;
  if (isLightLed1 || isLightLed2) {
    ticker.attach_ms(isWLLed ? 666 : 5, ledLightType);
  } else {
    ticker.once_ms(1, ledLightType);
  }
}

// led类型
void ledLightType() {
  tickerIndex++;
  if (isWLLed && tickerIndex == 1) {
    tickerIndex++;
  }
  switch (tickerIndex) {
    case 1:
      ledLight(0);
      break;
    case 2:
      if (isLightLed1) {
        ledLight(1);
      }
      break;
    case 3:
      if (isLightLed2) {
        ledLight(2);
      }
      tickerIndex = 0;
      break;
  }
}

// led操作
void ledLight(uint8_t type) {
  switch (type) {
    case 0:
      digitalWrite(L1, LOW);
      digitalWrite(L2, LOW);
      break;
    case 1:
      digitalWrite(L1, HIGH);
      digitalWrite(L2, LOW);
      break;
    case 2:
      digitalWrite(L1, LOW);
      digitalWrite(L2, HIGH);
      break;
  }
}

void offLed() {
  ledLight(0);
}
