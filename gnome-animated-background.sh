#!/bin/bash

#check for arguments
if [ "$#" == 0 ]; then
	echo "no frames specified, please add frames"
	echo "example: $0 path/to/image.png"
	echo "type $0 --help for help"
	exit
fi

#defaults to avoid breaking without specifying options
option=zoom
duration=4
validoptions=("wallpaper" "centered" "scaled" "stretched" "spanned" "zoom")

if [ "$1" == "--help" ] || [ "$1" == "-h" ]; then
	echo "this is the help page for $0"
	echo -e "\n\e[1msyntax:\e[0m\n$0 ./image1 ./image2 -t 4 -zoom\n"
	echo -e "\n-h --help       displays this help page\n-t \e[4mTIME\e[0m         set time in seconds       [default = $duration]\n\n"
	echo "-${validoptions[0]}      wallpaper background mode"
	echo "-${validoptions[1]}       centered background mode"
	echo "-${validoptions[2]}         scaled background mode"
	echo "-${validoptions[3]}      stretched background mode"
	echo "-${validoptions[4]}        spanned background mode"
	echo -e "-${validoptions[5]}           zoom background mode      [default]\n"
	exit
fi


#check argument and store neccesary info
for argument in "$@"; do
	if [ -f "$argument" ]; then
		filenames+=("$argument")
	elif [ "$argument" == "-t" ]; then
		nextisdur=1
	elif [ "$nextisdur" == 1 ]; then
		duration=$argument
		nextisdur=0
	else
		for isvalid in "${validoptions[@]}"; do
			if [ "$argument" == "-$isvalid" ]; then
				option=$isvalid
			fi
		done
	fi
done

# check that there is at least 1 image 
if [ ${#filenames[@]} == 0 ]; then
	echo "no images provided"
	exit
fi

for i in "${filenames[@]}"; do
	echo "$i"
done

echo "option: $option"
echo "frame duration: $duration"
echo "files: ${filenames[*]}"

# check all files for filetype and save in array
for filecheck in "${filenames[@]}"; do
	if [[ "$filecheck" == *.png ]] || [[ "$filecheck" == *.PNG ]] ; then
		filetype+=("png")
	elif [[ "$filecheck" == *.jpeg ]] || [[ "$filecheck" == *.JPEG ]] || [[ "$filecheck" == *.jpg ]] || [[ "$filecheck" == *.JPG ]]; then
		filetype+=("jpeg")
	elif [[ "$filecheck" == *.gif ]] || [[ "$filecheck" == *.GIF ]]; then
		filetype+=("gif")
	else
		echo "ERROR: error on file $filecheck"
		echo "unknown filetype? if you are confident this is an image that should work as a background please create an issue on GitHub"
		echo "currently recognised file formats: png, jpeg, gif (not animated)"
		echo "names with spaces currently don't work, make sure nothing in the path has a space"
		exit
	fi
done


# for (( i=0; i<${#filenames[@]}; i++)); do
# 	echo "${filenames[i]}.${filetype[i]}"
# done

#ask for filename
echo "filename (ex: nyancat)"
read -r filename

#create file with start of .xml file
rm -f /home/"$USER"/.local/share/backgrounds/"$filename".xml
echo "<background>" > /home/"$USER"/.local/share/backgrounds/"$filename".xml

#copy image and write to file for every image
for ((i=0; i<${#filenames[@]}; i++)); do
cp "${filenames[i]}" /home/"$USER"/.local/share/backgrounds/"$filename"."$i"."${filetype[i]}"
cat << EOF >> /home/"$USER"/.local/share/backgrounds/"$filename".xml

<static>
<duration>$duration</duration>
<file>/home/$USER/.local/share/backgrounds/$filename.$i.${filetype[i]}</file>
</static>
EOF
done

#end of file
echo "</background>" >> /home/"$USER"/.local/share/backgrounds/"$filename".xml

while [ "$appearingnome" != "y" ] && [ "$appearingnome" != "n" ]; do
	echo "do you wish for this animation to appear in gnome-settings? (y/n)"
	read -r appearingnome
	if [ "$appearingnome" != "y" ] && [ "$appearingnome" != "n" ]; then
		echo "invalid input, please try again"
	fi
done

#ask if user want all images of animation to show up in gnome-settings
if [ "$appearingnome" == "y" ]; then
	while [ "$appearingnomeall" != "y" ] && [ "$appearingnomeall" != "n" ]; do
	echo "do you wish for all images to appear in gnome-settings? (y/n)"
	read -r appearingnomeall
	done
fi

if [ "$appearingnome" == "y" ]; then
cat << EOF > ./prop."$filename".xml
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
if [ "$appearingnomeall" == "y" ]; then

#add all images to file
for (( i=0; i<${#filenames[@]}; i++)); do
cat << EOF >> ./prop."$filename".xml

  <wallpaper deleted="false">
          <name>$filename.$i</name>
          <filename>/home/$USER/.local/share/backgrounds/$filename.$i.${filetype[i]}</filename>
          <options>$option</options>
          <shade_type>solid</shade_type>
  </wallpaper>
EOF
done
fi
echo "</wallpapers>" >> ./prop."$filename".xml
sudo mv ./prop."$filename".xml /usr/share/gnome-background-properties/"$filename".xml
fi

#does user want to apply background
while [ "$answer" != "y" ] && [ "$answer" != "n" ]; do
	echo "do you want to apply this background now? (y/n):"
	read -r answer
	if [ "$answer" = "y" ]; then
		dconf write /org/gnome/desktop/background/picture-uri "'file:///home/$USER/.local/share/backgrounds/$filename.xml'"
		dconf write /org/gnome/desktop/background/picture-options "'$option'"
	elif [ "$answer" != "n" ]; then
		echo "not a valid option, try again"
	fi
done

#check if background was applied
checkap=$(dconf read /org/gnome/desktop/background/picture-uri)
echo "current image: $checkap"
if [ "$checkap" == "'file:///home/$USER/.local/share/backgrounds/$filename.xml'" ]; then
	echo "animation of ${#filenames[@]} frames created and applied"
else
	echo "animation of ${#filenames[@]} frames was created, but was not applied"
	echo "use dconf to change '/org/gnome/desktop/background/picture-uri' to 'file:///home/$USER/.local/share/backgrounds/$filename.xml' manually"
fi
