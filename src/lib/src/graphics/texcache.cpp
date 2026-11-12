// src/lib/src/graphics/texcache.cpp
// Original: C:\lib\src\graphics\TexCache.cpp
// Contents: The texture surface cache: a std::map from a (texture header, palette) pair to the
//   DirectDraw surface made from it, with lookup, insertion, release by key, by texture and all,
//   and eviction by size. The map and list code the cache instantiates follows the cache's own
//   functions (0x0069da10..0x0069f910, markers in include/lib/texcache_templates.h).
//
// Every container function is the library's own (VC5's <map> and <list>); nothing is instantiated
// by hand and nothing is aliased. This file needs the C++ front end of VS97 SP3 (c1xx.dll
// 11.00.7303): the RTM front end rejects std::list<Cache::iterator> and compiles
// every in-class member of an instantiated template, which puts the template code out of retail's
// order. The key's first component is the pointer it is: with an integer there the library's
// _Tree::insert comes out 8 bytes longer than retail's.
/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include <map>
#include <list>
#include <ddraw.h>
#include <string.h>
#include "lib/types/graphics.h"
#include "lib/api/graphics.h"

#pragma intrinsic(memset)

// The key is the texture header and the palette the surface was made with; it orders by the
// library's operator< for pairs.
typedef std::pair<GfxTexHeader *, unsigned int> TexCacheKey;
typedef std::map<TexCacheKey, LPDIRECTDRAWSURFACE> Cache;
typedef std::list<Cache::iterator> Candidates;

// The cache is a class static data member: the compiler constructs it through a .CRT$XCU
// initializer and destroys it once at exit, guarded by bit zero of the one-byte static that
// follows the map. It is defined before the counters: the SP3 front
// end emits a static object's initializer functions at the next declaration after it, and retail
// has them ahead of the first cache function. The owning class's name is not known; VC5 orders the
// uninitialized .bss by the hash of the qualified name, and retail's map precedes
// the guard, which this name keeps (TexCache::entries would follow it).
struct TextureCache {
    static Cache entries;
};

// GLOBAL: FF7 0x00db2ce8
Cache TextureCache::entries;

// clang-format off
extern "C" {
// GLOBAL: FF7 0x00db2cfc
unsigned int g_TexCacheCount = 0;
unsigned char g_bssPad_00db2d00[4] = {0};
// GLOBAL: FF7 0x00db2d04
unsigned int g_TexCacheMaxCount = 0;
// GLOBAL: FF7 0x00db2d08
unsigned int g_TexCacheTotalBytes = 0;
// GLOBAL: FF7 0x00db2d0c
unsigned int g_TexCacheMaxBytes = 0;
}
// clang-format on

/*
 * Looks up a DirectDraw surface in the texture cache by key.
 */
// FUNCTION: FF7 0x0069cc39
LPDIRECTDRAWSURFACE __cdecl Gfx_TextureCacheLookupSurface(const TexCacheKey &key)
{
    Cache::iterator p = TextureCache::entries.find(key);
    if (p != TextureCache::entries.end())
        return p->second;
    return 0;
}

/*
 * Builds the key from its two components and looks the surface up.
 */
// FUNCTION: FF7 0x0069cd04
extern "C" LPDIRECTDRAWSURFACE __cdecl Gfx_TextureCacheFindSurface(GfxTexHeader *header, unsigned int palette)
{
    TexCacheKey key(header, palette);
    return Gfx_TextureCacheLookupSurface(key);
}

/*
 * Releases all DirectDraw surfaces and clears the texture cache.
 */
// FUNCTION: FF7 0x0069cd26
void __cdecl Gfx_TextureCacheReleaseAllSurfaces(void)
{
    unsigned int val;
    for (Cache::iterator p = TextureCache::entries.begin(); p != TextureCache::entries.end(); ++p) {
        val = p->second->Release();
    }
    TextureCache::entries.clear();
}

/*
 * Records a 128 KB texture surface in the cache and updates the high-water counters.
 */
// FUNCTION: FF7 0x0069cdd4
extern "C" bool __cdecl Gfx_TextureCacheInsertSurface(GfxTexHeader *header, unsigned int palette, LPDIRECTDRAWSURFACE texture)
{
    g_TexCacheCount++;
    g_TexCacheMaxCount = g_TexCacheMaxCount > g_TexCacheCount ? g_TexCacheMaxCount : g_TexCacheCount;
    g_TexCacheTotalBytes += 0x20000;
    g_TexCacheMaxBytes = g_TexCacheMaxBytes > g_TexCacheTotalBytes ? g_TexCacheMaxBytes : g_TexCacheTotalBytes;
    TexCacheKey p(header, palette);
    Cache::value_type q(p, texture);
    std::pair<Cache::iterator, bool> r = TextureCache::entries.insert(q);
    return true;
}

/*
 * Releases every cached surface of one texture header.
 */
// FUNCTION: FF7 0x0069ce8e
bool __cdecl Gfx_TextureCacheReleaseByTexture(GfxTexHeader *header)
{
    Candidates p;
    for (Cache::iterator s = TextureCache::entries.begin(); s != TextureCache::entries.end(); ++s) {
        if (s->first.first == header)
            p.push_back(s);
    }
    for (Candidates::iterator r = p.begin(); r != p.end(); ++r) {
        unsigned int u = (*r)->second->Release();
        TextureCache::entries.erase(*r);
        g_TexCacheTotalBytes -= 0x20000;
        g_TexCacheCount--;
    }
    return true;
}

/*
 * Releases the cached surface of one key and takes it out of the accounting.
 */
// FUNCTION: FF7 0x0069d156
bool __cdecl Gfx_TextureCacheReleaseEntry(GfxTexHeader *header, unsigned int palette)
{
    TexCacheKey p(header, palette);
    Cache::iterator r = TextureCache::entries.find(p);
    if (r == TextureCache::entries.end())
        return false;
    unsigned int q = r->second->Release();
    TextureCache::entries.erase(r);
    g_TexCacheTotalBytes -= 0x20000;
    g_TexCacheCount--;
    return true;
}

/*
 * Member forwarder to Gfx_TextureCacheReleaseAllSurfaces.
 */
// FUNCTION: FF7 0x0069d269
extern "C" void __fastcall Gfx_TextureCacheReleaseAllSurfacesMember(void *self)
{
    Gfx_TextureCacheReleaseAllSurfaces();
}

/*
 * Frees cached surfaces until `needed` bytes are released. Only entries made with a palette other
 * than the one their texture has in use are candidates; no usage order is kept. The first pass
 * releases the first candidate at least `needed` bytes large; the second releases candidates in
 * cache order until their sizes add up to `needed`. Returns false when GetSurfaceDesc fails or the
 * candidates are too small.
 */
// FUNCTION: FF7 0x0069d279
bool __cdecl Gfx_TextureCacheEvictForBytes(unsigned int needed)
{
    DDSURFACEDESC t;
    // Preserve the original reversed arguments: count is zero.
    memset(&t, sizeof(t), 0);
    t.dwSize = sizeof(t);
    Candidates p;
    unsigned int q = 0;
    for (Cache::iterator s = TextureCache::entries.begin(); s != TextureCache::entries.end(); ++s) {
        GfxTexHeader *u = s->first.first;
        unsigned int v = s->first.second;
        if (v != u->row)
            p.push_back(s);
    }
    for (Candidates::iterator r = p.begin(); r != p.end(); ++r) {
        IDirectDrawSurface *surface = (*r)->second;
        // STRING: FF7 0x009121ec
        if (CheckDirectDrawResult(surface->GetSurfaceDesc(&t), "C:\\lib\\src\\graphics\\TexCache.cpp", 0xa2)) {
            unsigned int bytes = t.lPitch * t.dwHeight;
            if (bytes >= needed) {
                unsigned int references = (*r)->second->Release();
                TextureCache::entries.erase(*r);
                g_TexCacheTotalBytes -= bytes;
                g_TexCacheCount--;
                return true;
            }
        } else
            return false;
    }
    for (r = p.begin(); r != p.end(); ++r) {
        IDirectDrawSurface *surface2 = (*r)->second;
        // STRING: FF7 0x00912210
        if (CheckDirectDrawResult(surface2->GetSurfaceDesc(&t), "C:\\lib\\src\\graphics\\TexCache.cpp", 0xba)) {
            unsigned int bytes2 = t.lPitch * t.dwHeight;
            q += bytes2;
            unsigned int references2 = (*r)->second->Release();
            TextureCache::entries.erase(*r);
            g_TexCacheTotalBytes -= bytes2;
            g_TexCacheCount--;
            if (q >= needed)
                return true;
        } else
            return false;
    }
    return false;
}

// clang-format off
extern "C" unsigned char g_rawData_009121e8[4] = {0x00, 0x00, 0x50, 0x00};
// clang-format on
