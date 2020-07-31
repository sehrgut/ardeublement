#include <arduino.h>
#include "Params.hpp"
#include "SerialConsole.hpp"

SerialConsole::SerialConsole() {
}

void SerialConsole::process_commands() {
  int i=0; // todo: will this need to pick up from other calls?
  
  while(Serial.available()){
    if (i < SC_CMD_BUFLEN-1) {
      this->cmdbuf[i++] = Serial.read();     
    } else {
      Serial.print("Error: command too long");
      while (Serial.available())
        (void)Serial.read();
    }
  }
  
  this->cmdbuf[i] = '\0';

  if (i>0) {
    char cmd[16];
    int value;
  
    sscanf(this->cmdbuf, "%s %d", cmd, &value);
  
    if (strcmp(cmd, "dev") == 0) {
      deviation = value;
      dirty = true;
      Serial.write("Deviation updated: ");
      Serial.print(value);
      Serial.write("\n");
    } else {
      Serial.write("Unknown command: ");
      Serial.print(cmd);
      Serial.write("\n");
    }
  }
}
