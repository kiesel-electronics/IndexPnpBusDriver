//
// Created by Friedrich on 10.06.2021.
//

#ifndef FW_GCODEPARSER_H
#define FW_GCODEPARSER_H

#ifndef GCODE_MAX_COMMANDS
  #define  GCODE_MAX_COMMANDS 20
#endif

#ifndef GCODE_BUFFER_LENGTH
  #define GCODE_BUFFER_LENGTH 64
#endif

#include "Arduino.h"

typedef void (*gcode_caller_t)(uint8_t, char**);

class gcodeCommand {
public:
    gcode_caller_t caller;
    unsigned long cmd_hash;
};



class gcodeParser {
public:
    gcodeParser(Stream* _interface, char termination_char);
    int RegisterCommand(char* command, gcode_caller_t caller);
    int RegisterCommand(const char* command, gcode_caller_t caller);
    void Handler(void);
    void HandleCmd();

    static unsigned long hash(unsigned char *str);

protected:
    Stream* interface;
    int number_of_cmds = 0;
    gcodeCommand cmd_list[GCODE_MAX_COMMANDS];
    char term_char;
    int msg_buffer_cnt;
    char msg_buffer[GCODE_BUFFER_LENGTH];
};


#endif //FW_GCODEPARSER_H
