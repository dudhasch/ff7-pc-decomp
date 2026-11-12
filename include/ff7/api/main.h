/* include/ff7/api/main.h: the functions and objects ff7/main defines that other TUs use.
 * One declaration per name. */
#ifndef FF7_API_MAIN_H
#define FF7_API_MAIN_H

#ifdef __cplusplus
extern "C" {
#endif
struct GfxEngineContext;
struct MainInputState;

/* initpath.cpp */
extern char *g_InitPath_AppPath;
extern char *g_InitPath_MoviePath;
extern int g_InitPath_SoundEnabled;
extern char *g_InitPath_MusicPath;
extern char *g_InitPath_SoundPath;
extern char *g_InitPath_MidiPath;
extern int g_InitPath_MidiEnabled;
extern char *g_InitPath_CdDataPath;
extern char *g_InitPath_DataPath;
extern char *g_InitPath_DataDrive;
void __cdecl InitPathsFromRegistry(void);
void __cdecl Engine_FreePathGlobals(void);
int __cdecl Engine_RegWriteVolumeSettings(int music, int sound);
int __cdecl ApplyRegistryVolumeSettings(void);
char *__cdecl GetDataPath(void);
char *__cdecl GetCdDataPath(void);
char *__cdecl GetMinigameDirForChocobo(void);
char *__cdecl GetMinigameDirForCondor(void);
char *__cdecl GetMinigameDirForSubmarine(void);
char *__cdecl GetMidiDirPath(void);
char *__cdecl GetSoundPath(void);
char *__cdecl BuildArchivePath(int archive);

/* Engine mode state (defined in field_map_script_state.cpp, read/written by main dispatcher and minigames). */
extern short g_EngineGameMode;
extern short g_EnginePreviousMode;

/* main.cpp */
extern int g_Main_FieldEntryPointActive;
void __cdecl Field_ClearMapTransitionRequest(void);
void __cdecl App_PlayCachedSfxUnlessMuted(int sfxId);
void __cdecl GameModeDispatcher_ReenterFieldNoOp(void);
void __cdecl Savemap_RestoreFieldEntryPoint(void);
void __cdecl Savemap_CaptureFieldEntryPoint(void);
void __cdecl Savemap_ResetFieldEntryPoint(void);
void __cdecl Battle_SetupEntryFromField(void);
int __cdecl App_OpenCoreArchives(void);
void __cdecl Lgp_CloseArchivesForDiscSwap(void);
void __cdecl App_FreeInputStateAndRsdModel(struct MainInputState *obj, struct GfxEngineContext *ctx);
void __cdecl App_ResetInputStateFlags(struct MainInputState *state);
struct MainInputState *__cdecl App_AllocInputState(void);
void __cdecl App_InstallDefaultViewMatrix(struct GfxEngineContext *frameContext);
void __cdecl App_BlankScreenForModeSwitch(struct GfxEngineContext *frameContext);
int __cdecl App_InitSubsystems(struct GfxEngineContext *ctx);
void __cdecl App_Shutdown(struct GfxEngineContext *ctx);
void __cdecl GameModeDispatcher_Enter(struct GfxEngineContext *ctx);
void __cdecl GameModeDispatcher_Exit(struct GfxEngineContext *ctx);
void __cdecl GameModeDispatcher_Frame(struct GfxEngineContext *ctx);
void __cdecl App_OnMouseMessage(int msgCode, int unused, int lParam, struct GfxEngineContext *ctx);
void __cdecl App_OnKeyMessage(int message, int key, int unused, struct GfxEngineContext *ctx);
void __cdecl App_OnActivateApp(int active);
void __cdecl App_OnDeviceChangeMessage(int event, int unused, int deviceData);
int __cdecl App_AllowCtrlQQuit(void);
void __cdecl App_Bootstrap(int first, int second);
void __cdecl App_DrawDebugTextGdi(int x, int y, const char *message, void *dc);

/* main_cd_check_screen.cpp */
extern char g_CdCheckEmptyPath[1];
extern int g_CdCheckDisplayMode;
extern char *g_cdCheckDiscTimNames[9];
extern char *g_cdCheckCharQuadTimNames[36];
extern char *g_cdCheckCharTimNames[9];
void __cdecl CdCheck_FillFadeQuad(unsigned char fadeLevel);
void __cdecl CdCheck_BuildScreenQuadsMode0(void);
void __cdecl CdCheck_BuildScreenQuadsMode1(void);
void __cdecl CdCheck_BuildScreenQuadsMode2(void);
void __cdecl CdCheck_LoadDiscCharXTims(void);
void __cdecl CdCheck_ModuleEnter(void);
void __cdecl CdCheck_ModuleExit(void);
void __cdecl CdCheck_ModuleFrame(struct GfxEngineContext *ctx);
void __cdecl CdCheck_ModuleKeyboard(void);
void __cdecl CdCheck_ModuleMouse(void);
int __cdecl CdCheck_GetGameDiscNumber(void);
void __cdecl CdCheck_HandleDeviceChange(int message, int deviceData);

/* main_game_over_screen.cpp */
extern char g_GameOverEmptyPath[1];
extern char *g_GameOverTimNamesMode2[6];
extern char *g_GameOverTimNamesMode01[2];
void __cdecl GameOver_FillFadeQuad(unsigned char alpha);
void __cdecl GameOver_BuildScreenQuadsMode0(void);
void __cdecl GameOver_BuildScreenQuadsMode1(void);
void __cdecl GameOver_BuildScreenQuadsMode2(void);
void __cdecl GameOver_LoadDiscEOverTims(void);
void __cdecl GameOver_ModuleEnter(void);
void __cdecl GameOver_ModuleExit(void);
void __cdecl GameOver_ModuleFrame(struct GfxEngineContext *ctx);
void __cdecl GameOver_ModuleKeyboard(int message, int wParam);
void __cdecl GameOver_ModuleMouse(void);

/* main_movie_camera_records.cpp */
int *__cdecl Movie_FetchCameraRecord(void);

/* main_registry_key_access.cpp */
int __cdecl RegOpenFf7Key(void);
int __cdecl RegCloseFf7Key(void);
int __cdecl RegGetSoundOptions(void *value);
int __cdecl RegGetMidiDeviceId(void *value);
int __cdecl RegGetMidiOptions(void *value);
int __cdecl RegSetSoundDeviceGuid(void *value);
int __cdecl RegSetDdrawDeviceGuid(char *value);
int __cdecl RegSetMidiDeviceId(void *value);
int __cdecl RegGetDdrawDeviceGuid(char *value);
int __cdecl RegGetMidiDataType(char *value);
int __cdecl RegSetMidiDataType(char *value);
int __cdecl Savemap_StartClockTimer(void);
void __cdecl Savemap_StopClockTimer(void);
void __stdcall Savemap_ClockTick(unsigned int timer, unsigned int message, unsigned long user, unsigned long first, unsigned long second);
int __cdecl Savemap_GetClockTickCount(void);

/* main_registry_graphics_config.cpp */
int __cdecl ApplyConfigToContext(struct GfxEngineContext *ctx);

/* main_registry_ui_scaling.cpp */
int __cdecl GetUiScalingMode(void);

/* main_no_op_hooks.cpp */
void __cdecl App_InitHookNoOp(void);
void __cdecl App_ShutdownHookNoOp(void);

/* main_no_op_hooks.cpp */
int __cdecl Trace_Printf(const char *format, ...);

/* main_registry_key_access.cpp */
int __cdecl RegGetSoundDeviceGuid(void *value);

/* initpath.cpp */
char *__cdecl GetMinigameDirForHighway(void);
char *__cdecl GetMinigameDirForCoaster(void);
char *__cdecl GetMinigameDirForSnowboard(void);

#ifdef __cplusplus
}
#endif

#endif /* FF7_API_MAIN_H */
