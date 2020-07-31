#ifndef SERIALCONSOLE_HPP
#define SERIALCONSOLE_HPP

#define SC_CMD_BUFLEN 16

class SerialConsole {
  private:
    char cmdbuf[SC_CMD_BUFLEN];

  public:
    SerialConsole();
    void process_commands();
};

#endif
