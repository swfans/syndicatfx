/******************************************************************************/
// SyndicatFX, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file guitext.c
 *     GUI text for menu and in-game display.
 * @par Purpose:
 *     Implement functions for handling text for game GUI, both in-game
 *     and in menu.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     22 Apr 2023 - 22 Oct 2025
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "guitext.h"

#include "bffile.h"
#include "bfmemory.h"
#include "bfmemut.h"

#include "applog.h"
#include "game_data.h"
/******************************************************************************/
char *gui_strings_data;
char *gui_strings_data_end;

/******************************************************************************/

static void read_strings_file(const char *datfname, char **strings_data, char **strings_data_end)
{
    int text_len;

    text_len = LbFileLength(datfname);
    if (text_len > 0) {
        (*strings_data) = (char *)LbMemoryAlloc(text_len);
        (*strings_data)[text_len - 1] = '\0';
    } else {
        (*strings_data) = NULL;
    }
    if ((*strings_data) != NULL) {
        (*strings_data_end) = (*strings_data) + text_len;
        LbFileLoadAt(datfname, *strings_data);
    } else {
        (*strings_data_end) = (*strings_data);
        LOGERR("Failed checking strings file '%s', size=%d", datfname, text_len);
    }
}

void read_gui_strings_file(void)
{
    char datfname[DISKPATH_SIZE];

    snprintf(datfname, DISKPATH_SIZE, "language/%s/guitext.dat", language_3str);

    read_strings_file(datfname, &gui_strings_data, &gui_strings_data_end);
}

static TbBool create_strings_list(char **strings, char *strings_data, char *strings_data_end)
{
    int text_idx;
    char *text_ptr;
    char **text_arr;
    text_arr = strings;
    text_idx = STRINGS_MAX;
    text_ptr = strings_data;
    while (text_idx >= 0)
    {
        if (text_ptr >= strings_data_end) {
            break;
        }
        *text_arr = text_ptr;
        text_arr++;
        //TODO store each string one time instead of 3
        if (text_arr < &strings[270*3]) {
            *text_arr = text_ptr;
            text_arr++;
            *text_arr = text_ptr;
            text_arr++;
        }
        char chr_prev;
        do {
            chr_prev = *text_ptr;
            text_ptr++;
        } while ((chr_prev != '\0') && (text_ptr < strings_data_end));
        text_idx--;
    }
    if (text_idx < 0) {
        LOGERR("Overflow on listing text strings; something got corrupted (%d bad writes)", -text_idx);
        return false;
    }
    if (text_idx >= STRINGS_MAX) {
        LOGERR("Listing strings found no text entries");
        return false;
    }
    LOGSYNC("Listed text strings (%d entries)", STRINGS_MAX - text_idx);
    return true;
}

TbBool create_gui_strings_list(void)
{
    return create_strings_list(gui_strings, gui_strings_data, gui_strings_data_end);
}

/******************************************************************************/
