/******************************************************************************/
// SyndicatFX, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file game.c
 *     Functions for high level game functionalities.
 * @par Purpose:
 *     Routines on game loop level, creating eneral design of the game.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     12 Nov 2008 - 25 May 2022
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "bfkeybd.h"
#include "bfmouse.h"
#include "bfdata.h"
#include "bffile.h"
#include "bfmemory.h"
#include "bfmemut.h"
#include "bfpalette.h"
#include "bfscreen.h"
#include "bfsprite.h"
#include "bfwindows.h"
#include "bfaudio.h"
#include "bfmusic.h"
#include "bfsound.h"
#include "bfsvaribl.h"
#include "bfscd.h"
#include "bfutility.h"
#include "ssampply.h"

#include <SDL.h>
#include "applog.h"
#include "game_data.h"
#include "display.h"
#include "dos.h"
#include "game.h"
#include "keyboard.h"
#include "mouse.h"
#include "osunix.h"
#include "oswindws.h"
#include "player.h"
#include "sound.h"
#include "util.h"
#include "timer.h"

#define SAVEGAME_PATH "save/"

/******************************************************************************/
// ASM imports with matching call convention
void _cdecl scroll_map(short dx, short dz);
void _cdecl transfer_people_into_player(ushort plyrno);
void _cdecl set_default_player(void);
ubyte _cdecl process_day(uint a1);
ubyte _cdecl menu_select(void);
void _cdecl reset_mission_info(void);
void _cdecl initialise_player(void);
void _cdecl process_players_turn(void);
void _cdecl multi_play(void);
void _cdecl single_play(void);
void _cdecl process_computer_players(void);
void _cdecl move_it(void);
ubyte _cdecl correct_buttons(void);
void _cdecl check_for_danger(void);
void _cdecl check_end_level(void);
void _cdecl swap_screen_vres16(void);
void _cdecl copy_back(void);
void _cdecl draw_mapwho(void);
void _cdecl draw_panel(void);
void _cdecl click_map(void);
void _cdecl free_map_level(void);
void _cdecl level_finished(void);


bool
game_initialise (void)
{
    TbResult ret;

    // Check for level data offsets consistency
    assert((int)((ubyte *)(level__CPObjectives) - (ubyte *)&level__Seed) == 0x9BEB2 - 0x80108);
    assert((int)((ubyte *)(level__CPObjectives + 128) - (ubyte *)&level__Seed) == 0x9C632 - 0x80108);

    ret = LbBaseInitialise();
    if (ret != Lb_SUCCESS)
    {
        LOGERR("Bullfrog Library initialization failed");
        return false;
    }

#ifdef __unix__
    /* clean up after SDL messing around where it shouldn't */
    unix_restore_signal_handlers();
#endif

    LbSetUserResourceMapping(AppResourceMapping);
    LbSetTitle(PACKAGE_NAME);
    LbSetIcon(1);

    // Make sure file names are properly converted before opening
    setup_file_names();

    return true;
}

void
game_handle_sdl_events (void)
{
    TbBool contn;
    contn = LbWindowsControl();
    if (!contn) {
        game_quit();
    }
    mouse_update_scaled_coords();
}

void
game_quit(void)
{
    host_reset();
    LbBaseReset();
    exit(0);
}

void
game_transform_path_full(const char *file_name, char *buffer, size_t size)
{
    if (strncasecmp (file_name, SAVEGAME_PATH,
             sizeof (SAVEGAME_PATH) - 1) == 0)
    {
        snprintf (buffer, size, "%s" FS_SEP_STR "%s", GetDirectoryUser(),
          file_name + sizeof (SAVEGAME_PATH) - 1);
        return;
    }

    /* abort on absolute paths */
    if (file_name[0] == '\\' || file_name[0] == '/'
        || (strlen (file_name) >= 2 && file_name[1] == ':'))
    {
        snprintf (buffer, size, "%s", file_name);
        return;
    }

    snprintf (buffer, size, "%s" FS_SEP_STR "%s", GetDirectoryHdd(), file_name);
}

void
game_transform_path(const char *file_name, char *result)
{
    game_transform_path_full (file_name, result, FILENAME_MAX);
}

static void
game_update_full(bool wait)
{
    const int max_fps = 16;
    const int32_t frame_duration = 1000 / max_fps;

    static int32_t last_frame_ticks;
    int32_t start_ticks;

    display_unlock();

    game_handle_sdl_events();

    start_ticks = SDL_GetTicks();

    if (wait && last_frame_ticks != 0)
    {
        int32_t last_frame_duration;

        last_frame_duration = (int32_t)(start_ticks - last_frame_ticks + 2);

        if (last_frame_duration < frame_duration)
        {
            int32_t total_sleep_time = frame_duration - last_frame_duration;
            const int32_t min_sleep_time = 1000 / 40;
            const int32_t max_sleep_time = 1000 / 20;

            total_sleep_time = frame_duration - last_frame_duration;

            if (total_sleep_time > 0)
            {
                float f = (float) total_sleep_time
                    * (min_sleep_time + max_sleep_time)
                    / (2 * min_sleep_time * max_sleep_time);
                int32_t base_sleep_time = (int32_t)(total_sleep_time / f + .5f);

                while (total_sleep_time > 0)
                {
                    int32_t sleep_time = min(base_sleep_time, total_sleep_time);
                    int32_t ticks = SDL_GetTicks();

                    SDL_Delay(sleep_time);

                    display_lock();
                    game_handle_sdl_events();
                    display_unlock();

                    total_sleep_time -= SDL_GetTicks() - ticks;
                }
            }
        }
    }

    last_frame_ticks = SDL_GetTicks();

    display_lock();
}

void reset_input(void)
{
    LbMouseReset();
    LbKeyboardClose();
}

void
game_update(void)
{
    game_update_full(true);
}

void host_reset(void)
{
# if defined(DOS)
    ShutdownMIDI();
    FreeSound();
#else
    FreeAudio();
#endif
    reset_input();
    LbScreenReset();
}

void nullsub_1()
{
}

/*
void set_default_player(void)
{
}

ubyte menu_select(void)
{
}


void reset_mission_info(void)
{
}

void initialise_player(void)
{
}

void process_players_turn(void)
{
}

void multi_play(void)
{
}

void single_play(void)
{
}

void process_computer_players(void)
{
}

void move_it(void)
{
}

ubyte correct_buttons(void)
{
}

void check_for_danger(void)
{
}

void check_end_level(void)
{
}

void swap_screen_vres16(void)
{
}

void copy_back(void)
{
}

void draw_mapwho(void)
{
}

void draw_panel(void)
{
}

void click_map(void)
{
}

void free_map_level(void)
{
}

void level_finished(void)
{
}
*/

void syndicate(void)
{
#if 0
    asm volatile ("call ASM_syndicate\n"
      :  : );
#endif
    set_default_player();
    while (!menu_select())
    {
        uint init_state;

        reset_mission_info();
        initialise_player();
        init_state = 0;
        while ((byte_60AFC & 1) != 0)
        {
            if (lbKeyOn[KC_F10])
                break;
            if (init_state != 0) {
                process_players_turn();
            }
            if (is_multiplayer_game)
            {
                multi_play();
            }
            else
            {
                single_play();
                process_computer_players();
            }
            move_it();
            correct_buttons();
            if (init_state > 2)
                BFSonundUnkn1();
            check_for_danger();
            process_day(1008u);
            check_end_level();
            LbScreenWaitVbi();
            nullsub_1();
            swap_screen_vres16();
            if (init_state == 2) {
                LbPaletteSet(GraphicsPalette);
            }
            scroll_map(lbDisplay__MMouseX_640, lbDisplay__MMouseY_400);
            scroll_map(lbDisplay__MMouseX_640, lbDisplay__MMouseY_400);
            copy_back();
            draw_mapwho();
            draw_panel();
            click_map();
            game_update();
            if (init_state < 3) {
                init_state++;
            }
        }
        if (GetMusicAble())
            BFMidiStopMusic();
        free_map_level();
        if (!lbKeyOn[KC_F10])
        {
            *(u32 *)&MouseSwap = 1;
            MouseOldW = 0;
            transfer_people_into_player(Network__Slot);
            level_finished();
            if (lbKeyOn[KC_F10] == 1)
                break;
        }
    }
}
/******************************************************************************/
