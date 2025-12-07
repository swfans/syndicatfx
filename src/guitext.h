/******************************************************************************/
// SyndicatFX, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file guitext.h
 *     Header file for guitext.c.
 * @par Purpose:
 *     GUI text for menu and in-game display.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     22 Apr 2023 - 22 Oct 2025
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef GUITEXT_H
#define GUITEXT_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#define STRINGS_MAX 800

#pragma pack(1)

enum GuiStrings {
    GSTR_COUNTRY_NAMES = 0 * 3,
    GSTR_CRYO_CHAMBER = 50 * 3,
    GSTR_SLOT_EMPTY = 117 * 3,
};

#pragma pack()
/******************************************************************************/
extern char *gui_strings[STRINGS_MAX];
extern ubyte language;

/** Read file with all the language-specific texts.
 */
void read_gui_strings_file(void);
TbBool create_gui_strings_list(void);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
