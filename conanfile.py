from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout, CMakeDeps, CMakeToolchain
from conan.tools.files import copy, get
import os

class ArgparseConan(ConanFile):
    name = "argparse-c"
    version = "0.1.0"
    license = "MIT"
    author = "Muhammad Fiaz"
    url = "https://github.com/muhammad-fiaz/argparse-c"
    homepage = "https://muhammad-fiaz.github.io/argparse-c"
    description = "Production-quality C/C++ argument parsing library inspired by Python's argparse"
    topics = ("argument-parsing", "cli", "c-library", "cpp")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}
    exports_sources = "CMakeLists.txt", "src/*", "include/*"
    exports = "LICENSE", "README.md"

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def configure(self):
        if self.options.shared:
            self.options.rm("fPIC")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        copy(self, "LICENSE", dst=os.path.join(self.package_folder, "licenses"), src=self.source_folder)
        copy(self, "*.h", dst=os.path.join(self.package_folder, "include"), src=os.path.join(self.source_folder, "include"))
        copy(self, "*.hpp", dst=os.path.join(self.package_folder, "include"), src=os.path.join(self.source_folder, "include"))
        cmake = CMake(self)
        cmake.install()

    def package_id(self):
        self.info.clear()

    def package_info(self):
        self.cpp_info.libs = ["argparse-c"]
        self.cpp_info.includedirs = ["include"]
        self.cpp_info.bindirs = []
        self.cpp_info.set_property("cmake_file_name", "argparse-c")
        self.cpp_info.set_property("cmake_target_name", "argparse-c::argparse-c")
        self.cpp_info.set_property("cmake_target_aliases", ["argparse-c::static"])
