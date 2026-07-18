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

#ifdef _WIN32
#include <windows.h>
#else
#include <time.h>
#endif

#define BENCH_ITERATIONS 10000
#define BENCH_ARGS 20

static double get_time_ms(void) {
#ifdef _WIN32
    static LARGE_INTEGER freq = {0};
    if (!freq.QuadPart) QueryPerformanceFrequency(&freq);
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    return (double)now.QuadPart / freq.QuadPart * 1000.0;
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000.0 + ts.tv_nsec / 1e6;
#endif
}

static void flush(void) { fflush(stdout); }

static void bench_parser_creation(void) {
    printf("=== Parser Creation Benchmark ===\n");
    printf("Creating %d parsers...\n", BENCH_ITERATIONS);
    flush();

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
    flush();
}

static void bench_option_parsing(void) {
    printf("=== Option Parsing Benchmark ===\n");
    flush();

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
    int argc = (int)(sizeof(argv) / sizeof(argv[0]));

    /* Warm up */
    for (int i = 0; i < 100; i++) {
        struct argparse_result *result = argparse_parse(parser, argc, argv);
        if (result) argparse_result_free(result);
    }

    /* Benchmark */
    double start = get_time_ms();

    for (int i = 0; i < BENCH_ITERATIONS; i++) {
        struct argparse_result *result = argparse_parse(parser, argc, argv);

        /* Read values to prevent optimization from removing the parse */
        if (result) {
            const char *input = argparse_get_string(result, "input");
            const char *output = argparse_get_string(result, "output");
            int count = argparse_get_int(result, "count");
            int verbose = argparse_get_bool(result, "verbose");
            (void)input;
            (void)output;
            (void)count;
            (void)verbose;
        }

        if (result) argparse_result_free(result);
    }

    double elapsed = get_time_ms() - start;
    printf("  Args: %d options + 1 positional\n", 5);
    printf("  Iterations: %d\n", BENCH_ITERATIONS);
    printf("  Total time: %.2f ms\n", elapsed);
    printf("  Per parse: %.3f us\n", elapsed * 1000.0 / BENCH_ITERATIONS);
    printf("  Parses/sec: %.0f\n", BENCH_ITERATIONS / (elapsed / 1000.0));
    printf("\n");
    flush();

    argparse_free(parser);
}

static void bench_manual_parsing(void) {
    printf("=== Manual Parsing Comparison ===\n");
    printf("Simulating manual getopt-style parsing...\n");
    flush();

    const char *argv[] = {"bench", "-v", "-o", "output.txt", "-n", "42",
                          "--debug", "-l", "3", "input.txt"};
    int argc = (int)(sizeof(argv) / sizeof(argv[0]));

    /* Simulate manual parsing */
    double start = get_time_ms();

    for (int i = 0; i < BENCH_ITERATIONS; i++) {
        int verbose = 0;
        int debug = 0;
        const char *output = NULL;
        const char *input = NULL;
        int count = 0;
        int level = 0;

        for (int j = 1; j < argc; j++) {
            if (strcmp(argv[j], "-v") == 0) {
                verbose = 1;
            } else if (strcmp(argv[j], "-o") == 0 && j + 1 < argc) {
                output = argv[++j];
            } else if (strcmp(argv[j], "-n") == 0 && j + 1 < argc) {
                count = atoi(argv[++j]);
            } else if (strcmp(argv[j], "--debug") == 0) {
                debug = 1;
            } else if (strcmp(argv[j], "-l") == 0 && j + 1 < argc) {
                level = atoi(argv[++j]);
            } else {
                input = argv[j];
            }
        }

        /* Prevent optimization */
        volatile int v = verbose;
        volatile int d = debug;
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
    flush();
}

static void bench_many_options(void) {
    printf("=== Many Options Benchmark ===\n");
    printf("Parsing %d options...\n", BENCH_ARGS);
    flush();

    struct argparse *parser = argparse_new("bench", "Many options");

    char long_name[64];
    char dest[64];
    for (int i = 0; i < BENCH_ARGS; i++) {
        snprintf(long_name, sizeof(long_name), "option%d", i);
        snprintf(dest, sizeof(dest), "opt%d", i);
        argparse_add_option(parser, 0, long_name, ARGPARSE_NARGS_1,
                            ARGPARSE_TYPE_STRING, "An option", dest);
    }

    /* Build argv with all options - allocate separate strings */
    const char **argv = (const char **)malloc((size_t)(BENCH_ARGS * 2 + 1) * sizeof(char *));
    char **arg_bufs = (char **)malloc((size_t)BENCH_ARGS * sizeof(char *));
    argv[0] = "bench";
    for (int i = 0; i < BENCH_ARGS; i++) {
        arg_bufs[i] = (char *)malloc(64);
        snprintf(arg_bufs[i], 64, "--option%d", i);
        argv[i * 2 + 1] = arg_bufs[i];
        argv[i * 2 + 2] = "value";
    }
    int argc = BENCH_ARGS * 2 + 1;

    /* Warm up */
    for (int i = 0; i < 100; i++) {
        struct argparse_result *result = argparse_parse(parser, argc, argv);
        if (result) argparse_result_free(result);
    }

    /* Benchmark */
    double start = get_time_ms();

    for (int i = 0; i < BENCH_ITERATIONS; i++) {
        struct argparse_result *result = argparse_parse(parser, argc, argv);
        if (result) argparse_result_free(result);
    }

    double elapsed = get_time_ms() - start;
    printf("  Total time: %.2f ms\n", elapsed);
    printf("  Per parse: %.3f us\n", elapsed * 1000.0 / BENCH_ITERATIONS);
    printf("  Parses/sec: %.0f\n", BENCH_ITERATIONS / (elapsed / 1000.0));
    printf("\n");
    flush();

    for (int i = 0; i < BENCH_ARGS; i++) {
        free(arg_bufs[i]);
    }
    free(arg_bufs);
    free(argv);
    argparse_free(parser);
}

static void bench_subcommands(void) {
    printf("=== Subcommands Benchmark ===\n");
    flush();

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
    int argc = 3;

    /* Warm up */
    for (int i = 0; i < 100; i++) {
        struct argparse_result *result = argparse_parse(parser, argc, argv);
        if (result) argparse_result_free(result);
    }

    /* Benchmark */
    double start = get_time_ms();

    for (int i = 0; i < BENCH_ITERATIONS; i++) {
        struct argparse_result *result = argparse_parse(parser, argc, argv);
        if (result) argparse_result_free(result);
    }

    double elapsed = get_time_ms() - start;
    printf("  Total time: %.2f ms\n", elapsed);
    printf("  Per parse: %.3f us\n", elapsed * 1000.0 / BENCH_ITERATIONS);
    printf("  Parses/sec: %.0f\n", BENCH_ITERATIONS / (elapsed / 1000.0));
    printf("\n");
    flush();

    argparse_free(parser);
}

int main(void) {
    printf("argparse-c Performance Benchmarks\n");
    printf("=================================\n\n");
    flush();

    bench_parser_creation();
    bench_option_parsing();
    bench_manual_parsing();
    bench_many_options();
    bench_subcommands();

    printf("Done.\n");
    flush();
    return 0;
}
