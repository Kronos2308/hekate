#include "gui_menu.h"
#include "../tools/touch2.h"
#include "utils/btn.h"
#include "../tools/fs_utils.h"
#include "utils/util.h"
#include "display/di.h"
#include "../../gfx/gfx.h"
#include "mem/heap.h"
#include "power/max17050.h"
#include "custom-gui.h"
#include <string.h>
#include "soc/t210.h"
#include "soc/fuse.h"
#include "../tools/tools.h"
#include <stdio.h>
#include "input/touch.h"


#define MINOR_VERSION 3
#define MAJOR_VERSION 0
#define REVI_VERSION LOAD_BUILD_VER

char Sversion[5];
bool haschange = false;
extern int main_menu;
u64 eme;

/* Render the menu */
//static void gui_menu_render_menu(gui_menu_t*);
static void gui_menu_draw_background(gui_menu_t*);
static void gui_menu_draw_entries(gui_menu_t*);

/* Update menu after an input */
//static int gui_menu_update(gui_menu_t*);

/* Handle input */
static int handle_touch_input(gui_menu_t*);
extern u32 isAMS;
gui_menu_t *gui_menu_create(const char *title,char *back)
{
	gui_menu_t *menu = (gui_menu_t *)malloc(sizeof(gui_menu_t));
    menu->custom_gui = custom_gui_load(back);
	strcpy(menu->title, title);
	menu->next_entry = 0;
	menu->selected_index = 0;
    //gui_menu_push_to_pool((void*)menu);
	return menu;
}

void gui_menu_append_entry(gui_menu_t *menu, gui_menu_entry_t *menu_entry)
{
	if (menu->next_entry == MAX_ENTRIES)
		return;

	menu->entries[menu->next_entry] = menu_entry;
	menu->next_entry++;
}

static void gui_menu_draw_background(gui_menu_t *menu)
{
    if(!render_custom_background(menu->custom_gui))
        gfx_clear_color( 0xFF191414);
    //StarDust version
    static bool a = true;
    if (a){
        //if (sd_mount())
        {
            char *str;
            void *buf;
            buf = read_file_string("StarDust/StarDustV.txt");
            str = buf;
            if (strlen(str)!=0)
            {
                    
                Sversion[0] = str[0];
                Sversion[1] = str[1];
                Sversion[2] = str[2];
                Sversion[3] = str[3];
                Sversion[4] = 0;	
            }
            
        }
        a = false;
    }

    gfx_con_setcol( 0xFFCCCCCC, 0xFFCCCCCC, 0xFF191414);
    gfx_con.scale = 2;
    gfx_con_setpos( 1200, 50);
    gfx_printf( "v%s\n", Sversion);

		
    u32 burntFuses = fusesB();
    //char* mindowngrade = fusesM();

	gfx_con_setpos( 1, 1);
//	gfx_con_setpos( 1145+jump, 60);
	gfx_printf("%k%d%k%k%d%k\n\n",0xFF00FF22, REVI_VERSION ,0xFFCCCCCC, 0xFFea2f1e, burntFuses ,0xFFCCCCCC);
	gfx_con_setcol( 0xFFF9F9F9, 0, 0xFF191414);
/* 
Rojo  0xFFea2f1e
azul  0xFF331ad8
      0xFFF9F9F9
Blank 0xFFCCCCCC
verde 0xFF00FF22
*/
}

static void gui_menu_render_menu(gui_menu_t* menu) 
{
    gui_menu_draw_background(menu);
    gui_menu_draw_entries(menu);
    gfx_swap_buffer();
}

static void gui_menu_draw_entries(gui_menu_t *menu)
{
    for (s16 i = 0; i < menu->next_entry; i++)
        gui_menu_render_entry(menu->entries[i]);
}

/*
static int gui_menu_update(gui_menu_t *menu)
{
    u32 res = 0;
    res = handle_touch_input(menu);
    if (eme != main_menu || haschange){
        res = 0;
    }
    gfx_swap_buffer();
    return res;
}
*/
int gui_menu_open(gui_menu_t *menu)
{
    gfx_con_setcol( 0xFFF9F9F9, 0, 0xFF191414);
    gui_menu_render_menu(menu);
    /* 
     * Render and flush at first render because blocking input won't allow us 
     * flush buffers
     */
    //gui_menu_render_menu(menu);
	//sd_unmount();
    haschange = false;
    int res = 1;
	while (res){
        res = handle_touch_input(menu);
        if (haschange){
            haschange = false;
            gui_menu_render_menu(menu);
            change_brightness(0);
        }
    }
    minerva_periodic_training();
	return res;
}

int gui_menu_boot(char* imagemenus)
{
    /* 
     * AutoBoot Screen
     */
		if (sd_file_exists("StarDust/autobootecho.txt")&& !sd_file_exists("StarDust/autoboot.inc"))
		{
            gfx_con_setcol( 0xFF008F39, 0xFF726F68, 0xFF191414);
            
            char* imagemenus = "background.bmp";
            custom_gui_t* cg = custom_gui_load(imagemenus);
			if(!render_custom_background(cg))
				gfx_clear_color( 0xFF191414);
            custom_gui_end(cg);

			gfx_con.scale = 3;
			gfx_con_setpos( 1070, 0);
			gfx_con_setcol( 0xFFF9F9F9, 0xFFFFFFFF, 0xFF191414);
			gfx_printf( "AutoBoot\n");
			gfx_con_setpos( 500, 0);
			gfx_printf( "Vol +: StarDustMenu\n");
			
			gfx_con_setpos( 50, 0);
			char *str;
			//if (sd_mount())
            {
				void *buf;
				buf = read_file_string("StarDust/autobootecho.txt");
				str = buf;
				Sversion[0] = str[0];
				Sversion[1] = str[1];
				Sversion[2] = str[2];
				Sversion[3] =  0;
			}
			if(strstr(Sversion,"A") != NULL)
			gfx_printf( "-> Atmosphere\n");

			if(strstr(Sversion,"T") != NULL)
			gfx_printf( "-> Android\n");

			if(strstr(Sversion,"U") != NULL)
			gfx_printf( "-> Ubuntu\n");

			if(strstr(Sversion,"S") != NULL)
			{gfx_printf( "-> SXOS\n"); }//isAMS = 0;

			gfx_con_setcol( 0xFFF9F9F9, 0, 0xFF191414);
			return 1;
		}
	return 0;
}

void gui_menu_destroy(gui_menu_t *menu)
{
    if (menu != NULL) {
        for (int i = 0; i < menu->next_entry; i++)
            gui_menu_entry_destroy(menu->entries[i]);
        custom_gui_end(menu->custom_gui);
        //free(menu->entries);
        free(menu);
    }
}


static int handle_touch_input(gui_menu_t *menu)
{
    gui_menu_entry_t *entry = NULL;
    touch_event event = touch_wait();

		if (event.type == STMFTS_EV_MULTI_TOUCH_LEAVE){
			/* After touch input check if any entry has ben tapped */
			for(int i = 0; i < menu->next_entry; i++)
			{
				entry = menu->entries[i];

				if (entry->handler != NULL && is_rect_touched(&event, entry->x, entry->y, entry->width, entry->height))
				{
                    menu->selected_index = i;
                    if (entry->type == 1){
                        
                        if (entry->bit){
                            entry->bit = !entry->bit;
                            if (entry->handler(entry->param) != 0)
                                return 0;
                        } else {
                            entry->bit = !entry->bit;
                            if (entry->handler2(entry->param2) != 0)
                                return 0;
                        }
                        //haschange = true;
                        
                    } else {
                        if (entry->handler(entry->param) != 0)
                            return 0;
                        
                    }
				}
			}
		}

    return 1;
}

void create(gui_menu_t *menu, char *path, int x, int y, int (*handler)(void *), void *param)
{
	bit_hdr_t file_header;
	dip_hdr_t file_info_header;
	FIL fptr;
	f_open(&fptr, theme(path), FA_READ);

	__off_t size = f_size(&fptr);

	f_read(&fptr, &file_header, sizeof(file_header), NULL);
	f_read(&fptr, &file_info_header, sizeof(file_info_header), NULL);

	f_lseek(&fptr, 0);

	void *buf = malloc(size);
	f_read(&fptr, buf, size, NULL);

	f_close(&fptr);

	gui_menu_append_entry(menu, gui_create_menu_entry("", buf, x, y, file_info_header.width, file_info_header.height, handler, param));
    free(buf);
}

void create_switch(gui_menu_t *menu, const char *name, char *path, char *path2, bool bit, int x, int y, int (*handler)(void *), void *param, int (*handler2)(void *), void *param2)
{
	bit_hdr_t file_header;
	dip_hdr_t file_info_header;
	FIL fptr;
	f_open(&fptr, theme(path), FA_READ);

	__off_t size = f_size(&fptr);

	f_read(&fptr, &file_header, sizeof(file_header), NULL);
	f_read(&fptr, &file_info_header, sizeof(file_info_header), NULL);

	f_lseek(&fptr, 0);

	void *buf = malloc(size);
	f_read(&fptr, buf, size, NULL);

	f_close(&fptr);

	gui_menu_append_entry(menu, gui_create_menu_entry_switch(name, buf,(u8*)sd_4_file_read2(theme(path2)),bit, x, y, file_info_header.width, file_info_header.height, handler, param, handler2, param2));
    free(buf);
}

void create_no_bitmap(gui_menu_t *menu, char *text, int x, int y, int width, int height)
{
	gui_menu_append_entry(menu, gui_create_menu_entry_no_bitmap(text, x, y, width, height, NULL, NULL));
}

char *them = "skins/xbox";

void loadTheme() {
    if (!sd_file_exists("/StarDust/theme"))
        return;

    char *temp = read_file_string("/StarDust/theme");
    if (temp && strlen(temp) > 6) {
        strcpy(them, temp); // Suponiendo que "them" es un char array predefinido
        free(temp); // Liberar la memoria asignada por read_file_string
    } else {
        free(temp); // Liberar la memoria antes de eliminar el archivo
        f_unlink("/StarDust/theme");
    }
}

void saveTheme(char *param)
{
    gfx_con_setpos(80, 10);
    gfx_printf( "%s\n",param);
    if (param && strlen(param) > 6) {
        them = param;
        sd_save_2_file(param, strlen(param), "/StarDust/theme");
    }
}

void *theme(char *path)
{
	char root[] = "/StarDust/";
	char *buff = malloc(strlen(path) + strlen(them) + strlen(root));
	u64 total = sizeof(buff);
	memset(buff, 0, total);
	strcat(buff, root);
	strcat(buff, them);
	strcat(buff, path);
	if (sd_file_exists(buff))
		return buff;
	memset(buff, 0, total);
	strcpy(buff, root);
	strcat(buff, "skins/Retro/");
	strcat(buff, path);
	if (sd_file_exists(buff))
		return buff;
	memset(buff, 0, total);
	strcpy(buff, root);
	strcat(buff, "skins/xbox/");
	strcat(buff, path);
	return buff;
}