add_rules("mode.debug", "mode.release")

target("argparse-c")
    set_kind("$(shell if is_mode(shared) then return \"shared\" else return \"static\" end)")
    add_files("src/*.c", "src/*.cpp")
    add_includedirs("include", "src")
    set_targetdir("$(buildir)")
    set_targetfile("argparse-c")
    add_headerfiles("include/argparse-c/*.h", "include/argparse-c/*.hpp")
    set_languages("c99", "cpp20")

    if is_plat("windows") then
        add_cflags("/W4")
        add_cxxflags("/W4")
    else
        add_cflags("-Wall", "-Wextra", "-Wpedantic")
        add_cxxflags("-Wall", "-Wextra", "-Wpedantic")
    end
