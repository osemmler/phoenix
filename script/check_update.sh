#!/bin/bash

git remote update &> /dev/null
X=`git status -uno | sed -n 2p`
X=${X:0:30}
if [ "$X" == "Your branch is up-to-date with" ]; then
    exit 0
else
    exit 1
fi
