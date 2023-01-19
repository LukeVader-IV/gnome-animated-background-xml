#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/input-output.h"

/**
 * function that moves the selected images to a safe location
 *
 * @param int framecount total amount of images
 * @param images array countaining the paths to files that will be moved
 *
 * @return true all files moved successfully
 *         false one or more file moves failed
 */

bool mvfile(int framecount, char** images){

	FILE *currinput;
	FILE *curroutput;
	char* outname[framecount];
	char* user = getenv("USER");
	for (int i = 0; i<framecount; i++){
		outname[i] = malloc(200);
		currinput = fopen(images[i], "r");
		//TODO: add filetypes
		sprintf(outname[i], "/var/home/%s/.local/share/backgrounds/image%d", user, i);
		curroutput = fopen(outname[i], "w+");
		if (currinput == NULL){
			printf("input file doesn't exist: %s\n", images[i]);
			return false;
		} else if (curroutput == NULL){
			printf("error moving file: %s to %s\n", images[i], outname[i]);
			return false;
		}
		fseek(currinput, 0, SEEK_END);
		int size = ftell(currinput);
		fseek(currinput, 0, SEEK_SET);
		for (int pos = 0; pos < size; pos++){
			fputc(fgetc(currinput), curroutput);
		}
		fclose(currinput);
		fclose(curroutput);
		images[i]=outname[i];
	}
	return true;

	return false;
}

/**
 * function that formats into XML
 *
 * @param framecount amount of images
 * @param images array of all images
 * @param timings array of each frame timing
 * @param scaling array of each frame scaling
 *
 * @return string in xml format, seen as an animated background by gnome
 */

char* xml_string(int framecount, char* images[], float* timings, int* scaling){
	int totalsize = 0;
	for (int i = 0; i < framecount; i++){
		totalsize += sizeof(images[i]) + sizeof(timings[i]) + sizeof(getmode(scaling[i])) + 200;
	}

	char* output = malloc(totalsize);

	sprintf(output, "<background>\n");

	for (int i = 0; i < framecount; i++){
		//TODO: scaling
		sprintf(output, "%s\t<static>\n\t\t<duration>%f</duration>\n\t\t<file>%s</file>\n\t</static>\n\n", output, timings[i], images[i]);
	}

	sprintf(output, "%s</background>", output);

	return output;
}
