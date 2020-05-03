#include <gfx/window/window.h>
#include <lemon/ipc.h>
#include <lemon/keyboard.h>
#include <gfx/window/filedialog.h>
#include <stdio.h>

extern "C"{
	#include "fb.h"
	#include "input.h"
	#include "rc.h"
	#include "defs.h"
}

Lemon::GUI::Window* win;
win_info_t winInfo;
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
		return Lemon::GUI::FileDialog("/initrd");
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
		fbSurf.width = fb.w = winInfo.width = 160;
		fbSurf.height = fb.h = winInfo.height = 144;
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

		winInfo.x = winInfo.y = 60;
		win = Lemon::GUI::CreateWindow(&winInfo);
		Lemon::GUI::SwapWindowBuffers(win);

		fbSurf.buffer = fb.ptr = win->surface.buffer;
		fbSurf = win->surface;
	}

	void vid_close()
	{
		Lemon::GUI::DestroyWindow(win);
	}

	void vid_settitle(char *title)
	{
		strcpy(win->info.title, title);
		Lemon::GUI::UpdateWindow(win);
	}

	void vid_begin()
	{
		Lemon::GUI::SwapWindowBuffers(win);
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
		ipc_message_t msg;
		while(Lemon::ReceiveMessage(&msg)){
			if(msg.msg == WINDOW_EVENT_KEY){
				event_t ev;
				ev.type = EV_PRESS;
				ev.code = msg.data;
				if(msg.data == KEY_ARROW_UP) ev.code = K_UP;
				if(msg.data == KEY_ARROW_DOWN) ev.code = K_DOWN;
				if(msg.data == KEY_ARROW_LEFT) ev.code = K_LEFT;
				if(msg.data == KEY_ARROW_RIGHT) ev.code = K_RIGHT;
				ev_postevent(&ev);
			} else if(msg.msg == WINDOW_EVENT_KEYRELEASED){
				event_t ev;
				ev.type = EV_RELEASE;
				ev.code = msg.data;
				if(msg.data == KEY_ARROW_UP) ev.code = K_UP;
				if(msg.data == KEY_ARROW_DOWN) ev.code = K_DOWN;
				if(msg.data == KEY_ARROW_LEFT) ev.code = K_LEFT;
				if(msg.data == KEY_ARROW_RIGHT) ev.code = K_RIGHT;
				ev_postevent(&ev);
			}
		}
	}
}
