CXX = $(shell fltk-config --cxx)
CXXFLAGS = -I/usr/include $(shell fltk-config --use-glut --use-images --cxxflags)
#LDFLAGS = $(shell fltk-config --use-glut --use-images --ldflags ) -ltiff
LDFLAGS = -lGL -lGLU -lglut $(shell fltk-config --use-glut --use-images --ldflags) -ltiff -lCg -lCgGL   #-L/usr/lib/x86_64-linux-gnu
LDSTATIC = $(shell fltk-config --use-glut --use-images --ldstaticflags) -ltiff
DEP = Vector3D.h Matrix3D.h gui.h FrameBuffer.h FrameBufferWrapper.h PPC.h \
	TriangleMesh.h AABB.h Texture.h Mipmap.h PSL.h TP.h CubeMap.h CGInterface.h
OBJS = main TestVector3D TestMatrix3D

FILETOTEST = Vector3D Matrix3D
FILEFORTEST = $(addprefix Test, $(FILETOTEST))
TESTSOURCE = $(addsuffix .c, $(FILETOTEST) $(FILEFORTEST))

.PHONY: clean

all: $(DEP)
	fluid -c gui.fl
	$(CXX) $(CXXFLAGS) -o main gui.cxx FrameBuffer.cpp FrameBufferWrapper.cpp Matrix3D.cpp Vector3D.cpp TriangleMesh.cpp PPC.cpp AABB.cpp Texture.cpp Mipmap.cpp PSL.cpp TP.cpp CubeMap.cpp CGInterface.cpp $(LDFLAGS)

test1: test.c
	$(CXX) $(CXXFLAGS) -o test test.c $(LDFLAGS)

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $<

test: $(patsubst %.c,%.o,$(TESTSOURCE))
	$(CXX) -o TestVector3D TestVector3D.o Vector3D.o $(LDFLAGS)
	$(CXX) -o TestMatrix3D TestMatrix3D.o Matrix3D.o Vector3D.o $(LDFLAGS)

clean:
	rm -f $(OBJS) *.o main
