#!/bin/bash

scripts_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
src_dir="${scripts_dir}/../src"

# all .h & .cpp files
find $src_dir -regex '.*/.*\.\(h\|cpp\)' | while read -r file; do
    
    endtext='If not, see <http:\/\/www.gnu.org\/licenses\/>.'
    l=$(grep -n "$endtext" $file | tail -1 | cut -d ":" -f 1)    
    if [[ $l == "" ]]; then
        l=0
    else
        l=$(($l+1))
    fi
    tail -n +$l $file | sed '/./,$!d' > /tmp/copyright_tmp

    cat $src_dir/doc/copyright/gpl2_header.txt /tmp/copyright_tmp > $file
    rm /tmp/copyright_tmp 

    echo "Updated" `realpath $file`
done
