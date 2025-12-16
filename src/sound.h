/******************************************************************************/
// SyndicatFX, source port of the classic game from Bullfrog.
/******************************************************************************/
/** @file sound.h
 *     Header file for sound.c.
 * @par Purpose:
 *     Sound samples playback wrappers.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     10 Oct 2012 - 13 Oct 2021
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef SOUND_H
#define SOUND_H

#include <stdbool.h>
#include <stdint.h>

#include "bftypes.h"
#include "bfaudio.h"
#include "mssal.h"
#include "timer.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#pragma pack(1)

struct BFSample {
  int field_0;
  int field_4;
  int field_8;
  short field_C;
  /** Pointer to start of data
   *  When in TAB file, this field contains position within DAT of the data.
   */
  int data_shifted;
  /** Data start, but shifted 12 bits left.
   *  When in TAB file, value is irrelevant - re-computed on sound bank setup.
   */
  void *data_start;
  short field_16;
  int field_18;
  short field_1C;
  /** Priority of the sample. During playback with low amount of channels,
   * samples with higher priority will discard lower priority ones and
   * prevent them from playing until finished. Priority of 0 means do not
   * bother playing at all.
   */
  ubyte field_1E;
  /** Allow to finish previous sample.
   * If another sample with the same priority is currently playing, switch
   * to his one only after the previous finishes playing. TODO: verify.
   */
  ubyte field_1F;
};

struct BFSampleStatus {
  ubyte field_0;
  ubyte field_1;
};

#pragma pack()
/******************************************************************************/

extern short startscr_samplevol;
extern short startscr_midivol;
extern short startscr_cdvolume;

extern ushort sndcard_irq;
extern ushort sndcard_dma;
extern ushort sndcard_ioaddr;

extern AudioInitOptions audOpts;

extern struct BFSample *smptable;
extern struct BFSample *smptable_end;
extern ubyte *smpdata;
extern struct BFSampleStatus sample_status[256];

void init_audio(void);
void audio_options_set_default(void);

int init_sound(ushort sc_irq, ushort sc_dma, ushort sc_ioaddr);
int InitMIDI(const char *bank_fname, char *drv_fname,
  ushort sc_irq, ushort sc_dma, ushort sc_ioaddr);
void ShutdownMIDI(void);
void BFMidiStartMusic(ushort song_no);
void BFMidiStopMusic(void);
void BFMidiPauseSong(void);
void BFMidiResumeSong(void);
bool BFMidiIsMusicPlaying(void);

int LoadMusic(ushort bankNo);

void sound_bank_setup(void);
void BFPlaySample(ubyte smp_id);
void BFSonundUnkn1(void);
void ClearBFSampleStatus(void);
void SetBFSampleStatus(ubyte smp_id, ubyte stat);
void BFSoundPause(void);
void BFSoundResume(void);

/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif // SOUND_H
