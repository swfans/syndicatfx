/******************************************************************************/
// SyndicatFX, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file game.h
 *     Header file for game.c.
 * @par Purpose:
 *     Functions for high level game functionalities.
 * @par Comment:
 *     Routines on game loop level, creating eneral design of the game.
 * @author   Tomasz Lis
 * @date     12 Nov 2008 - 25 May 2022
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
extern ubyte byte_60AFC;
extern ubyte is_multiplayer_game;

bool game_initialise (void);
void game_handle_sdl_events (void);
void game_update (void);
void game_quit (void);
void host_reset(void);
void reset_input(void);
void game_transform_path (const char *file_name, char *result);
const char *game_get_data_path (void);
const char *game_get_user_path (void);

void syndicate(void);
/******************************************************************************/
#ifdef __cplusplus
};
#endif
#endif // GAME_H
