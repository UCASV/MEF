#!/bin/sh -f

# OutputFile: $2/$1.log
# ErrorFile: $2/$1.err

rm -f "$2/$1.log"
rm -f "$2/$1.err"
rm -f "$2/$1.post.res"
rm -f "$2/$1.post.dat"

"$3/heat2d $2/$1"
