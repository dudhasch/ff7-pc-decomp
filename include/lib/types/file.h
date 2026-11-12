/* include/lib/types/file.h: the types lib/file TUs share.
 * One definition per type. */
#ifndef LIB_TYPES_FILE_H
#define LIB_TYPES_FILE_H


/* Request block for OpenFileEx: mode, flags, LGP archive id and a callback. */
typedef struct FileOpenRequest {
    int mode;
    int fromArchive;
    int archiveId;
    int callback;
} FileOpenRequest;

/* An open file: its name, the C descriptor (or the LGP entry offset) and a copy of the request it was opened with. */
typedef struct FileHandle {
    char *name;
    int handle;
    FileOpenRequest request;
} FileHandle;

/* Directory listing from BuildDirectoryListing: entry count and 32-byte entry array. */
typedef struct FileDirectoryListing {
    int entryCount;
    int entries;
} FileDirectoryListing;

/* The file descriptors of the 18 LGP archive slots, followed by the lookup directory prefix set by
 * SetLgpLookupDirPrefix. The prefix's biased endpoints (pathBuffer one byte before dirPrefix,
 * dirPrefixTail one after) overlap the last descriptor. */
#pragma pack(push, 1)
typedef struct FileLgpLookupDir {
    union {
        int fileHandles[18];
        struct {
            unsigned char pad_00[0x47];
            char pathBuffer;
        };
    };
    char dirPrefix[128];
} FileLgpLookupDir;
#pragma pack(pop)




/* LGP table-of-contents entry (24 bytes): the file name, the archive offset of the entry that
 * overwrites its last four bytes, and a conflict link (0 when the name is unique). */
typedef struct FileLgpTocEntry {
    char name[16];
    void *filePtr;
    unsigned char typeByte;
    unsigned char pad_15[0x1];
    unsigned short conflictLink;
} FileLgpTocEntry;

/* Table-of-contents block of one open LGP archive: entry array pointer and entry count. */
typedef struct FileLgpTocBlock {
    FileLgpTocEntry *entries;
    int entryCount;
} FileLgpTocBlock;

/* One cell of an LGP lookup table: the 1-based index of the first TOC entry and the entry count
 * for the names whose first two characters pick the cell. A table is 30 rows of 30 cells. */
typedef struct FileLgpLookupCell {
    unsigned short first;
    unsigned short count;
} FileLgpLookupCell;
typedef FileLgpLookupCell FileLgpLookupRow[30];

/* A directory a conflicting LGP name can live in (0x82 bytes): the directory and the TOC index. */
#pragma pack(push, 1)
typedef struct FileLgpConflictDir {
    char name[128];
    unsigned short tocIndex;
} FileLgpConflictDir;
#pragma pack(pop)

/* One name-conflict record in g_LgpConflictTable (8 bytes; 1000 records per archive slot):
 * directory count (read as a signed byte in loop bounds and loaded via FileReadU16) and heap array. */
typedef struct FileLgpConflictRecord {
    union {
        char count;
        unsigned short countWord;
    };
    unsigned char pad_02[0x2];
    FileLgpConflictDir *dirs;
} FileLgpConflictRecord;



/* File descriptor record passed to OpenFileFromRecord / LoadFileFromRecord. */
typedef struct FileRecord {
    int kind;
    char *name;
    char *directory;
    char unknown[20];
} FileRecord;

#endif /* LIB_TYPES_FILE_H */
