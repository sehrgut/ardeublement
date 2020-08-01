#ifndef SERIALCONSOLE_HPP
#define SERIALCONSOLE_HPP

#define SC_CMD_BUFLEN 16
#define SC_MSG_BUFLEN 48

//todo: change composer
//todo: show params

typedef enum {
  On, Off, Key, Deviation, Tonality, BPM, Help, SerialCommandsMax = Help
} SerialCommands;

class SerialConsole {
  private:
    char cmdbuf[SC_CMD_BUFLEN];
    char msgbuf[SC_MSG_BUFLEN];
    void write(const char *message, ...);
    void run_command(char *cmd, int value);

  public:
    SerialConsole();
    void process_commands();
};

#endif
