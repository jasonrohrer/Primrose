if [ $# -ne 1 ]
then
	echo "Usage:"
	echo "./makeSourceBuild.sh new_directory_name"
	exit 1
fi

dirName=$1

if [ -e "$dirName" ]; then

	echo "Directory $dirName already exists"
	exit 1
fi

mkdir $dirName

if [ -d "$dirName" ]; then
	cp runToBuild $dirName/
	cd $dirName

	../exportSrc
	../cleanSrc
	cd ..
else
	echo "Failed to make directory $dirName"
	exit 1
fi
