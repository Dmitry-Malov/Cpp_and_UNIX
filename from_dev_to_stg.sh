#!/bin/bash

# Скрипт синхронизации ветки dev в stg с созданием тега
set -e

# 1. Проверка, что мы в git-репозитории
if ! git rev-parse --is-inside-work-tree >/dev/null 2>&1; then
  echo "Ошибка: это не git-репозиторий"
  exit 1
fi

# 2. Получаем последний коммит из dev
git fetch origin dev
LAST_DEV_COMMIT=$(git rev-parse origin/dev)

# 3. Проверяем, есть ли изменения для переноса
if git merge-base --is-ancestor origin/stg $LAST_DEV_COMMIT; then
  echo "Нет новых изменений для переноса из dev в stg"
  exit 0
fi

# 4. Создаем тег с timestamp
TIMESTAMP=$(date +"%Y%m%d_%H%M%S")
TAG_NAME="stg_$TIMESTAMP"

# 5. Переносим изменения
git checkout -B stg origin/stg
git merge --no-ff $LAST_DEV_COMMIT -m "Merge dev to stg ($TAG_NAME)"
git tag -a $TAG_NAME -m "Release to STG $TIMESTAMP"

# 6. Пушим изменения
git push origin stg
git push origin $TAG_NAME

echo "Успешно: ветка stg обновлена до $LAST_DEV_COMMIT"
echo "Создан тег: $TAG_NAME"