//
// Created by Friedrich Kiesel on 10.06.2021.
//

#include "gcodeParser.h"


gcodeParser::gcodeParser(Stream* _interface, char termination_char) {
  interface = _interface;
  term_char = termination_char;
  number_of_cmds = 0;
  msg_buffer_cnt = 0;
}


int gcodeParser::RegisterCommand(const char *command, gcode_caller_t caller) {
  strcpy(msg_buffer, command);
  return this->RegisterCommand(msg_buffer, caller);
}


int gcodeParser::RegisterCommand(char *command, gcode_caller_t caller) {
  // make sure, only plausible codes are added
  if (strlen(command) > 5)
    return 1;
  // calculate hash of command String and store it in the cmd_list
  unsigned long cmdHash = gcodeParser::hash((unsigned char*)command);
  cmd_list[number_of_cmds].cmd_hash = cmdHash;
  cmd_list[number_of_cmds].caller = caller;
  number_of_cmds++;
  return 0;
}


void gcodeParser::Handler() {
  char c;

  while(interface->available()) {
    c = interface->read();
    if (c == term_char) {
      // end of line
      msg_buffer[msg_buffer_cnt] = '\0';
      HandleCmd();
      msg_buffer_cnt = 0;
      break;
    } else {
      msg_buffer[msg_buffer_cnt++] = c;
    }
  }
}


void gcodeParser::HandleCmd() {
  char* cmd;
  char* params[8];
  uint8_t number_of_params = 0;
  const char delims[] = {' ', '\t'};
  unsigned long hash_val;
  // isolate command
  cmd = strtok(msg_buffer, delims);
  // parameters
  for(int i = 0; i < 7; i++) {
    params[i] = strtok(NULL, delims);
    if(params[i] == NULL){
      number_of_params = i;
      break;
    }
  }
  // calc cmd hash
  hash_val = hash((unsigned char *)cmd);
  for (int i = 0; i < number_of_cmds; i++) {
    if (hash_val == cmd_list[i].cmd_hash) {
      // found cmd
      if (cmd_list[i].caller != NULL)
        cmd_list[i].caller(number_of_params, params);
      return;
    }
  }
  // didn't find cmd
  interface->println("Unknown command!");
}


unsigned long gcodeParser::hash(unsigned char *str) {
  unsigned long hash = 5381;
  int c;

  while (c = *str++)
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

  return hash;
}