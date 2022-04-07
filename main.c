#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <stdbool.h>
#include <unistd.h>

#include "headers/functions.h"

gint main (gint argc, gchar *argv[])
{
	g_set_prgname ("gnome-animation");
	g_set_application_name ("gnome-animation");

	if (argc == 1){
		printnoargs(argv[0]);
		exit(EXIT_FAILURE);
	}

	if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0){
		printhelp(argv[0]);
		exit(EXIT_SUCCESS);
	}

   /* define defaults */
   char option[256] = "zoom";
   float duration = 4;
   int imagecount = 0;
   char *images[256];


	if (inputparser(option, &duration, &imagecount, images, argv, argc) == 1){
		exit(EXIT_FAILURE);
	}

	moveimages(images, imagecount);


	/* printf("%s\n", option); */
   /* imagecount = inputparser(&option[], &duration, &images, argc, argv); */

	/* printdata(option, &duration, &imagecount, images, argv, argc); */

	writexml(option, duration, imagecount, images);

	writeindex();

	return 0;
}

