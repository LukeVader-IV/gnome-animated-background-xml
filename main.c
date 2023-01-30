#include <glib.h>
#include <stdio.h>
#include "headers/input-output.h"

int main (int argc, char *argv[]){
	g_set_prgname ("gnome-xml-background-creator");
	g_set_application_name ("gnome-xml-background-creator");

	if (system("gnome-shell --help > /dev/null") != 0){
		printf("gnome-shell was not detected on your system, this tool is intended for the gnome desktop environment\n");
		return 1;
	}

	float timings[argc-1];
	int scaling[argc-1];
	char *images[argc-1];
	int framecount = input_parser(argc, argv, images, timings, scaling);
	if (framecount < 1){
		return framecount;
	}
	mvfile(framecount, images);
	xml_maker(framecount, images, timings, scaling);

	if (system("ostree --version > /dev/null") == 0){
		//immutable, uses ostree
	} else {
		//classic
	}


	return 0;
}

