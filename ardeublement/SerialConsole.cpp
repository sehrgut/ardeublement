#include <arduino.h>
#include "Params.hpp"
#include "SerialConsole.hpp"
#include "Logger.hpp"

#define LOGGER "SerialConsole"

// todo: better globals interface
// todo: compile-out debug lines
// todo: midi note number format specifier
// todo: interpret scientific pitch notation for key
// todo: command to print current params

const char* SerialCommandNames[SerialCommandsMax+1] = { "on", "off", "key", "dev", "ton", "coh", "bpm", "p", "?" };

SerialConsole::SerialConsole() {
  Serial.begin(9600);
}

// todo: should this be static?
void SerialConsole::write(const char *message, ...) {
  va_list args;
  va_start(args, message);
  
  vsnprintf(this->msgbuf, SC_MSG_BUFLEN, message, args);
  va_end(args);

  Serial.write("[Console] ");
  Serial.write(this->msgbuf);
  //Serial.write("\n"); // todo: separate writeLine function
}

void SerialConsole::run_command(char *cmd, int value) {
  // todo: test space efficiency of early-return vs set-dirty-every-time
  // todo: strncmp?

  int i;
  for (i=0; i <= SerialCommands::SerialCommandsMax; i++) {
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
        case Key:
          if (value < 0 || value > 127) {
            this->write("Key out of range: %d\n", value);
          } else {
            GLOBAL_PARAMS.center = value;
            GLOBAL_PARAMS.dirty = true;
            this->write("Key updated: %d\n", value);
          }
          break;
        case Deviation:
          GLOBAL_PARAMS.deviation = value;
          GLOBAL_PARAMS.dirty = true;
          this->write("Deviation updated: %d\n", value);
          break;
        case Tonality:
          if (value < 0 || value > 100) {
            this->write("Tonality must be between 0 and 100.\n");
          } else {
            GLOBAL_PARAMS.tonality = (double)value / 100.0;
            GLOBAL_PARAMS.dirty = true;
            this->write("Tonality updated: %d\n", value);
          }
          break;
        case Coherence:
          if (value < 0 || value > 100) {
            this->write("Coherence must be between 0 and 100.\n");
          } else {
            GLOBAL_PARAMS.coherence = (double)value / 100.0;
            GLOBAL_PARAMS.dirty = true;
            this->write("Coherence updated: %d\n", value);
          }
          break;
        case Print:
          this->write("DEV=%d TON=%d COH=%d R=%d KEY=%d BPM=%d\n",
            GLOBAL_PARAMS.deviation,
            (int)(GLOBAL_PARAMS.tonality * 100),
            (int)(GLOBAL_PARAMS.coherence * 100),
			GLOBAL_PARAMS.range,
            GLOBAL_PARAMS.center,
            GLOBAL_PARAMS.bpm
            );
          break;
        case BPM:
          if (value < 0) {
            this->write("BPM must be > 0.\n");
          } else {
            GLOBAL_PARAMS.bpm = value;
            GLOBAL_PARAMS.dirty = true;
            this->write("BPM updated: %d\n", value);
          }
          break;
        case Help:
          this->write("Usage: [[dev|key|bpm|ton|coh] INT] |\n");
          this->write("       [on|off|p|?]\n");
          break;
        default:
          this->write("Unknown command: '%s'\n", cmd);
          break;
       }
     }
   }
}

void SerialConsole::process_commands() {
  int i=0; // todo: will this need to pick up from other calls?
  //Logger::log(LOGGER, "(process_commands)");
  while(Serial.available() > 0){
    if (i < SC_CMD_BUFLEN-1) {
      this->cmdbuf[i++] = Serial.read();     
    } else {
      this->write("Error: command too long\n"); // todo: stop on newline?
      i = 0; //ignore command later
      while (Serial.available() > 0) //consume buffer
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
