#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <unistd.h>
#include <stdbool.h>
#include <unistd.h>

#include "../headers/functions.h"

void printnoargs(char *name){
	printf(	"no frames specified, please add frames\n"
					"example: %s path/to/image.png"
					"\ntype %s --help for help\n"
					, name, name);
}

void printhelp(char *name){
	printf("this is the help page for %s\n"
	       "\n\e[1msyntax:\e[0m\n%s ./image1 ./image2 -t 4 -zoom\n"
	       "\n-h --help       displays this help page\n-t \e[4mTIME\e[0m         set time in seconds       [default = $duration]\n\n"
	       "\t\t\e[1mBackground Modes:\e[0m\n"
	       "-%s\n"
	       "-%s\n"
	       "-%s\n"
	       "-%s\n"
	       "-%s\n"
	       "-%s\t\t[default]\n",
	       name, name, "wallpaper", "centered", "scaled", "stretched", "spanned", "zoom");
}

int inputparser(char option[256], float *duration, int *imagecount, char *images[], char *arguments[], int argcount){
	bool timeset = false;
	for (int i = 1; i<argcount; i++){
		if (memcmp("-", arguments[i], 1)==0){

			/* starts with '-'; aka flag */
			if      (strcmp(arguments[i], "-wallpaper") == 0)strcpy(option, "wallpaper") ;
			else if (strcmp(arguments[i], "-centered") == 0) strcpy(option, "centered" );
			else if (strcmp(arguments[i], "-scaled") == 0)   strcpy(option, "scaled" );
			else if (strcmp(arguments[i], "-stretched") == 0)strcpy(option, "stretched" );
			else if (strcmp(arguments[i], "-spanned") == 0)  strcpy(option, "spanned" );
			else if (strcmp(arguments[i], "-stretched") == 0)strcpy(option, "stretched" );
			else if (strcmp(arguments[i], "-zoom") == 0)     strcpy(option, "zoom" );
			else if (strchr(arguments[i], 't') && !timeset){
				if (argcount <= ++i || sscanf(arguments[i], "%f", duration) != 1){
					printf("-t specified but next argument is not an integer\n");
					return(1);
				}
				timeset = true;
			}
			else {
				printf("an unknown flag was used in \"%s\"\n", arguments[i]);
			}
		} else if (!access(arguments[i], R_OK)) {
			images[(*imagecount)] = arguments[i];
			(*imagecount)++;
				/* printf("\"%s\" is not a regular file\n", arguments[i]); */
		} else {
			char response = 'n';
			printf("unknown argument: %s (maybe add \" \" if name contains special characters; --help for help)\n", arguments[i]);
			printf("Ignore? (Y/N) ");
			scanf("%c", &response);
			printf("\n");
			if (response != 'y' && response != 'Y')
				return(1);
		}
	}
	return 0;
}

void printdata(char option[256], float *duration, int *imagecount, char *images[], char *arguments[], int argcount){
	printf("%s duration: %f   imagecount: %d\n", option, *duration, *imagecount);
	for (int i = 0; i<*imagecount; i++){
		printf("%s\n", images[i]);
	}
}

int moveimages(char *images[], int imagecount){
	system("mkdir -p ~/.local/share/backgrounds/.hidden");
	int syserrout;
	char command[256];
	for (int i = 0; i < imagecount; i++){
		snprintf(command, 256, "cp %s ~/.local/share/backgrounds/.hidden/image%d", images[i], i);
		if ((syserrout = system(command)) != 0){
			return syserrout;
		}
		/* printf("%s\n", command); */
	}
}

int writexml(char option[256], float duration, int imagecount, char *images[]){
	char filename[256];
	/* scanf("%s", option); */
	char username[256]; getlogin_r(username, 256);

	FILE *xmlbg; xmlbg = fopen("animatedbg.xml" , "w");
	fprintf(xmlbg, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n\n<background>\n\n");

	for (int i = 0; i < imagecount; i++){
		fprintf(xmlbg, "\t<static>\n\t\t<duration>%f</duration>\n\t\t<image>/home/%s/.local/share/backgrounds/.hidden/image%d</image>\n\t</static>\n\n", duration, username, i);
	}

	fprintf(xmlbg, "</background>\n");
	fclose(xmlbg);

	return system("mv ./animatedbg.xml ~/.local/share/baackgrounds/");
}

int writeindex(){
	char username[256]; getlogin_r(username, 256);
	FILE *xmlindex; xmlindex = fopen("animatedbgindex.xml" , "w");
	fprintf(xmlindex, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
			        "<!DOCKTYPE wallpapers SYSTEM \"gnome-wp-list.dtd\">\n\n"
	              "<wallpapers>\n"
	              	"\t<wallpaper deleted=\"false\">\n"
	              		"\t\t<name>animatedbg</name>\n"
	              		"\t\t<filename>/home/%s/.local/share/backgrounds/animatedbg.xml</filename>\n"
	              		"\t\t<options>zoom</options>\n"
	              		"\t\t<shade_type>solid</shade_type>\n"
	              	"\t</wallpaper>\n"
	              "</wallpapers>\n"
			  , username);
	fclose(xmlindex);

	return system("sudo mv ./animatedbgindex.xml /usr/share/gnome-background-properties/");

}
