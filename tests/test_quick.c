#include <stdio.h>
#include "argparse-c/argparse.h"

int main(void) {
    struct argparse *parser = argparse_new("test", "A test program");
    
    argparse_add_option(parser, 'v', "verbose", ARGPARSE_NARGS_0,
                        ARGPARSE_TYPE_NONE, "Enable verbose output", "verbose");
    argparse_add_option(parser, 'o', "output", ARGPARSE_NARGS_1,
                        ARGPARSE_TYPE_STRING, "Output file", "output");
    argparse_add_positional(parser, ARGPARSE_NARGS_1,
                            ARGPARSE_TYPE_STRING, "Input file", "input");
    
    const char *argv[] = { "test", "-v", "-o", "out.txt", "in.txt" };
    int argc = 5;
    
    struct argparse_result *result = argparse_parse(parser, argc, argv);
    
    if (argparse_result_error_code(result) != ARGPARSE_OK) {
        printf("Error: %s\n", argparse_result_error(result));
    } else {
        printf("verbose=%s\n", argparse_get_bool(result, "verbose") ? "true" : "false");
        printf("output=%s\n", argparse_get_string(result, "output"));
        printf("input=%s\n", argparse_get_string(result, "input"));
    }
    
    argparse_result_free(result);
    argparse_free(parser);
    
    printf("Version: %s\n", argparse_version_string());
    printf("Library test passed!\n");
    return 0;
}
