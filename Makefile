CXX	= kos-c++
CXXFLAGS = -Ofast -flto -m4-single-only -fsingle-precision-constant -ftree-vectorize -fdata-sections -ffunction-sections -fargument-noalias-global -Isrc -std=c++11  -I ngl -I. -Idreamcast

#Including OPUS Libraries
LDFLAGS = -nodefaultlibs -lc -lm -Wl,--as-needed -Wl,--gc-sections -s -flto

DEFINES = -Wall
OUTPUT = hcl.elf

OBJS += $(patsubst %.cpp, %.o, $(shell find . -name \*.cpp))

SRCDIR		= ./ ./dreamcast ./ngl
VPATH		= $(SRCDIR)
SRC_C		= $(foreach dir, $(SRCDIR), $(wildcard $(dir)/*.c))
SRC_CP		= $(foreach dir, $(SRCDIR), $(wildcard $(dir)/*.cpp))
OBJ_C		= $(notdir $(patsubst %.c, %.o, $(SRC_C)))
OBJ_CP		= $(notdir $(patsubst %.cpp, %.o, $(SRC_CP)))
OBJS		= $(OBJ_C) $(OBJ_CP)

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
	rm *.o *.img src/*.o src/sdl/*.o src/dc/*.o src/enemies/*.o ${OUTPUT} *.BIN *.iso *.mds *.mdf *.obj ./xbox/*.obj ./ngl/*.obj *.d ./xbox/*.d ./ngl/*.d *.exe
