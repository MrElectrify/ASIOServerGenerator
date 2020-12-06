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

for f in $(find base -not \( -path "base/out" -prune \) -name '*.cpp' -or -name '*.h' -or -name 'CMakeLists.txt'); do
	name=${f//__PROJNAME__/$projName}
	name=${name:5}
	echo $name
	contents=$(<$f)
	#substitute __PROJNAME__ for the project name
	contents=${contents//__PROJNAME__/$projName}
	#substitute __PROJNAMEUPPER__ for the uppercase project name
	contents=${contents//__PROJNAMEUPPER__/${projName^^}}
	#substitute __DATE__ for the date
	contents=${contents//__DATE__/$(date "+%D")}
	#substitute __TIME__ for the time
	contents=${contents//__TIME__/$(date "+%H:%M")}
	echo "$contents"
done
