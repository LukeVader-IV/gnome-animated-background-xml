# gnome-animated-background-xml

this is a shell script that creates an animated / dynamic background from a list of images <br>
it uses .xml files, the same method many distros use for creating dynamic backgrounds that change on time of day or weather

# NOTE:
this tool does not work with Gnome 42 dark-mode yet, this will be working as soon as the C-lang rewrite is finished
Update: it seems to be inconsistant? if you have this issue, try to delete the background and create it again, that might fix it.

## usage:
```./gnome-animated-background.sh path/to/image1.png path/to/image2.png``` <br>
##### example: <br>
```./gnome-animated-background.sh -t 1 -centered ./nyancat1.png ./nyancat2.png ./nyancat3.png ./nyancat4.png ./nyancat5.png ./nyancat6.png```<br><br>



https://user-images.githubusercontent.com/15455108/133536051-4c44bfff-bf48-4562-8e51-2bc1e9a506e5.mp4







## manual

##### timing <br>
`-t n` set a time for the upcoming list of images of "n" seconds <br>
ex: `-t 10800` for 3 hours <br>
ex: `-t 0.03` for 30fps (1/30)
this is a global value, all frames will stay on screen for the same amount of time

default: 4 seconds per frame <br>

!! for anything below 1seconf per frame, the 
<a href="https://extensions.gnome.org/extension/4761/speed-background/" >speed background extension</a> 
is required.

##### background modes <br>

any options must be placed before any images <br>
default: 'zoom' <br>

`-centered` set the background to be centered <br>
`-wallpaper` set background to "wallpaper" mode <br>
`-scaled` set background to scaled mode <br>
`-stretched` set background to stretched mode <br>
`-spanned` set background to spanned mode

## help <br>

#### background not applying: <br>
if the previously applied background has the same name as the new one, just log out and log back in <br>
otherwise set it manually using this command (replace `$filename` with what you named it): <br>
`dconf write /org/gnome/desktop/background/picture-uri "'file:///home/$USER/.local/share/backgrounds/$filename.xml'"` <br>

#### my desktop environment refuses to load:
this means something f-ed up with the .xml file,  this can be fixed by temporarily replacing the background with another image: <br>

press `ctl + alt + f2` to get into a terminal <br>
login <br>
type: `dconf write /org/gnome/desktop/background/picture-uri "'file:///home/$USER/path/to/image.png'"` <br>
(replace `path/to/image.png` with the path to any image) <br>
press `ctl + alt + f1` to get back to the login screen <br>
consider opening a GitHub issue if this keeps happening when applying the background
