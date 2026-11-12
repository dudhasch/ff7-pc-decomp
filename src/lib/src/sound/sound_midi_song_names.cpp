// src/lib/src/sound/sound_midi_song_names.cpp
// Original: unknown (no retail __FILE__ path names this TU)
// Contents: Returns the song-name string for a MIDI song index.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/types/sound.h"
#include <windows.h>
#include <mmsystem.h>
#include <string.h>
#include <stdio.h>
#include "lib/api/sound.h"
extern "C" {

#pragma intrinsic(memcpy)

// clang-format off
// GLOBAL: FF7 0x009684c8
char *g_SoundMidiSongNames[99] = {
    // STRING: FF7 0x00968654
    "NONE",
    // STRING: FF7 0x0096865c
    "OA",
    // STRING: FF7 0x00968660
    "OB",
    // STRING: FF7 0x00968664
    "DUN2",
    // STRING: FF7 0x0096866c
    "GUITAR2",
    // STRING: FF7 0x00968674
    "FANFARE",
    // STRING: FF7 0x0096867c
    "MAKORO",
    // STRING: FF7 0x00968684
    "BAT",
    // STRING: FF7 0x00968688
    "FIDDLE",
    // STRING: FF7 0x00968690
    "KURAI",
    // STRING: FF7 0x00968698
    "CHU",
    // STRING: FF7 0x0096869c
    "KETC",
    // STRING: FF7 0x009686a4
    "EARIS",
    // STRING: FF7 0x009686ac
    "TA",
    // STRING: FF7 0x009686b0
    "TB",
    // STRING: FF7 0x009686b4
    "SATO",
    // STRING: FF7 0x009686bc
    "PARADE",
    // STRING: FF7 0x009686c4
    "COMICAL",
    // STRING: FF7 0x009686cc
    "YUME",
    // STRING: FF7 0x009686d4
    "MATI",
    // STRING: FF7 0x009686dc
    "SIDO",
    // STRING: FF7 0x009686e4
    "SIERA",
    // STRING: FF7 0x009686ec
    "WALZ",
    // STRING: FF7 0x009686f4
    "CORNEO",
    // STRING: FF7 0x009686fc
    "HORROR",
    // STRING: FF7 0x00968704
    "CANYON",
    // STRING: FF7 0x0096870c
    "RED",
    // STRING: FF7 0x00968710
    "SETO",
    // STRING: FF7 0x00968718
    "AYASI",
    // STRING: FF7 0x00968720
    "SINRA",
    // STRING: FF7 0x00968728
    "SINRASLO",
    // STRING: FF7 0x00968734
    "DOKUBO",
    // STRING: FF7 0x0096873c
    "BOKUJO",
    // STRING: FF7 0x00968744
    "TM",
    // STRING: FF7 0x00968748
    "TIFA",
    // STRING: FF7 0x00968750
    "COSTA",
    // STRING: FF7 0x00968758
    "ROCKET",
    // STRING: FF7 0x00968760
    "EARISLO",
    // STRING: FF7 0x00968768
    "CHASE",
    // STRING: FF7 0x00968770
    "RUKEI",
    // STRING: FF7 0x00968778
    "CEPHIROS",
    // STRING: FF7 0x00968784
    "BARRET",
    // STRING: FF7 0x0096878c
    "COREL",
    // STRING: FF7 0x00968794
    "BOO",
    // STRING: FF7 0x00968798
    "ELEC",
    // STRING: FF7 0x009687a0
    "RHYTHM",
    // STRING: FF7 0x009687a8
    "FAN2",
    // STRING: FF7 0x009687b0
    "HIKU",
    // STRING: FF7 0x009687b8
    "CANNON",
    // STRING: FF7 0x009687c0
    "DATE",
    // STRING: FF7 0x009687c8
    "CINTRO",
    // STRING: FF7 0x009687d0
    "CINCO",
    // STRING: FF7 0x009687d8
    "CHU2",
    // STRING: FF7 0x009687e0
    "YUFI",
    // STRING: FF7 0x009687e8
    "ASERI",
    // STRING: FF7 0x009687f0
    "GOLD1",
    // STRING: FF7 0x009687f8
    "MURA1",
    // STRING: FF7 0x00968800
    "YADO",
    // STRING: FF7 0x00968808
    "OVER2",
    // STRING: FF7 0x00968810
    "CRWIN",
    // STRING: FF7 0x00968818
    "CRLOST",
    // STRING: FF7 0x00968820
    "ODDS",
    // STRING: FF7 0x00968828
    "GEKI",
    // STRING: FF7 0x00968830
    "JUNON",
    // STRING: FF7 0x00968838
    "TENDER",
    // STRING: FF7 0x00968840
    "WIND",
    // STRING: FF7 0x00968848
    "VINCENT",
    // STRING: FF7 0x00968850
    "BEE",
    // STRING: FF7 0x00968854
    "JUKAI",
    // STRING: FF7 0x0096885c
    "SADBAR",
    // STRING: FF7 0x00968864
    "ASERI2",
    // STRING: FF7 0x0096886c
    "KITA",
    // STRING: FF7 0x00968874
    "SID2",
    // STRING: FF7 0x0096887c
    "SADSID",
    // STRING: FF7 0x00968884
    "ISEKI",
    // STRING: FF7 0x0096888c
    "HEN",
    // STRING: FF7 0x00968890
    "UTAI",
    // STRING: FF7 0x00968898
    "SNOW",
    // STRING: FF7 0x009688a0
    "YUFI2",
    // STRING: FF7 0x009688a8
    "MEKYU",
    // STRING: FF7 0x009688b0
    "CONDOR",
    // STRING: FF7 0x009688b8
    "LB2",
    // STRING: FF7 0x009688bc
    "GUN",
    // STRING: FF7 0x009688c0
    "WEAPON",
    // STRING: FF7 0x009688c8
    "PJ",
    // STRING: FF7 0x009688cc
    "SEA",
    // STRING: FF7 0x009688d0
    "LD",
    // STRING: FF7 0x009688d4
    "LB1",
    // STRING: FF7 0x009688d8
    "SENSUI",
    // STRING: FF7 0x009688e0
    "RO",
    // STRING: FF7 0x009688e4
    "JYRO",
    // STRING: FF7 0x009688ec
    "NOINTRO",
    // STRING: FF7 0x009688f4
    "RIKU",
    // STRING: FF7 0x009688fc
    "SI",
    // STRING: FF7 0x00968900
    "MOGU",
    // STRING: FF7 0x00968908
    "PRE",
    // STRING: FF7 0x0096890c
    "FIN",
    // STRING: FF7 0x00968910
    "HEART",
    // STRING: FF7 0x00968918
    "ROLL"};
// clang-format on
#pragma intrinsic(strcpy, strcat, memset, strcmp)

/*
 * Returns the song name at index when index is at most 98; returns zero for larger
 * indices. Negative indices are not rejected.
 */
// FUNCTION: FF7 0x0074a0d0
char *__cdecl MidiGetSongName(int index)
{
    if (index <= 98) {
        return g_SoundMidiSongNames[index];
    } else {
        return 0;
    }
}
} /* extern "C" */
