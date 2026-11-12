/* include/lib/enums/movie.h: the codes lib/movie compares DirectShow results with. */
#ifndef LIB_ENUMS_MOVIE_H
#define LIB_ENUMS_MOVIE_H

/* The success codes of the DirectShow stream sample calls (CompletionStatus/Update). */
enum MovieStreamStatus {
    MOVIE_STATUS_PENDING = 0x40001,         /* MS_S_PENDING: no new frame yet */
    MOVIE_STATUS_END_OF_STREAM = 0x40003    /* MS_S_ENDOFSTREAM */
};

/* E_INVALIDARG, which DrawPrimitive returns for bad parameters. */
enum MovieErrorCode {
    MOVIE_E_INVALID_ARG = (int)0x80070057
};

#endif
