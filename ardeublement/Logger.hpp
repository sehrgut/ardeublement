#ifndef LOGGER_HPP
#define LOGGER_HPP

// todo: auto-append newlines

class Logger {
  private:
    Logger();

  public:
    static void log(const char* logger, const char* message, ...);
};

#endif
