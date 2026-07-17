#pragma once

#ifdef __cplusplus

/**
 * @file argparse.hpp
 * @brief Modern C++ wrapper for the argparse-c command-line parsing library.
 *
 * Provides RAII-based classes, method chaining, std::optional value access,
 * and exception-based error handling over the underlying C implementation.
 */

#include <string>
#include <string_view>
#include <optional>
#include <vector>
#include <functional>
#include <memory>
#include <stdexcept>
#include <utility>

extern "C" {
#include "argparse_types.h"
#include "argparse_argument.h"
#include "argparse_namespace.h"
#include "argparse_format.h"
#include "argparse_memory.h"
#include "argparse_error.h"
#include "argparse_group.h"
#include "argparse_mutex.h"
#include "argparse_command.h"
#include "argparse_validator.h"
#include "argparse_config.h"
}

namespace argparse_c {

/**
 * @brief Exception thrown when argument parsing fails.
 *
 * Wraps the C error code and provides a human-readable message.
 */
class ParseError : public std::runtime_error {
public:
    /**
     * @brief Construct a ParseError.
     * @param message Human-readable error description.
     * @param error_code Numeric error code from the C library.
     */
    explicit ParseError(const std::string& message, int error_code = 0);

    /**
     * @brief Get the numeric error code.
     * @return The C library error code.
     */
    int error_code() const noexcept;

private:
    int error_code_;
};

/**
 * @brief RAII wrapper around a C argparse_option handle.
 *
 * Supports fluent method chaining for configuring option properties.
 * Does not own the underlying handle; lifetime is managed by the Parser.
 */
class Option {
public:
    /**
     * @brief Construct from a C option handle.
     * @param handle Pointer to the C argparse_option. Must not be NULL.
     */
    explicit Option(struct argparse_option* handle);

    /**
     * @brief Set the default value for this option.
     * @param value Default value as a string.
     * @return Reference to this Option for chaining.
     */
    Option& default_value(std::string_view value);

    /**
     * @brief Set whether this option is required.
     * @param req True to require the option, false to make it optional.
     * @return Reference to this Option for chaining.
     */
    Option& required(bool req);

    /**
     * @brief Set the metavar for help text.
     * @param name Metavar name shown in usage/help output.
     * @return Reference to this Option for chaining.
     */
    Option& metavar(std::string_view name);

    /**
     * @brief Set the action performed when this option is encountered.
     * @param act The action to perform.
     * @return Reference to this Option for chaining.
     */
    Option& action(argparse_action act);

    /**
     * @brief Set the allowed choices for this option.
     * @param choice_list Vector of allowed string values.
     * @return Reference to this Option for chaining.
     */
    Option& choices(std::vector<std::string> choice_list);

    /**
     * @brief Set a custom validator for this option's value.
     *
     * The validator function receives the parsed value and an error string
     * to populate if validation fails.
     *
     * @param fn Validator function: returns true if valid, false otherwise.
     *            On failure, writes the reason into the second parameter.
     * @return Reference to this Option for chaining.
     */
    Option& validator(std::function<bool(const std::string&, std::string&)> fn);

    /**
     * @brief Get the underlying C handle.
     * @return Pointer to the C argparse_option.
     */
    struct argparse_option* handle() const noexcept;

private:
    struct argparse_option* handle_;
};

/**
 * @brief RAII wrapper around a C argparse_group handle.
 *
 * Represents a named group of options displayed together in help output.
 */
class Group {
public:
    /**
     * @brief Construct from a C group handle.
     * @param handle Pointer to the C argparse_group. Must not be NULL.
     */
    explicit Group(struct argparse_group* handle);

    /**
     * @brief Add an option to this group.
     * @param short_name Single-character short option name (e.g., 'v'), or '\0' for none.
     * @param long_name Long option name (e.g., "verbose").
     * @param nargs Number of arguments the option accepts.
     * @param type Type of the parsed value.
     * @param help Help text for this option.
     * @param dest Destination variable name for storing the parsed value.
     * @return Option wrapper for further configuration via method chaining.
     */
    Option add_option(char short_name, std::string_view long_name,
                      argparse_nargs nargs, argparse_type type,
                      std::string_view help, std::string_view dest);

    /**
     * @brief Add a positional argument to this group.
     * @param nargs Number of arguments this positional accepts.
     * @param type Type of the parsed value.
     * @param help Help text for this positional argument.
     * @param dest Destination variable name for storing the parsed value.
     * @return Option wrapper for further configuration via method chaining.
     */
    Option add_positional(argparse_nargs nargs, argparse_type type,
                          std::string_view help, std::string_view dest);

    /**
     * @brief Get the underlying C handle.
     * @return Pointer to the C argparse_group.
     */
    struct argparse_group* handle() const noexcept;

private:
    struct argparse_group* handle_;
};

/**
 * @brief RAII wrapper around a C argparse_mutex handle.
 *
 * Represents a mutually exclusive group of options.
 */
class Mutex {
public:
    /**
     * @brief Construct from a C mutex handle.
     * @param handle Pointer to the C argparse_mutex. Must not be NULL.
     */
    explicit Mutex(struct argparse_mutex* handle);

    /**
     * @brief Add an option to this mutually exclusive group.
     * @param option Reference to the Option to add.
     */
    void add_option(Option& option);

    /**
     * @brief Get the underlying C handle.
     * @return Pointer to the C argparse_mutex.
     */
    struct argparse_mutex* handle() const noexcept;

private:
    struct argparse_mutex* handle_;
};

/**
 * @brief RAII wrapper around a C argparse_result handle.
 *
 * Owns the result handle and frees it on destruction.
 * Provides type-safe accessors that return std::optional.
 * Non-copyable, movable.
 */
class Result {
public:
    /**
     * @brief Construct from a C result handle, taking ownership.
     * @param handle Pointer to the C argparse_result. May be NULL.
     */
    explicit Result(struct argparse_result* handle);

    /**
     * @brief Destructor. Frees the underlying C result.
     */
    ~Result();

    /** @brief Copy is prohibited. */
    Result(const Result&) = delete;
    /** @brief Copy assignment is prohibited. */
    Result& operator=(const Result&) = delete;

    /**
     * @brief Move constructor.
     * @param other Result to move from. Left in a valid empty state.
     */
    Result(Result&& other) noexcept;

    /**
     * @brief Move assignment operator.
     * @param other Result to move from. Left in a valid empty state.
     * @return Reference to this.
     */
    Result& operator=(Result&& other) noexcept;

    /**
     * @brief Get a string value from the result.
     * @param dest Destination variable name.
     * @return The value if present, std::nullopt otherwise.
     */
    std::optional<std::string> get_string(std::string_view dest) const;

    /**
     * @brief Get a signed integer value from the result.
     * @param dest Destination variable name.
     * @return The value if present, std::nullopt otherwise.
     */
    std::optional<int> get_int(std::string_view dest) const;

    /**
     * @brief Get an unsigned integer value from the result.
     * @param dest Destination variable name.
     * @return The value if present, std::nullopt otherwise.
     */
    std::optional<unsigned int> get_uint(std::string_view dest) const;

    /**
     * @brief Get a float value from the result.
     * @param dest Destination variable name.
     * @return The value if present, std::nullopt otherwise.
     */
    std::optional<float> get_float(std::string_view dest) const;

    /**
     * @brief Get a double value from the result.
     * @param dest Destination variable name.
     * @return The value if present, std::nullopt otherwise.
     */
    std::optional<double> get_double(std::string_view dest) const;

    /**
     * @brief Get a boolean value from the result.
     * @param dest Destination variable name.
     * @return The value if present, std::nullopt otherwise.
     */
    std::optional<bool> get_bool(std::string_view dest) const;

    /**
     * @brief Check whether the parse result contains an error.
     * @return True if an error occurred during parsing.
     */
    bool has_error() const noexcept;

    /**
     * @brief Get the error message string.
     * @return The error message, or an empty string if no error.
     */
    std::string error() const;

    /**
     * @brief Get the numeric error code.
     * @return The C error code, or ARGPARSE_OK if no error.
     */
    int error_code() const noexcept;

    /**
     * @brief Get the underlying C handle.
     * @return Pointer to the C argparse_result.
     */
    struct argparse_result* handle() const noexcept;

private:
    struct argparse_result* handle_;
};

/**
 * @brief Main argument parser class.
 *
 * Owns the underlying C parser handle and provides a modern C++ API
 * for configuring options, groups, mutexes, and parsing command-line arguments.
 * Supports method chaining for configuration and RAII for resource management.
 * Non-copyable, movable.
 */
class Parser {
public:
    /**
     * @brief Construct a new parser.
     * @param prog Program name (shown in usage/help). Empty for auto-detect.
     * @param description Program description shown in help output.
     */
    explicit Parser(std::string_view prog = "", std::string_view description = "");

    /**
     * @brief Destructor. Frees the underlying C parser and all owned resources.
     */
    ~Parser();

    /** @brief Copy is prohibited. */
    Parser(const Parser&) = delete;
    /** @brief Copy assignment is prohibited. */
    Parser& operator=(const Parser&) = delete;

    /**
     * @brief Move constructor.
     * @param other Parser to move from. Left in a valid empty state.
     */
    Parser(Parser&& other) noexcept;

    /**
     * @brief Move assignment operator.
     * @param other Parser to move from. Left in a valid empty state.
     * @return Reference to this.
     */
    Parser& operator=(Parser&& other) noexcept;

    /**
     * @brief Set the program name.
     * @param prog Program name string.
     * @return Reference to this Parser for chaining.
     */
    Parser& set_prog(std::string_view prog);

    /**
     * @brief Set the program description.
     * @param description Description shown in help output.
     * @return Reference to this Parser for chaining.
     */
    Parser& set_description(std::string_view description);

    /**
     * @brief Set the epilog text shown after options in help output.
     * @param epilog Epilog text.
     * @return Reference to this Parser for chaining.
     */
    Parser& set_epilog(std::string_view epilog);

    /**
     * @brief Set a custom usage string.
     * @param usage Custom usage line (overrides auto-generated usage).
     * @return Reference to this Parser for chaining.
     */
    Parser& set_usage(std::string_view usage);

    /**
     * @brief Add an option with both short and long names.
     * @param short_name Single-character short option name, or '\0' for none.
     * @param long_name Long option name (e.g., "output").
     * @param nargs Number of arguments the option accepts.
     * @param type Type of the parsed value.
     * @param help Help text for this option.
     * @param dest Destination variable name for storing the parsed value.
     * @return Option wrapper for further configuration.
     */
    Option add_option(char short_name, std::string_view long_name,
                      argparse_nargs nargs = ARGPARSE_NARGS_1,
                      argparse_type type = ARGPARSE_TYPE_STRING,
                      std::string_view help = "",
                      std::string_view dest = "");

    /**
     * @brief Add an option with only a long name.
     * @param long_name Long option name (e.g., "output").
     * @param nargs Number of arguments the option accepts.
     * @param type Type of the parsed value.
     * @param help Help text for this option.
     * @param dest Destination variable name for storing the parsed value.
     * @return Option wrapper for further configuration.
     */
    Option add_option(std::string_view long_name,
                      argparse_nargs nargs = ARGPARSE_NARGS_1,
                      argparse_type type = ARGPARSE_TYPE_STRING,
                      std::string_view help = "",
                      std::string_view dest = "");

    /**
     * @brief Add a positional argument.
     * @param nargs Number of arguments this positional accepts.
     * @param type Type of the parsed value.
     * @param help Help text for this positional argument.
     * @param dest Destination variable name for storing the parsed value.
     * @return Option wrapper for further configuration.
     */
    Option add_positional(argparse_nargs nargs = ARGPARSE_NARGS_1,
                          argparse_type type = ARGPARSE_TYPE_STRING,
                          std::string_view help = "",
                          std::string_view dest = "");

    /**
     * @brief Add an option group for organized help output.
     * @param title Group title shown in help.
     * @param description Optional group description.
     * @return Group wrapper for adding options to the group.
     */
    Group add_group(std::string_view title, std::string_view description = "");

    /**
     * @brief Add a mutually exclusive option group.
     * @param group Optional parent group. If non-null, the mutex is added to this group.
     * @return Mutex wrapper for adding options.
     */
    Mutex add_mutex(Group* group = nullptr);

    /**
     * @brief Parse command-line arguments from argc/argv.
     * @param argc Argument count.
     * @param argv Argument vector.
     * @return Parse result. Check has_error() before accessing values.
     */
    Result parse(int argc, const char** argv);

    /**
     * @brief Parse command-line arguments from a string vector.
     * @param args Vector of argument strings.
     * @return Parse result. Check has_error() before accessing values.
     */
    Result parse(const std::vector<std::string>& args);

    /**
     * @brief Print help text to stdout.
     */
    void print_help() const;

    /**
     * @brief Print usage text to stdout.
     */
    void print_usage() const;

    /**
     * @brief Print version text to stdout.
     */
    void print_version() const;

    /**
     * @brief Get the underlying C handle.
     * @return Pointer to the C argparse parser.
     */
    struct argparse* handle() const noexcept;

private:
    struct argparse* handle_;
};

} // namespace argparse_c

#endif // __cplusplus
