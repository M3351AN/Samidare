#pragma once
#include <XorStr.h>

#include <string>
#include <vector>

#include "../Overlay.h"

#define AUTO_CONFIG_VARS                   \
  X(AimBot, bool)                          \
  X(AimBotHotKey.uKey, int)                \
  X(AimBotHotKey.nMode, int)               \
  X(AimBullet, int)                        \
  X(AimScopeOnly, bool)                    \
  X(AimIgnoreFlash, bool)                  \
  X(AimFov, float)                         \
  X(AimFovMin, float)                      \
  X(AimSmooth, float)                      \
  X(AimMaxRecoil, float)                   \
  X(HitboxList, std::vector<unsigned int>) \
  X(TriggerBot, bool)                      \
  X(TriggerBotHotKey.uKey, int)            \
  X(TriggerBotHotKey.nMode, int)           \
  X(TriggerDelay, int)                     \
  X(ShotDuration, int)                     \
  X(TriggerMaxRecoil, float)               \
  X(TriggerScopeOnly, bool)                \
  X(TriggerIgnoreFlash, bool)              \
  X(RCS, bool)                             \
  X(RCSScale.x, float)                     \
  X(RCSScale.y, float)                     \
  X(RCSBullet, int)                        \
  X(ESP, bool)                             \
  X(ESPInfo, bool)                         \
  X(ESPBox, bool)                          \
  X(ESPName, bool)                         \
  X(ESPHealth, bool)                       \
  X(C4ESP, bool)                           \
  X(Radar, bool)                           \
  X(RadarCrossLine, bool)                  \
  X(RadarPointSize, float)                 \
  X(RadarProportion, float)                \
  X(RadarRange, float)                     \
  X(Sonar, bool)                           \
  X(PitchIndicator, bool)                  \
  X(C4Timer, bool)                         \
  X(SpecList, bool)                        \
  X(InfoString, bool)                      \
  X(FastStop, bool)                        \
  X(FastStopMinVelocity, float)            \
  X(TeamCheck, bool)                       \
  X(WorkInSpec, bool)                      \
  X(BypassCapture, bool)                   \
  X(DormantTime, float)                    \
  X(Style, int)                            \
  X(RenderFPS, int)                        \
  X(GlobalVarsInterval, int)               \
  X(EntityInterval, int)                   \
  X(ParserInterval, int)                   \
  X(AimInterval, int)                      \
  X(ViewInterval, int)                     \
  X(MemoryInterval, int)                   \
  X(NonMemoryInterval, int)

#ifndef UKIARPM_CONFIGS
#define UKIARPM_CONFIGS
namespace config {
inline std::string path = "";
inline bool ShowMenu = false;
inline int RenderInterval = 0;

inline bool AimBot = false;
inline KeyBind_t AimBotHotKey(ImGuiKey_MouseX1, EKeyBindMode::HOLD);
inline float AimFov = 5;
inline float AimFovMin = 0.f;
inline float AimSmooth = 15.0f;
inline float AimMaxRecoil = 15.f;
inline bool AimScopeOnly = false;
inline bool AimIgnoreFlash = false;
inline int AimBullet = 0;
inline std::vector<unsigned int> HitboxList = {6, 5, 4};

inline bool TriggerBot = false;
inline KeyBind_t TriggerBotHotKey(ImGuiKey_MouseX2, EKeyBindMode::HOLD);
inline int TriggerDelay = 90;
inline int ShotDuration = 200;
inline float TriggerMaxRecoil = 15.f;
inline bool TriggerScopeOnly = false;
inline bool TriggerIgnoreFlash = false;

inline bool RCS = false;
inline Vector2 RCSScale = {2.f, 2.f};
inline int RCSBullet = 2;

inline bool ESP = false;
inline bool ESPInfo = false;
inline bool ESPBox = true;
inline bool ESPName = true;
inline bool ESPHealth = true;

inline bool C4ESP = false;

inline bool Radar = false;
inline bool RadarCrossLine = true;
inline float RadarPointSize = 1.f;
inline float RadarProportion = 3300.f;
inline float RadarRange = 150.f;

inline bool Sonar = false;
inline bool PitchIndicator = false;
inline bool C4Timer = false;
inline bool SpecList = false;
inline bool InfoString = false;
inline bool FastStop = false;
inline float FastStopMinVelocity = 24.f;

inline bool TeamCheck = true;
inline bool WorkInSpec = true;
inline bool BypassCapture = false;
inline float DormantTime = 1.f;
inline int Style = 0;

inline int RenderFPS = 0;
inline int GlobalVarsInterval = 15;
inline int EntityInterval = 1;
inline int ParserInterval = 100;
inline int AimInterval = 1;
inline int ViewInterval = 1;
inline int MemoryInterval = 15;
inline int NonMemoryInterval = 1;
}  // namespace config
#endif

#define AUTO_LANG_VARS                      \
  Y(greek, bool)                            \
  Y(vietnamese, bool)                       \
  Y(kanji, bool)                            \
  Y(korean, bool)                           \
  Y(arabic, bool)                           \
  Y(thai, bool)                             \
  Y(textfortranslator, std::string)         \
  Y(TabLegit, std::string)                  \
  Y(TabVisuals, std::string)                \
  Y(TabMiscs, std::string)                  \
  Y(TabSettings, std::string)               \
  Y(TabExperimental, std::string)           \
  Y(ChildAimAssist, std::string)            \
  Y(SwitchEnableAimBot, std::string)        \
  Y(SliderAimFov, std::string)              \
  Y(SliderAimFovMin, std::string)           \
  Y(SliderAimSmooth, std::string)           \
  Y(SliderAimMaxRecoil, std::string)        \
  Y(SwitchAimScopeOnly, std::string)        \
  Y(SwitchAimIgnoreFlash, std::string)      \
  Y(SliderAimBullet, std::string)           \
  Y(ChildHitBox, std::string)               \
  Y(ChildTriggerbot, std::string)           \
  Y(SwitchEnableTriggerbot, std::string)    \
  Y(SliderTriggerDelay, std::string)        \
  Y(SliderShotDuration, std::string)        \
  Y(SliderTriggerMaxRecoil, std::string)    \
  Y(SwitchTriggerScopeOnly, std::string)    \
  Y(SwitchTriggerIgnoreFlash, std::string)  \
  Y(ChildRCS, std::string)                  \
  Y(SwitchEnableRCS, std::string)           \
  Y(SliderRCSScale, std::string)            \
  Y(SliderRCSStartBullet, std::string)      \
  Y(ChildESP, std::string)                  \
  Y(SwitchEnableESP, std::string)           \
  Y(SwitchPlayerInfo, std::string)          \
  Y(SwitchBox, std::string)                 \
  Y(SwitchPlayerName, std::string)          \
  Y(SwitchHealthBar, std::string)           \
  Y(ChildWorld, std::string)                \
  Y(SwitchC4ESP, std::string)               \
  Y(ChildVisualOthers, std::string)         \
  Y(SwitchEnableRadar, std::string)         \
  Y(SwitchRadarCrossline, std::string)      \
  Y(SliderRadarPointSize, std::string)      \
  Y(SliderRadarProportion, std::string)     \
  Y(SliderRadarRange, std::string)          \
  Y(ChildMiscGlobals, std::string)          \
  Y(SwitchSonar, std::string)               \
  Y(SwitchPitchIndicator, std::string)      \
  Y(SwitchC4Timer, std::string)             \
  Y(SwitchSpecList, std::string)            \
  Y(ChildMisc, std::string)                 \
  Y(SwitchEnemyInfo, std::string)           \
  Y(ChildMovement, std::string)             \
  Y(SwitchFastStop, std::string)            \
  Y(SliderFastStopMinVelocity, std::string) \
  Y(ChildInfo, std::string)                 \
  Y(TextLastBuild, std::string)             \
  Y(TextBuildVersion, std::string)          \
  Y(TextCurrentGameVersion, std::string)    \
  Y(TextCompiler, std::string)              \
  Y(TextLicenceToUser, std::string)         \
  Y(ChildConfigList, std::string)           \
  Y(TextConfigAuthor, std::string)          \
  Y(ButtonLoad, std::string)                \
  Y(ButtonReLoad, std::string)              \
  Y(ButtonSave, std::string)                \
  Y(ButtonDelete, std::string)              \
  Y(TextDeleteConfirm, std::string)         \
  Y(ButtonYes, std::string)                 \
  Y(ButtonNo, std::string)                  \
  Y(ChildSettings, std::string)             \
  Y(SwitchTeamCheck, std::string)           \
  Y(SwitchBypassCapture, std::string)       \
  Y(SliderDormantTime, std::string)         \
  Y(ComboStyle, std::string)                \
  Y(ButtonCreateConfig, std::string)        \
  Y(TextNewConfigName, std::string)         \
  Y(TextConfigAuthorName, std::string)      \
  Y(ButtonCreate, std::string)              \
  Y(ButtonOpenFolder, std::string)          \
  Y(ButtonExtendFonts, std::string)         \
  Y(ButtonApply, std::string)               \
  Y(ButtonUnhook, std::string)              \
  Y(ChildLoopInterval, std::string)         \
  Y(SliderRenderFPS, std::string)           \
  Y(SliderGlobalVarsInterval, std::string)  \
  Y(SliderEntityInterval, std::string)      \
  Y(SliderParserInterval, std::string)      \
  Y(SliderAimInterval, std::string)         \
  Y(SliderViewInterval, std::string)        \
  Y(SliderMemoryInterval, std::string)      \
  Y(SliderNonMemoryInterval, std::string)   \
  Y(TextFPSBased, std::string)              \
  Y(TextBombOn, std::string)                \
  Y(TextBombNotPlanted, std::string)        \
  Y(TextBombEstimDamage, std::string)       \
  Y(TextDefuseCount, std::string)           \
  Y(TextCanDefuse, std::string)             \
  Y(TextCanNotDefuse, std::string)          \
  Y(TextNotDefusing, std::string)           \
  Y(TextPress, std::string)                 \
  Y(TextNone, std::string)                  \
  Y(TextHold, std::string)                  \
  Y(TextToggle, std::string)                \
  Y(TextDisable, std::string)

namespace LangSettings {
inline std::string path = "";
inline bool greek = false;
inline bool vietnamese = false;
inline bool kanji = false;
inline bool korean = false;
inline bool arabic = false;
inline bool thai = false;
inline std::string textfortranslator = XorStr("");

inline std::string TabLegit = XorStr("Legit");
inline std::string TabVisuals = XorStr("Visuals");
inline std::string TabMiscs = XorStr("Miscs");
inline std::string TabSettings = XorStr("Settings");
inline std::string TabExperimental = XorStr("Experimental");

inline std::string ChildAimAssist = XorStr("Aim Assist");
inline std::string SwitchEnableAimBot = XorStr("Enable AimBot");
inline std::string SliderAimFov = XorStr("Aim FOV");
inline std::string SliderAimFovMin = XorStr("Min Aim FOV");
inline std::string SliderAimSmooth = XorStr("Aim Smooth");
inline std::string SliderAimMaxRecoil = XorStr("Max Recoil##Aim");
inline std::string SwitchAimScopeOnly = XorStr("Scoped only##Aim");
inline std::string SwitchAimIgnoreFlash = XorStr("Ignore flash##Aim");
inline std::string SliderAimBullet = XorStr("Start bullet##Aim");

inline std::string ChildHitBox = XorStr("HitBox");

inline std::string ChildTriggerbot = XorStr("Triggerbot");
inline std::string SwitchEnableTriggerbot = XorStr("Enable triggerbot");
inline std::string SliderTriggerDelay = XorStr("Delay##Trigger");
inline std::string SliderShotDuration = XorStr("Shot duration");
inline std::string SliderTriggerMaxRecoil = XorStr("Max Recoil##Trigger");
inline std::string SwitchTriggerScopeOnly = XorStr("Scoped only##Trigger");
inline std::string SwitchTriggerIgnoreFlash = XorStr("Ignore flash##Trigger");

inline std::string ChildRCS = XorStr("RCS");
inline std::string SwitchEnableRCS = XorStr("Enable RCS");
inline std::string SliderRCSScale = XorStr("Scale X,Y##RCS");
inline std::string SliderRCSStartBullet = XorStr("Start bullet##RCS");

inline std::string ChildESP = XorStr("ESP");
inline std::string SwitchEnableESP = XorStr("Enable ESP");
inline std::string SwitchPlayerInfo = XorStr("Player info");
inline std::string SwitchBox = XorStr("Box");
inline std::string SwitchPlayerName = XorStr("Player name");
inline std::string SwitchHealthBar = XorStr("Health bar");

inline std::string ChildWorld = XorStr("World");
inline std::string SwitchC4ESP = XorStr("Bomb ESP");

inline std::string ChildVisualOthers = XorStr("Others");
inline std::string SwitchEnableRadar = XorStr("Enable radar");
inline std::string SwitchRadarCrossline = XorStr("Crossline");
inline std::string SliderRadarPointSize = XorStr("Point size");
inline std::string SliderRadarProportion = XorStr("Proportion");
inline std::string SliderRadarRange = XorStr("Range");

inline std::string ChildMiscGlobals = XorStr("Globals");
inline std::string SwitchSonar = XorStr("Sonar");
inline std::string SwitchPitchIndicator = XorStr("Pitch indicator");
inline std::string SwitchC4Timer = XorStr("Bomb timer");
inline std::string SwitchSpecList = XorStr("Spec list");

inline std::string ChildMisc = XorStr("Misc");
inline std::string SwitchEnemyInfo = XorStr("Enemy info");

inline std::string ChildMovement = XorStr("Movement");
inline std::string SwitchFastStop = XorStr("Fast stop");
inline std::string SliderFastStopMinVelocity = XorStr("Min velocity");

inline std::string ChildInfo = XorStr("Info");
inline std::string TextLastBuild = XorStr("Last build: %s");
inline std::string TextBuildVersion = XorStr("Build for version: %s");
inline std::string TextCurrentGameVersion = XorStr("Current version: %s");
inline std::string TextCompiler = XorStr("Compiler:");
inline std::string TextLicenceToUser = XorStr("Copy licenced to: %s");

inline std::string ChildConfigList = XorStr("Config List");
inline std::string TextConfigAuthor = XorStr("Author: %s");
inline std::string ButtonLoad = XorStr("Load");
inline std::string ButtonReLoad = XorStr("Reload");
inline std::string ButtonSave = XorStr("Save");
inline std::string ButtonDelete = XorStr("Delete");
inline std::string TextDeleteConfirm = XorStr("Are you sure to delete %s?");
inline std::string ButtonYes = XorStr("Yes");
inline std::string ButtonNo = XorStr("No");
inline std::string ChildSettings = XorStr("Settings");
inline std::string SwitchTeamCheck = XorStr("Team Check");
inline std::string SwitchBypassCapture = XorStr("Bypass Capture");
inline std::string SliderDormantTime = XorStr("Max Dormant Time");
inline std::string ComboStyle = XorStr("Style");
inline std::string ButtonCreateConfig = XorStr("Create Config");
inline std::string TextNewConfigName = XorStr("New Config Name");
inline std::string TextConfigAuthorName = XorStr("Author Name");
inline std::string ButtonCreate = XorStr("Create");
inline std::string ButtonOpenFolder = XorStr("Open Folder");
inline std::string ButtonExtendFonts = XorStr("Extend Fonts");
inline std::string ButtonApply = XorStr("Apply");
inline std::string ButtonUnhook = XorStr("Unhook");

inline std::string ChildLoopInterval = XorStr("Performances");
inline std::string SliderRenderFPS = XorStr("Render FPS");
inline std::string SliderGlobalVarsInterval = XorStr("GlobalVars interval");
inline std::string SliderEntityInterval = XorStr("Entity interval");
inline std::string SliderParserInterval = XorStr("Map parser interval");
inline std::string SliderAimInterval = XorStr("Aim interval");
inline std::string SliderViewInterval = XorStr("View interval");
inline std::string SliderMemoryInterval = XorStr("Memory func interval");
inline std::string SliderNonMemoryInterval = XorStr("NonMemory func interval");
inline std::string TextFPSBased = XorStr("Based on FPS");

inline std::string TextBombOn = XorStr("Bomb on %s.Remain %.3f s.");
inline std::string TextBombNotPlanted = XorStr("C4 not planted");
inline std::string TextBombEstimDamage = XorStr("Estim Damage : %d");
inline std::string TextDefuseCount = XorStr("C4 %s in %.3f s.");
inline std::string TextCanDefuse = XorStr("can be defused");
inline std::string TextCanNotDefuse = XorStr("cannot be defused");
inline std::string TextNotDefusing = XorStr("C4 is not being defused");

inline std::string TextPress = XorStr("Press");
inline std::string TextNone = XorStr("None");
inline std::string TextHold = XorStr("Hold");
inline std::string TextToggle = XorStr("Toggle");
inline std::string TextDisable = XorStr("Disable");
};  // namespace LangSettings
