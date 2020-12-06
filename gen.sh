#!/bin/bash

if [ "$#" -ne 2 ]; then
	echo "Usage: $0 <emptyGitPath:path> <projName:string>"
	exit
fi

gitURL="$1"
projName="$2"

#read the git URL
if [  ! -d $gitURL ]; then
	echo "Repo not found"
	exit
fi

for f in base/*; do
	contents=$(<$f)
	#substitute __PROJNAME__ for the project name
	contents=${contents//__PROJNAME__/$projName}
	echo "$contents"
done
