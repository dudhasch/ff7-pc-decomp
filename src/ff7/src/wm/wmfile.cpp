// src/ff7/src/wm/wmfile.cpp
// Original: C:\FF7\src\wm\wmfile.cpp
// Contents: Loads, frees and tracks the world map's stage and leader models, their textures and
//   compressed assets, and checks whether a page is one of the animated texture pages.

/* C++ TU (retail .cpp); extern "C" keeps C symbol linkage. */
#include "lib/enums/file.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "ff7/api/battle.h"
#include "ff7/api/wm.h"
#include "lib/api/sound.h"
#include "lib/types/file.h"
#include "lib/types/polygon.h"
#include "ff7/enums/wm.h"
#include "ff7/types/wm.h"
#include "lib/enums/graphics.h"
#include "lib/types/graphics.h"
#include "lib/api/file.h"
#include "lib/api/graphics.h"
#include "lib/api/polygon.h"
#include "lib/api/mem.h"
#include "lib/pfile.h"
#include "lib/crt.h"
extern "C" {

#pragma intrinsic(memset, strcpy, strcat, strcmp)

// clang-format off
struct PolygonAnimation;
/* The world map's static tables: texture page names and origins, stage and leader model
 * definitions. World_LoadTerrainTextures indexes a page-name table by page number and hands the
 * element to Gfx_CreateRenderObject as the file name. */
// GLOBAL: FF7 0x00969ce8
char *g_WorldTexturePageNamesOverworld[282] = {
    "pond.tim", "riv_m2.tim", "was_gs.tim", "wpcltr.tim",
    "wpcltr2.tim", "bzdun.tim", "bone.tim", "bone2.tim",
    "bornwd.tim", "bridge.tim", "bridge2.tim", "cave.tim",
    "cave2.tim", "cave_s.tim", "cdl_cl2.tim", "cf01.tim",
    "clf_bgs.tim", "clf_ggl.tim", "clf_ggs.tim", "clf_l.tim",
    "clf_ld.tim", "clf_lf.tim", "clf_lg.tim", "clf_lr.tim",
    "clf_lsg.tim", "clf_r.tim", "clf_s.tim", "clf_sd.tim",
    "clf_sf.tim", "clf_sg.tim", "clf_sg2.tim", "clf_sr.tim",
    "clf_ss.tim", "clf_ssd.tim", "clf_ssw.tim", "clf_sw.tim",
    "clf_w02.tim", "clf_w03.tim", "clf_was.tim", "clfeg.tim",
    "clfegd.tim", "clftop.tim", "clftop2.tim", "cndl_cl.tim",
    "cndlf.tim", "cndlf02.tim", "comtr.tim", "cosinn.tim",
    "cosinn2.tim", "csmk.tim", "csmk2.tim", "cstds01.tim",
    "cstds02.tim", (char *)g_WorldEmptyTextureName0, "desert.tim", "desor.tim",
    (char *)g_WorldEmptyTextureNames, (char *)g_WorldEmptyTextureNames + 0x4, (char *)g_WorldEmptyTextureNames + 0x8, "dsrt_d.tim",
    "dsrt_e.tim", (char *)g_WorldEmptyTextureNames + 0xc, "elm01.tim", "elm02.tim",
    "elm_gro.tim", "elm_r.tim", "elm_r2.tim", (char *)g_WorldEmptyTextureNames + 0x10,
    "farm01.tim", "farm02.tim", "farm_g.tim", "farm_r.tim",
    "fld.tim", "fld_02.tim", "fld_s.tim", "fld_s2.tim",
    "fld_sw.tim", "fld_v.tim", "fld_vd.tim", "fld_vd2.tim",
    "fvedge.tim", "gclf_d.tim", "gclf_g.tim", "gclfwa.tim",
    "gclfwa2.tim", "gclfwag.tim", "gg_gro.tim", "gg_mts.tim",
    "ggmk.tim", "ggmt.tim", "ggmt_e.tim", "ggmt_ed.tim",
    "ggmt_eg.tim", "ggmtd.tim", "ggs_g.tim", "ggshr.tim",
    "ggshrg.tim", "gia.tim", "gia2.tim", "gia_d.tim",
    "gia_d2.tim", "gia_g.tim", "gia_g2.tim", "gmt_eda.tim",
    "gonclf.tim", "gredge.tim", "hyouga.tim", "iceclf.tim",
    "iceclfd.tim", "iceclfg.tim", "jun.tim", "jun_d.tim",
    "jun_e.tim", "jun_gro.tim", "junmk.tim", "junn01.tim",
    "junn02.tim", "junn03.tim", "junn04.tim", "jutmpl01.tim",
    "lake-e.tim", "lake_ef.tim", "lake_fl.tim", "lostclf.tim",
    "lostmt.tim", "lostmtd.tim", "lostmts.tim", "lostwd_e.tim",
    "lostwod.tim", (char *)g_WorldEmptyTextureNames + 0x14, "lstwd_e2.tim", "mzes.tim",
    "mzmt_e.tim", "mzmt_ed.tim", "mzmt_edw.tim", "mzmt_ew.tim",
    "mzmt_o.tim", "mzmt_od.tim", "mzmt_s.tim", "mzmt_sd.tim",
    "md01.tim", "md02.tim", "md03.tim", "md04.tim",
    "md05.tim", "md06.tim", "md07.tim", "md_mt.tim",
    "md_mtd.tim", "md_mts.tim", "md_snow.tim", "md_snw2.tim",
    "md_snwd.tim", "md_snwe.tim", "md_snws.tim", "md_snwt.tim",
    "md_snww.tim", "md_sw_s.tim", "md_swd2.tim", "md_swnp.tim",
    "mdsrt_e.tim", "mdsrt_ed.tim", "mdsrt_eg.tim", "midil.tim",
    "midild.tim", "mt_ewg.tim", "mt_road.tim", "mt_se.tim",
    "mt_se2.tim", "mt_sg01.tim", "mt_sg02.tim", "mt_sg03.tim",
    "mt_sg04.tim", "mtcoin.tim", "mtwas_e.tim", "mtwas_ed.tim",
    "ncol_gro.tim", "ncole01.tim", "ncole02.tim", "nivl_gro.tim",
    "nivl_mt.tim", "nivl_top.tim", "nivlr.tim", "port.tim",
    "port_d.tim", "rzclf02.tim", "rct_gro.tim", "riv_cls.tim",
    (char *)g_WorldEmptyTextureNames + 0x18, "riv_m.tim", "rivr.tim", "rivrclf.tim",
    (char *)g_WorldEmptyTextureNames + 0x1c, "rivshr.tim", "rivssr.tim", "rivstrt.tim",
    (char *)g_WorldEmptyTextureNames + 0x20, "rocet.tim", "rs_ss.tim", "sango.tim",
    "sango2.tim", "sango3.tim", "sango4.tim", "sdun.tim",
    "sdun02.tim", (char *)g_WorldEmptyTextureNames + 0x24, (char *)g_WorldEmptyTextureNames + 0x28, "shedge.tim",
    (char *)g_WorldEmptyTextureNames + 0x2c, "shol.tim", "shol_s.tim", "shor.tim",
    "shor_s.tim", "shor_s2.tim", "shor_v.tim", "silo.tim",
    "slope.tim", "snow_es.tim", "snow_es2.tim", "snow_es3.tim",
    "snw_mt.tim", "snw_mtd.tim", "snw_mte.tim", "snw_mted.tim",
    "snw_mts.tim", "snw_mts2.tim", "snwfld.tim", "snwfld_s.tim",
    "snwgra.tim", "snwhm01.tim", "snwhm02.tim", "snwods.tim",
    "snwood.tim", "snwtrk.tim", (char *)g_WorldEmptyTextureNames + 0x30, (char *)g_WorldEmptyTextureNames + 0x34,
    (char *)g_WorldEmptyTextureNames + 0x38, "stown_r.tim", "stw_gro.tim", "subrg2.tim",
    "susbrg.tim", "sw_se.tim", "swclf_l.tim", "swclf_ld.tim",
    "swclf_lg.tim", "swclf_s.tim", "swclf_sd.tim", "swclf_sg.tim",
    "swclf_wg.tim", "swfld_s2.tim", "swfld_s3.tim", "swmd_cg.tim",
    "swmd_clf.tim", (char *)g_WorldEmptyTextureNames + 0x3c, "trk.tim", "tyo_f.tim",
    "tyosnw.tim", (char *)g_WorldEmptyTextureNames + 0x40, "utai01.tim", "utai02.tim",
    "utai_gro.tim", "utaimt.tim", "utaimtd.tim", "utaimtg.tim",
    (char *)g_WorldEmptyTextureNames + 0x44, (char *)g_WorldEmptyTextureNames + 0x48, "wzshr.tim", "wzshr2.tim",
    "wzshrs.tim", "was.tim", "was_d.tim", "was_g.tim",
    "was_s.tim", "wasfld.tim", "wdedge.tim", (char *)g_WorldEmptyTextureNames + 0x4c,
    (char *)g_WorldEmptyTextureNames + 0x50, "wedged.tim", "wod-e2.tim", "wood.tim",
    "wood_d.tim", "wtrk.tim"
};
// GLOBAL: FF7 0x0096a150
char *g_WorldTextureAnimFrameNames[108] = {
    "wa1.tim", "wa2.tim", "wa3.tim", "wa4.tim",
    "wa5.tim", "wa6.tim", "wa7.tim", "wa8.tim",
    "we1.tim", "we2.tim", "we3.tim", "we4.tim",
    "we5.tim", "we6.tim", "we7.tim", "we8.tim",
    "wzs1.tim", "wzs2.tim", "wzs3.tim", "wzs4.tim",
    "wzs5.tim", "wzs6.tim", "wzs7.tim", "wzs8.tim",
    "we_s1.tim", "we_s2.tim", "we_s3.tim", "we_s4.tim",
    "we_s5.tim", "we_s6.tim", "we_s7.tim", "we_s8.tim",
    "fall1.tim", "fall2.tim", "fall3.tim", "fall4.tim",
    "fall5.tim", "fall6.tim", "fall7.tim", "fall8.tim",
    "uf1.tim", "uf2.tim", "uf3.tim", "uf4.tim",
    "lst1.tim", "lst2.tim", "lst3.tim", "lst4.tim",
    "sst1.tim", "sst2.tim", "sst3.tim", "sst4.tim",
    "rm1.tim", "rm2.tim", "rm3.tim", "rm4.tim",
    "ds1.tim", "ds2.tim", "ds3.tim", "ds4.tim",
    "sh1.tim", "sh2.tim", "sh3.tim", "sh4.tim",
    "ssee1.tim", "ssee2.tim", "ssee3.tim", "ssee4.tim",
    "riv_l1.tim", "riv_l2.tim", "riv_l3.tim", "riv_l4.tim",
    "ds_s1.tim", "ds_s2.tim", "ds_s3.tim", "ds_s4.tim",
    "sh_s1.tim", "sh_s2.tim", "sh_s3.tim", "sh_s4.tim",
    "sse_s1.tim", "sse_s2.tim", "sse_s3.tim", "sse_s4.tim",
    "swp1.tim", "swp2.tim", "swp3.tim", "swp4.tim",
    "dsee1.tim", "dsee2.tim", "dsee3.tim", "dsee4.tim",
    "shlm_1.tim", "shlm_2.tim", "shlm_3.tim", "shlm_4.tim",
    "des01.tim", "des02.tim", "des03.tim", "des04.tim",
    "edes01.tim", "edes02.tim", "edes03.tim", "edes04.tim",
    "rivs1.tim", "rivs2.tim", "rivs3.tim", "rivs4.tim"
};
// GLOBAL: FF7 0x0096a300
char *g_WorldTexturePageNamesUnderwater[8] = {
    "cltr.tim", "lake_a.tim", "rock.tim", "scave.tim",
    "ssand.tim", "swall02.tim", "sng01.tim", "sng02.tim"
};
// GLOBAL: FF7 0x0096a320
char *g_WorldTexturePageNamesSnow[4] = {
    "hokola01.tim", "hokola02.tim", "snwfldl.tim", "snwfld2.tim"
};
// The (u, v) origin of each texture page, parallel to the page-name tables above; the terrain
// geometry reads them through g_WorldTextureAuxTable.
// GLOBAL: FF7 0x0096a330
WorldTextureOrigin g_WorldTextureOriginsOverworld[282] = {
    {0x0, 0x160}, {0x80, 0x40}, {0x40, 0xc0}, {0x0, 0x100},
    {0xa0, 0x40}, {0xc0, 0xc0}, {0xe0, 0x180}, {0xe0, 0x1a0},
    {0xa0, 0x140}, {0xc0, 0x0}, {0xe0, 0x0}, {0xe0, 0x1c0},
    {0xe0, 0x140}, {0xa0, 0xe0}, {0x60, 0x60}, {0xc0, 0x120},
    {0xc0, 0x180}, {0x80, 0x100}, {0xc0, 0x160}, {0x0, 0x0},
    {0x40, 0x0}, {0x80, 0x0}, {0x0, 0x60}, {0x80, 0x0},
    {0x40, 0x40}, {0x0, 0x60}, {0xc0, 0x0}, {0xc0, 0x20},
    {0x0, 0x40}, {0x20, 0x60}, {0x0, 0xa0}, {0x20, 0x60},
    {0x20, 0x80}, {0x0, 0xe0}, {0xe0, 0x20}, {0xc0, 0x20},
    {0x80, 0x40}, {0xc0, 0x40}, {0x40, 0x40}, {0xc0, 0x140},
    {0x0, 0x140}, {0xc0, 0x40}, {0x80, 0x40}, {0x60, 0x80},
    {0xa0, 0x40}, {0xd0, 0x40}, {0x90, 0x60}, {0xe0, 0x1a0},
    {0xc0, 0x1c0}, {0x40, 0x40}, {0x60, 0x40}, {0xe0, 0xa0},
    {0x0, 0x1c0}, {0xa0, 0x140}, {0x80, 0x80}, {0xa0, 0x40},
    {0x0, 0x100}, {0xc0, 0x120}, {0x60, 0x120}, {0xe0, 0x0},
    {0x40, 0x80}, {0xe0, 0x140}, {0xa0, 0x0}, {0x40, 0x60},
    {0x0, 0x60}, {0xc0, 0x0}, {0xe0, 0x0}, {0x80, 0x100},
    {0xa0, 0x20}, {0xc0, 0x20}, {0x80, 0x40}, {0x80, 0x30},
    {0x40, 0x60}, {0x0, 0x40}, {0x0, 0xa0}, {0xe0, 0x100},
    {0x80, 0xc0}, {0x0, 0x80}, {0x60, 0x80}, {0xc0, 0x1a0},
    {0x0, 0x0}, {0x80, 0x80}, {0xe0, 0x80}, {0x40, 0x140},
    {0xa0, 0x140}, {0x20, 0x140}, {0x40, 0x1c0}, {0x0, 0x80},
    {0x80, 0x1c0}, {0x0, 0x0}, {0x0, 0x60}, {0x80, 0x60},
    {0x60, 0xe0}, {0x80, 0x0}, {0x20, 0x40}, {0xc0, 0x60},
    {0xe0, 0x60}, {0x40, 0xe0}, {0x0, 0xe0}, {0x80, 0xe0},
    {0x40, 0xe0}, {0xc0, 0xe0}, {0x80, 0xe0}, {0x0, 0x160},
    {0x60, 0x40}, {0xc0, 0xc0}, {0xc0, 0x40}, {0x40, 0x60},
    {0x80, 0x60}, {0x20, 0xe0}, {0xc0, 0xc0}, {0x80, 0xc0},
    {0x0, 0xf0}, {0x0, 0x40}, {0x0, 0x60}, {0xa0, 0x70},
    {0xc0, 0x70}, {0xe0, 0x70}, {0x40, 0x80}, {0x80, 0xc0},
    {0x60, 0xc0}, {0x80, 0xe0}, {0xa0, 0xe0}, {0x80, 0x180},
    {0x80, 0x1c0}, {0x80, 0x1e0}, {0xa0, 0x180}, {0x40, 0x1e0},
    {0x0, 0x1c0}, {0xc0, 0x100}, {0x60, 0x1e0}, {0xe0, 0x80},
    {0x80, 0x40}, {0x80, 0x80}, {0x80, 0xa0}, {0x0, 0x80},
    {0x40, 0x1a0}, {0x40, 0x1c0}, {0x0, 0xc0}, {0x0, 0xa0},
    {0x60, 0x10}, {0x0, 0x0}, {0x70, 0x40}, {0x80, 0x10},
    {0x60, 0x0}, {0x60, 0x30}, {0x70, 0x30}, {0x80, 0x0},
    {0x0, 0x0}, {0x40, 0xa0}, {0x80, 0x0}, {0x80, 0x20},
    {0x0, 0x80}, {0x60, 0x140}, {0x80, 0x80}, {0x0, 0xc0},
    {0xe0, 0xe0}, {0x0, 0x0}, {0xc0, 0x100}, {0x0, 0x60},
    {0x80, 0xc0}, {0x80, 0xe0}, {0x40, 0xe0}, {0x20, 0xc0},
    {0xe0, 0xc0}, {0x40, 0x60}, {0xc0, 0x80}, {0xa0, 0x1a0},
    {0x80, 0x100}, {0x0, 0xe0}, {0x20, 0xe0}, {0x0, 0xc0},
    {0xa0, 0x180}, {0x0, 0x100}, {0x0, 0xe0}, {0x0, 0xe0},
    {0x40, 0x180}, {0xe0, 0x180}, {0xc0, 0x1a0}, {0x80, 0x180},
    {0x0, 0x0}, {0x0, 0x40}, {0xc0, 0x180}, {0x60, 0xe0},
    {0xa0, 0x0}, {0x80, 0x80}, {0x0, 0x1a0}, {0x40, 0x0},
    {0x60, 0x140}, {0x0, 0xe0}, {0x40, 0xe0}, {0x80, 0xc0},
    {0x80, 0x140}, {0xc0, 0xc0}, {0xc0, 0xe0}, {0xc0, 0xa0},
    {0x20, 0x120}, {0x80, 0xa0}, {0x60, 0xe0}, {0xe0, 0x140},
    {0xe0, 0x160}, {0x80, 0x180}, {0x0, 0x180}, {0x0, 0xa0},
    {0x40, 0xa0}, {0x20, 0x100}, {0xe0, 0x120}, {0xa0, 0xa0},
    {0xc0, 0x140}, {0x80, 0x60}, {0xc0, 0xc0}, {0x0, 0x0},
    {0x80, 0xc0}, {0xe0, 0x1a0}, {0xc0, 0x0}, {0xe0, 0x20},
    {0x0, 0x180}, {0xc0, 0x1e0}, {0xe0, 0x1e0}, {0xe0, 0x1c0},
    {0x0, 0x0}, {0x80, 0x0}, {0x0, 0x60}, {0x40, 0x60},
    {0x40, 0x0}, {0x80, 0xc0}, {0x0, 0x40}, {0x80, 0x80},
    {0xc0, 0xc0}, {0x20, 0x0}, {0x20, 0x20}, {0xe0, 0xc0},
    {0xc0, 0x80}, {0x60, 0x100}, {0x20, 0x140}, {0x40, 0x120},
    {0xe0, 0x100}, {0xc0, 0x100}, {0x0, 0x180}, {0xe0, 0xa0},
    {0xc0, 0x60}, {0x0, 0x0}, {0x40, 0x80}, {0xc0, 0x80},
    {0x0, 0xc0}, {0x80, 0x60}, {0xc0, 0x60}, {0x20, 0xc0},
    {0xc0, 0xc0}, {0x80, 0xa0}, {0xa0, 0xc0}, {0x80, 0xc0},
    {0x40, 0xc0}, {0x0, 0x120}, {0x80, 0x0}, {0x80, 0x80},
    {0x40, 0x180}, {0xa0, 0x100}, {0x20, 0x60}, {0xe0, 0x40},
    {0x80, 0x60}, {0x0, 0x80}, {0x60, 0x60}, {0x60, 0x80},
    {0xc0, 0x140}, {0x80, 0x120}, {0xa0, 0x20}, {0x20, 0x80},
    {0x20, 0xa0}, {0x0, 0x60}, {0x0, 0xe0}, {0x0, 0xc0},
    {0x80, 0x0}, {0x40, 0x100}, {0x40, 0xa0}, {0x60, 0x100},
    {0xa0, 0x120}, {0x80, 0xa0}, {0x40, 0xe0}, {0xc0, 0x0},
    {0xc0, 0xa0}, {0x40, 0x60}
};
// GLOBAL: FF7 0x0096a798
WorldTextureOrigin g_WorldTextureOriginsUnderwater[8] = {
    {0x0, 0x0}, {0x0, 0x0}, {0x0, 0x0}, {0x0, 0x0},
    {0x0, 0x0}, {0x0, 0x0}, {0x80, 0x0}, {0x80, 0x0}
};
// GLOBAL: FF7 0x0096a7b8
WorldTextureOrigin g_WorldTextureOriginsSnow[4] = {
    {0x0, 0x0}, {0x0, 0x80}, {0x0, 0x40}, {0x0, 0xc0}
};
// The animation file names the model records point into: one run per model, animCount long.
// GLOBAL: FF7 0x0096a7c8
char *g_WorldAnimFileNames[82] = {
    "AAE.a", 0, "AGE.a", "AHA.a",
    "AHB.a", "AHC.a", "AHD.a", "AHE.a",
    "AIE.a", 0, "ASC.a", "ASD.a",
    "ASE.a", 0, "AZE.a", "BAA.a",
    "BAB.a", "BAC.a", "BAD.a", "BAE.a",
    "BBA.a", "BBB.a", "BBC.a", "BBD.a",
    "BID.a", "BIE.a", "BJA.a", "BJB.a",
    "BJC.a", "BJD.a", "BJE.a", "BKA.a",
    "BKB.a", "BKC.a", "BLD.a", "BME.a",
    "BTD.a", "BTE.a", "BUA.a", "BUB.a",
    "BUC.a", 0, "CEA.a", "CEB.a",
    "CFB.a", "CGC.a", "CIC.a", "CMA.a",
    "CNA.a", "COB.a", "CPB.a", "CQB.a",
    "DDA.a", "DDB.a", "DDC.a", "DFE.a",
    "DIB.a", "DJB.a", "DKB.a", "DLA.a",
    "DSE.a", "DTA.a", "DTB.a", "DTC.a",
    "DTD.a", "DTE.a", "DUA.a", "DUB.a",
    "DUC.a", "DUD.a", "DUE.a", "DYA.a",
    "EIC.a", "EID.a", "EIE.a", "EJA.a",
    "EJB.a", "EJC.a", "EJD.a", "EKD.a",
    "ENA.a", 0
};
// The stage-model tables World_LoadStageModels publishes through g_WorldStageModelTable, one per
// stage (the two warship stages share one). g_WorldStageModelsUnused follows the first and is
// never read.
// GLOBAL: FF7 0x0096a910
WorldModelDef g_WorldStageModelsEarly[10] = {
    {"choko", "/aja.hrc", 3, 0x200, &g_WorldAnimFileNames[10]},
    {"tybrco", "/dva.hrc", 1, 0x280, &g_WorldAnimFileNames[71]},
    {"buggy", "/aba.hrc", 6, 0x200, &g_WorldAnimFileNames[2]},
    {"barrier", "/aaa.hrc", 1, 0xdac, &g_WorldAnimFileNames[0]},
    {"condor", "/bkd.hrc", 1, 0xc80, &g_WorldAnimFileNames[34]},
    {"rocket", "/coc.hrc", 1, 0x8fc, &g_WorldAnimFileNames[50]},
    {"rocketbase", "/cpc.hrc", 1, 0x8fc, &g_WorldAnimFileNames[51]},
    {"goldsaucer", "/cfc.hrc", 1, 0xaf0, &g_WorldAnimFileNames[45]},
    {"canon", "/aia.hrc", 1, 0x1000, &g_WorldAnimFileNames[8]},
    {"rock", "/cnb.hrc", 1, 0x834, &g_WorldAnimFileNames[49]}
};
WorldModelDef g_WorldStageModelsUnused[12] = {
    {"choko", "/aja.hrc", 3, 0x200, &g_WorldAnimFileNames[10]},
    {"choko", "/aja.hrc", 3, 0x200, &g_WorldAnimFileNames[10]},
    {"highwind", "/cgd.hrc", 1, 0x300, &g_WorldAnimFileNames[46]},
    {"sbmrn", "/ddd.hrc", 1, 0x200, &g_WorldAnimFileNames[55]},
    {"condor", "/bkd.hrc", 1, 0xc80, &g_WorldAnimFileNames[34]},
    {"rocket", "/coc.hrc", 1, 0x8fc, &g_WorldAnimFileNames[50]},
    {"rocketbase", "/cpc.hrc", 1, 0x8fc, &g_WorldAnimFileNames[51]},
    {"goldsaucer", "/cfc.hrc", 1, 0xaf0, &g_WorldAnimFileNames[45]},
    {"canon", "/aia.hrc", 1, 0x1000, &g_WorldAnimFileNames[8]},
    {"tail", "/dkc.hrc", 1, 0x300, &g_WorldAnimFileNames[59]},
    {"barrier", "/aaa.hrc", 1, 0xdac, &g_WorldAnimFileNames[0]},
    {"diaweapon", "/bna.hrc", 5, 0x400, &g_WorldAnimFileNames[36]}
};
// GLOBAL: FF7 0x0096aac8
WorldModelDef g_WorldStageModelsWarship[2] = {
    {"warship", "/eke.hrc", 1, 0x1b8, &g_WorldAnimFileNames[80]},
    {"canon", "/aia.hrc", 1, 0x1000, &g_WorldAnimFileNames[8]}
};
// GLOBAL: FF7 0x0096aaf0
WorldModelDef g_WorldStageModelsDiamondCannon[13] = {
    {"choko", "/aja.hrc", 3, 0x200, &g_WorldAnimFileNames[10]},
    {"choko", "/aja.hrc", 3, 0x200, &g_WorldAnimFileNames[10]},
    {"highwind", "/cgd.hrc", 1, 0x300, &g_WorldAnimFileNames[46]},
    {"sbmrn", "/ddd.hrc", 1, 0x200, &g_WorldAnimFileNames[55]},
    {"condor", "/bkd.hrc", 1, 0xc80, &g_WorldAnimFileNames[34]},
    {"rocket", "/coc.hrc", 1, 0x8fc, &g_WorldAnimFileNames[50]},
    {"rocketbase", "/cpc.hrc", 1, 0x8fc, &g_WorldAnimFileNames[51]},
    {"goldsaucer", "/cfc.hrc", 1, 0xaf0, &g_WorldAnimFileNames[45]},
    {"canon", "/aia.hrc", 1, 0x1000, &g_WorldAnimFileNames[8]},
    {"tail", "/dkc.hrc", 1, 0x300, &g_WorldAnimFileNames[59]},
    {"barrier", "/aaa.hrc", 1, 0xdac, &g_WorldAnimFileNames[0]},
    {"diaweapon", "/bna.hrc", 5, 0x400, &g_WorldAnimFileNames[36]},
    {"rock", "/cnb.hrc", 1, 0x834, &g_WorldAnimFileNames[49]}
};
// GLOBAL: FF7 0x0096abf8
WorldModelDef g_WorldStageModelsDiamondMidgulCannon[13] = {
    {"choko", "/aja.hrc", 3, 0x200, &g_WorldAnimFileNames[10]},
    {"choko", "/aja.hrc", 3, 0x200, &g_WorldAnimFileNames[10]},
    {"highwind", "/cgd.hrc", 1, 0x300, &g_WorldAnimFileNames[46]},
    {"sbmrn", "/ddd.hrc", 1, 0x200, &g_WorldAnimFileNames[55]},
    {"condor", "/bkd.hrc", 1, 0xc80, &g_WorldAnimFileNames[34]},
    {"rocket", "/coc.hrc", 1, 0x8fc, &g_WorldAnimFileNames[50]},
    {"rocketbase", "/cpc.hrc", 1, 0x8fc, &g_WorldAnimFileNames[51]},
    {"goldsaucer", "/cfc.hrc", 1, 0xaf0, &g_WorldAnimFileNames[45]},
    {"midgulcanon", "/cmb.hrc", 1, 0x1000, &g_WorldAnimFileNames[48]},
    {"tail", "/dkc.hrc", 1, 0x300, &g_WorldAnimFileNames[59]},
    {"barrier", "/aaa.hrc", 1, 0xdac, &g_WorldAnimFileNames[0]},
    {"diaweapon", "/bna.hrc", 5, 0x400, &g_WorldAnimFileNames[36]},
    {"rock", "/cnb.hrc", 1, 0x834, &g_WorldAnimFileNames[49]}
};
// GLOBAL: FF7 0x0096ad00
WorldModelDef g_WorldStageModelsDiamondCannonHw02[13] = {
    {"choko", "/aja.hrc", 3, 0x200, &g_WorldAnimFileNames[10]},
    {"choko", "/aja.hrc", 3, 0x200, &g_WorldAnimFileNames[10]},
    {"highwind02", "/cid.hrc", 1, 0x300, &g_WorldAnimFileNames[47]},
    {"sbmrn", "/ddd.hrc", 1, 0x200, &g_WorldAnimFileNames[55]},
    {"condor", "/bkd.hrc", 1, 0xc80, &g_WorldAnimFileNames[34]},
    {"rocket", "/coc.hrc", 1, 0x8fc, &g_WorldAnimFileNames[50]},
    {"rocketbase", "/cpc.hrc", 1, 0x8fc, &g_WorldAnimFileNames[51]},
    {"goldsaucer", "/cfc.hrc", 1, 0xaf0, &g_WorldAnimFileNames[45]},
    {"canon", "/aia.hrc", 1, 0x1000, &g_WorldAnimFileNames[8]},
    {"tail", "/dkc.hrc", 1, 0x300, &g_WorldAnimFileNames[59]},
    {"barrier", "/aaa.hrc", 1, 0xdac, &g_WorldAnimFileNames[0]},
    {"diaweapon", "/bna.hrc", 5, 0x400, &g_WorldAnimFileNames[36]},
    {"rock", "/cnb.hrc", 1, 0x834, &g_WorldAnimFileNames[49]}
};
// GLOBAL: FF7 0x0096ae08
WorldModelDef g_WorldStageModelsDiamondMidgulHw02[13] = {
    {"choko", "/aja.hrc", 3, 0x200, &g_WorldAnimFileNames[10]},
    {"choko", "/aja.hrc", 3, 0x200, &g_WorldAnimFileNames[10]},
    {"highwind02", "/cid.hrc", 1, 0x300, &g_WorldAnimFileNames[47]},
    {"sbmrn", "/ddd.hrc", 1, 0x200, &g_WorldAnimFileNames[55]},
    {"condor", "/bkd.hrc", 1, 0xc80, &g_WorldAnimFileNames[34]},
    {"rocket", "/coc.hrc", 1, 0x8fc, &g_WorldAnimFileNames[50]},
    {"rocketbase", "/cpc.hrc", 1, 0x8fc, &g_WorldAnimFileNames[51]},
    {"goldsaucer", "/cfc.hrc", 1, 0xaf0, &g_WorldAnimFileNames[45]},
    {"midgulcanon", "/cmb.hrc", 1, 0x1000, &g_WorldAnimFileNames[48]},
    {"tail", "/dkc.hrc", 1, 0x300, &g_WorldAnimFileNames[59]},
    {"barrier", "/aaa.hrc", 1, 0xdac, &g_WorldAnimFileNames[0]},
    {"diaweapon", "/bna.hrc", 5, 0x400, &g_WorldAnimFileNames[36]},
    {"rock", "/cnb.hrc", 1, 0x834, &g_WorldAnimFileNames[49]}
};
// GLOBAL: FF7 0x0096af10
WorldModelDef g_WorldStageModelsUltima[13] = {
    {"choko", "/aja.hrc", 3, 0x200, &g_WorldAnimFileNames[10]},
    {"choko", "/aja.hrc", 3, 0x200, &g_WorldAnimFileNames[10]},
    {"highwind", "/cgd.hrc", 1, 0x300, &g_WorldAnimFileNames[46]},
    {"sbmrn", "/ddd.hrc", 1, 0x200, &g_WorldAnimFileNames[55]},
    {"condor", "/bkd.hrc", 1, 0xc80, &g_WorldAnimFileNames[34]},
    {"rocket", "/coc.hrc", 1, 0x8fc, &g_WorldAnimFileNames[50]},
    {"rocketbase", "/cpc.hrc", 1, 0x8fc, &g_WorldAnimFileNames[51]},
    {"goldsaucer", "/cfc.hrc", 1, 0xaf0, &g_WorldAnimFileNames[45]},
    {"midgulcanon", "/cmb.hrc", 1, 0x1000, &g_WorldAnimFileNames[48]},
    {"tail", "/dkc.hrc", 1, 0x300, &g_WorldAnimFileNames[59]},
    {"barrier", "/aaa.hrc", 1, 0xdac, &g_WorldAnimFileNames[0]},
    {"ultima", "/dyb.hrc", 7, 0x200, &g_WorldAnimFileNames[72]},
    {"rock", "/cnb.hrc", 1, 0x834, &g_WorldAnimFileNames[49]}
};
// GLOBAL: FF7 0x0096b018
WorldModelDef g_WorldStageModelsUltimaHw02[13] = {
    {"choko", "/aja.hrc", 3, 0x200, &g_WorldAnimFileNames[10]},
    {"choko", "/aja.hrc", 3, 0x200, &g_WorldAnimFileNames[10]},
    {"highwind02", "/cid.hrc", 1, 0x300, &g_WorldAnimFileNames[47]},
    {"sbmrn", "/ddd.hrc", 1, 0x200, &g_WorldAnimFileNames[55]},
    {"condor", "/bkd.hrc", 1, 0xc80, &g_WorldAnimFileNames[34]},
    {"rocket", "/coc.hrc", 1, 0x8fc, &g_WorldAnimFileNames[50]},
    {"rocketbase", "/cpc.hrc", 1, 0x8fc, &g_WorldAnimFileNames[51]},
    {"goldsaucer", "/cfc.hrc", 1, 0xaf0, &g_WorldAnimFileNames[45]},
    {"midgulcanon", "/cmb.hrc", 1, 0x1000, &g_WorldAnimFileNames[48]},
    {"tail", "/dkc.hrc", 1, 0x300, &g_WorldAnimFileNames[59]},
    {"barrier", "/aaa.hrc", 1, 0xdac, &g_WorldAnimFileNames[0]},
    {"ultima", "/dyb.hrc", 7, 0x200, &g_WorldAnimFileNames[72]},
    {"rock", "/cnb.hrc", 1, 0x834, &g_WorldAnimFileNames[49]}
};
// GLOBAL: FF7 0x0096b120
WorldModelDef g_WorldStageModelsRuby[12] = {
    {"choko", "/aja.hrc", 3, 0x200, &g_WorldAnimFileNames[10]},
    {"choko", "/aja.hrc", 3, 0x200, &g_WorldAnimFileNames[10]},
    {"highwind", "/cgd.hrc", 1, 0x300, &g_WorldAnimFileNames[46]},
    {"sbmrn", "/ddd.hrc", 1, 0x200, &g_WorldAnimFileNames[55]},
    {"condor", "/bkd.hrc", 1, 0xc80, &g_WorldAnimFileNames[34]},
    {"rocket", "/coc.hrc", 1, 0x8fc, &g_WorldAnimFileNames[50]},
    {"rocketbase", "/cpc.hrc", 1, 0x8fc, &g_WorldAnimFileNames[51]},
    {"goldsaucer", "/cfc.hrc", 1, 0xaf0, &g_WorldAnimFileNames[45]},
    {"midgulcanon", "/cmb.hrc", 1, 0x1000, &g_WorldAnimFileNames[48]},
    {"tail", "/dkc.hrc", 1, 0x300, &g_WorldAnimFileNames[59]},
    {"ruby", "/cqc.hrc", 3, 0x280, &g_WorldAnimFileNames[52]},
    {"rock", "/cnb.hrc", 1, 0x834, &g_WorldAnimFileNames[49]}
};
// GLOBAL: FF7 0x0096b210
WorldModelDef g_WorldStageModelsRubyHw02[12] = {
    {"choko", "/aja.hrc", 3, 0x200, &g_WorldAnimFileNames[10]},
    {"choko", "/aja.hrc", 3, 0x200, &g_WorldAnimFileNames[10]},
    {"highwind02", "/cid.hrc", 1, 0x300, &g_WorldAnimFileNames[47]},
    {"sbmrn", "/ddd.hrc", 1, 0x200, &g_WorldAnimFileNames[55]},
    {"condor", "/bkd.hrc", 1, 0xc80, &g_WorldAnimFileNames[34]},
    {"rocket", "/coc.hrc", 1, 0x8fc, &g_WorldAnimFileNames[50]},
    {"rocketbase", "/cpc.hrc", 1, 0x8fc, &g_WorldAnimFileNames[51]},
    {"goldsaucer", "/cfc.hrc", 1, 0xaf0, &g_WorldAnimFileNames[45]},
    {"midgulcanon", "/cmb.hrc", 1, 0x1000, &g_WorldAnimFileNames[48]},
    {"tail", "/dkc.hrc", 1, 0x300, &g_WorldAnimFileNames[59]},
    {"ruby", "/cqc.hrc", 3, 0x280, &g_WorldAnimFileNames[52]},
    {"rock", "/cnb.hrc", 1, 0x834, &g_WorldAnimFileNames[49]}
};
// GLOBAL: FF7 0x0096b300
WorldModelDef g_WorldStageModelsUnderwater[7] = {
    {"sbmrn", "/ddd.hrc", 1, 0x200, &g_WorldAnimFileNames[55]},
    {"gernika", "/cec.hrc", 1, 0x1000, &g_WorldAnimFileNames[44]},
    {"underseemk", "/eje.hrc", 1, 0x1000, &g_WorldAnimFileNames[79]},
    {"coralkey", "/ble.hrc", 1, 0x300, &g_WorldAnimFileNames[35]},
    {"seeobject", "/dic.hrc", 1, 0x800, &g_WorldAnimFileNames[57]},
    {"sbmrn2", "/dga.hrc", 1, 0x200, &g_WorldAnimFileNames[56]},
    {"emerald", "/bud.hrc", 2, 0x280, &g_WorldAnimFileNames[42]}
};
// GLOBAL: FF7 0x0096b390
WorldModelDef g_WorldStageModelsSnowfield[3] = {
    {"snowpole", "/djc.hrc", 1, 0x300, &g_WorldAnimFileNames[58]},
    {"snowpole", "/djc.hrc", 1, 0x300, &g_WorldAnimFileNames[58]},
    {"snowpole", "/djc.hrc", 1, 0x300, &g_WorldAnimFileNames[58]}
};
// The three leader-model definitions World_LoadLeaderModel publishes through
// g_WorldLeaderModelTable with a count of one. Each sits 0x18 bytes from the next although a
// WorldModelDef is 0x14; the extra four bytes are never read.
// GLOBAL: FF7 0x0096b3d0
WorldModelDef g_WorldLeaderModelCloud = {"cloud", "/bbe.hrc", 10, 0x200, &g_WorldAnimFileNames[24]};
// GLOBAL: FF7 0x0096b3e8
WorldModelDef g_WorldLeaderModelTifa = {"tifa", "/dlb.hrc", 11, 0x200, &g_WorldAnimFileNames[60]};
// GLOBAL: FF7 0x0096b400
WorldModelDef g_WorldLeaderModelCid = {"cid", "/ata.hrc", 10, 0x200, &g_WorldAnimFileNames[14]};
// GLOBAL: FF7 0x0096b414
int g_WorldCurrentTextureSetId = (-1);
// GLOBAL: FF7 0x0096b418
short g_WorldAnimTexturePages[22] = {53, 56, 57, 58, 61, 67, 129, 188, 192, 196, 205, 206, 208, 234, 235, 236, 253, 257, 264, 265, 275, 276};
// GLOBAL: FF7 0x0096b448
short g_WorldAnimGroupPageOrder[22] = {264, 275, 265, 276, 67, 257, 129, 236, 196, 56, 205, 235, 188, 57, 206, 234, 253, 58, 208, 53, 61, 192};
// GLOBAL: FF7 0x00e2bbd0
WorldModelDef * g_WorldStageModelTable = 0;
// GLOBAL: FF7 0x00e2bbd4
int g_WorldLoadedStageModelCount = 0;
// World_LoadTerrainTextures writes one entry per texture page (up to 282 entries, followed by
// 920 bytes of unreferenced .bss slack before the next global).
// GLOBAL: FF7 0x00e2bbd8
GfxRenderObject *g_WorldTexturePageRenderObjects[282] = {0};
unsigned char g_bssPad_00e2c040[920] = {0};
// GLOBAL: FF7 0x00e2c3d8
GfxRenderObject *g_WorldRadarRenderObject = 0;
// GLOBAL: FF7 0x00e2c3dc
GfxRenderObject *g_WorldMiniMapRenderObject = 0;
// GLOBAL: FF7 0x00e2c3e0
GfxRenderObject *g_WorldTerrainRenderObject = 0;
// GLOBAL: FF7 0x00e2c3e4
GfxRenderObject *g_WorldMapRenderObject = 0;
// GLOBAL: FF7 0x00e2c3e8
GfxRenderObject *g_WorldSkyRenderObject = 0;
// GLOBAL: FF7 0x00e2c3ec
void * g_WorldCloudRenderObject = 0;
// GLOBAL: FF7 0x00e2c3f0
void * g_WorldMeteorRenderObject = 0;
// GLOBAL: FF7 0x00e2c3f4
GfxRenderObject *g_WorldScreenOverlayRenderObjects = 0;
// GLOBAL: FF7 0x00e2c3f8
unsigned char g_WorldSnowRenderObjects[4] = {0};
// GLOBAL: FF7 0x00e2c3fc
int g_WorldSnow5RenderObject = 0;
// GLOBAL: FF7 0x00e2c400
void * g_WorldShadowRenderObject = 0;
// GLOBAL: FF7 0x00e2c404
GfxRenderObject *g_WorldDfxRenderObject0 = 0;
// GLOBAL: FF7 0x00e2c408
GfxRenderObject *g_WorldDfxRenderObject1 = 0;
// GLOBAL: FF7 0x00e2c40c
void * g_WorldDfxRenderObject2 = 0;
// GLOBAL: FF7 0x00e2c410
GfxRenderObject *g_WorldDfxRenderObject3 = 0;
// GLOBAL: FF7 0x00e2c414
GfxRenderObject *g_WorldMidgarRenderObject = 0;
unsigned char g_bssPad_00e2c418[4] = {0};
// GLOBAL: FF7 0x00e2c41c
unsigned int g_WorldTerrainTextureCount = 0;
// GLOBAL: FF7 0x00e2c420
unsigned int g_WorldTexturePageCount = 0;
// GLOBAL: FF7 0x00e2c424
int g_WorldViewportX = 0;
// GLOBAL: FF7 0x00e2c428
int g_WorldViewportY = 0;
unsigned char g_bssPad_00e2c42c[4] = {0};
// GLOBAL: FF7 0x00e2c430
WorldModelRecord g_WorldLeaderModels[3] = {0};
// GLOBAL: FF7 0x00e2c658
void * g_WorldTerrainAnimFrameRenderObjects[108] = {0};
// GLOBAL: FF7 0x00e2c808
WorldModelRecord g_WorldStageModels[13] = {0};
// GLOBAL: FF7 0x00e2d160
WorldModelDef * g_WorldLeaderModelTable = 0;
// GLOBAL: FF7 0x00e2d164
int g_WorldLoadedLeaderModelCount = 0;
// GLOBAL: FF7 0x00e2d168
void ** g_WorldTextureAuxTable = 0;
unsigned char g_WorldEmptyTextureName0[4] = {0};
unsigned char g_WorldEmptyTextureNames[84] = {0};
// GLOBAL: FF7 0x00e2d1c4
char g_WorldTexturePathOverworld[4] = {0};
// GLOBAL: FF7 0x00e2d1c8
char g_WorldTexturePathUnderwater[4] = {0};
// GLOBAL: FF7 0x00e2d1cc
char g_WorldTexturePathSnow[4] = {0};
// GLOBAL: FF7 0x00e2d1d0
char g_WorldTexturePathAnimFrames[4] = {0};
// GLOBAL: FF7 0x00e2d1d4
char g_WorldTexturePathSnowEffect[4] = {0};
// Default (empty) directory prefix World_LoadStageTextures prepends when loading
// fixed world-map stage textures.
// GLOBAL: FF7 0x00e2d1d8
char g_WorldStageTextureDir[8] = {0};
// GLOBAL: FF7 0x00e2d1e0
VECTOR g_WorldEffectOrigin = {0};
// GLOBAL: FF7 0x00e2d1f0
struct WorldEffectOffset g_WorldEffectOffset = {0};
// GLOBAL: FF7 0x00e2d1f4
short g_WorldEffectOffsetZ = 0;
// GLOBAL: FF7 0x00e2d1f8
struct WorldEffectOffset g_WorldEffectVelocity = {0};
// GLOBAL: FF7 0x00e2d1fc
short g_WorldEffectVelocityZ = 0;
// GLOBAL: FF7 0x00e2d200
short g_WorldEffectAngle = 0;
unsigned char g_bssPad_00e2d204[4] = {0};
unsigned char g_bssPad_00e2d208[976] = {0};
// The world camera's view vector (vx, vy, vz, pad), which wm_world_camera_accessors.cpp's camera update points s.position at.
// GLOBAL: FF7 0x00e2d5d8
short g_WorldCameraViewVector[4] = {0};
// clang-format on

/*
 * Returns whether page equals one of the 22 signed-short entries at g_WorldAnimTexturePages,
 * preserving the comparison against the unsigned page argument.
 */
// FUNCTION: FF7 0x007592e0
int __cdecl World_IsAnimatedTexturePage(unsigned int page)
{
    int i;

    for (i = 0; i < 22; i++) {
        if (g_WorldAnimTexturePages[i] == page) {
            return 1;
        }
    }
    return 0;
}
/*
 * Splits the supplied path, concatenates only its basename and extension and searches the world
 * map archive for that name. Reads the entry into the supplied buffer with the supplied size and returns zero
 * when found; returns one when absent. Every caller passes a fourth argument, 0, that the body
 * never reads.
 */
// FUNCTION: FF7 0x0075931b
int __cdecl WorldMap_LoadLgpAsset(char *path, int size, void *buffer, int unused)
{
    struct {
        char drive[4];
        char directory[256];
        char extension[256];
        void *lgpEntry;
        char fileName[256];
    } frame; /* Stack frame layout: see FRAME UNION in lib/codegen.h. */

    _splitpath(path, frame.drive, frame.directory, frame.fileName, frame.extension);
    strcat(frame.fileName, frame.extension);
    frame.lgpEntry = FindLgpFile(frame.fileName, WORLD_ARCHIVE_ID);
    if (frame.lgpEntry) {
        ReadLgpEntry(frame.lgpEntry, WORLD_ARCHIVE_ID, buffer, size);
        return 0;
    }
    return 1;
}
/*
 * Opens the named loose file, seeks to offset, reads size bytes into buffer, and closes the file.
 * Returns 1 only when open fails, otherwise zero. Both callers pass a fifth argument, 0, that the
 * body never reads.
 */
// FUNCTION: FF7 0x007593cd
int __cdecl World_ReadLooseFileBlock(int fileName, int offset, int size, void *buffer, int unused)
{
    struct FileHandle *handle;

    handle = 0;
    handle = (struct FileHandle *)OpenFileSimple(FILE_OPEN_READ, (void *)fileName);
    if (handle != 0) {
        SetFilePosition(handle, offset);
        ReadFileData(size, buffer, handle);
        CloseFileEx(handle);
        return 0;
    }
    return 1;
}

#pragma intrinsic(strcpy)
#pragma intrinsic(strcmp)


/*
 * Loads the models one world-map stage draws, the way World_LoadLeaderModel loads the leader's.
 * The switch publishes the stage's WorldModelDef table through g_WorldStageModelTable with its
 * count.
 *
 * Two model names are recognised by content rather than by index: "choko" gets the secondary light
 * group, and "barrier" gets the averaging blend mode and the record's needsScenePass flag. Stage
 * WORLD_STAGE_UNDERWATER gives three of its models an extra pair of option bits, set in two
 * statements.
 */
// FUNCTION: FF7 0x0075942c
void __cdecl World_LoadStageModels(int stageId)
{
    /* Local names are chosen for their stack slots; see World_LoadLeaderModel. */
    char spec[200];
    char model[256];
    struct GfxEngineContext *host;
    char head[256];
    void *anim;
    int entryIndex;
    char afile[200];
    int motionIdx;
    void *body;
    int mode;
    PfileLoadOptions opt;

    host = GetEngineContext();
    entryIndex = 0;
    motionIdx = 0;
    if (g_WorldLoadedStageModelCount != 0) {
        World_FreeStageModels();
    }
    switch (stageId) {
    case WORLD_STAGE_OVERWORLD_EARLY:
        g_WorldStageModelTable = g_WorldStageModelsEarly;
        g_WorldLoadedStageModelCount = 0xA;
        break;
    case WORLD_STAGE_OVERWORLD_DIAMOND_CANNON:
        g_WorldStageModelTable = g_WorldStageModelsDiamondCannon;
        g_WorldLoadedStageModelCount = 0xD;
        break;
    case WORLD_STAGE_OVERWORLD_DIAMOND_MIDGUL_CANNON:
        g_WorldStageModelTable = g_WorldStageModelsDiamondMidgulCannon;
        g_WorldLoadedStageModelCount = 0xD;
        break;
    case WORLD_STAGE_OVERWORLD_DIAMOND_CANNON_HW02:
        g_WorldStageModelTable = g_WorldStageModelsDiamondCannonHw02;
        g_WorldLoadedStageModelCount = 0xD;
        break;
    case WORLD_STAGE_OVERWORLD_DIAMOND_MIDGUL_HW02:
        g_WorldStageModelTable = g_WorldStageModelsDiamondMidgulHw02;
        g_WorldLoadedStageModelCount = 0xD;
        break;
    case WORLD_STAGE_OVERWORLD_ULTIMA:
        g_WorldStageModelTable = g_WorldStageModelsUltima;
        g_WorldLoadedStageModelCount = 0xD;
        break;
    case WORLD_STAGE_OVERWORLD_ULTIMA_HW02:
        g_WorldStageModelTable = g_WorldStageModelsUltimaHw02;
        g_WorldLoadedStageModelCount = 0xD;
        break;
    case WORLD_STAGE_OVERWORLD_RUBY:
        g_WorldStageModelTable = g_WorldStageModelsRuby;
        g_WorldLoadedStageModelCount = 0xC;
        break;
    case WORLD_STAGE_OVERWORLD_RUBY_HW02:
        g_WorldStageModelTable = g_WorldStageModelsRubyHw02;
        g_WorldLoadedStageModelCount = 0xC;
        break;
    case WORLD_STAGE_WARSHIP:
    case WORLD_STAGE_WARSHIP_LATE:
        g_WorldStageModelTable = g_WorldStageModelsWarship;
        g_WorldLoadedStageModelCount = 2;
        break;
    case WORLD_STAGE_UNDERWATER:
        g_WorldStageModelTable = g_WorldStageModelsUnderwater;
        g_WorldLoadedStageModelCount = 7;
        break;
    case WORLD_STAGE_SNOWFIELD:
        g_WorldStageModelTable = g_WorldStageModelsSnowfield;
        g_WorldLoadedStageModelCount = 3;
        break;
    }
    for (entryIndex = 0; entryIndex < g_WorldLoadedStageModelCount; entryIndex++) {
        Pfile_InitLoadOptions(&opt);
        opt.file.fromArchive = 1;
        opt.file.archiveId = WORLD_ARCHIVE_ID;
        opt.file.callback = 0;
        head[0] = 0;
        strcpy(model, g_WorldStageModelTable[entryIndex].hrcFile);
        g_WorldStageModels[entryIndex].modelScale =
            (float)g_WorldStageModelTable[entryIndex].scale > 512.0f
                ? (float)g_WorldStageModelTable[entryIndex].scale / 512.0f
                : 1.0f;
        g_WorldStageModels[entryIndex].modelScale =
            g_WorldStageModels[entryIndex].modelScale * 15.2f;
        opt.scale = g_WorldStageModels[entryIndex].modelScale;
        opt.flags = 0x2100000;
        if (stageId == WORLD_STAGE_UNDERWATER && (entryIndex == 1 || entryIndex == 2 || entryIndex == 4)) {
            opt.paletteFlags |= 1;
            opt.paletteFlags |= 2;
        }
        // STRING: FF7 0x0096d410
        if (strcmp(g_WorldStageModelTable[entryIndex].name, "choko") == 0) {
            opt.lightGroup = (int)g_pWorldSecondaryLightGroup;
        } else {
            opt.lightGroup = 0;
        }
        // STRING: FF7 0x0096d418
        if (strcmp(g_WorldStageModelTable[entryIndex].name, "barrier") == 0) {
            Pfile_SetLoadOptionsBlendMode(BLEND_AVERAGE, &opt);
            g_WorldStageModels[entryIndex].needsScenePass = 1;
        } else {
            g_WorldStageModels[entryIndex].needsScenePass = 0;
        }
        mode = 0;
        body = (void *)Skel_LoadHrc((int)&opt.file, mode, (int)model);
        if (body != 0) {
            Skel_BuildHierarchyAndLoadParts((int)head, (PolygonSkelHierarchy *)body, &opt, (int)host);
            Skel_SetPartsVisible(1, (PolygonSkelHierarchy *)body);
            g_WorldStageModels[entryIndex].skeleton = body;
            g_WorldStageModels[entryIndex].active = 0;
            AnimPlacement_Init((PolygonAnimPlacement *)g_WorldStageModels[entryIndex].bonePlacement);
            g_WorldStageModels[entryIndex].animations =
                (struct WorldAnimationHeader **)GameHeapAllocRaw(g_WorldStageModelTable[entryIndex].animCount * 4,
                                 // STRING: FF7 0x0096d420
                                 "C:\\FF7\\src\\wm\\wmfile.cpp", 277);
            g_WorldStageModels[entryIndex].animationCount = g_WorldStageModelTable[entryIndex].animCount;
            for (motionIdx = 0; motionIdx < g_WorldStageModelTable[entryIndex].animCount; motionIdx++) {
                StripFileExtension(
                    g_WorldStageModelTable[entryIndex].animFiles[motionIdx], spec);
                // STRING: FF7 0x0096d440
                sprintf(afile, "%s.%s", spec,
                        // STRING: FF7 0x0096d43c
                        "A");
                if (Anim_CompileTextToBinary(
                        (int)g_WorldStageModelTable[entryIndex].animFiles[motionIdx],
                        (int)afile) != 0) {
                    anim = (void *)Anim_LoadBinary(&opt.file, (int)afile);
                    if (anim != 0) {
                        g_WorldStageModels[entryIndex].animations[motionIdx] = (struct WorldAnimationHeader *)anim;
                        Anim_ScaleRootTranslation(
                            g_WorldStageModels[entryIndex].modelScale, (PolygonAnimation *)anim);
                    }
                }
            }
        }
    }
}
/*
 * Loads the models the world map draws the party leader with.
 *
 * The switch publishes one of three single-record tables through g_WorldLeaderModelTable with a
 * count of one, and everything after it walks that count, so the loop runs once.
 *
 * Each WorldModelDef carries a name, an .hrc, an animation count and a scale divisor. The divisor is
 * an unsigned int converted to float twice, once for the comparison and once for the division. The
 * result becomes the record's modelScale and the load options' scale. Skel_LoadHrc and
 * Anim_LoadBinary are handed &opt.file, not &opt.
 */
// FUNCTION: FF7 0x00759a65
void __cdecl World_LoadLeaderModel(int modelId)
{
    /* Local names are chosen for their stack slots: VC5 orders them by a hash of the name. */
    char spec[200];
    char model[256];
    struct GfxEngineContext *host;
    char head[256];
    void *anim;
    int entryIndex;
    char afile[200];
    int motionIdx;
    void *body;
    int mode;
    PfileLoadOptions opt;

    host = GetEngineContext();
    entryIndex = 0;
    motionIdx = 0;
    if (g_WorldLoadedLeaderModelCount != 0) {
        World_FreeLeaderModels();
    }
    switch (modelId) {
    case WORLD_MODEL_LEADER_0:
        g_WorldLeaderModelTable = &g_WorldLeaderModelCloud;
        g_WorldLoadedLeaderModelCount = 1;
        break;
    case WORLD_MODEL_LEADER_1:
        g_WorldLeaderModelTable = &g_WorldLeaderModelTifa;
        g_WorldLoadedLeaderModelCount = 1;
        break;
    case WORLD_MODEL_LEADER_2:
        g_WorldLeaderModelTable = &g_WorldLeaderModelCid;
        g_WorldLoadedLeaderModelCount = 1;
        break;
    }
    for (entryIndex = 0; entryIndex < g_WorldLoadedLeaderModelCount; entryIndex++) {
        Pfile_InitLoadOptions(&opt);
        opt.file.fromArchive = 1;
        opt.file.archiveId = WORLD_ARCHIVE_ID;
        opt.file.callback = 0;
        head[0] = 0;
        strcpy(model, g_WorldLeaderModelTable[entryIndex].hrcFile);
        g_WorldLeaderModels[entryIndex].modelScale =
            (float)g_WorldLeaderModelTable[entryIndex].scale > 512.0f
                ? (float)g_WorldLeaderModelTable[entryIndex].scale / 512.0f
                : 1.0f;
        g_WorldLeaderModels[entryIndex].modelScale =
            g_WorldLeaderModels[entryIndex].modelScale * 15.2f;
        opt.scale = g_WorldLeaderModels[entryIndex].modelScale;
        g_WorldLeaderModels[entryIndex].needsScenePass = 0;
        opt.flags = 0x2100000;
        opt.lightGroup = (int)g_pWorldSecondaryLightGroup;
        mode = 0;
        body = (void *)Skel_LoadHrc((int)&opt.file, mode, (int)model);
        if (body != 0) {
            Skel_BuildHierarchyAndLoadParts((int)head, (PolygonSkelHierarchy *)body, &opt, (int)host);
            Skel_SetPartsVisible(1, (PolygonSkelHierarchy *)body);
            g_WorldLeaderModels[entryIndex].skeleton = body;
            g_WorldLeaderModels[entryIndex].active = 0;
            AnimPlacement_Init((PolygonAnimPlacement *)g_WorldLeaderModels[entryIndex].bonePlacement);
            g_WorldLeaderModels[entryIndex].animations =
                (struct WorldAnimationHeader **)GameHeapAllocRaw(g_WorldLeaderModelTable[entryIndex].animCount * 4,
                                 // STRING: FF7 0x0096d448
                                 "C:\\FF7\\src\\wm\\wmfile.cpp", 410);
            g_WorldLeaderModels[entryIndex].animationCount = g_WorldLeaderModelTable[entryIndex].animCount;
            for (motionIdx = 0; motionIdx < g_WorldLeaderModelTable[entryIndex].animCount; motionIdx++) {
                StripFileExtension(
                    g_WorldLeaderModelTable[entryIndex].animFiles[motionIdx], spec);
                // STRING: FF7 0x0096d468
                sprintf(afile, "%s.%s", spec,
                        // STRING: FF7 0x0096d464
                        "A");
                if (Anim_CompileTextToBinary(
                        (int)g_WorldLeaderModelTable[entryIndex].animFiles[motionIdx],
                        (int)afile) != 0) {
                    anim = (void *)Anim_LoadBinary(&opt.file, (int)afile);
                    if (anim != 0) {
                        g_WorldLeaderModels[entryIndex].animations[motionIdx] = (struct WorldAnimationHeader *)anim;
                        Anim_ScaleRootTranslation(
                            g_WorldLeaderModels[entryIndex].modelScale, (PolygonAnimation *)anim);
                    }
                }
            }
        }
    }
}
/*
 * Returns the number of loaded world stage models, g_WorldLoadedStageModelCount.
 */
// FUNCTION: FF7 0x00759ee0
int __cdecl World_GetLoadedStageModelCount(void)
{
    return g_WorldLoadedStageModelCount;
}
/*
 * Frees every loaded stage model record: its skeleton, each of its animations (count from the
 * model table g_WorldStageModelTable) and the animation array, and clears the record's active
 * flag.
 */
// FUNCTION: FF7 0x00759eea
void __cdecl World_FreeStageModels(void)
{
    int modelIdx;
    int animIdx;

    for (modelIdx = 0; modelIdx < g_WorldLoadedStageModelCount; modelIdx++) {
        if (g_WorldStageModels[modelIdx].skeleton != 0) {
            Skel_Free((PolygonSkelHierarchy *)(g_WorldStageModels[modelIdx].skeleton));
            g_WorldStageModels[modelIdx].skeleton = 0;
            for (animIdx = 0; animIdx < g_WorldStageModelTable[modelIdx].animCount; animIdx++) {
                if (g_WorldStageModels[modelIdx].animations[animIdx] != 0) {
                    Anim_Free((PolygonAnimBuffers *)(g_WorldStageModels[modelIdx].animations[animIdx]));
                    g_WorldStageModels[modelIdx].animations[animIdx] = 0;
                }
            }
        }
        // STRING: FF7 0x0096d470
        GameHeapFree(g_WorldStageModels[modelIdx].animations, "C:\\FF7\\src\\wm\\wmfile.cpp", 466);
        g_WorldStageModels[modelIdx].animations = 0;
        g_WorldStageModels[modelIdx].active = 0;
    }
}
/*
 * Frees every loaded leader model record: skeleton, animations (count from
 * g_WorldLeaderModelTable), animation array, and clears the active flag.
 */
// FUNCTION: FF7 0x0075a01d
void __cdecl World_FreeLeaderModels(void)
{
    int modelIdx;
    int animIdx;

    for (modelIdx = 0; modelIdx < g_WorldLoadedLeaderModelCount; modelIdx++) {
        if (g_WorldLeaderModels[modelIdx].skeleton != 0) {
            Skel_Free((PolygonSkelHierarchy *)(g_WorldLeaderModels[modelIdx].skeleton));
            g_WorldLeaderModels[modelIdx].skeleton = 0;
            for (animIdx = 0; animIdx < g_WorldLeaderModelTable[modelIdx].animCount; animIdx++) {
                if (g_WorldLeaderModels[modelIdx].animations[animIdx] != 0) {
                    Anim_Free((PolygonAnimBuffers *)(g_WorldLeaderModels[modelIdx].animations[animIdx]));
                    g_WorldLeaderModels[modelIdx].animations[animIdx] = 0;
                }
            }
        }
        // STRING: FF7 0x0096d48c
        GameHeapFree(g_WorldLeaderModels[modelIdx].animations, "C:\\FF7\\src\\wm\\wmfile.cpp", 490);
        g_WorldLeaderModels[modelIdx].animations = 0;
        g_WorldLeaderModels[modelIdx].active = 0;
    }
}
/*
 * Maps a world stage id to its WorldTextureSet: the overworld for stages 0 and 9, the overworld
 * with the large mini-map for stages 1..8 and 10, and the underwater and snow sets for 11 and 12.
 */
// FUNCTION: FF7 0x0075a150
int __cdecl World_GetStageTextureSetId(int stageId)
{
    /* The original never initialises result and has no default arm, so a stage above
       WORLD_STAGE_SNOWFIELD returns whatever the slot held; kept as is. */
    int result;

    switch (stageId) {
    case WORLD_STAGE_OVERWORLD_EARLY:
    case WORLD_STAGE_WARSHIP:
        result = WORLD_TEXTURE_SET_OVERWORLD;
        break;
    case WORLD_STAGE_OVERWORLD_DIAMOND_CANNON:
    case WORLD_STAGE_OVERWORLD_DIAMOND_MIDGUL_CANNON:
    case WORLD_STAGE_OVERWORLD_DIAMOND_CANNON_HW02:
    case WORLD_STAGE_OVERWORLD_DIAMOND_MIDGUL_HW02:
    case WORLD_STAGE_OVERWORLD_ULTIMA:
    case WORLD_STAGE_OVERWORLD_ULTIMA_HW02:
    case WORLD_STAGE_OVERWORLD_RUBY:
    case WORLD_STAGE_OVERWORLD_RUBY_HW02:
    case WORLD_STAGE_WARSHIP_LATE:
        result = WORLD_TEXTURE_SET_OVERWORLD | WORLD_TEXTURE_SET_LARGE_MINIMAP;
        break;
    case WORLD_STAGE_UNDERWATER:
        result = WORLD_TEXTURE_SET_UNDERWATER;
        break;
    case WORLD_STAGE_SNOWFIELD:
        result = WORLD_TEXTURE_SET_SNOW;
        break;
    }
    return result;
}
/*
 * Loads the assets of world stage stageId: switches to the stage's texture set (releasing the
 * previous one) when it differs from g_WorldCurrentTextureSetId, then the stage's models.
 */
// FUNCTION: FF7 0x0075a1c6
void __cdecl World_LoadStageAssets(int stageId)
{
    /* padWord, engine and inDummy are dead; the original reserves their frame slots, so they stay. */
    int padWord;
    GfxEngineContext *engine;
    int texSet;
    int inDummy;

    padWord = 0;
    inDummy = 0;
    texSet = World_GetStageTextureSetId(stageId);
    engine = GetEngineContext();
    Pfile_SetAssetSourceMode(0);
    if (g_WorldCurrentTextureSetId != (texSet & WORLD_TEXTURE_SET_MASK)) {
        if (g_WorldTexturePageCount != 0) {
            World_ReleaseStageTextures();
        }
        g_WorldCurrentTextureSetId = texSet & WORLD_TEXTURE_SET_MASK;
        World_LoadStageTextures(texSet);
        if (g_pWorldPrimaryLightGroup != 0) {
            World_FreePrimaryLightGroup();
        }
        g_pWorldPrimaryLightGroup = (struct GfxLightOwner *)World_CreatePrimaryLightGroup(texSet & WORLD_TEXTURE_SET_MASK);
        if (g_pWorldSecondaryLightGroup != 0) {
            World_FreeSecondaryLightGroup();
        }
        g_pWorldSecondaryLightGroup = World_CreateSecondaryLightGroup(texSet & WORLD_TEXTURE_SET_MASK);
    }
    World_LoadStageModels(stageId);
}
/*
 * Loads the terrain page textures for one texture set, plus the extra sets the overworld and snow
 * sets own.
 *
 * The switch picks everything at once: the page count, the page-name table, the texture origins,
 * the path buffer for the loader, and which of the three terrain flush routines the renderer will
 * call.
 *
 * The 0x6C animated-frame loop computes a flag and then tests it, and both arms that clear it are
 * written out even though it is already zero. The two snow file names are a ternary, not a local
 * assigned in an if/else.
 */
// FUNCTION: FF7 0x0075a283
void __cdecl World_LoadTerrainTextures(int setId)
{
    /* keep and opt are spelled for their stack slots, not for reading. */
    struct GfxEngineContext *ctx;
    int keep;
    char **table;
    int groupIndex;
    unsigned int pageIndex;
    PfileLoadOptions opt;

    table = 0;
    ctx = GetEngineContext();
    keep = 0;
    groupIndex = 0;
    Pfile_InitLoadOptionsWithBlend(BLEND_NONE, &opt);
    opt.file.fromArchive = 1;
    opt.file.archiveId = WORLD_ARCHIVE_ID;
    opt.file.callback = 0;
    switch (setId) {
    case WORLD_TEXTURE_SET_OVERWORLD:
        g_WorldTerrainTextureCount = 0x8F;
        g_WorldTexturePageCount = 0x11A;
        table = g_WorldTexturePageNamesOverworld;
        g_WorldTextureAuxTable = (void **)g_WorldTextureOriginsOverworld;
        opt.directory = g_WorldTexturePathOverworld;
        g_WorldTerrainFlushFn = World_FlushTerrainBatches;
        break;
    case WORLD_TEXTURE_SET_UNDERWATER:
        g_WorldTerrainTextureCount = 7;
        g_WorldTexturePageCount = 8;
        table = g_WorldTexturePageNamesUnderwater;
        g_WorldTextureAuxTable = (void **)g_WorldTextureOriginsUnderwater;
        opt.directory = g_WorldTexturePathUnderwater;
        g_WorldTerrainFlushFn = World_FlushTerrainBatchesUnderwater;
        break;
    case WORLD_TEXTURE_SET_SNOW:
        g_WorldTerrainTextureCount = 5;
        g_WorldTexturePageCount = 4;
        table = g_WorldTexturePageNamesSnow;
        g_WorldTextureAuxTable = (void **)g_WorldTextureOriginsSnow;
        opt.directory = g_WorldTexturePathSnow;
        g_WorldTerrainFlushFn = World_FlushTerrainBatchesSnow;
        break;
    }
    opt.emitterFlags |= 2;
    Pfile_SetLoadOptionsBlendMode(BLEND_NONE, &opt);
    for (pageIndex = 0; pageIndex < g_WorldTexturePageCount; pageIndex++) {
        if (World_IsAnimatedTexturePage(pageIndex) == 0) {
            g_WorldTexturePageRenderObjects[pageIndex] = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_GOURAUD_TRI_2D, &opt, table[pageIndex],
                                                     (void *)ctx->renderObjectBank);
        }
    }
    if (setId == WORLD_TEXTURE_SET_OVERWORLD && g_WorldTexturePageCount != 0) {
        table = g_WorldTextureAnimFrameNames;
        opt.directory = g_WorldTexturePathAnimFrames;
        opt.emitterFlags |= 2;
        Pfile_SetLoadOptionsBlendMode(BLEND_NONE, &opt);
        for (pageIndex = 0; pageIndex < 0x6C; pageIndex++) {
            keep = 0;
            if (pageIndex < 0x28 && pageIndex % 8 != 0) {
                keep = 0;
            } else if (pageIndex >= 0x28 && pageIndex % 4 != 0) {
                keep = 0;
            } else {
                keep = 1;
            }
            g_WorldTerrainAnimFrameRenderObjects[pageIndex] = Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_GOURAUD_TRI_2D, &opt, table[pageIndex],
                                                                                     (void *)ctx->renderObjectBank);
            if (keep != 0) {
                g_WorldTexturePageRenderObjects[g_WorldAnimGroupPageOrder[groupIndex]] = (GfxRenderObject *)g_WorldTerrainAnimFrameRenderObjects[pageIndex];
                groupIndex++;
            }
        }
    } else if (setId == WORLD_TEXTURE_SET_SNOW) {
        Pfile_InitLoadOptionsWithBlend(BLEND_NONE, &opt);
        opt.file.fromArchive = 1;
        opt.file.archiveId = WORLD_ARCHIVE_ID;
        opt.file.callback = 0;
        opt.directory = g_WorldTexturePathSnowEffect;
        Pfile_SetLoadOptionsBlendMode(BLEND_ADDITIVE, &opt);
        for (pageIndex = 0; pageIndex < 2; pageIndex++) {
            (*(void *(*)[2]) & g_WorldSnowRenderObjects)[pageIndex] = Gfx_CreateRenderObject(
                1, GFX_DRAW_TEXTURED_QUAD_2D, &opt,
                pageIndex == 0
                    // STRING: FF7 0x0096d4a8
                    ? "snow4.tim"
                    // STRING: FF7 0x0096d4b4
                    : "snow5.tim",
                (void *)ctx->renderObjectBank);
        }
    }
}
/*
 * Creates every render object a world-map stage needs, then hands the texture set on to
 * World_LoadTerrainTextures.
 *
 * One options block is built once and reused for all the loads; the blend mode moves between
 * calls through Pfile_SetLoadOptionsBlendMode. WORLD_TEXTURE_SET_LARGE_MINIMAP picks the larger
 * mini-map and is then masked away, so every test after that sees only the set. Each of the four
 * "dfx.tim" loads carries its own literal.
 */
// FUNCTION: FF7 0x0075a5d5
void __cdecl World_LoadStageTextures(int setId)
{
    /* b is spelled for its stack slot; it is the one blend mode passed through a variable. */
    struct GfxEngineContext *ctx;
    int b;
    PfileLoadOptions opts;
    char *name;

    ctx = GetEngineContext();
    Pfile_InitLoadOptionsWithBlend(BLEND_NONE, &opts);
    opts.file.fromArchive = 1;
    opts.file.archiveId = WORLD_ARCHIVE_ID;
    opts.file.callback = 0;
    opts.directory = g_WorldStageTextureDir;
    b = 0;
    Pfile_SetLoadOptionsBlendMode(b, &opts);
    if ((setId & WORLD_TEXTURE_SET_LARGE_MINIMAP) != 0) {
        // STRING: FF7 0x0096d4c0
        name = "midlmap2.tim";
        setId &= WORLD_TEXTURE_SET_MASK;
    } else {
        // STRING: FF7 0x0096d4d0
        name = "midlmap.tim";
    }
    g_WorldMiniMapRenderObject = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &opts, name,
                                          (void *)ctx->renderObjectBank);
    // STRING: FF7 0x0096d4dc
    name = "radar.tim";
    g_WorldRadarRenderObject = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &opts, name,
                                          (void *)ctx->renderObjectBank);
    // STRING: FF7 0x0096d4e8
    name = "map.tim";
    g_WorldMapRenderObject = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &opts, name,
                                          (void *)ctx->renderObjectBank);
    opts.renderFlags |= 0x4000;
    g_WorldScreenOverlayRenderObjects = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_FLAT_QUAD_2D, &opts, 0,
                                          (void *)ctx->renderObjectBank);
    opts.renderFlags &= ~0x4000;
    opts.emitterFlags |= 2;
    Pfile_SetLoadOptionsBlendMode(BLEND_ADDITIVE, &opts);
    g_WorldTerrainRenderObject = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_GOURAUD_TRI_2D, &opts, 0,
                                          (void *)ctx->renderObjectBank);
    if (setId != WORLD_TEXTURE_SET_UNDERWATER) {
        // STRING: FF7 0x0096d4f0
        name = "shadow.tim";
        Pfile_SetLoadOptionsBlendMode(BLEND_AVERAGE, &opts);
        g_WorldShadowRenderObject = Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &opts, name,
                                              (void *)ctx->renderObjectBank);
    }
    // STRING: FF7 0x0096d4fc
    name = "dfx.tim";
    Pfile_SetLoadOptionsBlendMode(BLEND_AVERAGE, &opts);
    g_WorldDfxRenderObject0 = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &opts, name,
                                          (void *)ctx->renderObjectBank);
    // STRING: FF7 0x0096d504
    name = "dfx.tim";
    Pfile_SetLoadOptionsBlendMode(BLEND_ADDITIVE, &opts);
    g_WorldDfxRenderObject1 = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &opts, name,
                                          (void *)ctx->renderObjectBank);
    // STRING: FF7 0x0096d50c
    name = "dfx.tim";
    Pfile_SetLoadOptionsBlendMode(BLEND_SUBTRACTIVE, &opts);
    g_WorldDfxRenderObject2 = Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &opts, name,
                                          (void *)ctx->renderObjectBank);
    // STRING: FF7 0x0096d514
    name = "dfx.tim";
    Pfile_SetLoadOptionsBlendMode(BLEND_QUARTER_ADDITIVE, &opts);
    g_WorldDfxRenderObject3 = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &opts, name,
                                          (void *)ctx->renderObjectBank);
    if (setId == WORLD_TEXTURE_SET_OVERWORLD) {
        // STRING: FF7 0x0096d51c
        name = "midg.tim";
        Pfile_SetLoadOptionsBlendMode(BLEND_NONE, &opts);
        g_WorldMidgarRenderObject = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &opts, name,
                                              (void *)ctx->renderObjectBank);
        g_WorldSkyRenderObject = (GfxRenderObject *)Gfx_CreateRenderObject(1, GFX_DRAW_GOURAUD_QUAD_2D, &opts, 0,
                                              (void *)ctx->renderObjectBank);
        // STRING: FF7 0x0096d528
        name = "wm_kumo.tim";
        Pfile_SetLoadOptionsBlendMode(BLEND_ADDITIVE, &opts);
        g_WorldCloudRenderObject = Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &opts, name,
                                              (void *)ctx->renderObjectBank);
        // STRING: FF7 0x0096d534
        name = "meteo.tim";
        g_WorldMeteorRenderObject = Gfx_CreateRenderObject(1, GFX_DRAW_TEXTURED_QUAD_2D, &opts, name,
                                              (void *)ctx->renderObjectBank);
    }
    World_LoadTerrainTextures(setId);
}
/*
 * Releases the world stage textures: the ten fixed render objects (radar, mini-map, terrain, map,
 * screen overlay, shadow and the four dfx objects), then each of the g_WorldTexturePageCount
 * texture pages (animated pages are only cleared), and the extra objects the sets with 0x11A or 4
 * pages own.
 */
// FUNCTION: FF7 0x0075a921
void __cdecl World_ReleaseStageTextures(void)
{
    /* The page count doubles as a discriminator: 0x11A and 4 are the two stage kinds that own extra
       texture sets. */
    unsigned int i;

    DestroyRenderObjectAndClear(&g_WorldMiniMapRenderObject);
    DestroyRenderObjectAndClear(&g_WorldRadarRenderObject);
    DestroyRenderObjectAndClear(&g_WorldTerrainRenderObject);
    DestroyRenderObjectAndClear(&g_WorldShadowRenderObject);
    DestroyRenderObjectAndClear(&g_WorldDfxRenderObject0);
    DestroyRenderObjectAndClear(&g_WorldDfxRenderObject1);
    DestroyRenderObjectAndClear(&g_WorldDfxRenderObject2);
    DestroyRenderObjectAndClear(&g_WorldDfxRenderObject3);
    DestroyRenderObjectAndClear(&g_WorldMapRenderObject);
    DestroyRenderObjectAndClear(&g_WorldScreenOverlayRenderObjects);
    for (i = 0; i < g_WorldTexturePageCount; i++) {
        if (World_IsAnimatedTexturePage(i) == 0) {
            if (g_WorldTexturePageRenderObjects[i] != 0) {
                DestroyRenderObjectAndClear(&g_WorldTexturePageRenderObjects[i]);
            }
        } else {
            g_WorldTexturePageRenderObjects[i] = 0;
        }
    }
    if (g_WorldTexturePageCount == 0x11A) {
        for (i = 0; i < 108; i++) {
            if (g_WorldTerrainAnimFrameRenderObjects[i] != 0) {
                DestroyRenderObjectAndClear(&g_WorldTerrainAnimFrameRenderObjects[i]);
            }
        }
        DestroyRenderObjectAndClear(&g_WorldMidgarRenderObject);
        DestroyRenderObjectAndClear(&g_WorldSkyRenderObject);
        DestroyRenderObjectAndClear(&g_WorldCloudRenderObject);
        DestroyRenderObjectAndClear(&g_WorldMeteorRenderObject);
    } else if (g_WorldTexturePageCount == 4) {
        for (i = 0; i < 2; i++) {
            if ((*(void *(*)[2]) & g_WorldSnowRenderObjects)[i] != 0) {
                DestroyRenderObjectAndClear(&(*(void *(*)[2]) & g_WorldSnowRenderObjects)[i]);
            }
        }
    }
    g_WorldTerrainTextureCount = g_WorldTexturePageCount = 0;
}
/*
 * Loads a compressed file `name` from disk, LZS-decompresses it into dst, calls the optional
 * onDone hook, frees the raw buffer and returns the raw file size (0 when the file was not found).
 */
// FUNCTION: FF7 0x0075aadd
int __cdecl World_LoadAndDecompressLzsFile(char *name, int unused, void *dst, void(__cdecl *onDone)(void))
{
    int size;
    void *raw;

    raw = LoadDiskFile(&size, name);
    if (raw != 0) {
        LzsDecompress(raw, dst);
        if (onDone != 0) {
            onDone();
        }
        // STRING: FF7 0x0096d540
        GameHeapFree(raw, "C:\\FF7\\src\\wm\\wmfile.cpp", 920);
    }
    return size;
}

/*
 * Clears both world model counts, g_WorldLoadedStageModelCount and g_WorldLoadedLeaderModelCount.
 */
// FUNCTION: FF7 0x0075ab32
void __cdecl World_ClearStageAndLeaderModelCounts(void)
{
    g_WorldLoadedStageModelCount = g_WorldLoadedLeaderModelCount = 0;
}
} /* extern "C" */
