/* include/ff7/enums/credits.h: named codes of ff7/credits.
 * Enums only, so any TU can include it: no types, no declarations. */
#ifndef FF7_ENUMS_CREDITS_H
#define FF7_ENUMS_CREDITS_H

/* States of g_CreditsSequenceState, which Credits_TickSequenceState switches on. The module
 * enters at g_CreditsStartSection (0, or 7 to play movie 0 first). */
enum CreditsSequenceState {
    CREDITS_SEQ_MOVIE = 0,          /* run the open movie; on stop: records, music volume 0x7F */
    CREDITS_SEQ_START_ROLL = 1,     /* music on, 3000-tick wait, fade in from opaque */
    CREDITS_SEQ_ROLL = 2,           /* draw the credit records until 0x1D ticks, then open movie 2 */
    CREDITS_SEQ_LOGO_MOVIE = 3,     /* run movie 2 with the logo faded in by its frame */
    CREDITS_SEQ_LOGO_HOLD = 4,      /* the logo for 3000 ms, then the strip, fade in */
    CREDITS_SEQ_LOGO_STRIP = 5,     /* logo and strip for 16000 ms, then fade out and music down */
    CREDITS_SEQ_FADE_OUT = 6,       /* wait for the fade; stop the music, reopen the movie */
    CREDITS_SEQ_FIRST_MOVIE = 7     /* run movie 0, then open movie 1 and go to CREDITS_SEQ_MOVIE */
};

/* g_CreditsVariant (Credits_SetVariant): which sequence the credits module plays. */
enum CreditsVariant {
    CREDITS_VARIANT_OPENING = 0,    /* GAME_MODE_CREDITS: the logo movies before the title menu */
    CREDITS_VARIANT_ENDING = 1      /* GAME_MODE_ENDING_CREDITS: the staff roll from people.bin */
};

/* g_CreditsTextPhase, the ending variant's phase, which Credits_TickTextPhase switches on. */
enum CreditsTextPhase {
    CREDITS_TEXT_PHASE_ROLL = 0,       /* Credits_DrawRecordWindow scrolls the staff roll */
    CREDITS_TEXT_PHASE_AFTER_ROLL = 1, /* the after-text quads until Credits_UpdateFadeStrips is done */
    CREDITS_TEXT_PHASE_FADE_OUT = 2,   /* the after-text quads until the fade ends, then movie 3 */
    CREDITS_TEXT_PHASE_END_MOVIE = 3   /* run movie 3, then end the module */
};

/* CreditsAnimationRecord.mode, which Credits_AdvanceAnimationRecord switches on: the directions
 * positionX and positionY move toward their targets (current8 and currentC move the other way). */
enum CreditsAnimationMode {
    CREDITS_ANIM_MODE_X_INC_Y_INC = 0,
    CREDITS_ANIM_MODE_X_DEC_Y_INC = 1,
    CREDITS_ANIM_MODE_X_INC_Y_DEC = 2,
    CREDITS_ANIM_MODE_X_DEC_Y_DEC = 3
};

/* Bytes of a credits glyph string (Credits_MeasureGlyphStringWidth, the Credits_DrawGlyphString*
 * drawers): 0..CREDITS_GLYPH_LAST are glyphs of the eight-column atlas; the others are codes. */
enum CreditsGlyphCode {
    CREDITS_GLYPH_LAST = 0x2F,
    CREDITS_GLYPH_TO_MIDDLE = 0x30, /* the pen jumps to the middle of the viewport; the measurer
                                       returns the whole viewport width */
    CREDITS_GLYPH_END = 0x7F
};

#endif
