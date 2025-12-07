/******************************************************************************/
// SyndicatFX, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file game_data.c
 *     Memory managing routines.
 * @par Purpose:
 *     Structures and functions defining data used by the game.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     10 Feb 2020 - 16 Oct 2020
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "game_data.h"

#include <assert.h>
#include <string.h>
#include "bftypes.h"
#include "bffile.h"
#include "bfdir.h"
#include "bffnuniq.h"
#include "applog.h"
#include "dos.h"
#include "osunix.h"
#include "oswindws.h"

#include "guitext.h"
#include "player.h"
#include "research.h"
#include "weapon.h"
#include "wrcountry.h"

/******************************************************************************/

char language_3str[4];

static char data_path_user[DISKPATH_SIZE] = "";
static char data_path_hdd[DISKPATH_SIZE] = "";
static char game_dir_savegame[] = "save";
static char game_dir_screenshots[] = "screenshots";

/******************************************************************************/

const char *GetDirectoryUser(void)
{
    if (data_path_user[0] == '\0')
    {
        if (!sys_get_user_path(data_path_user, sizeof(data_path_user)))
        {
            snprintf(data_path_user, sizeof(data_path_user), ".");
        }
        LOGDBG("Dir for user files '%s'", data_path_user);
        //char path_create[DISKPATH_SIZE];
        //snprintf(path_create, sizeof(path_create), "%s/%s", data_path_user, game_dir_savegame);
        LbDirectoryMake(game_dir_savegame, true);
        //snprintf(path_create, sizeof(path_create), "%s/%s", data_path_user, game_dir_screenshots);
        LbDirectoryMake(game_dir_screenshots, true);
    }
    return data_path_user;
}

const char *GetDirectoryHdd(void)
{
    if (data_path_hdd[0] == '\0')
    {
        if (!sys_get_data_path(data_path_hdd, sizeof(data_path_hdd)))
        {
            snprintf(data_path_hdd, sizeof(data_path_hdd), "%s", ".");
        }
        LOGDBG("Dir with HDD data '%s'",data_path_hdd);
    }
    return data_path_hdd;
}


static void replace_backslash_with_fs_separator(char *path)
{
    size_t n;
    for (n = 0; path[n] != '\0'; n++)
    {
#if FS_SEP == '\\'
        if (path[n] == '/')
#else
        if (path[n] == '\\')
#endif
            path[n] = FS_SEP;
    }
}

void SyndFXFileNameTransform(char *out_fname, const char *inp_fname)
{
    char fs_fname[DISKPATH_SIZE-1];
    const char *base_dir;

    base_dir = GetDirectoryHdd();

    // Switch the input folder separators to proper ones for current os
    strncpy(fs_fname, inp_fname, sizeof(fs_fname));
    replace_backslash_with_fs_separator(fs_fname);
    // Add base path only if the input one is not absolute
    if (fs_fname[0] == FS_SEP || (strlen(fs_fname) >= 2 && fs_fname[1] == ':')) {
        snprintf (out_fname, DISKPATH_SIZE, "%s", fs_fname);
    } else {
        snprintf(out_fname, DISKPATH_SIZE, "%s" FS_SEP_STR "%s", base_dir, fs_fname);
    }
}

void setup_file_names(void)
{
    lbFileNameTransform = SyndFXFileNameTransform;
    // This fills the path variable; for user, it also creates the folder
    GetDirectoryHdd();
    GetDirectoryUser();
}

void load_save_text(char *p_desc, int slot)
{
    char fname[DISKPATH_SIZE];
    TbFileHandle fh;

    p_desc[0] = '\0';
    sprintf(fname, "%s/%02d.gam", "save", slot);
    fh = LbFileOpen(fname, Lb_FILE_MODE_READ_ONLY);

    if (fh != INVALID_FILE)
    {
        LbFileRead(fh, p_desc, GAME_DESC_TEXT_MAX_LEN);
        LbFileClose(fh);
    }

    if (p_desc[0] == '\0')
        strcpy(p_desc, gui_strings[GSTR_SLOT_EMPTY + language]);
    p_desc[GAME_DESC_TEXT_MAX_LEN-1] = '\0';
}

void load_player(int slot)
{
    char fname[DISKPATH_SIZE];
    TbFileHandle fh;

    assert(sizeof(players) == 8376);
    assert(sizeof(country_states) == 500);
    assert(sizeof(struct EquipmentInfo) * WEP_TYPES_COUNT == 10020);
    assert(sizeof(struct PartsInfo) * MOD_TYPES_COUNT == 8838);

    sprintf(fname, "%s/%02d.gam", "save", slot);
    fh = LbFileOpen(fname, Lb_FILE_MODE_READ_ONLY);
    if (fh == INVALID_FILE)
        return;

    LbFileRead(fh, players, GAME_DESC_TEXT_MAX_LEN); // skip header
    LbFileRead(fh, players, sizeof(players));
    LbFileRead(fh, country_states, sizeof(country_states));
    LbFileRead(fh, weapons, sizeof(struct EquipmentInfo) * WEP_TYPES_COUNT);
    LbFileRead(fh, cybmods, sizeof(struct PartsInfo) * MOD_TYPES_COUNT);
    LbFileRead(fh, &research, sizeof(struct Research));
    LbFileRead(fh, selected_team, sizeof(selected_team));

    LbFileClose(fh);
}

TbBool save_player(char *p_desc, int slot)
{
    char fname[DISKPATH_SIZE];
    uint creds;
    TbFileHandle fh;

    creds = players[Network__Slot].Credits;
    if (creds > 30000)
        players[Network__Slot].Credits -= 10 * creds / 100;

    sprintf(fname, "%s/%02d.gam", "save", slot);
    fh = LbFileOpen(fname, Lb_FILE_MODE_NEW);
    if (fh == INVALID_FILE)
        return false;

    LbFileWrite(fh, p_desc, GAME_DESC_TEXT_MAX_LEN);
    LbFileWrite(fh, players, sizeof(players));
    LbFileWrite(fh, country_states, sizeof(country_states));
    LbFileWrite(fh, weapons, sizeof(struct EquipmentInfo) * WEP_TYPES_COUNT);
    LbFileWrite(fh, cybmods, sizeof(struct PartsInfo) * MOD_TYPES_COUNT);
    LbFileWrite(fh, &research, sizeof(struct Research));
    LbFileWrite(fh, selected_team, sizeof(selected_team));

    LbFileClose(fh);
    return true;
}

/******************************************************************************/
