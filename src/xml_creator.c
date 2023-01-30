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
	char* homedir = getenv("HOME");
	for (int i = 0; i<framecount; i++){
		outname[i] = malloc(200);
		currinput = fopen(images[i], "r");
		//TODO: add filetypes
		sprintf(outname[i], "%s/.local/share/backgrounds/image%d", homedir, i);
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

bool xml_maker(int framecount, char* images[], float* timings, int* scaling){

	char outname[100];
	char* homedir = getenv("HOME");
	sprintf(outname, "%s/.local/share/backgrounds/custom_bg.xml", homedir);
	FILE* output;
	output = fopen(outname, "w+");

	fprintf(output, "<background>\n");

	for (int i = 0; i < framecount; i++){
		//TODO: scaling
		fprintf(output, "\t<static>\n\t\t<duration>%f</duration>\n\t\t<file>%s</file>\n\t</static>\n\n", timings[i], images[i]);
	}

	fprintf(output, "</background>");
	fclose(output);
	printf("wrote xml to %s\n", outname);

	return true;
}
