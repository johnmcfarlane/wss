from conans import ConanFile, CMake


class WssConan(ConanFile):
    name = "wss"
    settings = "os", "compiler", "build_type", "arch"
    requires = "catch2/[2.x]", "fmt/[8.x]", "ms-gsl/[3.x]", "lyra/[1.5.x]"
    generators = "cmake_find_package"

    def build(self):
        cmake = CMake(self)
        cmake.verbose = True
        cmake.configure()
        cmake.build()
        cmake.test(output_on_failure=True)
        cmake.install()
