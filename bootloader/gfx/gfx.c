/*
 * Copyright (c) 2018 naehrwert
 * Copyright (c) 2018-2022 CTCaer
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdarg.h>
#include <string.h>
#include "gfx.h"

#define TRANSPARENT_COLOR 0xFF1D1919

// Global gfx console and context.
gfx_ctxt_t gfx_ctxt;
gfx_con_t gfx_con;

static bool gfx_con_init_done = false;

static const u8 _gfx_font[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Char 032 ( )
	0x00, 0x30, 0x30, 0x18, 0x18, 0x00, 0x0C, 0x00, // Char 033 (!)
	0x00, 0x22, 0x22, 0x22, 0x00, 0x00, 0x00, 0x00, // Char 034 (")
	0x00, 0x66, 0x66, 0xFF, 0x66, 0xFF, 0x66, 0x66, // Char 035 (#)
	0x00, 0x18, 0x7C, 0x06, 0x3C, 0x60, 0x3E, 0x18, // Char 036 ($)
	0x00, 0x46, 0x66, 0x30, 0x18, 0x0C, 0x66, 0x62, // Char 037 (%)
	0x00, 0x3C, 0x66, 0x3C, 0x1C, 0xE6, 0x66, 0xFC, // Char 038 (&)
	0x00, 0x18, 0x0C, 0x06, 0x00, 0x00, 0x00, 0x00, // Char 039 (')
	0x00, 0x30, 0x18, 0x0C, 0x0C, 0x18, 0x30, 0x00, // Char 040 (()
	0x00, 0x0C, 0x18, 0x30, 0x30, 0x18, 0x0C, 0x00, // Char 041 ())
	0x00, 0x66, 0x3C, 0xFF, 0x3C, 0x66, 0x00, 0x00, // Char 042 (*)
	0x00, 0x18, 0x18, 0x7E, 0x18, 0x18, 0x00, 0x00, // Char 043 (+)
	0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x0C, 0x00, // Char 044 (,)
	0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, 0x00, // Char 045 (-)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, // Char 046 (.)
	0x00, 0x40, 0x60, 0x30, 0x18, 0x0C, 0x06, 0x00, // Char 047 (/)
	0x00, 0x3C, 0x66, 0x76, 0x6E, 0x66, 0x3C, 0x00, // Char 048 (0)
	0x00, 0x18, 0x1C, 0x18, 0x18, 0x18, 0x7E, 0x00, // Char 049 (1)
	0x00, 0x3C, 0x62, 0x30, 0x0C, 0x06, 0x7E, 0x00, // Char 050 (2)
	0x00, 0x3C, 0x62, 0x38, 0x60, 0x66, 0x3C, 0x00, // Char 051 (3)
	0x00, 0x6C, 0x6C, 0x66, 0xFE, 0x60, 0x60, 0x00, // Char 052 (4)
	0x00, 0x7E, 0x06, 0x7E, 0x60, 0x66, 0x3C, 0x00, // Char 053 (5)
	0x00, 0x3C, 0x06, 0x3E, 0x66, 0x66, 0x3C, 0x00, // Char 054 (6)
	0x00, 0x7E, 0x30, 0x30, 0x18, 0x18, 0x18, 0x00, // Char 055 (7)
	0x00, 0x3C, 0x66, 0x3C, 0x66, 0x66, 0x3C, 0x00, // Char 056 (8)
	0x00, 0x3C, 0x66, 0x7C, 0x60, 0x66, 0x3C, 0x00, // Char 057 (9)
	0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x18, 0x00, // Char 058 (:)
	0x00, 0x00, 0x18, 0x00, 0x18, 0x18, 0x0C, 0x00, // Char 059 (;)
	0x00, 0x70, 0x1C, 0x06, 0x06, 0x1C, 0x70, 0x00, // Char 060 (<)
	0x00, 0x00, 0x3E, 0x00, 0x3E, 0x00, 0x00, 0x00, // Char 061 (=)
	0x00, 0x0E, 0x38, 0x60, 0x60, 0x38, 0x0E, 0x00, // Char 062 (>)
	0x00, 0x3C, 0x66, 0x30, 0x18, 0x00, 0x18, 0x00, // Char 063 (?)
	0x00, 0x3C, 0x66, 0x76, 0x76, 0x06, 0x46, 0x3C, // Char 064 (@)
	0x00, 0x3C, 0x66, 0x7E, 0x66, 0x66, 0x66, 0x00, // Char 065 (A)
	0x00, 0x3E, 0x66, 0x3E, 0x66, 0x66, 0x3E, 0x00, // Char 066 (B)
	0x00, 0x3C, 0x66, 0x06, 0x06, 0x66, 0x3C, 0x00, // Char 067 (C)
	0x00, 0x1E, 0x36, 0x66, 0x66, 0x36, 0x1E, 0x00, // Char 068 (D)
	0x00, 0x7E, 0x06, 0x1E, 0x06, 0x06, 0x7E, 0x00, // Char 069 (E)
	0x00, 0x3E, 0x06, 0x1E, 0x06, 0x06, 0x06, 0x00, // Char 070 (F)
	0x00, 0x3C, 0x66, 0x06, 0x76, 0x66, 0x3C, 0x00, // Char 071 (G)
	0x00, 0x66, 0x66, 0x7E, 0x66, 0x66, 0x66, 0x00, // Char 072 (H)
	0x00, 0x3C, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00, // Char 073 (I)
	0x00, 0x78, 0x30, 0x30, 0x30, 0x36, 0x1C, 0x00, // Char 074 (J)
	0x00, 0x66, 0x36, 0x1E, 0x1E, 0x36, 0x66, 0x00, // Char 075 (K)
	0x00, 0x06, 0x06, 0x06, 0x06, 0x06, 0x7E, 0x00, // Char 076 (L)
	0x00, 0x46, 0x6E, 0x7E, 0x56, 0x46, 0x46, 0x00, // Char 077 (M)
	0x00, 0x66, 0x6E, 0x7E, 0x76, 0x66, 0x66, 0x00, // Char 078 (N)
	0x00, 0x3C, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x00, // Char 079 (O)
	0x00, 0x3E, 0x66, 0x3E, 0x06, 0x06, 0x06, 0x00, // Char 080 (P)
	0x00, 0x3C, 0x66, 0x66, 0x66, 0x3C, 0x70, 0x00, // Char 081 (Q)
	0x00, 0x3E, 0x66, 0x3E, 0x1E, 0x36, 0x66, 0x00, // Char 082 (R)
	0x00, 0x3C, 0x66, 0x0C, 0x30, 0x66, 0x3C, 0x00, // Char 083 (S)
	0x00, 0x7E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, // Char 084 (T)
	0x00, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x00, // Char 085 (U)
	0x00, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x18, 0x00, // Char 086 (V)
	0x00, 0x46, 0x46, 0x56, 0x7E, 0x6E, 0x46, 0x00, // Char 087 (W)
	0x00, 0x66, 0x3C, 0x18, 0x3C, 0x66, 0x66, 0x00, // Char 088 (X)
	0x00, 0x66, 0x66, 0x3C, 0x18, 0x18, 0x18, 0x00, // Char 089 (Y)
	0x00, 0x7E, 0x30, 0x18, 0x0C, 0x06, 0x7E, 0x00, // Char 090 (Z)
	0x00, 0x3C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x3C, // Char 091 ([)
	0x00, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x40, 0x00, // Char 092 (\)
	0x00, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x30, 0x3C, // Char 093 (])
	0x00, 0x18, 0x3C, 0x66, 0x00, 0x00, 0x00, 0x00, // Char 094 (^)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, // Char 095 (_)
	0x00, 0x0C, 0x18, 0x30, 0x00, 0x00, 0x00, 0x00, // Char 096 (`)
	0x00, 0x00, 0x3C, 0x60, 0x7C, 0x66, 0x7C, 0x00, // Char 097 (a)
	0x00, 0x06, 0x06, 0x3E, 0x66, 0x66, 0x3E, 0x00, // Char 098 (b)
	0x00, 0x00, 0x3C, 0x06, 0x06, 0x06, 0x3C, 0x00, // Char 099 (c)
	0x00, 0x60, 0x60, 0x7C, 0x66, 0x66, 0x7C, 0x00, // Char 100 (d)
	0x00, 0x00, 0x3C, 0x66, 0x7E, 0x06, 0x3C, 0x00, // Char 101 (e)
	0x00, 0x38, 0x0C, 0x3E, 0x0C, 0x0C, 0x0C, 0x00, // Char 102 (f)
	0x00, 0x00, 0x7C, 0x66, 0x7C, 0x40, 0x3C, 0x00, // Char 103 (g)
	0x00, 0x06, 0x06, 0x3E, 0x66, 0x66, 0x66, 0x00, // Char 104 (h)
	0x00, 0x18, 0x00, 0x1C, 0x18, 0x18, 0x3C, 0x00, // Char 105 (i)
	0x00, 0x30, 0x00, 0x30, 0x30, 0x30, 0x1E, 0x00, // Char 106 (j)
	0x00, 0x06, 0x06, 0x36, 0x1E, 0x36, 0x66, 0x00, // Char 107 (k)
	0x00, 0x1C, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00, // Char 108 (l)
	0x00, 0x00, 0x66, 0xFE, 0xFE, 0xD6, 0xC6, 0x00, // Char 109 (m)
	0x00, 0x00, 0x3E, 0x66, 0x66, 0x66, 0x66, 0x00, // Char 110 (n)
	0x00, 0x00, 0x3C, 0x66, 0x66, 0x66, 0x3C, 0x00, // Char 111 (o)
	0x00, 0x00, 0x3E, 0x66, 0x66, 0x3E, 0x06, 0x00, // Char 112 (p)
	0x00, 0x00, 0x7C, 0x66, 0x66, 0x7C, 0x60, 0x00, // Char 113 (q)
	0x00, 0x00, 0x3E, 0x66, 0x06, 0x06, 0x06, 0x00, // Char 114 (r)
	0x00, 0x00, 0x7C, 0x06, 0x3C, 0x60, 0x3E, 0x00, // Char 115 (s)
	0x00, 0x18, 0x7E, 0x18, 0x18, 0x18, 0x70, 0x00, // Char 116 (t)
	0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x7C, 0x00, // Char 117 (u)
	0x00, 0x00, 0x66, 0x66, 0x66, 0x3C, 0x18, 0x00, // Char 118 (v)
	0x00, 0x00, 0xC6, 0xD6, 0xFE, 0x7C, 0x6C, 0x00, // Char 119 (w)
	0x00, 0x00, 0x66, 0x3C, 0x18, 0x3C, 0x66, 0x00, // Char 120 (x)
	0x00, 0x00, 0x66, 0x66, 0x7C, 0x60, 0x3C, 0x00, // Char 121 (y)
	0x00, 0x00, 0x7E, 0x30, 0x18, 0x0C, 0x7E, 0x00, // Char 122 (z)
	0x00, 0x18, 0x08, 0x08, 0x04, 0x08, 0x08, 0x18, // Char 123 ({)
	0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, // Char 124 (|)
	0x00, 0x0C, 0x08, 0x08, 0x10, 0x08, 0x08, 0x0C, // Char 125 (})
	0x00, 0x00, 0x00, 0x4C, 0x32, 0x00, 0x00, 0x00  // Char 126 (~)
};

void gfx_clear_grey(u8 color)
{
	memset(gfx_ctxt.fb, color, gfx_ctxt.width * gfx_ctxt.height * 4);
}

void gfx_clear_partial_grey(u8 color, u32 pos_x, u32 height)
{
	memset(gfx_ctxt.fb + pos_x * gfx_ctxt.stride, color, height * 4 * gfx_ctxt.stride);
}

void gfx_clear_color(u32 color)
{
	for (u32 i = 0; i < gfx_ctxt.width * gfx_ctxt.height; i++)
		gfx_ctxt.fb[i] = color;
}

void gfx_init_ctxt(u32 *fb, u32 width, u32 height, u32 stride)
{
	gfx_ctxt.fb = fb;
	//gfx_ctxt.fb = fb;
	gfx_ctxt.width = width;
	gfx_ctxt.height = height;
	gfx_ctxt.stride = stride;
    //gfx_ctxt.fb = fb + gfx_ctxt.width * gfx_ctxt.stride * 4;
    //memset(gfx_ctxt.fb, 0, gfx_ctxt.width * gfx_ctxt.stride * 4);
}

void gfx_clear_buffer()
{
    memset(gfx_ctxt.fb, 0, gfx_ctxt.width * gfx_ctxt.stride * 4);
}
void gfx_swap_buffer()
{
    /*
    gfx_con_setpos( 300, 10);
    gfx_printf( "size:%d", gfx_ctxt.fb);

    //gfx_con.mute=1;
    u32* tmp = gfx_ctxt.fb;
    gfx_clear_buffer_M();
    gfx_ctxt.fb = gfx_ctxt.fb;
    gfx_ctxt.fb = tmp;
    //set_active_framebuffer(gfx_ctxt->fb);
    //gfx_clear_buffer(gfx_ctxt);
    //gfx_con.mute=0;
    */
}
void gfx_con_init()
{
	gfx_con.gfx_ctxt = &gfx_ctxt;
	gfx_con.fntsz = 16;
	gfx_con.x = 0;
	gfx_con.y = 0;
	gfx_con.savedx = 0;
	gfx_con.savedy = 0;
	gfx_con.fgcol = TXT_CLR_DEFAULT;
	gfx_con.fillbg = 1;
	gfx_con.bgcol = TXT_CLR_BG;
	gfx_con.mute = 0;

	gfx_con_init_done = true;
}

void gfx_con_setcol(u32 fgcol, int fillbg, u32 bgcol)
{
	gfx_con.fgcol = fgcol;
	gfx_con.fillbg = fillbg;
	gfx_con.bgcol = bgcol;
}

void gfx_con_getpos(u32 *x, u32 *y)
{
	*x = gfx_con.x;
	*y = gfx_con.y;
}

static int gfx_column = 0;
void gfx_con_setpos(u32 x, u32 y)
{
	gfx_con.x = x;
	gfx_con.y = y;

	if (!x)
		gfx_column = 0;
}

void gfx_putc(char c)
{
	// Duplicate code for performance reasons.
	switch (gfx_con.fntsz)
	{
	case 16:
	default:
		if (c >= 32 && c <= 126)
		{
			u8 *cbuf = (u8 *)&_gfx_font[8 * (c - 32)];
			for (u32 i = 0; i < 16; i += 2)
			{
				u8 v = *cbuf;
				for (u32 k = 0; k < 2; k++)
				{
					u32 fb_off = gfx_con.y + i + k + (gfx_ctxt.width - gfx_con.x) * gfx_ctxt.stride;
					for (u32 j = 0; j < 16; j += 2)
					{
						for (u32 l = 0; l < 2; l++)
						{
							if (v & 1)
								gfx_ctxt.fb[fb_off - (j + l) * gfx_ctxt.stride] = gfx_con.fgcol;
							else if (gfx_con.fillbg)
								gfx_ctxt.fb[fb_off - (j + l) * gfx_ctxt.stride] = gfx_con.bgcol;
						}
						v >>= 1;
					}
					v = *cbuf;
				}
				cbuf++;
			}
			gfx_con.x += 16;
			if (gfx_con.x > gfx_ctxt.width - 16)
			{
				gfx_con.x = gfx_column;
				gfx_con.y += 16;
				if (gfx_con.y > gfx_ctxt.height - 33)
				{
					gfx_con.y = 0;

					if (!gfx_column)
						gfx_column = 1080;
					else
						gfx_column = 0;
					gfx_con.x = gfx_column;
				}
			}
		}
		else if (c == '\n')
		{
			gfx_con.x = gfx_column;
			gfx_con.y += 16;
			if (gfx_con.y > gfx_ctxt.height - 33)
			{
				gfx_con.y = 0;

				if (!gfx_column)
					gfx_column = 1080;
				else
					gfx_column = 0;
				gfx_con.x = gfx_column;
			}
		}
		break;
	case 8:
		if (c >= 32 && c <= 126)
		{
			u8 *cbuf = (u8 *)&_gfx_font[8 * (c - 32)];
			for (u32 i = 0; i < 8; i++)
			{
				u8 v = *cbuf++;
				u32 fb_off = gfx_con.y + i + (gfx_ctxt.width - gfx_con.x) * gfx_ctxt.stride;
				for (u32 j = 0; j < 8; j++)
				{
					if (v & 1)
						gfx_ctxt.fb[fb_off - (j * gfx_ctxt.stride)] = gfx_con.fgcol;
					else if (gfx_con.fillbg)
						gfx_ctxt.fb[fb_off - (j * gfx_ctxt.stride)] = gfx_con.bgcol;
					v >>= 1;
				}
			}
			gfx_con.x += 8;
			if (gfx_con.x > gfx_ctxt.width + gfx_column - 8)
			{
				gfx_con.x = gfx_column;
				gfx_con.y += 8;
				if (gfx_con.y > gfx_ctxt.height - 33)
				{
					gfx_con.y = 0;

					if (!gfx_column)
						gfx_column = 1080;
					else
						gfx_column = 0;
					gfx_con.x = gfx_column;
				}
			}
		}
		else if (c == '\n')
		{
			gfx_con.x = gfx_column;
			gfx_con.y += 8;
			if (gfx_con.y > gfx_ctxt.height - 33)
			{
				gfx_con.y = 0;

				if (!gfx_column)
					gfx_column = 1080;
				else
					gfx_column = 0;
				gfx_con.x = gfx_column;
			}
		}
		break;
	}
}

void gfx_puts(const char *s)
{
	if (!s || !gfx_con_init_done || gfx_con.mute)
		return;

	for (; *s; s++)
		gfx_putc(*s);
}

static void _gfx_putn(u32 v, int base, char fill, int fcnt)
{
	static const char digits[] = "0123456789ABCDEF";

	char *p;
	char buf[65];
	int c = fcnt;
	bool negative = false;

	if (base != 10 && base != 16)
		return;

	// Account for negative numbers.
	if (base == 10 && v & 0x80000000)
	{
		negative = true;
		v = (int)v * -1;
		c--;
	}

	p = buf + 64;
	*p = 0;
	do
	{
		c--;
		*--p = digits[v % base];
		v /= base;
	} while (v);

	if (negative)
		*--p = '-';

	if (fill != 0)
	{
		while (c > 0 && p > buf)
		{
			*--p = fill;
			c--;
		}
	}

	gfx_puts(p);
}

void gfx_put_small_sep()
{
	u8 prevFontSize = gfx_con.fntsz;
	gfx_con.fntsz = 8;
	gfx_putc('\n');
	gfx_con.fntsz = prevFontSize;
}

void gfx_put_big_sep()
{
	u8 prevFontSize = gfx_con.fntsz;
	gfx_con.fntsz = 16;
	gfx_putc('\n');
	gfx_con.fntsz = prevFontSize;
}

void gfx_printf(const char *fmt, ...)
{
	if (!gfx_con_init_done || gfx_con.mute)
		return;

	va_list ap;
	int fill, fcnt;

	va_start(ap, fmt);
	while (*fmt)
	{
		if (*fmt == '%')
		{
			fmt++;
			fill = 0;
			fcnt = 0;
			if ((*fmt >= '0' && *fmt <= '9') || *fmt == ' ')
			{
				fcnt = *fmt;
				fmt++;
				if (*fmt >= '0' && *fmt <= '9')
				{
					fill = fcnt;
					fcnt = *fmt - '0';
					fmt++;
				}
				else
				{
					fill = ' ';
					fcnt -= '0';
				}
			}
			switch(*fmt)
			{
			case 'c':
				gfx_putc(va_arg(ap, u32));
				break;
			case 's':
				gfx_puts(va_arg(ap, char *));
				break;
			case 'd':
				_gfx_putn(va_arg(ap, u32), 10, fill, fcnt);
				break;
			case 'p':
			case 'P':
			case 'x':
			case 'X':
				_gfx_putn(va_arg(ap, u32), 16, fill, fcnt);
				break;
			case 'k':
				gfx_con.fgcol = va_arg(ap, u32);
				break;
			case 'K':
				gfx_con.bgcol = va_arg(ap, u32);
				gfx_con.fillbg = 1;
				break;
			case '%':
				gfx_putc('%');
				break;
			case '\0':
				goto out;
			default:
				gfx_putc('%');
				gfx_putc(*fmt);
				break;
			}
		}
		else
			gfx_putc(*fmt);
		fmt++;
	}

	out:
	va_end(ap);
}

static void _gfx_cputs(u32 color, const char *s)
{
	gfx_con.fgcol = color;
	gfx_puts(s);
	gfx_putc('\n');
	gfx_con.fgcol = TXT_CLR_DEFAULT;
}

void gfx_wputs(const char *s) { _gfx_cputs(TXT_CLR_WARNING, s); }
void gfx_eputs(const char *s) { _gfx_cputs(TXT_CLR_ERROR,   s); }

void gfx_hexdump(u32 base, const void *buf, u32 len)
{
	if (!gfx_con_init_done || gfx_con.mute)
		return;

	u8 *buff = (u8 *)buf;

	u8 prevFontSize = gfx_con.fntsz;
	gfx_con.fntsz = 8;
	for (u32 i = 0; i < len; i++)
	{
		if (i % 0x10 == 0)
		{
			if (i != 0)
			{
				gfx_puts("| ");
				for (u32 j = 0; j < 0x10; j++)
				{
					u8 c = buff[i - 0x10 + j];
					if (c >= 32 && c <= 126)
						gfx_putc(c);
					else
						gfx_putc('.');
				}
				gfx_putc('\n');
			}
			gfx_printf("%08x: ", base + i);
		}
		gfx_printf("%02x ", buff[i]);
		if (i == len - 1)
		{
			int ln = len % 0x10 != 0;
			u32 k = 0x10 - 1;
			if (ln)
			{
				k = (len & 0xF) - 1;
				for (u32 j = 0; j < 0x10 - k; j++)
					gfx_puts("   ");
			}
			gfx_puts("| ");
			for (u32 j = 0; j < (ln ? k : k + 1); j++)
			{
				u8 c = buff[i - k + j];
				if (c >= 32 && c <= 126)
					gfx_putc(c);
				else
					gfx_putc('.');
			}
			gfx_putc('\n');
		}
	}
	gfx_putc('\n');
	gfx_con.fntsz = prevFontSize;
}

static int abs(int x)
{
	if (x < 0)
		return -x;
	return x;
}

void gfx_set_pixel(u32 x, u32 y, u32 color)
{
	gfx_ctxt.fb[y + (gfx_ctxt.width - x) * gfx_ctxt.stride] = color;
}

void gfx_line(int x0, int y0, int x1, int y1, u32 color)
{
	int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
	int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
	int err = (dx > dy ? dx : -dy) / 2, e2;

	while (1)
	{
		gfx_set_pixel(x0, y0, color);
		if (x0 == x1 && y0 == y1)
			break;
		e2 = err;
		if (e2 >-dx)
		{
			err -= dy;
			x0 += sx;
		}
		if (e2 < dy)
		{
			err += dx;
			y0 += sy;
		}
	}
}

void gfx_set_rect_grey(const u8 *buf, u32 size_x, u32 size_y, u32 pos_x, u32 pos_y)
{
	u32 pos = 0;
	for (u32 y = pos_y; y < (pos_y + size_y); y++)
	{
		for (u32 x = pos_x; x < (pos_x + size_x); x++)
		{
			memset(&gfx_ctxt.fb[x + y*gfx_ctxt.stride], buf[pos], 4);
			pos++;
		}
	}
}


void gfx_set_rect_rgb(const u8 *buf, u32 size_x, u32 size_y, u32 pos_x, u32 pos_y)
{
	u32 pos = 0;
	for (u32 y = pos_y; y < (pos_y + size_y); y++)
	{
		for (u32 x = pos_x; x < (pos_x + size_x); x++)
		{
			gfx_ctxt.fb[x + y * gfx_ctxt.stride] = buf[pos + 2] | (buf[pos + 1] << 8) | (buf[pos] << 16);
			pos+=3;
		}
	}
}

void gfx_set_rect_argb(const u32 *buf, u32 size_x, u32 size_y, u32 pos_x, u32 pos_y)
{
	u32 *ptr = (u32 *)buf;
	for (u32 y = pos_y; y < (pos_y + size_y); y++)
		for (u32 x = pos_x; x < (pos_x + size_x); x++)
			gfx_ctxt.fb[x + y * gfx_ctxt.stride] = *ptr++;
}
/*
void gfx_render_bmp_argb(const u32 *buf, u32 size_x, u32 size_y, u32 pos_x, u32 pos_y)
{
	for (u32 y = pos_y; y < (pos_y + size_y); y++)
	{
		for (u32 x = pos_x; x < (pos_x + size_x); x++)
			gfx_ctxt.fb[x + y * gfx_ctxt.stride] = buf[(size_y + pos_y - 1 - y ) * size_x + x - pos_x];
	}
}
*/
void gfx_render_bmp_argb(const u32 *buf, u32 size_x, u32 size_y, u32 pos_x, u32 pos_y)
{
    gfx_render_bmp_argb_transparent(buf, size_x, size_y, pos_x, pos_y, TRANSPARENT_COLOR);
}

void gfx_render_bmp_argb_transparent(const u32 *buf, u32 size_x, u32 size_y, u32 pos_x, u32 pos_y, u32 transparent_color)
{
    for (u32 y = pos_y; y < (pos_y + size_y); y++)
    {
        for (u32 x = pos_x; x < (pos_x + size_x); x++) 
        {
            u32 render_color = buf[(size_y + pos_y - 1 - y) * size_x + x - pos_x];
        
            if (render_color != transparent_color)
                gfx_set_pixel(x, y, render_color);
        }
    }
}


void gfx_render_bmp_arg_file(char *path, u32 x, u32 y, u32 width, u32 height)
{
    u8 *bitmap = (u8 *)sd_file_read2(path);
    gfx_render_bmp_arg_bitmap(bitmap, x, y, width, height);
}

void gfx_render_bmp_arg_bitmap_transparent(u8 *bitmap, u32 x, u32 y, u32 width, u32 height, u32 transparent_color)
{
    bmp_data_t bmp_data;
    u8 *image = NULL;
    bool image_found = false;

    if (bitmap != NULL)
    {
        // Get values manually to avoid unaligned access.
        bmp_data.size = bitmap[2] | bitmap[3] << 8 |
                        bitmap[4] << 16 | bitmap[5] << 24;
        bmp_data.offset = bitmap[10] | bitmap[11] << 8 |
                          bitmap[12] << 16 | bitmap[13] << 24;
        bmp_data.size_x = bitmap[18] | bitmap[19] << 8 |
                          bitmap[20] << 16 | bitmap[21] << 24;
        bmp_data.size_y = bitmap[22] | bitmap[23] << 8 |
                          bitmap[24] << 16 | bitmap[25] << 24;
        // Sanity check.
        if (bitmap[0] == 'B' &&
            bitmap[1] == 'M' &&
            bitmap[28] == 32 && //
            bmp_data.size_x <= width &&
            bmp_data.size_y <= height)
        {
            if ((bmp_data.size - bmp_data.offset) <= 0x400000)
            {
                // Avoid unaligned access from BM 2-byte MAGIC and remove header.
                image = (u8 *)malloc(0x400000);
                memcpy(image, bitmap + bmp_data.offset, bmp_data.size - bmp_data.offset);
                bmp_data.pos_x = (width - bmp_data.size_x) >> 1;
                bmp_data.pos_y = (height - bmp_data.size_y) >> 1;

                // Get background color from 1st pixel.
				if (bmp_data.size_x < width || bmp_data.size_y < height)
                    gfx_clear_color(*(u32 *)image);
                    
                image_found = true;
            }
        }
    }
    if (image_found)
    {
        gfx_render_bmp_argb_transparent((u32 *)image, bmp_data.size_x, bmp_data.size_y,
                            bmp_data.pos_x + x, bmp_data.pos_y + y, transparent_color);
    }
    free(image);
}


void gfx_render_bmp_arg_bitmap(u8 *bitmap, u32 x, u32 y, u32 width, u32 height)
{
    gfx_render_bmp_arg_bitmap_transparent(bitmap, x, y, width, height, TRANSPARENT_COLOR);
}

void gfx_render_splash(u8 *bitmap)
{
    bmp_data_t bmp_data;
    u8 *image = NULL;
    bool image_found = false;

    if (bitmap != NULL)
    {
        // Get values manually to avoid unaligned access.
        bmp_data.size = bitmap[2] | bitmap[3] << 8 |
                        bitmap[4] << 16 | bitmap[5] << 24;
        bmp_data.offset = bitmap[10] | bitmap[11] << 8 |
                          bitmap[12] << 16 | bitmap[13] << 24;
        bmp_data.size_x = bitmap[18] | bitmap[19] << 8 |
                          bitmap[20] << 16 | bitmap[21] << 24;
        bmp_data.size_y = bitmap[22] | bitmap[23] << 8 |
                          bitmap[24] << 16 | bitmap[25] << 24;
        // Sanity check.
        if (bitmap[0] == 'B' &&
            bitmap[1] == 'M' &&
            bitmap[28] == 32 && //
            bmp_data.size_x <= gfx_ctxt.height &&
            bmp_data.size_y <= gfx_ctxt.width)
        {
            if ((bmp_data.size - bmp_data.offset) <= 0x400000)
            {
                // Avoid unaligned access from BM 2-byte MAGIC and remove header.
                image = (u8 *)malloc(0x400000);
                memcpy(image, bitmap + bmp_data.offset, bmp_data.size - bmp_data.offset);
                bmp_data.pos_x = (gfx_ctxt.height - bmp_data.size_x) >> 1;
                bmp_data.pos_y = (gfx_ctxt.width - bmp_data.size_y) >> 1;
/*
                // Get background color from 1st pixel.
				if (bmp_data.size_x < gfx_ctxt.height || bmp_data.size_y < gfx_ctxt.width)
                    gfx_clear_color(*(u32 *)image);
*/                    
                image_found = true;
            }
        }
    }
    if (image_found)
    {
        u32* buf = (u32*)image;
        for (u32 y = bmp_data.pos_y; y < (bmp_data.pos_y + bmp_data.size_y); y++)
        {
            for (u32 x =  bmp_data.pos_x; x < (bmp_data.pos_x + bmp_data.size_x); x++)
	            gfx_ctxt.fb[x + y * gfx_ctxt.stride] = buf[(bmp_data.size_y + bmp_data.pos_y - 1 - y ) * bmp_data.size_x + x - bmp_data.pos_x];        
        }

    }
    free(image);
}
