package("argparse-c")
    set_homepage("https://muhammad-fiaz.github.io/argparse-c")
    set_description("Production-quality C/C++ argument parsing library inspired by Python's argparse")
    set_license("MIT")

    set_urls("https://github.com/muhammad-fiaz/argparse-c.git")

    add_versions("0.1.0", "main")

    on_install(function (package)
        import("package-manager.cmake.install")(package, {
            build_shared = package:config("shared"),
        })
    end)

    on_test(function (package)
        assert(package:check_cxxsnippets({embed = true}, [[
            #include <argparse-c/argparse.h>
            int main() {
                struct argparse *p = argparse_new("test", NULL);
                argparse_free(p);
                return 0;
            }
        ]]))
    end)
