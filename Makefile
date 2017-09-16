CXX = $(shell fltk-config --cxx)
CXXFLAGS = $(shell fltk-config --use-glut --use-images --cxxflags) -I.
#LDFLAGS = $(shell fltk-config --use-glut --use-images --ldflags ) -ltiff
LDFLAGS = $(shell fltk-config --use-glut --use-images --ldflags) -ltiff
LDSTATIC = $(shell fltk-config --use-gl --use-images --ldstaticflags) -ltiff
DEP = Vector3D.h Matrix3D.h gui.h FrameBuffer.h FrameBufferWrapper.h
OBJS = main TestVector3D TestMatrix3D

FILETOTEST = Vector3D Matrix3D
FILEFORTEST = $(addprefix Test, $(FILETOTEST))
TESTSOURCE = $(addsuffix .c, $(FILETOTEST) $(FILEFORTEST))

.PHONY: clean

all: $(DEP)
	fluid -c gui.fl
	$(CXX) -o main gui.cxx FrameBuffer.cpp FrameBufferWrapper.cpp Matrix3D.cpp Vector3D.cpp $(LDFLAGS)

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $<

test: $(patsubst %.c,%.o,$(TESTSOURCE))
	$(CXX) -o TestVector3D TestVector3D.o Vector3D.o $(LDFLAGS)
	$(CXX) -o TestMatrix3D TestMatrix3D.o Matrix3D.o Vector3D.o $(LDFLAGS)

clean:
	rm -f $(OBJS) *.o
