#include "ButtonHandler.h"

ButtonHandler::ButtonHandler() {
  for (int i = 0; i < 4; i++) {
    lastDebounceTime[i] = 0;
  }
}

void ButtonHandler::begin() {
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_SELECT, INPUT_PULLUP);
  pinMode(BTN_BACK, INPUT_PULLUP);
}

Button ButtonHandler::getButtonPress() {
  if (digitalRead(BTN_UP) == LOW && isDebounced(0)) {
    lastDebounceTime[0] = millis();
    return BUTTON_UP;
  }
  
  if (digitalRead(BTN_DOWN) == LOW && isDebounced(1)) {
    lastDebounceTime[1] = millis();
    return BUTTON_DOWN;
  }
  
  if (digitalRead(BTN_SELECT) == LOW && isDebounced(2)) {
    lastDebounceTime[2] = millis();
    return BUTTON_SELECT;
  }
  
  if (digitalRead(BTN_BACK) == LOW && isDebounced(3)) {
    lastDebounceTime[3] = millis();
    return BUTTON_BACK;
  }
  
  return BUTTON_NONE;
}

bool ButtonHandler::isButtonPressed(Button button) {
  int pin = getPinForButton(button);
  if (pin == -1) return false;
  
  return digitalRead(pin) == LOW;
}

bool ButtonHandler::isDebounced(int buttonIndex) {
  unsigned long now = millis();
  return (now - lastDebounceTime[buttonIndex]) > BUTTON_DEBOUNCE_MS;
}

int ButtonHandler::getPinForButton(Button button) {
  switch (button) {
    case BUTTON_UP:     return BTN_UP;
    case BUTTON_DOWN:   return BTN_DOWN;
    case BUTTON_SELECT: return BTN_SELECT;
    case BUTTON_BACK:   return BTN_BACK;
    default:            return -1;
  }
}
