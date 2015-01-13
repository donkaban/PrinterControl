
HOST 	= ARM
TARGET	= ./pcontrol

# -------------------------------------------------------------------------------------------

SOURCES = main.cpp get_ip.cpp
HEADERS =

# -------------------------------------------------------------------------------------------
MACOS_CXX = g++
LINUX_CXX = g++
ARM_CXX   = arm-angstrom-linux-gnueabi-g++

COMMON_CXX_FLAGS = -c -Wall -Wextra -pedantic -O3 -std=c++0x
COMMON_LNK_FLAGS = -lX11

MACOS_CXX_FLAGS  = -I/opt/X11/include -Wno-variadic-macros
MACOS_LINK_FLAGS = -L/opt/X11/lib

CXX=$($(HOST)_CXX)

OBJECTS=$(SOURCES:.cpp=.o)

all: $(SOURCES) $(HEADERS) $(TARGET) Makefile
	rm -f $(OBJECTS)

$(TARGET): $(OBJECTS) $(HEADERS)  Makefile
	$(CXX) $(OBJECTS)  $(COMMON_LNK_FLAGS)  $($(HOST)_LINK_FLAGS)  -o $@
	
.cpp.o: $(SOURCES)  $(HEADERS) 
	$(CXX)  $(COMMON_CXX_FLAGS)  $($(HOST)_CXX_FLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET)
	rm -f $(OBJECTS)
	
	
	