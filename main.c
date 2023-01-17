#include <glib.h>
#include <stdio.h>
#include "headers/input-output.h"

int main (int argc, char *argv[]){
	g_set_prgname ("gnome-xml-background-creator");
	g_set_application_name ("gnome-xml-background-creator");

	float timings[argc-1];
	int scaling[argc-1];
	char *images[argc-1];
	int stostate = input_parser(argc, argv, images, timings, scaling);
	if (stostate < 1){
		return stostate;
	}
	for (int i = 0; i<stostate; i++){
		printf("%s : %.2fs : scale %d\n", images[i], timings[i], scaling[i]);
	}

	return 0;
}

