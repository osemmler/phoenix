#!/bin/bash

FILE="systemconfig.h"
GIT_VERSION=`git describe --always --tags`

echo "#ifndef SYSTEMCONFIG_H" > $FILE
echo "#define SYSTEMCONFIG_H" >> $FILE
echo "" >> $FILE
echo "// This file is auto-generated by script $0" >> $FILE
echo "// WARNING: Do not modify this file manualy!" >> $FILE
echo "" >> $FILE
echo "#define GIT_VERSION \"$GIT_VERSION\"" >> $FILE
echo "" >> $FILE
echo "#endif //SYSTEMCONFIG_H" >> $FILE