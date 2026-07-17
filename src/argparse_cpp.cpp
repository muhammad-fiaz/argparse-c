/**
 * @file argparse_cpp.cpp
 * @brief Implementation of the argparse-c C++ wrapper.
 */

#include "argparse-c/argparse.hpp"

#include <cstring>
#include <algorithm>
#include <unordered_map>
#include <vector>

// ---------------------------------------------------------------------------
// Internal helpers
// ---------------------------------------------------------------------------

namespace {

using ValidatorFn = std::function<bool(const std::string&, std::string&)>;
using ValidatorMap = std::unordered_map<struct argparse_option*,
                                       std::shared_ptr<ValidatorFn>>;

ValidatorMap& validator_registry() {
    static ValidatorMap map;
    return map;
}

bool validator_bridge(const char* value, char* error_buf,
                      size_t error_buf_size) {
    (void)value;
    (void)error_buf;
    (void)error_buf_size;
    return true;
}

} // anonymous namespace

// ---------------------------------------------------------------------------
// ParseError
// ---------------------------------------------------------------------------

namespace argparse_c {

ParseError::ParseError(const std::string& message, int error_code)
    : std::runtime_error(message), error_code_(error_code) {}

int ParseError::error_code() const noexcept {
    return error_code_;
}

// ---------------------------------------------------------------------------
// Option
// ---------------------------------------------------------------------------

Option::Option(struct argparse_option* handle) : handle_(handle) {}

Option& Option::default_value(std::string_view value) {
    argparse_option_set_default(handle_, std::string(value).c_str());
    return *this;
}

Option& Option::required(bool req) {
    argparse_option_set_required(handle_, req);
    return *this;
}

Option& Option::metavar(std::string_view name) {
    argparse_option_set_metavar(handle_, std::string(name).c_str());
    return *this;
}

Option& Option::action(argparse_action act) {
    argparse_option_set_action(handle_, act);
    return *this;
}

Option& Option::choices(std::vector<std::string> choice_list) {
    std::vector<const char*> c_strs;
    c_strs.reserve(choice_list.size());
    for (const auto& s : choice_list) {
        c_strs.push_back(s.c_str());
    }
    argparse_option_set_choices(handle_, c_strs.data(), c_strs.size());
    return *this;
}

Option& Option::validator(
    std::function<bool(const std::string&, std::string&)> fn) {
    (void)fn;
    argparse_option_set_validator(handle_, validator_bridge);
    return *this;
}

struct argparse_option* Option::handle() const noexcept {
    return handle_;
}

// ---------------------------------------------------------------------------
// Group
// ---------------------------------------------------------------------------

Group::Group(struct argparse_group* handle) : handle_(handle) {}

Option Group::add_option(char short_name, std::string_view long_name,
                         argparse_nargs nargs, argparse_type type,
                         std::string_view help, std::string_view dest) {
    auto* opt = argparse_add_option(
        nullptr, short_name, std::string(long_name).c_str(),
        nargs, type, std::string(help).c_str(),
        std::string(dest).c_str());
    if (opt) {
        argparse_group_add_option(handle_, opt);
    }
    return Option(opt);
}

Option Group::add_positional(argparse_nargs nargs, argparse_type type,
                             std::string_view help, std::string_view dest) {
    (void)nargs;
    (void)type;
    (void)help;
    (void)dest;
    return Option(nullptr);
}

struct argparse_group* Group::handle() const noexcept {
    return handle_;
}

// ---------------------------------------------------------------------------
// Mutex
// ---------------------------------------------------------------------------

Mutex::Mutex(struct argparse_mutex* handle) : handle_(handle) {}

void Mutex::add_option(Option& option) {
    argparse_mutex_add_option(handle_, option.handle());
}

struct argparse_mutex* Mutex::handle() const noexcept {
    return handle_;
}

// ---------------------------------------------------------------------------
// Result
// ---------------------------------------------------------------------------

Result::Result(struct argparse_result* handle) : handle_(handle) {}

Result::~Result() {
    if (handle_) {
        argparse_result_free(handle_);
        handle_ = nullptr;
    }
}

Result::Result(Result&& other) noexcept : handle_(other.handle_) {
    other.handle_ = nullptr;
}

Result& Result::operator=(Result&& other) noexcept {
    if (this != &other) {
        if (handle_) {
            argparse_result_free(handle_);
        }
        handle_ = other.handle_;
        other.handle_ = nullptr;
    }
    return *this;
}

std::optional<std::string> Result::get_string(std::string_view dest) const {
    const char* val = argparse_get_string(handle_, std::string(dest).c_str());
    if (val) {
        return std::string(val);
    }
    return std::nullopt;
}

std::optional<int> Result::get_int(std::string_view dest) const {
    const char* val = argparse_get_string(handle_, std::string(dest).c_str());
    if (!val) {
        return std::nullopt;
    }
    return argparse_get_int(handle_, std::string(dest).c_str());
}

std::optional<unsigned int> Result::get_uint(std::string_view dest) const {
    const char* val = argparse_get_string(handle_, std::string(dest).c_str());
    if (!val) {
        return std::nullopt;
    }
    return argparse_get_uint(handle_, std::string(dest).c_str());
}

std::optional<float> Result::get_float(std::string_view dest) const {
    const char* val = argparse_get_string(handle_, std::string(dest).c_str());
    if (!val) {
        return std::nullopt;
    }
    return argparse_get_float(handle_, std::string(dest).c_str());
}

std::optional<double> Result::get_double(std::string_view dest) const {
    const char* val = argparse_get_string(handle_, std::string(dest).c_str());
    if (!val) {
        return std::nullopt;
    }
    return argparse_get_double(handle_, std::string(dest).c_str());
}

std::optional<bool> Result::get_bool(std::string_view dest) const {
    const char* val = argparse_get_string(handle_, std::string(dest).c_str());
    if (!val) {
        return std::nullopt;
    }
    return argparse_get_bool(handle_, std::string(dest).c_str());
}

bool Result::has_error() const noexcept {
    if (!handle_) return false;
    return argparse_result_error_code(handle_) != ARGPARSE_OK;
}

std::string Result::error() const {
    if (!handle_) {
        return "";
    }
    const char* msg = argparse_result_error(handle_);
    return msg ? std::string(msg) : "";
}

int Result::error_code() const noexcept {
    if (!handle_) {
        return ARGPARSE_OK;
    }
    return (int)argparse_result_error_code(handle_);
}

struct argparse_result* Result::handle() const noexcept {
    return handle_;
}

// ---------------------------------------------------------------------------
// Parser
// ---------------------------------------------------------------------------

Parser::Parser(std::string_view prog, std::string_view description)
    : handle_(argparse_new(std::string(prog).c_str(),
                           std::string(description).c_str())) {}

Parser::~Parser() {
    if (handle_) {
        argparse_free(handle_);
        handle_ = nullptr;
    }
}

Parser::Parser(Parser&& other) noexcept : handle_(other.handle_) {
    other.handle_ = nullptr;
}

Parser& Parser::operator=(Parser&& other) noexcept {
    if (this != &other) {
        if (handle_) {
            argparse_free(handle_);
        }
        handle_ = other.handle_;
        other.handle_ = nullptr;
    }
    return *this;
}

Parser& Parser::set_prog(std::string_view prog) {
    argparse_set_prog(handle_, std::string(prog).c_str());
    return *this;
}

Parser& Parser::set_description(std::string_view description) {
    argparse_set_description(handle_, std::string(description).c_str());
    return *this;
}

Parser& Parser::set_epilog(std::string_view epilog) {
    argparse_set_epilog(handle_, std::string(epilog).c_str());
    return *this;
}

Parser& Parser::set_usage(std::string_view usage) {
    argparse_set_usage(handle_, std::string(usage).c_str());
    return *this;
}

Option Parser::add_option(char short_name, std::string_view long_name,
                          argparse_nargs nargs, argparse_type type,
                          std::string_view help, std::string_view dest) {
    auto* opt = argparse_add_option(
        handle_, short_name, std::string(long_name).c_str(),
        nargs, type, std::string(help).c_str(),
        std::string(dest).c_str());
    return Option(opt);
}

Option Parser::add_option(std::string_view long_name,
                          argparse_nargs nargs, argparse_type type,
                          std::string_view help, std::string_view dest) {
    auto* opt = argparse_add_option(
        handle_, '\0', std::string(long_name).c_str(),
        nargs, type, std::string(help).c_str(),
        std::string(dest).c_str());
    return Option(opt);
}

Option Parser::add_positional(argparse_nargs nargs, argparse_type type,
                              std::string_view help, std::string_view dest) {
    auto* opt = argparse_add_positional(
        handle_, nargs, type, std::string(help).c_str(),
        std::string(dest).c_str());
    return Option(opt);
}

Group Parser::add_group(std::string_view title, std::string_view description) {
    auto* grp = argparse_add_group(handle_, std::string(title).c_str(),
                                   std::string(description).c_str());
    return Group(grp);
}

Mutex Parser::add_mutex(Group* group) {
    struct argparse_group* grp = group ? group->handle() : nullptr;
    auto* mtx = argparse_add_mutex(handle_, grp);
    return Mutex(mtx);
}

Result Parser::parse(int argc, const char** argv) {
    auto* result = argparse_parse(handle_, argc, argv);
    return Result(result);
}

Result Parser::parse(const std::vector<std::string>& args) {
    std::vector<const char*> c_args;
    c_args.reserve(args.size());
    for (const auto& a : args) {
        c_args.push_back(a.c_str());
    }
    auto* result = argparse_parse(handle_,
                                  static_cast<int>(c_args.size()),
                                  c_args.data());
    return Result(result);
}

void Parser::print_help() const {
    argparse_print_help(handle_);
}

void Parser::print_usage() const {
    argparse_print_usage(handle_);
}

void Parser::print_version() const {
    argparse_print_version(handle_);
}

struct argparse* Parser::handle() const noexcept {
    return handle_;
}

} // namespace argparse_c
