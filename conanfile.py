from conans import ConanFile, CMake

class WssConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    requires = "clara/1.1.5@bincrafters/stable",\
               "fmt/6.0.0@bincrafters/stable",\
               "gsl_microsoft/2.0.0@bincrafters/stable"
    generators = "cmake", "gcc", "txt"
