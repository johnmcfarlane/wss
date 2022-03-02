from conans import ConanFile, CMake


class WssConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    requires = "fmt/[8.x]", "ms-gsl/[3.x]", "lyra/[1.5.x]"
    generators = "cmake_find_package"

    def build(self):
        cmake = CMake(self)
        cmake.verbose = False
        cmake.configure()
        cmake.build()
        cmake.test(output_on_failure=True)
        cmake.install()
