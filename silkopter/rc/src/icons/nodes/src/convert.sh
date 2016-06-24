
#! /bin/sh
for file in *.png; do convert $file -resize 32x32 -negate ../$file; done
