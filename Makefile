
prefix = /usr/local
exec_prefix = ${prefix}
bindir = ${exec_prefix}/bin

CC = x86_64-lemon-gcc
LD = $(CC)
AS = $(CC)
INSTALL = /usr/bin/install -c

CFLAGS =  -pedantic -Wall -O3 -fstrength-reduce -fthread-jumps  -fcse-follow-jumps -fcse-skip-blocks -frerun-cse-after-loop  -fexpensive-optimizations -fforce-addr -fomit-frame-pointer
LDFLAGS = $(CFLAGS)  -s
ASFLAGS = $(CFLAGS)

TARGETS =  lemongnuboy

ASM_OBJS = 

SYS_DEFS = -DHAVE_CONFIG_H -DIS_LITTLE_ENDIAN  -DIS_LINUX
SYS_OBJS = sys/nix/nix.o $(ASM_OBJS)
SYS_INCS = -I/usr/local/include  -I./sys/nix

FB_OBJS =  sys/dummy/nojoy.o sys/dummy/nosound.o
FB_LIBS = 

SVGA_OBJS = sys/svga/svgalib.o sys/pc/keymap.o sys/dummy/nojoy.o sys/dummy/nosound.o
SVGA_LIBS = -L/usr/local/lib -lvga

SDL_OBJS = sys/sdl/sdl.o sys/sdl/sdl-audio.o sys/sdl/keymap.o
SDL_LIBS = 
SDL_CFLAGS = 

LEMON_OBJS = sys/lemon/lemon.o sys/lemon/video.o sys/pc/keymap.o
LEMON_LIBS = -llemon -lfreetype -lstdc++ -lpng -lz 
LEMON_CFLAGS = -std=c++14

X11_OBJS = sys/x11/xlib.o sys/x11/keymap.o sys/dummy/nojoy.o sys/dummy/nosound.o
X11_LIBS =  -lX11 -lXext

all: $(TARGETS)

include Rules

fbgnuboy: $(OBJS) $(SYS_OBJS) $(FB_OBJS)
	$(LD) $(LDFLAGS) $(OBJS) $(SYS_OBJS) $(FB_OBJS) -o $@ $(FB_LIBS)

sgnuboy: $(OBJS) $(SYS_OBJS) $(SVGA_OBJS)
	$(LD) $(LDFLAGS) $(OBJS) $(SYS_OBJS) $(SVGA_OBJS) -o $@ $(SVGA_LIBS)

sdlgnuboy: $(OBJS) $(SYS_OBJS) $(SDL_OBJS)
	$(LD) $(LDFLAGS) $(OBJS) $(SYS_OBJS) $(SDL_OBJS) -o $@ $(SDL_LIBS)

sys/sdl/sdl.o: sys/sdl/sdl.c
	$(MYCC) $(SDL_CFLAGS) -c $< -o $@

sys/sdl/keymap.o: sys/sdl/keymap.c
	$(MYCC) $(SDL_CFLAGS) -c $< -o $@

xgnuboy: $(OBJS) $(SYS_OBJS) $(X11_OBJS)
	$(LD) $(LDFLAGS) $(OBJS) $(SYS_OBJS) $(X11_OBJS) -o $@ $(X11_LIBS)

lemongnuboy: $(OBJS) $(SYS_OBJS) $(LEMON_OBJS)
	$(LD) $(LDFLAGS) $(OBJS) $(SYS_OBJS) $(LEMON_OBJS) -o $@ $(LEMON_LIBS)

sys/lemon/video.o: sys/lemon/video.cpp
	x86_64-lemon-g++ $(MY__) $(LEMON_CFLAGS) -c $< -o $@
	
sys/lemon/lemon.o: sys/lemon/lemon.c
	$(MYCC) $(LEMON_CFLAGS) -c $< -o $@

joytest: joytest.o sys/dummy/nojoy.o
	$(LD) $(LDFLAGS) $^ -o $@

install: all
	$(INSTALL) -d $(bindir)
	$(INSTALL) -m 755 $(TARGETS) $(bindir)

clean:
	rm -f *gnuboy gmon.out *.o sys/*.o sys/*/*.o asm/*/*.o $(OBJS)

distclean: clean
	rm -f config.* sys/nix/config.h Makefile




