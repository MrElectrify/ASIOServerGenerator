#!/bin/bash

if [ "$#" -ne 2 ]; then
	echo "Usage: $0 <emptyGitPath:path> <projName:string>"
	exit
fi

gitPath="$1"
projName="$2"

#read the git path
if [  ! -d $gitPath ] || [ ! -d "$gitPath/.git" ]; then
	echo "Repo not found"
	exit
fi

gitPath=$(realpath "$gitPath")

for f in $(find base -not \( -path "base/out" -prune \) -name '*.cpp' -or -name '*.h' -or -name 'CMakeLists.txt' -or -name '.gitignore' -or -name '.gitattributes'); do
	name=${f//__PROJNAME__/$projName}
	contents=$(<$f)
	#substitute __PROJNAME__ for the project name
	contents=${contents//__PROJNAME__/$projName}
	#substitute __PROJNAMEUPPER__ for the uppercase project name
	contents=${contents//__PROJNAMEUPPER__/${projName^^}}
	#substitute __DATE__ for the date
	contents=${contents//__DATE__/$(date "+%D")}
	#substitute __TIME__ for the time
	contents=${contents//__TIME__/$(date "+%H:%M")}
	#copy the file to the output directory
	name=${name:5}
	if [[ $name = *"/"* ]]; then
		#make the directory
		mkdir -p "$gitPath/${name%/*}"
	fi
	echo "$contents" > "$gitPath/$name"
done

#enter the directory
pushd $gitPath > /dev/null
branch=$(git branch)
if [ -z "$branch" ]; then
	#init the branch
	git add --all
	#add submodules
	cd extern
	git submodule add https://github.com/chriskohlhoff/asio.git
	git submodule add https://github.com/gabime/spdlog.git
	cd ..
	git commit -m "Initial commit"
	git branch -M master
	git push
fi
popd > /dev/null
