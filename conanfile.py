from conan import ConanFile
from conan.tools.cmake import CMake, CMakeDeps


class WssConan(ConanFile):
    name = "wss"
    settings = "os", "compiler", "build_type", "arch"
    requires = "catch2/[>=2 <3]", "fmt/[>=8]", "ms-gsl/[>=3]", "lyra/[>=1.5]"
    generators = ["CMakeDeps", "CMakeToolchain"]

    def build(self):
        cmake = CMake(self)
        cmake.verbose = False
        cmake.configure()
        cmake.build()
        cmake.test()
        cmake.install()
