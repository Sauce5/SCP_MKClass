CC = gcc
#CFLAGS = -O3
#Replace the CFLAGS line above with the CFLAGS line below to compile with 
#gcc on a 64-bit machine
CFLAGS = -O3 -m64 -mtune=native
LDLIBS = -lm
LOCALBIN = ./
IMGUI_DIR = ../imgui/

all: mkclass mkprelim srebin0 smooth2

mkclass: code3spt.o ffopen.o lstsqr.o lummin.o mkclass.o not_normal3.o \
pec2.o sp4class.o spt2min.o spt3code.o sptCaK.o spthydrogen.o sptKMd.o \
sptmetal3.o templateDSO2.o util.o
code3spt.o: code3spt.c
ffopen.o: ffopen.c
lstsqr.o: lstsqr.c util.h
lummin.o: lummin.c util.h
mkclass.o: mkclass.c util.h
not_normal3.o: not_normal3.c util.h
pec2.o: pec2.c util.h
sp4class.o: sp4class.c util.h
spt2min.o: spt2min.c util.h
spt3code.o: spt3code.c
sptCaK.o: sptCaK.c util.h
spthydrogen.o: spthydrogen.c util.h
sptKMd.o: sptKMd.c util.h
sptmetal3.o: sptmetal3.c util.h
templateDSO.o: templateDSO2.c
util.o: util.c util.h

mkprelim: mkprelim.o gaussj.o util.o
mkprelim.o: mkprelim.c util.h
gaussj.o: gaussj.c util.h

srebin0: srebin0.o getspectrum.o util.o
srebin0.o: srebin0.c util.h
getspectrum.o: getspectrum.c

smooth2: smooth2.o util.o
smooth2.o: smooth2.c util.h

install: 
#mv -f mkclass $(LOCALBIN)
#mv -f mkprelim $(LOCALBIN)
#mv -f srebin0 $(LOCALBIN)
#mv -f smooth2 $(LOCALBIN)
	cp -f mkclass.exe $(IMGUI_DIR)
	cp -f mkprelim.exe $(IMGUI_DIR)
	cp -f srebin0.exe $(IMGUI_DIR)
	cp -f smooth2.exe $(IMGUI_DIR)
	rm -f *.o

clean:
	rm -f mkclass
	rm -f mkprelim
	rm -f srebin0
	rm -f smooth2
	rm -f .mat
	rm -f out.out
	rm -f temp.out
	rm -f test.out
	rm -f normal.dat
	rm -f *.o
