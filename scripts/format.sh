#!/usr/bin/env bash
# Format source code with clang-format

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="$(dirname "$SCRIPT_DIR")"

echo "Formatting source code..."

if ! command -v clang-format &>/dev/null; then
    echo "Error: clang-format not found. Install it with:"
    echo "  Ubuntu/Debian: sudo apt-get install clang-format"
    echo "  macOS: brew install clang-format"
    echo "  Windows: choco install llvm"
    exit 1
fi

cd "$PROJECT_DIR"

# Format all C and C++ source files
find src include -name '*.c' -o -name '*.cpp' -o -name '*.h' | while read -r file; do
    echo "  Formatting: $file"
    clang-format -i "$file"
done

# Format test files if they exist
if [ -d tests ]; then
    find tests -name '*.c' -o -name '*.cpp' | while read -r file; do
        echo "  Formatting: $file"
        clang-format -i "$file"
    done
fi

# Format example files if they exist
if [ -d examples ]; then
    find examples -name '*.c' -o -name '*.cpp' | while read -r file; do
        echo "  Formatting: $file"
        clang-format -i "$file"
    done
fi

echo "Done."
