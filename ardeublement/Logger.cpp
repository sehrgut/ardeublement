#include <arduino.h>
#include <stdarg.h>
#include "Logger.hpp"

// todo: put buffer in logger so compile-time shutoff can save space

#define MSGLEN 128
char MSGBUF[MSGLEN];

Logger::Logger() {

}

void Logger::log(const char* message, ...) {
  va_list args;
  va_start(args, message);
  
  vsnprintf(MSGBUF, MSGLEN, message, args);
  va_end(args);

  Serial.write(MSGBUF);
}
