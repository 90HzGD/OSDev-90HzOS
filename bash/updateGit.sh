#!/usr/bin/bash

printf "Commit Name: "
read commit_name

git add -A
git commit -m "$commit_name"
git pull --rebase origin main
git push origin -u main



