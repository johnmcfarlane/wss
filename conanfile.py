from conans import ConanFile, CMake

class WssConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    requires = "fmt/6.0.0@bincrafters/stable", \
               "gsl_microsoft/2.0.0@bincrafters/stable", \
               "lyra/1.3.0@_/_"
    generators = "cmake"
