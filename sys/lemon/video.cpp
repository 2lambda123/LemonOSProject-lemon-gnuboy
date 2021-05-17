#include <Lemon/GUI/Window.h>
#include <Lemon/Core/Keyboard.h>
#include <Lemon/GUI/FileDialog.h>
#include <stdio.h>

extern "C"{
	#include "fb.h"
	#include "input.h"
	#include "rc.h"
	#include "defs.h"
}

Lemon::GUI::Window* win;
surface_t fbSurf;

rcvar_t vid_exports[] =
{
	RCV_END
};

rcvar_t joy_exports[] =
{
	RCV_END
};

rcvar_t pcm_exports[] =
{
	RCV_END
};
struct fb fb;

extern "C"{

	char* filedialog(){
		return Lemon::GUI::FileDialog("/system");
	}

	/* keymap - mappings of the form { scancode, localcode } - from pc/keymap.c */
	extern int keymap[][2];

	static int mapscancode(int scan)
	{
		int i;
		for (i = 0; keymap[i][0]; i++)
			if (keymap[i][0] == scan)
				return keymap[i][1];
		return 0;
	}

	void vid_preinit()
	{

	}

	void vid_setpal(){}

	void vid_init()
	{
		fbSurf.width = fb.w = 160;
		fbSurf.height = fb.h = 144;
		fb.pelsize = 4;
		fb.pitch = fb.w * fb.pelsize;
		fb.enabled = 1;
		fb.dirty = 0;
		fb.indexed = 0;

		/*fb.yuv = 1;
		fb.cc[0].r = fb.cc[1].r = fb.cc[2].r = fb.cc[3].r = 0;
		fb.cc[0].l = 0;
		fb.cc[1].l = 24;
		fb.cc[2].l = 8;
		fb.cc[3].l = 16;*/

		fb.cc[0].r = fb.cc[1].r = fb.cc[2].r = fb.cc[3].r = 0;
		fb.cc[0].l = 16;
		fb.cc[1].l = 8;
		fb.cc[2].l = 0;
		fb.cc[3].l = 0;

		printf("gnuboy: Creating Window...");

		win = new Lemon::GUI::Window("Gnuboy", {160, 144}, Lemon::GUI::WindowType::Basic);
		win->Paint();

		fbSurf.buffer = fb.ptr = win->surface.buffer;
		fbSurf = win->surface;
	}

	void vid_close()
	{
		delete win;
	}

	void vid_settitle(char *title)
	{
		win->SetTitle(title);
	}

	void vid_begin()
	{
		win->Paint();
		fbSurf.buffer = fb.ptr = win->surface.buffer;
		fbSurf = win->surface;
	}

	void vid_end()
	{
	}

	void kb_init()
	{
	}

	void kb_close()
	{
	}

	void kb_poll()
	{
	}

	void ev_poll()
	{
		Lemon::LemonEvent event;
		while(win->PollEvent(event)){
			if(event.event == Lemon::EventKeyPressed){
				event_t ev;
				ev.type = EV_PRESS;
				ev.code = event.key;
				if(event.key == KEY_ARROW_UP) ev.code = K_UP;
				if(event.key == KEY_ARROW_DOWN) ev.code = K_DOWN;
				if(event.key == KEY_ARROW_LEFT) ev.code = K_LEFT;
				if(event.key == KEY_ARROW_RIGHT) ev.code = K_RIGHT;
				ev_postevent(&ev);
			} else if(event.event == Lemon::EventKeyReleased){
				event_t ev;
				ev.type = EV_RELEASE;
				ev.code = event.key;
				if(event.key == KEY_ARROW_UP) ev.code = K_UP;
				if(event.key == KEY_ARROW_DOWN) ev.code = K_DOWN;
				if(event.key == KEY_ARROW_LEFT) ev.code = K_LEFT;
				if(event.key == KEY_ARROW_RIGHT) ev.code = K_RIGHT;
				ev_postevent(&ev);
			} else if(event.event == Lemon::EventWindowClosed){
                exit(0);
            }
		}
	}
}
