# Contributing

Thank you for your interest in contributing to argparse-c!

## Getting Started

1. **Fork** the repository on GitHub
2. **Clone** your fork:
   ```bash
   git clone https://github.com/YOUR_USERNAME/argparse-c.git
   cd argparse-c
   ```
3. **Create a branch**:
   ```bash
   git checkout -b feature/my-feature
   ```

## Development Setup

### Prerequisites

- C compiler (GCC 9+ or Clang 10+)
- CMake 3.14+
- Ninja (recommended)
- clang-format
- clang-tidy

### Build

```bash
cmake -B build -G Ninja \
  -DCMAKE_BUILD_TYPE=Debug \
  -DARGPARSE_BUILD_TESTS=ON \
  -DARGPARSE_BUILD_EXAMPLES=ON

cmake --build build --parallel
```

### Run Tests

```bash
ctest --test-dir build --output-on-failure
```

### Format Code

```bash
./scripts/format.sh
```

### Run Static Analysis

```bash
./scripts/lint.sh
```

## Making Changes

1. Create a topic branch from `main`
2. Make your changes following the style guide
3. Add tests for new functionality
4. Update documentation if needed
5. Run the full test suite
6. Commit with a clear message

## Commit Messages

Use [Conventional Commits](https://www.conventionalcommits.org/):

```
type(scope): description

feat(parser): add support for repeated options
fix(memory): prevent double-free on cleanup
docs(api): update argument examples
test(groups): add edge case tests
```

Types: `feat`, `fix`, `docs`, `style`, `refactor`, `test`, `chore`

## Pull Requests

1. Ensure all tests pass
2. Update documentation for API changes
3. Fill out the PR template
4. Link related issues
5. Keep PRs focused

## Code Style

Key points:
- C99 standard
- 4 spaces indentation
- K&R brace style
- `snake_case` for C, `PascalCase` for C++ classes
- Doxygen comments for all public API

## Questions?

- Check the [FAQ](https://muhammad-fiaz.github.io/argparse-c/guide/faq)
- Search [discussions](https://github.com/muhammad-fiaz/argparse-c/discussions)
- Open a new discussion

Thank you for contributing!
