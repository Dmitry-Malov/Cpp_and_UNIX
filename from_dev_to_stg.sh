#!/bin/bash
dt=$(date +%Y%m%d%H%M%S)
git checkout stg
git merge dev
git commit -m "merged with dev"
git tag "$dt"
git push
