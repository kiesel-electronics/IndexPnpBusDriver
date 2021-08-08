 /******************************************************************************
 *  The MIT License (MIT)
 *
 *  Copyright (c) 2021 Friedrich Kiesel
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 *******************************************************************************/

#include"arduinoHelpers.h"


void PrintHex8(uint8_t *data, uint8_t length, Stream *ser) {
  char tmp[length*3+1];
  byte first;
  int j=0;
  for (uint8_t i=0; i<length; i++) {           // iterate over bytes
    first = (data[i] >> 4) | 48;               // convert upper nibble to ASCII 0..9
    if (first > 57) tmp[j] = first + (byte)7;  // convert upper nibble to ASCII A..F
    else tmp[j] = first ;
    j++;

    first = (data[i] & 0x0F) | 48;             // convert upper nibble to ASCII 0..9
    if (first > 57) tmp[j] = first + (byte)7;  // convert upper nibble to ASCII A..F
    else tmp[j] = first;
    j++;
    tmp[j++] = 32;                             // add whitespace
  }
  tmp[length*3] = '\0';                        // add \0 to terminate string
  ser->println(tmp);                           // print string
}

void PrintHex16(uint8_t *data, uint8_t length, Stream *ser, boolean add_whitespace) {
  char tmp[length/2*5+1];
  byte first;
  int j=0;
  for (uint8_t i=0; i<length; i++) {           // iterate over bytes
    first = (data[i] >> 4) | 48;               // convert upper nibble to ASCII 0..9
    if (first > 57) tmp[j] = first + (byte)7;  // convert upper nibble to ASCII A..F
    else tmp[j] = first ;
    j++;

    first = (data[i] & 0x0F) | 48;             // convert upper nibble to ASCII 0..9
    if (first > 57) tmp[j] = first + (byte)7;  // convert upper nibble to ASCII A..F
    else tmp[j] = first;
    j++;
    if (add_whitespace) {
      if((i%2) == 1)                             // add whitespace every two bytes
        tmp[j++] = 32;
    }
  }
  tmp[j++] = '\0';                             // add \0 to terminate string
  ser->println(tmp);                           // print string
}


char hex_char_to_uint(char hex) {
  char hex_upper = toupper(hex);
  if ((hex_upper >= 48) && (hex_upper <= 57)) {
    // ASCII 0..9
    return (hex_upper - 48);
  } else if ((hex_upper >= 65) && (hex_upper <= 70)) {
    // ASCII A..F
    return (hex_upper - 65 + 10);
  } else {
    // no Hex character
    return 0;
  }
}


void conv_hex_to_uint8(char *hex, uint8_t *bytes, uint8_t byte_num) {
  for (uint8_t i=0; i<byte_num; i++) {
    uint8_t lower = hex_char_to_uint(hex[i*2]);
    uint8_t upper = hex_char_to_uint(hex[i*2+1]);
    bytes[i] = lower | (upper << 4);
  }
}

