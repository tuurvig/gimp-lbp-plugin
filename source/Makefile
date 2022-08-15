NAME=LBPKvasnica
CXX=g++
LD=g++
CXX_FLAGS=-Wall -pedantic -O2
OBJS=LBPKvasnica.o ImageProcessor.o ImageRegion.o InImageRegion.o OutImageRegion.o Circle.o ProcessTile.o LBP.o ULBP.o mLBP.o LBPAbstract.o

all: build

install: installbin #delete 

clean:
	rm -f $(OBJS)

delete: clean
	rm -f $(NAME)

build: $(OBJS)
	$(LD) -o $(NAME) $(OBJS) $$( gimptool-2.0 --libs )

installbin: build
	gimptool-2.0 --install-bin $(NAME)

uninstall:
	gimptool-2.0 --uninstall-bin $(NAME)

%.o: %.cpp
	$(CXX) $(CXX_FLAGS) $$( gimptool-2.0 --cflags ) -c -o $@ $< 2>/dev/null

LBPKvasnica.o: LBPKvasnica.cpp
ImageProcessor.o: ImageProcessor.cpp
InImageRegion.o: InImageRegion.cpp
OutImageRegion.o: OutImageRegion.cpp
ImageRegion.o: ImageRegion.cpp
Circle.o: Circle.cpp
ProcessTile.o: ProcessTile.cpp
LBPAbstract.o: LBPAbstract.cpp
LBP.o: LBP.cpp
ULBP.o: ULBP.cpp
mLBP.o: mLBP.cpp