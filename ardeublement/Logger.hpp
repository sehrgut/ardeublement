#ifndef LOGGER_HPP
#define LOGGER_HPP

// todo: auto-append newlines
// todo: https://www.arduino.cc/reference/en/language/functions/communication/serial/availableforwrite/
#define LOGGER_BUFLEN 128

class Logger {
  private:
    static char msgbuf[LOGGER_BUFLEN];
    Logger();

  public:
    static void log(const char* logger, const char* message, ...);
};

#endif
