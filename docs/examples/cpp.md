# C++ Integration

Using argparse-c with modern C++ (C++20).

## Basic C++ Example

```cpp
#include <argparse-c/argparse.hpp>
#include <iostream>
#include <string>

int main(int argc, const char **argv) {
    argparse::Parser parser("greet", "A greeting program");

    parser.add_option("-n", "--name", argparse::Nargs::ONE,
                      "Your name", "name");
    parser.add_option("-c", "--count", argparse::Nargs::ONE,
                      "Number of greetings", "count");
    parser.add_option("-u", "--uppercase", argparse::Nargs::ZERO,
                      "Uppercase output", "uppercase");

    parser.add_positional(argparse::Nargs::ONE,
                          "Greeting message", "message");

    auto result = parser.parse(argc, argv);

    if (!result) {
        std::cerr << "Error: " << result.error() << std::endl;
        return 1;
    }

    std::string name = result.get<std::string>("name").value_or("World");
    int count = result.get<int>("count").value_or(1);
    bool uppercase = result.get<bool>("uppercase").value_or(false);
    std::string message = result.get<std::string>("message").value_or("Hello");

    for (int i = 0; i < count; i++) {
        std::string output = message + ", " + name + "!";
        if (uppercase) {
            std::transform(output.begin(), output.end(), output.begin(),
                          ::toupper);
        }
        std::cout << output << std::endl;
    }

    return 0;
}
```

## RAII Wrapper

The C++ wrapper provides automatic cleanup:

```cpp
#include <argparse-c/argparse.hpp>

void process() {
    argparse::Parser parser("app", "Application");

    parser.add_option("-v", "--verbose", argparse::Nargs::ZERO,
                      "Verbose mode", "verbose");

    auto result = parser.parse(argc, argv);

    // No need to manually free - RAII handles cleanup
    // Parser and result are automatically freed when they go out of scope
}
```

## Optional Values

```cpp
auto result = parser.parse(argc, argv);

// Get optional string
auto name = result.get<std::string>("name");
if (name) {
    std::cout << "Name: " << *name << std::endl;
}

// Get with default
std::string output = result.get<std::string>("output").value_or("stdout");

// Get optional int
auto count = result.get<int>("count");
if (count) {
    std::cout << "Count: " << *count << std::endl;
}
```

## Error Handling

```cpp
auto result = parser.parse(argc, argv);

if (!result) {
    // Check specific error type
    switch (result.error_code()) {
    case ARGPARSE_ERROR_MISSING_ARGUMENT:
        std::cerr << "Missing required argument" << std::endl;
        break;
    case ARGPARSE_ERROR_INVALID_VALUE:
        std::cerr << "Invalid value: " << result.error() << std::endl;
        break;
    default:
        std::cerr << "Error: " << result.error() << std::endl;
    }
    return 1;
}
```

## Lambda Callbacks

```cpp
parser.add_option("-v", "--verbose", argparse::Nargs::ZERO,
                  "Verbose mode", "verbose")
    .callback([](argparse::Parser &p, void *data) {
        std::cout << "Verbose mode enabled" << std::endl;
    });
```

## Key Points

- The C++ wrapper provides RAII semantics
- Use `std::optional` for optional values
- `value_or()` provides defaults for missing values
- Error handling uses `operator bool()` on the result
- The wrapper requires C++20
