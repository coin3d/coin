#!/bin/bash

# Crude hack to take a directory of gifs, to a scenegraph, and then inline the
# scenegraph files in a cpp file. Filename for the cpp-file must be supplied
# as the first argument. REQUIRES that the example in the Texture2
# documentation, for inlining textures, are built and exist in path as
# 'inline_texture'.

tmplate='#Inventor V2.1 ascii \n
# Texture @TEX@ embedded in a scenegraph \n
    Texture2{ \n
        filename @TEX@  \n
    } \n
'

ctmplate_begin='static const char * @NODE@[] = {'
ctmplate_end='NULL};'

for i in $(ls *.gif); 
  do
  fnam=${i%.gif}
    echo "Generating source for $i"
    echo -e $tmplate | sed s/@TEX@/$i/g > texture-$fnam.tmp
    inline_texture < texture-$fnam.tmp > texture-$fnam.iv
    rm texture-$fnam.tmp
    echo "$ctmplate_begin" | sed s/@NODE@/$fnam/g >> $1
    cat texture-$fnam.iv | sed 's/^/"/g' | sed 's/$/\\n",/g' >> $1
    echo -e "$ctmplate_end" >> $1
    rm texture-$fnam.iv
  done
