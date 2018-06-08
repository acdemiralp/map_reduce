from   conans       import ConanFile, CMake, tools
from   conans.tools import download, unzip
import os

class Project(ConanFile):
    name            = "map_reduce"
    description     = "Conan package for map_reduce."
    version         = "1.0.0"                
    url             = "https://github.com/acdemiralp/map_reduce"
    settings        = "arch", "build_type", "compiler", "os"
    generators      = "cmake"
    requires        = (("TBB/2018_U3@RWTH-VR/thirdparty" ),
                       ("catch2/2.2.0@bincrafters/stable"))

    def imports(self):
       self.copy("*.dylib*", dst="", src="lib")
       self.copy("*.dll"   , dst="", src="bin")

    def source(self):
        zip_name = "v%s.zip" % self.version
        download ("%s/archive/%s" % (self.url, zip_name), zip_name, verify=False)
        unzip    (zip_name)
        os.unlink(zip_name)

    def package(self):
        include_folder = "%s-%s/include" % (self.name, self.version)
        self.copy("*.h"  , dst="include", src=include_folder)
        self.copy("*.hpp", dst="include", src=include_folder)
        self.copy("*.inl", dst="include", src=include_folder)
