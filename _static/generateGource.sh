#! /bin/bash -e


git log --pretty="%at|%as %s" > caption.txt

# gource -s 0.1 -1280x720 -o - | ffmpeg -y -r 60 -f image2pipe -vcodec ppm -i - -vcodec libx264 -preset ultrafast -pix_fmt yuv420p -crf 1 -threads 0 -bf 0 gource.mp4
# gource --load-config gource.config -o gource.ppm
xvfb-run gource --load-config gource.config -o gource.ppm


ffmpeg -y -r 60 -f image2pipe -vcodec ppm -i gource.ppm -vcodec libx264 -preset medium -pix_fmt yuv420p -crf 25 -threads 0 -bf 0 gource.mp4

# ffmpeg -sseof -3 -i gource.mp4 -update 1 -q:v 1 last.png
ffmpeg -sseof -8 -i gource.mp4 -vframes 1 last.png -y
