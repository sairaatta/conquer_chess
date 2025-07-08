
# From https://stackoverflow.com/a/44542839/3364162
# color=$( convert filename.png -format "%[pixel:p{0,0}]" info:- )
# convert filename.png -alpha off -bordercolor $color -border 1 \
#     \( +clone -fuzz 30% -fill none -floodfill +0+0 $color \
#        -alpha extract -geometry 200% -blur 0x0.5 \
#        -morphology erode square:1 -geometry 50% \) \
#     -compose CopyOpacity -composite -shave 1 outputfilename.png

# From https://stackoverflow.com/a/24511102/3364162
convert original.png -fuzz 10% -transparent white transparent.png
