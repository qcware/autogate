# The name of the autogate plugin
NAME = autogate_plugin

# C++ source files for your plugin. By default we grab all *.cpp files.
CXXSRC = $(notdir $(wildcard *.cpp))

# Complilers, flags, includes, and libraries
CXX = /usr/bin/g++

CXXFLAGS = \
    -std=c++11 \
    -fPIC \
    `python3 -m pybind11 --includes` \
    -O3 \
    -Wall \
    -Wuninitialized \
    -Wsign-compare \
    -Wno-unknown-pragmas \

INCLUDES = \

LIBRARIES = \

LDFLAGS = \

# Used to determine linking flags.
UNAME = $(shell uname)

# Add the flags needed for shared library creation
ifeq ($(UNAME), Linux)
    LDFLAGS += -shared
endif
ifeq ($(UNAME), Darwin)
    LDFLAGS += -shared -undefined dynamic_lookup
    CXXFLAGS += -fno-common
endif

TARGET = $(NAME).so

# Start the compilation rules
default:: $(TARGET)

# The object files
BINOBJ += $(CXXSRC:%.cpp=%.o)

%.o: %.cpp
	$(CXX) $(CXXDEFS) $(CXXFLAGS) $(INCLUDES) -c $<

$(TARGET): $(BINOBJ)
	$(CXX) $(LDFLAGS) -o $@ $^ $(CXXDEFS) $(LIBRARIES)

# Erase all compiled intermediate files
clean:
	rm -f $(BINOBJ) $(TARGET) *.d *.pyc 

