# Contributing to argparse-c

Thank you for your interest in contributing to argparse-c! This guide will help you get started.

## Table of Contents

- [Code of Conduct](#code-of-conduct)
- [Getting Started](#getting-started)
- [Development Setup](#development-setup)
- [Making Changes](#making-changes)
- [Code Style](#code-style)
- [Testing](#testing)
- [Submitting a Pull Request](#submitting-a-pull-request)
- [Reporting Bugs](#reporting-bugs)
- [Requesting Features](#requesting-features)

## Code of Conduct

This project follows the [Contributor Covenant Code of Conduct](CODE_OF_CONDUCT.md). By participating, you agree to abide by its terms.

## Getting Started

1. **Fork** the repository on GitHub
2. **Clone** your fork locally:
   ```bash
   git clone https://github.com/YOUR_USERNAME/argparse-c.git
   cd argparse-c
   ```
3. **Create a branch** for your change:
   ```bash
   git checkout -b feature/my-feature
   ```

## Development Setup

### Prerequisites

- C compiler (GCC 9+ or Clang 10+)
- CMake 3.14+
- Ninja (recommended) or Make
- clang-format (for code formatting)
- clang-tidy (for static analysis)

### Build

```bash
cmake -B build -G Ninja \
  -DCMAKE_BUILD_TYPE=Debug \
  -DARGPARSE_BUILD_TESTS=ON \
  -DARGPARSE_BUILD_EXAMPLES=ON \
  -DARGPARSE_BUILD_BENCHMARKS=ON

cmake --build build --parallel
```

### Run Tests

```bash
ctest --test-dir build --output-on-failure
```

## Making Changes

1. **Create a topic branch** from `main`
2. **Make your changes** following the code style guide
3. **Add tests** for new functionality
4. **Update documentation** if needed
5. **Run the full test suite** to ensure nothing is broken
6. **Commit** with a clear, descriptive message

### Commit Messages

Use [Conventional Commits](https://www.conventionalcommits.org/) format:

```
type(scope): description

[optional body]

[optional footer]
```

Types:
- `feat`: New feature
- `fix`: Bug fix
- `docs`: Documentation changes
- `style`: Code style changes (formatting, no logic change)
- `refactor`: Code refactoring
- `test`: Adding or updating tests
- `chore`: Maintenance tasks

Examples:
```
feat(parser): add support for repeated options
fix(memory): prevent double-free on parser cleanup
docs(api): update argument creation examples
test(groups): add edge case tests for empty groups
```

## Code Style

- **C standard**: C99
- **Indentation**: 4 spaces (no tabs)
- **Braces**: K&R style (opening brace on same line)
- **Naming**: `snake_case` for functions and variables, `UPPER_CASE` for macros
- **Documentation**: Doxygen-style comments for all public API functions

Run the formatter before committing:

```bash
./scripts/format.sh
```

## Testing

- All new features must include tests
- Bug fixes must include a regression test
- Tests should be self-contained and not depend on external state
- Use Google Test framework for C++ tests
- Place C tests in `tests/` directory

## Submitting a Pull Request

1. **Ensure all tests pass**
2. **Update documentation** if your change affects the public API
3. **Fill out the PR template** completely
4. **Link related issues** using GitHub keywords (e.g., "Closes #42")
5. **Keep PRs focused** — one feature or fix per PR

### PR Review Process

- All PRs require at least one review
- CI must pass (build, test, static analysis)
- Maintainers may request changes before merging

## Reporting Bugs

Use the [bug report template](https://github.com/muhammad-fiaz/argparse-c/issues/new?template=bug_report.yml) when filing bugs. Include:

- Environment details (OS, compiler, version)
- Minimal reproduction case
- Expected vs actual behavior
- Any relevant error messages

## Requesting Features

Use the [feature request template](https://github.com/muhammad-fiaz/argparse-c/issues/new?template=feature_request.yml). Describe:

- The problem you're trying to solve
- Your proposed solution
- Alternatives you've considered

## Questions?

- Check the [FAQ](https://muhammad-fiaz.github.io/argparse-c/guide/faq)
- Search [existing discussions](https://github.com/muhammad-fiaz/argparse-c/discussions)
- Open a new discussion if needed

Thank you for contributing!
