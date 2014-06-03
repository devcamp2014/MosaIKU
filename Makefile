CXX      = g++
CFLG     = 
INCLUDES =
LIBS     = -L/usr/lib/x86_64-linux-gnu/ -lopencv_core -lopencv_highgui 
TARGET   = mudou
OBJS	 = main.o

all:	$(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

clean:
	-rm -f $(TARGET) $(OBJS)

.cpp.o:
	$(CXX) $(CFLAGS) $(INCLUDES) -c $<

