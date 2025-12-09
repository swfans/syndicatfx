/******************************************************************************/
// SyndicatFX, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file display.c
 *     Display and graphics mode related function.
 * @par Purpose:
 *     Wrappers for bflibrary functions and other helpers related to display.
 * @par Comment:
 *     Extended wrappers for bflibrary functionalities.
 * @author   Tomasz Lis
 * @date     12 Nov 2008 - 25 May 2022
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "bftext.h"
#include "bfmemory.h"
#include "bfmemut.h"
#include "bfmouse.h"
#include "bfpalette.h"
#include "bfscreen.h"
#include "bfscrsurf.h"
#include "bfsprite.h"

#include "applog.h"
#include "display.h"
#include "util.h"

/******************************************************************************/

ubyte *VGABuffer = NULL;

/******************************************************************************/

#if defined(WIN32)

const char * AppResourceMapping(short index)
{
    switch (index)
    {
    case 1:
        return "A";
        //return MAKEINTRESOURCE(110); -- may work for other resource compilers
    default:
        return NULL;
    }
}

#else

const char * AppResourceMapping(short index)
{
    switch (index)
    {
    case 1:
        return "syndicatfx_icon64.png";
    default:
        return NULL;
    }
}

#endif

TbResult AppScreenSetup(TbScreenMode mode)
{
    TbScreenModeInfo *mdinfo;
    TbResult ret;
    ushort ratio_mul;

    ratio_mul = 1;
    // Mapping of video modes
    if (mode == 19)
        mode = Lb_SCREEN_MODE_320_200_8;
    else if (mode == 18)
        mode = Lb_SCREEN_MODE_640_480_8;
    lbDisplay.WScreen = VGABuffer + 640*480;

    mdinfo = LbScreenGetModeInfo(mode);
    LOGSYNC("Entering mode %d, %dx%d", (int)mode, (int)mdinfo->Width, (int)mdinfo->Height);
    ret = LbScreenSetup(mode, mdinfo->Width, mdinfo->Height, GraphicsPalette);
    // This app requires mouse window larger than screen resolution
    // For the ration change to have any effect, the project nees to be
    // built with `--enable-lb-mouse-move-ratio`
    LbMouseChangeMoveRatio(ratio_mul << 8, ratio_mul << 8);
    LbMouseSetWindow(0, 0, lbDisplay.GraphicsScreenWidth * ratio_mul,
      lbDisplay.GraphicsScreenHeight * ratio_mul);
    return ret;
}

void display_set_full_screen(bool full_screen)
{
    int i;

    if (lbScreenSurface != NULL)
        return;

    for (i = 1; i < LB_MAX_SCREEN_MODES_COUNT; ++i)
    {
        TbScreenModeInfo *mdinfo;

        mdinfo = LbScreenGetModeInfo(i);
        if (mdinfo->Width == 0) break;
        if (full_screen) {
            mdinfo->VideoMode &= ~Lb_VF_WINDOWED;
        } else {
            mdinfo->VideoMode |= Lb_VF_WINDOWED;
        }
    }
}

void display_set_lowres_stretch(bool stretch)
{
    if (stretch)
        LbScreenSetMinScreenSurfaceDimension(400);
    else
        LbScreenSetMinScreenSurfaceDimension(1);
}

void display_create_vga_buffer(void)
{
    // First for 4bpp buffer, second for 8BPP WScreen buffer
    VGABuffer = LbMemoryAlloc(640*480 * 2 + 16);
}

void display_free_vga_buffer(void)
{
    LbMemoryFree(VGABuffer);
    VGABuffer = NULL;
}

void display_lock(void)
{
    LbScreenLock();
}

void display_unlock(void)
{
    LbScreenUnlock();
}

void update_vscreen_whole_vres16(void)
{
    const ubyte *i;
    ubyte *o;
    int n, k;

    LbMemorySet(VScreen, 0, 640*480);

    i = WScreen + 0 * 400*80;
    o = VScreen;
    for (k = 400; k > 0; k--)
    {
        for (n = 80; n > 0; n--)
        {
            o[7] |= ((*i) >> 0) & 0x01;
            o[6] |= ((*i) >> 1) & 0x01;
            o[5] |= ((*i) >> 2) & 0x01;
            o[4] |= ((*i) >> 3) & 0x01;
            o[3] |= ((*i) >> 4) & 0x01;
            o[2] |= ((*i) >> 5) & 0x01;
            o[1] |= ((*i) >> 6) & 0x01;
            o[0] |= ((*i) >> 7) & 0x01;
            o += 8;
            i++;
        }
    }

    i = WScreen + 1 * 400*80;
    o = VScreen;
    for (k = 400; k > 0; k--)
    {
        for (n = 80; n > 0; n--)
        {
            o[7] |= ((*i) << 1) & 0x02;
            o[6] |= ((*i) >> 0) & 0x02;
            o[5] |= ((*i) >> 1) & 0x02;
            o[4] |= ((*i) >> 2) & 0x02;
            o[3] |= ((*i) >> 3) & 0x02;
            o[2] |= ((*i) >> 4) & 0x02;
            o[1] |= ((*i) >> 5) & 0x02;
            o[0] |= ((*i) >> 6) & 0x02;
            o += 8;
            i++;
        }
    }

    i = WScreen + 2 * 400*80;
    o = VScreen;
    for (k = 400; k > 0; k--)
    {
        for (n = 80; n > 0; n--)
        {
            o[7] |= ((*i) << 2) & 0x04;
            o[6] |= ((*i) << 1) & 0x04;
            o[5] |= ((*i) >> 0) & 0x04;
            o[4] |= ((*i) >> 1) & 0x04;
            o[3] |= ((*i) >> 2) & 0x04;
            o[2] |= ((*i) >> 3) & 0x04;
            o[1] |= ((*i) >> 4) & 0x04;
            o[0] |= ((*i) >> 5) & 0x04;
            o += 8;
            i++;
        }
    }

    i = WScreen + 3 * 400*80;
    o = VScreen;
    for (k = 400; k > 0; k--)
    {
        for (n = 80; n > 0; n--)
        {
            o[7] |= ((*i) << 3) & 0x08;
            o[6] |= ((*i) << 2) & 0x08;
            o[5] |= ((*i) << 1) & 0x08;
            o[4] |= ((*i) >> 0) & 0x08;
            o[3] |= ((*i) >> 1) & 0x08;
            o[2] |= ((*i) >> 2) & 0x08;
            o[1] |= ((*i) >> 3) & 0x08;
            o[0] |= ((*i) >> 4) & 0x08;
            o += 8;
            i++;
        }
    }
}

void blit_wscreen_vres16(const ubyte *buf)
{
    LbMemoryCopy(lbDisplay.WScreen, buf, 640*480);
}

void blit_wscreen_mcga(const ubyte *buf)
{
    LbMemoryCopy(lbDisplay.WScreen, buf, 320*200);
}

void swap_wscreen(void)
{
    TbBool was_locked;
    was_locked = LbScreenIsLocked();
    if (!was_locked)
        LbScreenLock();
    if (DrawFlags & DrwF_ScreenVres16)
        blit_wscreen_vres16(VGABuffer);
    else
        blit_wscreen_mcga(WScreen);
    LbScreenUnlock();
    LbScreenSwap();
    if ( was_locked )
    {
      while ( LbScreenLock() != Lb_SUCCESS )
        ;
    }
}

TbResult ApSpriteDrawMCGA(long x, long y, const TbSprite *spr)
{
    TbSprite loc_spr;
    void *scr_bkp;
    TbResult ret;
    scr_bkp = lbDisplay.WScreen;
    lbDisplay.WScreen = WScreen;
    loc_spr.SWidth = spr->SWidth/2;
    loc_spr.SHeight = spr->SHeight/2;
    loc_spr.Data = spr->Data;
    ret = LbSpriteDraw(x/2, y/2, &loc_spr);
    lbDisplay.WScreen = scr_bkp;
    return ret;
}


/******************************************************************************/
