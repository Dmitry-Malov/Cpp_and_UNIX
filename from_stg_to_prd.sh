#!/bin/bash
dt=$(date +%Y%m%d%H%M%S)
git checkout prd
git merge stg
git commit -m "merged with stg"
git tag "$dt"
git push
