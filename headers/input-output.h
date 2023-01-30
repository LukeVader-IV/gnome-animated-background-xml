#ifndef INPUTOUTPUT
#define INPUTOUTPUT
#include <stdbool.h>

int input_parser(int argc, char *argv[], char *images[], float *timings, int *scaling);
int arg_passer(char *arg);
char* getmode(int mode);
char* xml_maker(int framecount, char* images[], float* timings, int* scaling);
bool mvfile(int framecount, char** images);
float time_parser(char* arg);

#endif

