#!/bin/sh
git add --all :/
git commit -m "$1"
git push -u 274proj2 master
