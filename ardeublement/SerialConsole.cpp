#include <arduino.h>
#include "Params.hpp"
#include "SerialConsole.hpp"

// todo: buffer for snprintf
// todo: better globals interface
// todo: compile-out debug lines
// todo: midi note number format specifier
// todo: interpret scientific pitch notation for key

const char* SerialCommandNames[5] = { "on", "off", "?", "key", "dev" };

SerialConsole::SerialConsole() {
}

// todo: should this be static?
void SerialConsole::write(const char *message, ...) {
  va_list args;
  va_start(args, message);
  
  vsnprintf(this->msgbuf, SC_MSG_BUFLEN, message, args);
  va_end(args);

  Serial.write("[Console] ");
  Serial.write(this->msgbuf);
  //Serial.write("\n"); // separate writeLine function
}

void SerialConsole::run_command(char *cmd, int value) {
  // todo: test space efficiency of early-return vs set-dirty-every-time
  // todo: strncmp?

  int i;
  for (i=0; i< SerialCommands::SerialCommandsMax; i++) {
    if (strcmp(cmd, SerialCommandNames[i]) == 0) {
      this->write("Received command '%s'\n", cmd);
      switch ((SerialCommands)i) {
        case On:
          if (GLOBAL_PARAMS.running == false) {
            GLOBAL_PARAMS.running = true;
            GLOBAL_PARAMS.dirty = true;
            this->write("Starting.\n");
          } else {
            this->write("Already running.\n");
          }
          break;
        case Off:
          if (GLOBAL_PARAMS.running == true) {
            GLOBAL_PARAMS.running = false;
            GLOBAL_PARAMS.dirty = true;
            this->write("Stopping.\n");
          } else {
            this->write("Not running.\n");
          }
          break;
        case Help:
          this->write("Usage: [[dev|key] INT] | [on|off|?]\n");
          break;
        case Key:
          if (value < 0 || value > 127) {
            this->write("Key out of range: %d\n", value);
          } else {
            GLOBAL_PARAMS.center = value;
            GLOBAL_PARAMS.dirty = true;
            this->write("Key updated: %d\n", value);
          }
          break;
        case Dev:
          GLOBAL_PARAMS.deviation = value;
          GLOBAL_PARAMS.dirty = true;
          this->write("Deviation updated: %d\n", value);
          break;
        default:
          this->write("Unknown command: '%s'\n", cmd);
          break;
       }
     }
   }

/*
  if (strcmp(cmd, "dev") == 0) {
    GLOBAL_PARAMS.deviation = value;
    GLOBAL_PARAMS.dirty = true;
    this->write("Deviation updated: %d\n", value);
  } else if (strcmp(cmd, "key") == 0) {
    if (value < 0 || value > 127) {
      this->write("Key out of range: %d\n", value);
    } else {
      GLOBAL_PARAMS.center = value;
      GLOBAL_PARAMS.dirty = true;
      this->write("Key updated: %d\n", value);
    }
  } else if (strcmp(cmd, "on") == 0) {
    return;
  } else if (strcmp(cmd, "off") == 0) {
    return;
  } else if (strcmp(cmd, "?") == 0) {
    this->write("Usage: [[dev|key] INT] | [on|off|?]\n");
  } else {
    this->write("Unknown command: '%s'\n", cmd);
  }
*/
}

void SerialConsole::process_commands() {
  int i=0; // todo: will this need to pick up from other calls?
  
  while(Serial.available()){
    if (i < SC_CMD_BUFLEN-1) {
      this->cmdbuf[i++] = Serial.read();     
    } else {
      this->write("Error: command too long\n"); // todo: stop on newline?
      i = 0; //ignore command later
      while (Serial.available()) //consume buffer
        (void)Serial.read();
    }
  }
  
  this->cmdbuf[i] = '\0'; // necessary for sscanf?

  if (i>0) { // i>0 == command available
    char cmd[16]; //todo: const-ify? This would make %16s hard to do tho
    int value;
  
    sscanf(this->cmdbuf, "%16s %d", cmd, &value); // check return val?
    this->run_command(cmd, value);
  }
}
