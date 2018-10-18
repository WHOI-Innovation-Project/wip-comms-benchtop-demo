#!/bin/bash

set -u -e

if [ "$#" -lt 2 ]; then
    echo "Usage: download_circleci_deb.sh circle_ci_build_num circle_ci_api_token [output_dir, default .]"
    exit 1
fi

BUILD_NUM=$1
CIRCLE_TOKEN=$2
OUTPUT_DIR="."
if [ "$#" -eq 3 ]; then
    OUTPUT_DIR=$3
fi


curl https://circleci.com/api/v1.1/project/github/GobySoft/progressive-imagery/${BUILD_NUM}/artifacts?circle-token=$CIRCLE_TOKEN > /tmp/artifacts.txt

cat /tmp/artifacts.txt | grep -o 'https://[^"]*' > /tmp/artifacts-url.txt
cat /tmp/artifacts.txt | grep '"path"' | grep -o 'root/[^"]*'  > /tmp/artifacts-path.txt

while read path <&3 && read url <&4; do     
    wget $url?circle-token=$CIRCLE_TOKEN -O $OUTPUT_DIR/$(basename $path)
done 3</tmp/artifacts-path.txt 4</tmp/artifacts-url.txt

