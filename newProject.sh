#/bin/bash

PROJECT_NAME=$1

if [ -e ${PROJECT_NAME} ] 
then 
	echo "A file called "${PROJECT_NAME}" exists"
#	exit -1
fi

mkdir $PROJECT_NAME

touch ${PROJECT_NAME}/${PROJECT_NAME}.cpp

#git add ${PROJECT_NAME}/Makefile
#git add ${PROJECT_NAME}/${PROJECT_NAME}.cpp

cat .MakefileTemplate | sed s/PROJECT_NAME/${PROJECT_NAME}/ > ${PROJECT_NAME}/Makefile

echo Created project ${PROJECT_NAME}
