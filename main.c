#include <glib.h>
#include <stdio.h>
#include "headers/input-output.h"

int main (int argc, char *argv[]){
	g_set_prgname ("gnome-xml-background-creator");
	g_set_application_name ("gnome-xml-background-creator");

	float timings[argc-1];
	int scaling[argc-1];
	char *images[argc-1];
	int framecount = input_parser(argc, argv, images, timings, scaling);
	if (framecount < 1){
		return framecount;
	}
	mvfile(framecount, images);
	/* for (int i = 0; i<framecount; i++){ */
	/* 	printf("%s : %.2fs : scale %d\n", images[i], timings[i], scaling[i]); */
	/* } */

	printf("%s\n", xml_string(framecount, images, timings, scaling));

	return 0;
}

