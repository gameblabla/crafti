CXX	= kos-c++
CXXFLAGS = -Ofast -flto -m4-single-only -fsingle-precision-constant -ftree-vectorize -fdata-sections -ffunction-sections -fargument-noalias-global -Isrc -std=c++11  -I ngl -I.

#Including OPUS Libraries
LDFLAGS = -nodefaultlibs -lc -lm -Wl,--as-needed -Wl,--gc-sections -s -flto

DEFINES = -Wall
OUTPUT = hcl.elf

OBJS += $(patsubst %.cpp, %.o, $(shell find . -name \*.cpp))

all: ${OUTPUT}

${OUTPUT}: ${OBJS}
	${CXX} ${CXXFLAGS} -o ${OUTPUT} ${OBJS} ${LDFLAGS} ${DEFINES} 

pack:
	rm -f IP.BIN
	sh-elf-objcopy -R .stack -O binary hcl.elf main.bin
	${KOS_BASE}/utils/scramble/scramble main.bin ./cd/1ST_READ.BIN
	makeip ip.txt IP.BIN
	mkisofs -C 0,$(shell lbacalc track1.raw) -V CRAFTI -G IP.BIN -joliet -rock -l -o "crafti.iso" ./cd
	mds4dc -c crafti.mds crafti.iso track1.raw
	
pack_emu:
	rm -f IP.BIN
	sh-elf-objcopy -R .stack -O binary hcl.elf main.bin
	${KOS_BASE}/utils/scramble/scramble main.bin ./cd/1ST_READ.BIN
	makeip ip.txt IP.BIN
	mkisofs -V crafti -G IP.BIN -joliet -rock -l -x cd -o "crafti.iso" ./cd
	cdi4dc crafti.iso crafti.cdi -d

	
clean:
	rm *.o *.img src/*.o src/sdl/*.o src/dc/*.o src/enemies/*.o ${OUTPUT} *.BIN *.iso *.mds *.mdf
