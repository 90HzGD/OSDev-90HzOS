#!/usr/bin/bash

printf "Commit Name: "
read commit_name

git add -A
git commit -m "$commit_name"

git push origin -u main



