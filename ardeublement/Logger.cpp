#include <arduino.h>
#include <stdarg.h>
#include "Logger.hpp"

// todo: getLogger that returns a lambda?

char Logger::msgbuf[LOGGER_BUFLEN];

Logger::Logger() {

}

void Logger::log(const char* logger, const char* message, ...) {
  va_list args;
  va_start(args, message);
  
  vsnprintf(msgbuf, LOGGER_BUFLEN, message, args);
  va_end(args);

  Serial.write("[");
  Serial.write(logger);
  Serial.write("] ");
  Serial.write(msgbuf);
  Serial.write("\n");
}
