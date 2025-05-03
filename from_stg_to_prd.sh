#!/bin/bash

# Скрипт синхронизации ветки stg в prd с созданием тега
set -e

# 1. Проверка, что мы в git-репозитории
if ! git rev-parse --is-inside-work-tree >/dev/null 2>&1; then
  echo "Ошибка: это не git-репозиторий" >&2
  exit 1
fi

# 2. Получаем последний коммит из stg
git fetch origin stg
LAST_STG_COMMIT=$(git rev-parse origin/stg)

# 3. Проверяем, есть ли изменения для переноса
if git merge-base --is-ancestor origin/prd "$LAST_STG_COMMIT"; then
  echo "Нет новых изменений для переноса из stg в prd"
  exit 0
fi

# 4. Создаем тег с timestamp
TIMESTAMP=$(date +"%Y%m%d_%H%M%S")
TAG_NAME="prd_$TIMESTAMP"

# 5. Переносим изменения
git checkout -B prd origin/prd || {
  echo "Ошибка при переключении на ветку prd" >&2
  exit 1
}

git merge --no-ff "$LAST_STG_COMMIT" -m "Merge stg to prd ($TAG_NAME)" || {
  echo "Ошибка при слиянии" >&2
  exit 1
}

git tag -a "$TAG_NAME" -m "Release to PRD $TIMESTAMP" || {
  echo "Ошибка при создании тега" >&2
  exit 1
}

# 6. Пушим изменения
git push origin prd || {
  echo "Ошибка при пуше ветки prd" >&2
  exit 1
}

git push origin "$TAG_NAME" || {
  echo "Ошибка при пуше тега" >&2
  exit 1
}

echo "Успешно: ветка prd обновлена до $LAST_STG_COMMIT"
echo "Создан тег: $TAG_NAME"