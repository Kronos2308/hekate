#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "display/di.h"
#include "../../gfx/gfx.h"
#include "../tools/update.h"
#include "utils/btn.h"
#include "../tools/tools.h"

#include "utils/util.h"
#include "../tools/fs_utils.h"
#include <bdk.h>
#include "../../config.h"

char *type = "*";
char *clip = "";
extern hekate_config h_cfg;

//ScriptHandler
void lineHandler(char line[])
{
//Simple Instruccion
	/* If line is a comment return */
	if (line[0] == '#')
		return;

	/* If line is a message print it */
	if (line[0] == '[')
	{
		memmove(line, line + 1, strlen(line));
		printerCU(line, "", 0);
		return;
	}
	
	/* If line is a Title print it */
	if (line[0] == '.')
	{
		memmove(line, line + 1, strlen(line));
		printerCU("", line, 0);
		return;
	}

	/* If line is a ArchBit */
	if (line[0] == ':')
	{
		memmove(line, line + 1, strlen(line));
		if (HasArchBit(line))
		{
			printerCU(line, "", 0);
			Killflags(line);
		}
		return;
	}

//Logic instruccion
	/* If line is copy */
	if (line[0] == '<')
	{
		memmove(line, line + 1, strlen(line));
		clip=line;
		return;
	}

	/* If line is paste */
	if (line[0] == '>')
	{
        if (sd_file_exists(clip))
        {
            memmove(line, line + 1, strlen(line));
            copyfile(clip, line);
        }
		return;
	}

	/* If line is rename */
	if (line[0] == '~')
	{
		memmove(line, line + 1, strlen(line));
		f_rename(clip, line);
		return;
	}

	/* If line is cyper */
	if (line[0] == '_')
	{
        if (sd_file_exists(clip))
        {
            memmove(line, line + 1, strlen(line));
            cyper(clip, line);
        }
		return;
	}


	/* If line is a type set type for directories */
	if (line[0] == '*')
	{
		memmove(line, line + 1, strlen(line));
		type = line;
		return;
	}

	/* If line is make folder */
	if (line[0] == '+')
	{
		memmove(line, line + 1, strlen(line));
		if (line[strlen(line) - 1] == '/')
		{
			line[strlen(line) - 1] = 0;
			f_mkdir(line);
			return;
		}
		printerCU(line,"",0);
		sd_save_2_file(clip, strlen(clip), line);
		return;
	}
	
	/* if line is delete */
	if (line[0] == '-')
	{
		memmove(line, line + 1, strlen(line));
		printerCU(line,"",2);

		/* If line is a directory delete it all*/
		if (line[strlen(line) - 1] == '/')
		{
			line[strlen(line) - 1] = 0;
			deleteall(line, type, "");
			//printerCU(line,"",0);
			return;
		}
		/* If line is a file delete */
		if (strlen(line) > 0)
		{
			if (line[0] == '/')
			{
				f_unlink(line);
				return;
			}
		}
	}
}

void clean_up()
{
	if (!sd_file_exists("fixer.del")){return;}
    if (btn_read() & BTN_VOL_DOWN){return;}


	FIL delet;
	f_open(&delet, "/StarDust/Main.del", FA_READ);
	FILINFO stats;

	f_stat("/StarDust/Main.del", &stats);
	__off_t size = stats.fsize;

	char buff[size];
	f_read(&delet, buff, size, NULL);

	f_close(&delet);

	strrep(buff, "\r", "");
	char *p = strtok(buff, "\n");
	while (p != NULL)
	{
		lineHandler(p);
		p = strtok(NULL, "\n");
	}

	//not pegascape units
	if (fusesB() > 5)
	{
		printerCU("Remove PegaScape Files", "CleanUP...", 0);
		deleteall("/pegascape", "*", "");
		f_unlink("/switch/fakenews-injector.nro");
	}
    if (h_cfg.rcm_patched) {
        f_unlink("/atmosphere/contents/010000000000000D/exefs.nsp");
        f_unlink("/atmosphere/contents/0100000000001013/exefs.nsp");
        //Start Updated Stardust Menu TODO...240326 ready
        //_launch_payload("StarDust/payloads/hekate.bin", false, false);
    }

	//Fix old Emunand transfer
	//fix_emu()
    //msleep(5000);power_set_state(POWER_OFF);
    f_unlink("fixer.del");
	printerCU("", "", 1); //flush print
}

void update_std(){
	//some test verify payload
	if (sd_file_exists("StarDust/flags/ONE.flag"))
	{
		f_unlink("StarDust/flags/ONE.flag");
		launch_payload("payload.bin");
	}
	//update stardust
	bool cancel_auto_chainloading = btn_read() & BTN_VOL_UP;

	if ((sd_file_exists("StarDust_update/fixer.del") || sd_file_exists("StarDust_update/payload.bin")) & !cancel_auto_chainloading)
	{
		moverall("/StarDust_update", "", "*", "Updating");
		printerCU("Clean Update", "", 0);
		deleteall("/StarDust_update", "*", "Clean Update");
		f_rename("/StarDust_update", "/StarDust_corrupt_update"); //just in case
		launch_payload("payload.bin");
	}

}

void fix_emu()
{
	if (!(sd_file_exists("sxos/eMMC/00") & sd_file_exists("sxos/eMMC/boot0") & sd_file_exists("sxos/eMMC/boot1")))
		return;

	printerCU("Fix Emu Transfer", "CleanUP...", 0);
	f_mkdir("emuMMC");
	f_mkdir("emuMMC/EF00");
	f_rename("/sxos/eMMC", "/emuMMC/EF00/eMMC");
	f_rename("/Emutendo", "/emuMMC/EF00/Nintendo");

	if (!(sd_file_exists("emuMMC/EF00/eMMC/00") & sd_file_exists("emuMMC/EF00/eMMC/boot0") & sd_file_exists("emuMMC/EF00/eMMC/boot1")))
		return;

	f_unlink("emummc/emummc.bak");
	f_rename("emummc/emummc.ini", "emummc/emummc.bak");
	f_mkdir("emummc");
	FIL fp;
	f_open(&fp, "emummc/emummc.ini", FA_WRITE | FA_CREATE_ALWAYS);
	f_puts("[emummc]\n", &fp);
	f_puts("enabled=1\n", &fp);
	f_puts("path=emuMMC/EF00\n", &fp);
	f_puts("nintendo_path=emuMMC/EF00/Nintendo\n", &fp);
	f_close(&fp);
	sd_save_2_file("", 0, "emuMMC/EF00/file_based");
}


void CheckLogs(char* directory) {
    if (!sd_file_exists(directory)) return;
    char* files = listfil(directory, "*.log", true); // o "*" si no hay extensión fija
	if (!files) return;

    u32 i = 0;
    while (files[i * 256] != 0) {
        char* fname = &files[i * 256];
        
		char sourcefile[256];
        s_printf(sourcefile, "%s/%s",directory,&files[i * 256]);
        
		// Comprobamos si el nombre contiene el patrón
        if (strstr(fname, "0100000000001000")) {
			f_unlink(sourcefile);
			printerCU(sourcefile,"LOGS",0);
			deleteall("/atmosphere/contents/0100000000001000/romfs/lyt", "*", "");
			//msleep(5000);power_set_state(POWER_OFF);
        }
        i++;
    }
    free(files);
}



/*
//Script Main
//Simple Instruccion
	# Coment
	[ Print Message
	. Print Title

//Logic instruccion
	: Fix Archive Bit
	< Copy to Clipboard
	> Paste
	~ Rename, Move
	+ Make Folder or file with the contents of Clipboard with < can be 0 bytes
	- Delete files or folders
		* Set name of file to delete recursive,  * is for all, only for folders with /
	_ cyper
	add / at the end of the line to delimtate if is a folder

*/