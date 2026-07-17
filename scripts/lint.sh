#!/usr/bin/env bash
# Run static analysis with clang-tidy

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="$(dirname "$SCRIPT_DIR")"

echo "Running static analysis..."

if ! command -v clang-tidy &>/dev/null; then
    echo "Error: clang-tidy not found. Install it with:"
    echo "  Ubuntu/Debian: sudo apt-get install clang-tidy"
    echo "  macOS: brew install llvm"
    echo "  Windows: choco install llvm"
    exit 1
fi

cd "$PROJECT_DIR"

# Create compile_commands.json if not present
if [ ! -f build/compile_commands.json ]; then
    echo "Generating compile_commands.json..."
    cmake -B build \
        -DCMAKE_BUILD_TYPE=Debug \
        -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
        -DARGPARSE_BUILD_TESTS=ON \
        -DARGPARSE_BUILD_EXAMPLES=ON
fi

# Run clang-tidy on source files
echo "Analyzing source files..."
find src -name '*.c' -o -name '*.cpp' | while read -r file; do
    echo "  Checking: $file"
    clang-tidy "$file" \
        -p build \
        --warnings-as-errors='*' \
        --extra-arg=-std=c99 \
        --extra-arg=-Wall \
        --extra-arg=-Wextra \
        --extra-arg=-Wpedantic || {
        echo "  Warning: clang-tidy reported issues in $file"
    }
done

echo "Done."
