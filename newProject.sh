#/bin/bash

PROJECT_NAME=$1

if [ -e ${PROJECT_NAME} ] 
then 
	echo "A file called "${PROJECT_NAME}" exists"
	exit -1
fi

mkdir $PROJECT_NAME

touch ${PROJECT_NAME}/${PROJECT_NAME}.cpp

cat .MakefileTemplate | sed s/PROJECT_NAME/${PROJECT_NAME}/ > ${PROJECT_NAME}/Makefile
cat .CPP_Template > ${PROJECT_NAME}/${PROJECT_NAME}.cpp

echo ${PROJECT_NAME}/${PROJECT_NAME} >> .gitignore

git add .gitignore
git add ${PROJECT_NAME}/Makefile
git add ${PROJECT_NAME}/${PROJECT_NAME}.cpp


echo Created project ${PROJECT_NAME}
