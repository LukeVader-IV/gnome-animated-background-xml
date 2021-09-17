#!/bin/bash



#check for arguments
if [ "$#" == 0 ]; then
	echo "no frames specified, please add frames"
        echo "example: ./animatedbg.sh path/to/image.png"
	exit
fi

let user
user=$(whoami)

#ask for filename & create .xml file
echo "filename (ex: nyancat)"
read filename

#remove existing file to avoid conflict and create file with start of file
rm -f /home/$user/.local/share/backgrounds/$filename.xml
touch /home/$user/.local/share/backgrounds/$filename.xml
echo "<background>" >> /home/$user/.local/share/backgrounds/$filename.xml

framenum=0

#for every argument
for var in "$@"; do
	#echo "yes"

	let gettime

	if [ "$gettime" == "1" ]; then

		#the duration of the image in seconds
		echo "$var seconds per image"
		if [ "$var" < 1 ]; then
			echo "gnome refreshes it's background once per second, be aware that this means images will be skipped with a duration of less then 1 second"
		fi
		gettime=0
		duration=$var

	elif [ -f $var ]; then
		#echo "image exists"

		#check file extension for filtype
		if [[ "$var" == *.png ]]; then
			filetype=png
		elif [[ "$var" == *.jpeg ]]; then
                	filetype=jpeg
                elif [[ "$var" == *.gif ]]; then
                	filetype=gif
                else
		echo "ERROR: error on file $var"
		echo "unknown filetype? if you are confident this is an image please create an issue on GitHub"
		echo "currently recognised file formats: png, jpeg, gif (not animated)"
		exit
		fi

		cp $var /home/$user/.local/share/backgrounds/$filename.$framenum.$filetype

		#write to .xml file
		echo "<static>" >> /home/$user/.local/share/backgrounds/$filename.xml
		echo "<duration>$duration</duration>" >> /home/$user/.local/share/backgrounds/$filename.xml
		echo "<file>/home/$user/.local/share/backgrounds/$filename.$framenum.$filetype</file>" >> /home/$user/.local/share/backgrounds/$filename.xml
		echo "</static>" >> /home/$user/.local/share/backgrounds/$filename.xml
		((framenum++))

	elif [ $var == -t ]; then
		gettime=1
		echo "next value is time (get time: $gettime)"
	else
		echo "image doesn't exist, skipping image"
	fi

done
((framenum++))
echo "</background>" >> /home/$user/.local/share/backgrounds/$filename.xml

while [ "$answerstate" == 0 ]; do
        echo "do you want to apply this background (y/n):"
        read answer
        if [ "$answer" = "y" ]; then
                dconf write /org/gnome/desktop/background/picture-uri "'file:///home/$user/.local/share/backgrounds/$filename.xml'"
                answerstate=1
        elif [ "$answer" = "n" ]; then
                answerstate=1
        else
            	echo "not a valid option, try again"
        fi
done

checkap=$(dconf read /org/gnome/desktop/background/picture-uri)
echo $checkap
if [ "$checkap" == "'file:///home/$user/.local/share/backgrounds/$filename.xml'" ]; then
	echo "animation of $framenum frames created and applied"
else
	echo "animation of $framenum frames was created, however an error accurred while applying it"
	echo "use dconf to change '/org/gnome/desktop/background/picture-uri' to 'file:///home/$user/.local/share/backgrounds/$filename.xml' manually"
fi
