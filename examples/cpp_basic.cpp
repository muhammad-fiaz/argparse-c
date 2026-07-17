/*
 * examples/cpp_basic.cpp - C++ basic usage
 *
 * Demonstrates using the argparse-c library from C++ code.
 * The library's C API works seamlessly with C++.
 *
 * Build:
 *   g++ -o cpp_basic cpp_basic.cpp -I../include -L../build -largparse-c
 *
 * Run:
 *   ./cpp_basic --name Alice --count 5
 *   ./cpp_basic --help
 */

#include <iostream>
#include <string>
#include <argparse-c/argparse.h>

int main(int argc, const char **argv) {
    /* Create parser */
    struct argparse *parser = argparse_new("cpp_basic",
        "C++ basic usage example");

    /* Add options */
    struct argparse_option *name_opt = argparse_add_option(parser, 'n', "name",
        ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING, "Your name", "name");
    argparse_option_set_default(name_opt, "World");

    argparse_add_option(parser, 'c', "count",
        ARGPARSE_NARGS_1, ARGPARSE_TYPE_INT, "Repeat count", "count");

    argparse_add_option(parser, 'v', "verbose",
        ARGPARSE_NARGS_0, ARGPARSE_TYPE_NONE, "Verbose output", "verbose");

    /* Add positional */
    argparse_add_positional(parser,
        ARGPARSE_NARGS_QUESTION, ARGPARSE_TYPE_STRING,
        "Optional message", "message");

    /* Parse */
    struct argparse_result *result = argparse_parse(parser, argc, argv);

    if (argparse_result_error_code(result) != ARGPARSE_OK) {
        std::cerr << "Error: " << argparse_result_error(result) << std::endl;
        argparse_result_free(result);
        argparse_free(parser);
        return 1;
    }

    /* Access values using C++ */
    std::string name = argparse_get_string(result, "name")
                       ? argparse_get_string(result, "name") : "World";
    int count = argparse_get_int(result, "count");
    bool verbose = argparse_get_bool(result, "verbose");
    const char *msg = argparse_get_string(result, "message");

    std::cout << "Hello, " << name << "!" << std::endl;
    std::cout << "Repeating " << count << " times:" << std::endl;
    for (int i = 0; i < count; i++) {
        if (verbose) {
            std::cout << "  [" << (i + 1) << "] ";
        }
        if (msg) {
            std::cout << msg << std::endl;
        } else {
            std::cout << "Hello, " << name << "!" << std::endl;
        }
    }

    argparse_result_free(result);
    argparse_free(parser);
    return 0;
}
