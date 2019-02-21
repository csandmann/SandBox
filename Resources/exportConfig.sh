#!/usr/bin/python

#read input
with open("config.ini", "r") as fin:
    content = fin.read()

#format for c++
content = content.replace('"', '\\"').replace("\n", "\\n")
content_size = len(content)

#write .h
with open("generated_config.h", "w") as fout:
    fout.write("#pragma once\n\n")
    fout.write("extern const char* pcConfig_content;\n")
    fout.write("extern const unsigned int nConfig_size;\n")

#write .cpp
with open("generated_config.cpp", "w") as fout:
    fout.write('#include "generated_config.h"\n\n')
    fout.write('const char* pcConfig_content = "{}";\n'.format(content))
    fout.write('const unsigned int nConfig_size = {};'.format(content_size))
