#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "../headers/input-output.h"

#define HELP "this is the help page for gnome-animated-background-xml\n\n -h --help\tdisplays this help page\n-t TIME\t sets the timing of a frame in seconds(default:2)\n\nbackground modes:\n\n-wallpaper\n-centered\n-scaled\n-streched\n-spanned\n-zoom(default)\n\nany argument is per-frame, and needs to be specified after the image, images will be default use the same arguments as the previous frame"

static char *bgmodes[6] = {"-zoom", "-wallpaper", "-centered", "-scaled", "-stretched", "-spanned"};

/**
 * function that parses the input
 *
 * @param argc amount of arguments at input
 * @param argv array of arguments at input
 * @param images array that will contain all image files
 * @param timings array that will contain all image timings
 * @param scaling array that will contain an integer representation of the scaling mode for all images
 *
 * @return # of images
 *         -1 if non-successful
 *         0 for help
 */
int input_parser(int argc, char *argv[], char *images[], float *timings, int *scaling){
	int imagecounter = 0;
	for (int i = 1; i<argc; i++){
		if (argv[i][0] == '-'){ //argument
			int stostate = arg_passer(argv[i]);
			if (stostate != 0 && imagecounter < 1){
				printf("ERROR: arguments must come after image");
				return(-1);
			}
			if (stostate > 0){
				scaling[imagecounter-1] = stostate-1;
			}else if (stostate == -1){
				i++;
				int stofloat;
				if (i == argc || (stofloat = atof(argv[i])) <= 0){
					printf("argument -t is missing a time value\n\nSyntax: -t TIME");
					return -1;
				}
				timings[imagecounter-1] = stofloat;
			}else {
				return stostate;
			}
		} else {
			if (access(argv[i], F_OK) == 0){
				images[imagecounter] = argv[i];
				if (imagecounter == 0){
					timings[imagecounter] = 2;
					scaling[imagecounter] = 0;
				} else {
					timings[imagecounter] = timings[imagecounter-1];
					scaling[imagecounter] = scaling[imagecounter-1];
				}
				imagecounter++;
			}
		}
	}

	return imagecounter;
}

/**
 * function that returns an integer representation of the input scaling mode, checks if it is -help or -t
 *
 * @param arg string that will be checked
 *
 * @return 1-6 representing a scaling mode
 *         0 argument was help
 *         -1 next argument is timing
 *         -2 not a valid argument
 */
int arg_passer(char *arg){
	for (int i = 0; i<6; i++){
		if (strcmp(bgmodes[i], arg) == 0){
			return i+1;
		}
	}
	if (strcmp(arg, "-h") == 0|| strcmp(arg, "--help") == 0){
		printf(HELP);
		return 0;
	}
	if (strcmp(arg, "-t") == 0){
		return -1;
	}
	printf("unrecognised argument\n");

	return -2;
}

/**
 * get string of background scaling mode
 *
 * @param mode value repr~/Pictures/Gorilla.jpgesenting the mode
 *
 * @return string with mode
 *         zoom default value in case of invalid value
 */
char* getmode(int mode){
	if (mode > 5 || mode < 0){
		printf("invalid mode error");
		return bgmodes[0]+1;
	}
	return bgmodes[mode]+1;
}
