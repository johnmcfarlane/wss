from conans import ConanFile, CMake

class WssConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    requires = "fmt/6.0.0", \
               "ms-gsl/2.0.0", \
               "lyra/1.3.0"
    generators = "cmake"
