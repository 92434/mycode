#!/bin/sh

if [ "$#" -ne 1 -a "$#" -ne 2 ]
then
  echo "Usage: $0 videourl [videoquality=normal|high|super|...]"
  echo ' e.g.'
  echo "   $0 http://v.youku.com/v_show/id_XMzMzMjE0MjE2.html super"
  exit 1
fi

files=$(~/dl.py "$1" "$2")

eval set "$files"

if [ "$#" -gt 1 ]
then
  mergedfile=$(echo "$1" | sed 's/[0-9]*.flv$/_merged.flv/')
  echo "starting merging files to file '$mergedfile'"
  ~/flvmerge "$mergedfile" "$@"
  echo "--- completely merging files to file '$mergedfile'"
  echo 
fi
