/******************************************************************************/
// SyndicatFX, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file mouse.h
 *     Header file for mouse.c.
 * @par Purpose:
 *     Handling mouse position and mouse pointer sprites.
 * @par Comment:
 *     Functions which wrap bflibrary routines for higher level mouse functions.
 * @author   Tomasz Lis
 * @date     10 Oct 2012 - 11 Oct 2024
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef MOUSE_H
#define MOUSE_H

#include "bftypes.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

struct TbPoint;

#pragma pack()
/******************************************************************************/
extern ushort MouseSwap;
extern int MouseOldW;

void mouse_update_scaled_coords(void);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif // MOUSE_H
