#ifndef BUTTON_HANDLER_H
#define BUTTON_HANDLER_H

#include <Arduino.h>
#include "Config.h"

enum Button {
  BUTTON_NONE = 0,
  BUTTON_UP,
  BUTTON_DOWN,
  BUTTON_SELECT,
  BUTTON_BACK
};

class ButtonHandler {
public:
  ButtonHandler();
  
  void begin();
  Button getButtonPress();
  bool isButtonPressed(Button button);
  
private:
  unsigned long lastDebounceTime[4];
  
  bool isDebounced(int buttonIndex);
  int getPinForButton(Button button);
};

#endif // BUTTON_HANDLER_H
