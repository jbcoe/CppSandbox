#/bin/bash

PROJECT_DIR=$1

PROJECT_NAME=$(echo $PROJECT_DIR | sed s#.*/##)

if [ -e ${PROJECT_DIR} ] 
then 
	echo "A file called "${PROJECT_DIR}" exists"
	exit -1
fi

mkdir -p $PROJECT_DIR

touch ${PROJECT_DIR}/${PROJECT_NAME}.cpp

cat .MakefileTemplate | sed s/PROJECT_NAME/${PROJECT_NAME}/ > ${PROJECT_DIR}/Makefile
cat .CPP_Template > ${PROJECT_DIR}/${PROJECT_NAME}.cpp

echo ${PROJECT_DIR}/${PROJECT_NAME} >> .gitignore

git add .gitignore
git add ${PROJECT_DIR}/Makefile
git add ${PROJECT_DIR}/${PROJECT_NAME}.cpp

echo Created project ${PROJECT_NAME} in ${PROJECT_DIR}
echo Changes pending but not yet committed
