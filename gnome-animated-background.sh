#!/bin/bash

#defaults to avoid breaking without specifying options
option=zoom
duration=4


#check for arguments
if [ "$#" == 0 ]; then
	echo "no frames specified, please add frames"
        echo "example: ./animatedbg.sh path/to/image.png"
	exit
fi


#ask for filename & create .xml file
echo "filename (ex: nyancat)"
read filename


#create file with start of .xml file
rm -f /home/$USER/.local/share/backgrounds/$filename.xml
echo "<background>" > /home/$USER/.local/share/backgrounds/$filename.xml

#create start of temporary file for gnome-background-properties
cat << EOF > ./prop.$filename.xml
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE wallpapers SYSTEM "gnome-wp-list.dtd">
<wallpapers>
  <wallpaper deleted="false">
          <name>$filename</name>
          <filename>/home/$USER/.local/share/backgrounds/$filename.xml</filename>
          <options>$option</options>
          <shade_type>solid</shade_type>
  </wallpaper>
EOF


framenum=0

#for every argument
for var in "$@"; do
	echo "yes"

	#variable to determine of the next argument is the duration
	let gettime

	if [ "$gettime" == "1" ]; then

		#set the duration of the image in seconds
		echo "$var seconds per image"
		if [ "$var" < 1 ]; then
			echo "gnome refreshes it's background once per second, be aware that this means images will be skipped with a duration of less then 1 second"
		fi
		gettime=0
		duration=$var

	elif [ -f $var ]; then
		#echo "image exists"

		#check file extension for filtype
		if [[ "$var" == *.png ]] || [[ "$var" == *.PNG ]] ; then
			filetype=png
		elif [[ "$var" == *.jpeg ]] || [[ "$var" == *.JPEG ]]; then
			filetype=jpeg
		elif [[ "$var" == *.gif ]] || [[ "$var" == *.GIF ]]; then
			filetype=gif
		else
		echo "ERROR: error on file $var"
		echo "unknown filetype? if you are confident this is an image that should work as a background please create an issue on GitHub"
		echo "currently recognised file formats: png, jpeg, gif (not animated)"
		exit
		fi

		cp $var /home/$USER/.local/share/backgrounds/$filename.$framenum.$filetype

		#add image to main .xml file
cat << EOF >> /home/$USER/.local/share/backgrounds/$filename.xml

<static>
<duration>$duration</duration>
<file>/home/$USER/.local/share/backgrounds/$filename.$framenum.$filetype</file>
</static>
EOF

		#add image to temporary prop file
cat << EOF >> ./prop.$filename.xml

  <wallpaper deleted="false">
          <name>$filename.$framenum</name>
          <filename>/home/$USER/.local/share/backgrounds/$filename.$framenum.$filetype</filename>
          <options>$option</options>
          <shade_type>solid</shade_type>
  </wallpaper>
EOF
		((framenum++))

	elif [ $var == -t ]; then
		gettime=1
		echo "next value is time (get time: $gettime)"
	
	elif [ $var == -wallpaper ]; then
		option=wallpaper
	
	elif [ $var == -centered ]; then
		option=centered
	
	elif [ $var == -scaled ]; then
		option=scaled
	
	elif [ $var == -stretched ]; then
		option=stretched
	
	elif [ $var == -spanned ]; then
		option=spanned
		
	else
		echo "image doesn't exist, skipping image"
	fi

done
((framenum++))


#finish both main .xml and prop .xml
echo "</background>" >> /home/$USER/.local/share/backgrounds/$filename.xml
echo "</wallpapers>" >> ./prop.$filename.xml

#ask if user wants to apply .xml file as background
answer=a
answerstate=0
while [ "$answerstate" == 0 ]; do
	echo "do you want to apply this background (y/n):"
	read answer
	if [ "$answer" = "y" ]; then
		dconf write /org/gnome/desktop/background/picture-uri "'file:///home/$USER/.local/share/backgrounds/$filename.xml'"
		dconf write /org/gnome/desktop/background/picture-options "'$option'"
		answerstate=1
	elif [ "$answer" = "n" ]; then
		answerstate=1
	else
		echo "not a valid option, try again"
	fi
done

echo $answerstate
echo $answer

#check if background was applied
checkap=$(dconf read /org/gnome/desktop/background/picture-uri)
echo "current image: $checkap"
if [ "$checkap" == "'file:///home/$USER/.local/share/backgrounds/$filename.xml'" ]; then
	echo "animation of $framenum frames created and applied"
else
	echo "animation of $framenum frames was created, but was not applied"
	echo "use dconf to change '/org/gnome/desktop/background/picture-uri' to 'file:///home/$USER/.local/share/backgrounds/$filename.xml' manually"
fi

#ask if user wants to add prop file to gnome-background-properties
answerstate=0
while [ "$answerstate" = 0 ]; do
	echo "do you want to add this background to gnome-background properties, this will cause the background to show up in the settings app (y/n):"
	read answer
	if [ "$answer" == "y" ]; then
		answerstate=1
		sudo rm -f /usr/share/gnome-background-properties/$filename.xml && sudo cp ./prop.$filename.xml /usr/share/gnome-background-properties/$filename.xml
		rm ./prop.$filename.xml
		echo "added to gnome-background-properties"
	elif [ "$answer" == "n" ]; then
		rm ./prop.$filename.xml
		echo "not added"
		exit
	else
		echo "not a valid option, try again"
	fi
done
