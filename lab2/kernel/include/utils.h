#ifndef UTILS_H
#define UTILS_H

#include "uart.h"

#define MAX_INPUT_LEN 256

int h2d(const char *hex);
int strcmp(const char *str1, const char *str2);
unsigned long align(unsigned long num, unsigned long alignment);
void input_string(char *input_buffer);
unsigned int atoi(const char *str);
unsigned int input_int(char *input_buffer);
unsigned int strlen(const char *str);
#endif