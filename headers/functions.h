#ifndef FUNCTIONS_H
#define FUNCTIONS_H

void printnoargs(char *name);

void printhelp(char *name);

int inputparser(char option[256], float *duration, int *imagecount, char *images[], char *arguments[], int argcount);

void printdata(char option[256], float *duration, int *imagecount, char *images[], char *arguments[], int argcount);

int writexml(char option[256], float duration, int imagecount, char *images[]);

void moveimages(char *images[], int imagecount);

#endif
