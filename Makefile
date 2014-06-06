.SUFFIXES:
.SUFFIXES: .o .c .cpp .h .d

CXX      = g++
RM       = rm

CXXFLAGS = -Wall -O2 -std=c++0x
LIBS     = -L/usr/lib/x86_64-linux-gnu/ -lopencv_core -lopencv_highgui -lopencv_imgproc
TARGET   = mudou

SRC      := $(wildcard *.cpp)
OBJECTS  := $(subst .cpp,.o,$(filter %.cpp,$(SRC)))
DEP_FILE := $(OBJECTS:.o=.d)

.PHONY : all
all:	$(TARGET)

.PHONY : clean
clean:
	-$(RM) -f $(TARGET) $(OBJECTS) $(DEP_FILE)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS) $(LIBS)

ifneq "$(MAKECMDGOALS)" "clean"
-include $(DEP_FILE)
endif

.cpp.o:
	$(CXX) $(CXXFLAGS) -MD -c $<


