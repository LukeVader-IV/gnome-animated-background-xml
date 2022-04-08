#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <stdbool.h>
#include <unistd.h>

#include "headers/functions.h"

#define REDBG "\e[41m"
#define RESET "\e[0m"

gint main (gint argc, gchar *argv[])
{
	g_set_prgname ("gnome-animation");
	g_set_application_name ("gnome-animation");

	/* if (getuid() != geteuid()){ */
	/* 	printf("\[41m this program is NOT intended to be run under sudo, please retry without sudo"); */
	/* } */
	if (geteuid()==0){
		char response = 'n';
		printf(REDBG "this program should NOT be run as sudo" RESET ", continue anyway? (y/n)");
		scanf("%c", &response);
		if (response != 'y' || response != 'Y'){
			exit(EXIT_FAILURE);
		}
	}

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

	if (moveimages(images, imagecount) != 0){
		printf("an error occurred moving the selected images to ~/.local/share/backgrounds/.hidden/\nplease make sure that the program has write-access to this folder");
		exit(EXIT_FAILURE);
	}


	/* printf("%s\n", option); */
   /* imagecount = inputparser(&option[], &duration, &images, argc, argv); */

	/* printdata(option, &duration, &imagecount, images, argv, argc); */

	if (writexml(option, duration, imagecount, images) != 0 ){
		printf("an error occurred moving the .xml file to ~/.local/share/backgrounds/\nplease make sure that the program has write-access to this folder");
	}

	if (writeindex() != 0){
		printf("there was an error moving the index, please check the program has access to /usr/share/gnome-background-properties");
		exit(EXIT_FAILURE);
	}

	return 0;
}

