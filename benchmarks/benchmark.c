/**
 * @file benchmark.c
 * @brief Performance benchmarks for argparse-c.
 *
 * Measures parser creation time, option parsing throughput,
 * memory allocation patterns, and compares with manual parsing.
 */

#include <argparse-c/argparse.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BENCH_ITERATIONS 10000
#define BENCH_ARGS 20

static double get_time_ms(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000.0 + ts.tv_nsec / 1e6;
}

static void bench_parser_creation(void) {
    printf("=== Parser Creation Benchmark ===\n");
    printf("Creating %d parsers...\n", BENCH_ITERATIONS);

    double start = get_time_ms();

    for (int i = 0; i < BENCH_ITERATIONS; i++) {
        struct argparse *parser = argparse_new("bench", "Benchmark program");

        argparse_add_option(parser, 'v', "verbose", ARGPARSE_NARGS_0,
                            ARGPARSE_TYPE_NONE, "Verbose output", NULL);
        argparse_add_option(parser, 'o', "output", ARGPARSE_NARGS_1,
                            ARGPARSE_TYPE_STRING, "Output file", "output");
        argparse_add_option(parser, 'n', "count", ARGPARSE_NARGS_1,
                            ARGPARSE_TYPE_INT, "Count", "count");
        argparse_add_option(parser, 'f', "file", ARGPARSE_NARGS_1,
                            ARGPARSE_TYPE_STRING, "Input file", "file");
        argparse_add_option(parser, 'h', "help", ARGPARSE_NARGS_0,
                            ARGPARSE_TYPE_NONE, "Show help", NULL);

        argparse_add_positional(parser, ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING,
                                "Target", "target");

        argparse_free(parser);
    }

    double elapsed = get_time_ms() - start;
    printf("  Time: %.2f ms\n", elapsed);
    printf("  Per parser: %.3f us\n", elapsed * 1000.0 / BENCH_ITERATIONS);
    printf("\n");
}

static void bench_option_parsing(void) {
    printf("=== Option Parsing Benchmark ===\n");

    struct argparse *parser = argparse_new("bench", "Benchmark program");

    argparse_add_option(parser, 'v', "verbose", ARGPARSE_NARGS_0,
                        ARGPARSE_TYPE_NONE, "Verbose output", "verbose");
    argparse_add_option(parser, 'o', "output", ARGPARSE_NARGS_1,
                        ARGPARSE_TYPE_STRING, "Output file", "output");
    argparse_add_option(parser, 'n', "count", ARGPARSE_NARGS_1,
                        ARGPARSE_TYPE_INT, "Count", "count");
    argparse_add_option(parser, 'd', "debug", ARGPARSE_NARGS_0,
                        ARGPARSE_TYPE_NONE, "Debug mode", "debug");
    argparse_add_option(parser, 'l', "level", ARGPARSE_NARGS_1,
                        ARGPARSE_TYPE_INT, "Level", "level");

    argparse_add_positional(parser, ARGPARSE_NARGS_1, ARGPARSE_TYPE_STRING,
                            "Input file", "input");

    const char *argv[] = {"bench", "-v", "-o", "output.txt", "-n", "42",
                          "--debug", "-l", "3", "input.txt"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    /* Warm up */
    for (int i = 0; i < 100; i++) {
        struct argparse_result *result = argparse_parse(parser, argc, argv);
        argparse_result_free(result);
    }

    /* Benchmark */
    double start = get_time_ms();

    for (int i = 0; i < BENCH_ITERATIONS; i++) {
        struct argparse_result *result = argparse_parse(parser, argc, argv);

        /* Read values to prevent optimization from removing the parse */
        volatile const char *input = argparse_get_string(result, "input");
        volatile const char *output = argparse_get_string(result, "output");
        volatile int count = argparse_get_int(result, "count");
        volatile bool verbose = argparse_get_bool(result, "verbose");
        (void)input;
        (void)output;
        (void)count;
        (void)verbose;

        argparse_result_free(result);
    }

    double elapsed = get_time_ms() - start;
    printf("  Args: %d options + 1 positional\n", 5);
    printf("  Iterations: %d\n", BENCH_ITERATIONS);
    printf("  Total time: %.2f ms\n", elapsed);
    printf("  Per parse: %.3f us\n", elapsed * 1000.0 / BENCH_ITERATIONS);
    printf("  Parses/sec: %.0f\n", BENCH_ITERATIONS / (elapsed / 1000.0));
    printf("\n");

    argparse_free(parser);
}

static void bench_manual_parsing(void) {
    printf("=== Manual Parsing Comparison ===\n");
    printf("Simulating manual getopt-style parsing...\n");

    const char *argv[] = {"bench", "-v", "-o", "output.txt", "-n", "42",
                          "--debug", "-l", "3", "input.txt"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    /* Simulate manual parsing */
    double start = get_time_ms();

    for (int i = 0; i < BENCH_ITERATIONS; i++) {
        bool verbose = false;
        bool debug = false;
        const char *output = NULL;
        const char *input = NULL;
        int count = 0;
        int level = 0;

        for (int j = 1; j < argc; j++) {
            if (strcmp(argv[j], "-v") == 0) {
                verbose = true;
            } else if (strcmp(argv[j], "-o") == 0 && j + 1 < argc) {
                output = argv[++j];
            } else if (strcmp(argv[j], "-n") == 0 && j + 1 < argc) {
                count = atoi(argv[++j]);
            } else if (strcmp(argv[j], "--debug") == 0) {
                debug = true;
            } else if (strcmp(argv[j], "-l") == 0 && j + 1 < argc) {
                level = atoi(argv[++j]);
            } else {
                input = argv[j];
            }
        }

        /* Prevent optimization */
        volatile bool v = verbose;
        volatile bool d = debug;
        volatile const char *o = output;
        volatile const char *in = input;
        volatile int c = count;
        volatile int l = level;
        (void)v;
        (void)d;
        (void)o;
        (void)in;
        (void)c;
        (void)l;
    }

    double elapsed = get_time_ms() - start;
    printf("  Total time: %.2f ms\n", elapsed);
    printf("  Per parse: %.3f us\n", elapsed * 1000.0 / BENCH_ITERATIONS);
    printf("  Parses/sec: %.0f\n", BENCH_ITERATIONS / (elapsed / 1000.0));
    printf("\n");
}

static void bench_many_options(void) {
    printf("=== Many Options Benchmark ===\n");
    printf("Parsing %d options...\n", BENCH_ARGS);

    struct argparse *parser = argparse_new("bench", "Many options");

    char long_name[64];
    char dest[64];
    for (int i = 0; i < BENCH_ARGS; i++) {
        snprintf(long_name, sizeof(long_name), "option%d", i);
        snprintf(dest, sizeof(dest), "opt%d", i);
        argparse_add_option(parser, 0, long_name, ARGPARSE_NARGS_1,
                            ARGPARSE_TYPE_STRING, "An option", dest);
    }

    /* Build argv with all options */
    const char **argv = malloc((BENCH_ARGS * 2 + 1) * sizeof(char *));
    argv[0] = "bench";
    for (int i = 0; i < BENCH_ARGS; i++) {
        snprintf(long_name, sizeof(long_name), "--option%d", i);
        argv[i * 2 + 1] = long_name;
        argv[i * 2 + 2] = "value";
    }
    int argc = BENCH_ARGS * 2 + 1;

    /* Warm up */
    for (int i = 0; i < 100; i++) {
        struct argparse_result *result = argparse_parse(parser, argc, argv);
        argparse_result_free(result);
    }

    /* Benchmark */
    double start = get_time_ms();

    for (int i = 0; i < BENCH_ITERATIONS; i++) {
        struct argparse_result *result = argparse_parse(parser, argc, argv);
        argparse_result_free(result);
    }

    double elapsed = get_time_ms() - start;
    printf("  Total time: %.2f ms\n", elapsed);
    printf("  Per parse: %.3f us\n", elapsed * 1000.0 / BENCH_ITERATIONS);
    printf("  Parses/sec: %.0f\n", BENCH_ITERATIONS / (elapsed / 1000.0));
    printf("\n");

    free(argv);
    argparse_free(parser);
}

static void bench_subcommands(void) {
    printf("=== Subcommands Benchmark ===\n");

    struct argparse *parser = argparse_new("bench", "Subcommands");

    struct argparse_command *cmd1 = argparse_add_command(parser, "init", "Init", NULL);
    struct argparse *sub1 = argparse_command_get_parser(cmd1);
    argparse_add_option(sub1, 'f', "force", ARGPARSE_NARGS_0,
                        ARGPARSE_TYPE_NONE, "Force init", NULL);

    struct argparse_command *cmd2 = argparse_add_command(parser, "build", "Build", NULL);
    struct argparse *sub2 = argparse_command_get_parser(cmd2);
    argparse_add_option(sub2, 'r', "release", ARGPARSE_NARGS_0,
                        ARGPARSE_TYPE_NONE, "Release build", NULL);

    const char *argv[] = {"bench", "build", "--release"};
    int argc = 4;

    /* Warm up */
    for (int i = 0; i < 100; i++) {
        struct argparse_result *result = argparse_parse(parser, argc, argv);
        argparse_result_free(result);
    }

    /* Benchmark */
    double start = get_time_ms();

    for (int i = 0; i < BENCH_ITERATIONS; i++) {
        struct argparse_result *result = argparse_parse(parser, argc, argv);
        argparse_result_free(result);
    }

    double elapsed = get_time_ms() - start;
    printf("  Total time: %.2f ms\n", elapsed);
    printf("  Per parse: %.3f us\n", elapsed * 1000.0 / BENCH_ITERATIONS);
    printf("  Parses/sec: %.0f\n", BENCH_ITERATIONS / (elapsed / 1000.0));
    printf("\n");

    argparse_free(parser);
}

int main(void) {
    printf("argparse-c Performance Benchmarks\n");
    printf("=================================\n\n");

    bench_parser_creation();
    bench_option_parsing();
    bench_manual_parsing();
    bench_many_options();
    bench_subcommands();

    printf("Done.\n");
    return 0;
}
