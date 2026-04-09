#!/bin/bash


OS=$(uname)
if [ "$OS" = "Darwin" ]; then
    LIB=./libft_malloc.dylib
    export DYLD_FORCE_FLAT_NAMESPACE=1
    export DYLD_INSERT_LIBRARIES=$LIB
$@
elif [ "$OS" = "Linux" ]; then
    LIB=./libft_malloc.so
    export LD_PRELOAD=$LIB
else
    echo "unsupported OS: $OS"
    exit 1
fi
if [ -z "$DYLD_INSERT_LIBRARIES" ]; then
    echo "❌ DYLD_INSERT_LIBRARIES not set"
else
    echo "✅ DYLD_INSERT_LIBRARIES=$DYLD_INSERT_LIBRARIES"
fi
echo "custom malloc working"

if [ ! -f "$LIB" ]; then
echo "❌ libft_malloc not found: $LIB"
exit 1
fi

# === Demande du fichier ===

FILE_PATH="./tests/test_files/"

echo "enter test file (ex: test2.c):"
read FILE

FILE_NAME="${FILE_PATH}${FILE}"
# verif file
if [ ! -f "$FILE_NAME" ]; then
echo "❌ File not found: $FILE_NAME"
exit 1
fi

# excutable
OUT=$(basename "$FILE_NAME" .o)

echo "🔧 cmpiling $FILE_NAME → $OUT"

gcc "$FILE_NAME" -o "$OUT" -I./includes/ -L. -lft_malloc

if [ $? -ne 0 ]; then
    exit 1
fi

echo "💡 /usr/bin/time → $OUT"
/usr/bin/time -l ./"$OUT"
"$OUT" 2>&1 | grep "free_unused_page" | wc -l

rm -f $OUT

