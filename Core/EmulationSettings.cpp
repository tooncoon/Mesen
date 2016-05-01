#include "stdafx.h"
#include "EmulationSettings.h"
#include "VsControlManager.h"

//Version 0.1.1
uint16_t EmulationSettings::_versionMajor = 0;
uint8_t EmulationSettings::_versionMinor = 1;
uint8_t EmulationSettings::_versionRevision = 1;

Language EmulationSettings::_displayLanguage = Language::English;

uint32_t EmulationSettings::_flags = 0;

uint32_t EmulationSettings::_audioLatency = 20000;
double EmulationSettings::_channelVolume[11] = { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 };
double EmulationSettings::_masterVolume = 1.0;
uint32_t EmulationSettings::_sampleRate = 44100;
StereoFilter EmulationSettings::_stereoFilter = StereoFilter::None;
int32_t EmulationSettings::_stereoDelay = 0;
double EmulationSettings::_stereoAngle = 0;
double EmulationSettings::_reverbStrength = 0;
double EmulationSettings::_reverbDelay = 0;

NesModel EmulationSettings::_model = NesModel::Auto;
PpuModel EmulationSettings::_ppuModel = PpuModel::Ppu2C03;

uint32_t EmulationSettings::_emulationSpeed = 100;

OverscanDimensions EmulationSettings::_overscan;
VideoFilterType EmulationSettings::_videoFilterType = VideoFilterType::None;
double EmulationSettings::_videoScale = 1;
VideoAspectRatio EmulationSettings::_aspectRatio = VideoAspectRatio::Auto;

ConsoleType EmulationSettings::_consoleType = ConsoleType::Nes;
ExpansionPortDevice EmulationSettings::_expansionDevice = ExpansionPortDevice::None;
ControllerType EmulationSettings::_controllerTypes[4] = { ControllerType::None, ControllerType::None, ControllerType::None, ControllerType::None };
KeyMappingSet EmulationSettings::_controllerKeys[4] = { KeyMappingSet(), KeyMappingSet(), KeyMappingSet(), KeyMappingSet() };
bool EmulationSettings::_needControllerUpdate = false;

uint32_t EmulationSettings::_ppuPaletteArgb[11][64] = {
	/* 2C02 */			{ 0xFF666666, 0xFF002A88, 0xFF1412A7, 0xFF3B00A4, 0xFF5C007E, 0xFF6E0040, 0xFF6C0600, 0xFF561D00, 0xFF333500, 0xFF0B4800, 0xFF005200, 0xFF004F08, 0xFF00404D, 0xFF000000, 0xFF000000, 0xFF000000, 0xFFADADAD, 0xFF155FD9, 0xFF4240FF, 0xFF7527FE, 0xFFA01ACC, 0xFFB71E7B, 0xFFB53120, 0xFF994E00, 0xFF6B6D00, 0xFF388700, 0xFF0C9300, 0xFF008F32, 0xFF007C8D, 0xFF000000, 0xFF000000, 0xFF000000, 0xFFFFFEFF, 0xFF64B0FF, 0xFF9290FF, 0xFFC676FF, 0xFFF36AFF, 0xFFFE6ECC, 0xFFFE8170, 0xFFEA9E22, 0xFFBCBE00, 0xFF88D800, 0xFF5CE430, 0xFF45E082, 0xFF48CDDE, 0xFF4F4F4F, 0xFF000000, 0xFF000000, 0xFFFFFEFF, 0xFFC0DFFF, 0xFFD3D2FF, 0xFFE8C8FF, 0xFFFBC2FF, 0xFFFEC4EA, 0xFFFECCC5, 0xFFF7D8A5, 0xFFE4E594, 0xFFCFEF96, 0xFFBDF4AB, 0xFFB3F3CC, 0xFFB5EBF2, 0xFFB8B8B8, 0xFF000000, 0xFF000000 } ,
	/* 2C03 */			{ 0xFF6D6D6D, 0xFF002491, 0xFF0000DA, 0xFF6D48DA, 0xFF91006D, 0xFFB6006D, 0xFFB62400, 0xFF914800, 0xFF6D4800, 0xFF244800, 0xFF006D24, 0xFF009100, 0xFF004848, 0xFF000000, 0xFF000000, 0xFF000000, 0xFFB6B6B6, 0xFF006DDA, 0xFF0048FF, 0xFF9100FF, 0xFFB600FF, 0xFFFF0091, 0xFFFF0000, 0xFFDA6D00, 0xFF916D00, 0xFF249100, 0xFF009100, 0xFF00B66D, 0xFF009191, 0xFF000000, 0xFF000000, 0xFF000000, 0xFFFFFFFF, 0xFF6DB6FF, 0xFF9191FF, 0xFFDA6DFF, 0xFFFF00FF, 0xFFFF6DFF, 0xFFFF9100, 0xFFFFB600, 0xFFDADA00, 0xFF6DDA00, 0xFF00FF00, 0xFF48FFDA, 0xFF00FFFF, 0xFF000000, 0xFF000000, 0xFF000000, 0xFFFFFFFF, 0xFFB6DAFF, 0xFFDAB6FF, 0xFFFFB6FF, 0xFFFF91FF, 0xFFFFB6B6, 0xFFFFDA91, 0xFFFFFF48, 0xFFFFFF6D, 0xFFB6FF48, 0xFF91FF6D, 0xFF48FFDA, 0xFF91DAFF, 0xFF000000, 0xFF000000, 0xFF000000 },
	/* 2C04-0001 */	{ 0xFFFFB6B6, 0xFFDA6DFF, 0xFFFF0000, 0xFF9191FF, 0xFF009191, 0xFF244800, 0xFF484848, 0xFFFF0091, 0xFFFFFFFF, 0xFF6D6D6D, 0xFFFFB600, 0xFFB6006D, 0xFF91006D, 0xFFDADA00, 0xFF6D4800, 0xFFFFFFFF, 0xFF6DB6FF, 0xFFDAB66D, 0xFF6D2400, 0xFF6DDA00, 0xFF91DAFF, 0xFFDAB6FF, 0xFFFFDA91, 0xFF0048FF, 0xFFFFDA00, 0xFF48FFDA, 0xFF000000, 0xFF480000, 0xFFDADADA, 0xFF919191, 0xFFFF00FF, 0xFF002491, 0xFF00006D, 0xFFB6DAFF, 0xFFFFB6FF, 0xFF00FF00, 0xFF00FFFF, 0xFF004848, 0xFF00B66D, 0xFFB600FF, 0xFF000000, 0xFF914800, 0xFFFF91FF, 0xFFB62400, 0xFF9100FF, 0xFF0000DA, 0xFFFF9100, 0xFF000000, 0xFF000000, 0xFF249100, 0xFFB6B6B6, 0xFF006D24, 0xFFB6FF48, 0xFF6D48DA, 0xFFFFFF00, 0xFFDA6D00, 0xFF004800, 0xFF006DDA, 0xFF009100, 0xFF242424, 0xFFFFFF6D, 0xFFFF6DFF, 0xFF916D00, 0xFF91FF6D },
	/* 2C04-0002 */	{ 0xFF000000, 0xFFFFB600, 0xFF916D00, 0xFFB6FF48, 0xFF91FF6D, 0xFFFF6DFF, 0xFF009191, 0xFFB6DAFF, 0xFFFF0000, 0xFF9100FF, 0xFFFFFF6D, 0xFFFF91FF, 0xFFFFFFFF, 0xFFDA6DFF, 0xFF91DAFF, 0xFF009100, 0xFF004800, 0xFF6DB6FF, 0xFFB62400, 0xFFDADADA, 0xFF00B66D, 0xFF6DDA00, 0xFF480000, 0xFF9191FF, 0xFF484848, 0xFFFF00FF, 0xFF00006D, 0xFF48FFDA, 0xFFDAB6FF, 0xFF6D4800, 0xFF000000, 0xFF6D48DA, 0xFF91006D, 0xFFFFDA91, 0xFFFF9100, 0xFFFFB6FF, 0xFF006DDA, 0xFF6D2400, 0xFFB6B6B6, 0xFF0000DA, 0xFFB600FF, 0xFFFFDA00, 0xFF6D6D6D, 0xFF244800, 0xFF0048FF, 0xFF000000, 0xFFDADA00, 0xFFFFFFFF, 0xFFDAB66D, 0xFF242424, 0xFF00FF00, 0xFFDA6D00, 0xFF004848, 0xFF002491, 0xFFFF0091, 0xFF249100, 0xFF000000, 0xFF00FFFF, 0xFF914800, 0xFFFFFF00, 0xFFFFB6B6, 0xFFB6006D, 0xFF006D24, 0xFF919191 },
	/* 2C04-0003 */	{ 0xFFB600FF, 0xFFFF6DFF, 0xFF91FF6D, 0xFFB6B6B6, 0xFF009100, 0xFFFFFFFF, 0xFFB6DAFF, 0xFF244800, 0xFF002491, 0xFF000000, 0xFFFFDA91, 0xFF6D4800, 0xFFFF0091, 0xFFDADADA, 0xFFDAB66D, 0xFF91DAFF, 0xFF9191FF, 0xFF009191, 0xFFB6006D, 0xFF0048FF, 0xFF249100, 0xFF916D00, 0xFFDA6D00, 0xFF00B66D, 0xFF6D6D6D, 0xFF6D48DA, 0xFF000000, 0xFF0000DA, 0xFFFF0000, 0xFFB62400, 0xFFFF91FF, 0xFFFFB6B6, 0xFFDA6DFF, 0xFF004800, 0xFF00006D, 0xFFFFFF00, 0xFF242424, 0xFFFFB600, 0xFFFF9100, 0xFFFFFFFF, 0xFF6DDA00, 0xFF91006D, 0xFF6DB6FF, 0xFFFF00FF, 0xFF006DDA, 0xFF919191, 0xFF000000, 0xFF6D2400, 0xFF00FFFF, 0xFF480000, 0xFFB6FF48, 0xFFFFB6FF, 0xFF914800, 0xFF00FF00, 0xFFDADA00, 0xFF484848, 0xFF006D24, 0xFF000000, 0xFFDAB6FF, 0xFFFFFF6D, 0xFF9100FF, 0xFF48FFDA, 0xFFFFDA00, 0xFF004848 },
	/* 2C04-0004 */	{ 0xFF916D00, 0xFF6D48DA, 0xFF009191, 0xFFDADA00, 0xFF000000, 0xFFFFB6B6, 0xFF002491, 0xFFDA6D00, 0xFFB6B6B6, 0xFF6D2400, 0xFF00FF00, 0xFF00006D, 0xFFFFDA91, 0xFFFFFF00, 0xFF009100, 0xFFB6FF48, 0xFFFF6DFF, 0xFF480000, 0xFF0048FF, 0xFFFF91FF, 0xFF000000, 0xFF484848, 0xFFB62400, 0xFFFF9100, 0xFFDAB66D, 0xFF00B66D, 0xFF9191FF, 0xFF249100, 0xFF91006D, 0xFF000000, 0xFF91FF6D, 0xFF6DB6FF, 0xFFB6006D, 0xFF006D24, 0xFF914800, 0xFF0000DA, 0xFF9100FF, 0xFFB600FF, 0xFF6D6D6D, 0xFFFF0091, 0xFF004848, 0xFFDADADA, 0xFF006DDA, 0xFF004800, 0xFF242424, 0xFFFFFF6D, 0xFF919191, 0xFFFF00FF, 0xFFFFB6FF, 0xFFFFFFFF, 0xFF6D4800, 0xFFFF0000, 0xFFFFDA00, 0xFF48FFDA, 0xFFFFFFFF, 0xFF91DAFF, 0xFF000000, 0xFFFFB600, 0xFFDA6DFF, 0xFFB6DAFF, 0xFF6DDA00, 0xFFDAB6FF, 0xFF00FFFF, 0xFF244800 },
	/* 2C05-01 */		{ 0xFF6D6D6D, 0xFF002491, 0xFF0000DA, 0xFF6D48DA, 0xFF91006D, 0xFFB6006D, 0xFFB62400, 0xFF914800, 0xFF6D4800, 0xFF244800, 0xFF006D24, 0xFF009100, 0xFF004848, 0xFF000000, 0xFF000000, 0xFF000000, 0xFFB6B6B6, 0xFF006DDA, 0xFF0048FF, 0xFF9100FF, 0xFFB600FF, 0xFFFF0091, 0xFFFF0000, 0xFFDA6D00, 0xFF916D00, 0xFF249100, 0xFF009100, 0xFF00B66D, 0xFF009191, 0xFF000000, 0xFF000000, 0xFF000000, 0xFFFFFFFF, 0xFF6DB6FF, 0xFF9191FF, 0xFFDA6DFF, 0xFFFF00FF, 0xFFFF6DFF, 0xFFFF9100, 0xFFFFB600, 0xFFDADA00, 0xFF6DDA00, 0xFF00FF00, 0xFF48FFDA, 0xFF00FFFF, 0xFF000000, 0xFF000000, 0xFF000000, 0xFFFFFFFF, 0xFFB6DAFF, 0xFFDAB6FF, 0xFFFFB6FF, 0xFFFF91FF, 0xFFFFB6B6, 0xFFFFDA91, 0xFFFFFF48, 0xFFFFFF6D, 0xFFB6FF48, 0xFF91FF6D, 0xFF48FFDA, 0xFF91DAFF, 0xFF000000, 0xFF000000, 0xFF000000 },
	/* 2C05-02 */		{ 0xFF6D6D6D, 0xFF002491, 0xFF0000DA, 0xFF6D48DA, 0xFF91006D, 0xFFB6006D, 0xFFB62400, 0xFF914800, 0xFF6D4800, 0xFF244800, 0xFF006D24, 0xFF009100, 0xFF004848, 0xFF000000, 0xFF000000, 0xFF000000, 0xFFB6B6B6, 0xFF006DDA, 0xFF0048FF, 0xFF9100FF, 0xFFB600FF, 0xFFFF0091, 0xFFFF0000, 0xFFDA6D00, 0xFF916D00, 0xFF249100, 0xFF009100, 0xFF00B66D, 0xFF009191, 0xFF000000, 0xFF000000, 0xFF000000, 0xFFFFFFFF, 0xFF6DB6FF, 0xFF9191FF, 0xFFDA6DFF, 0xFFFF00FF, 0xFFFF6DFF, 0xFFFF9100, 0xFFFFB600, 0xFFDADA00, 0xFF6DDA00, 0xFF00FF00, 0xFF48FFDA, 0xFF00FFFF, 0xFF000000, 0xFF000000, 0xFF000000, 0xFFFFFFFF, 0xFFB6DAFF, 0xFFDAB6FF, 0xFFFFB6FF, 0xFFFF91FF, 0xFFFFB6B6, 0xFFFFDA91, 0xFFFFFF48, 0xFFFFFF6D, 0xFFB6FF48, 0xFF91FF6D, 0xFF48FFDA, 0xFF91DAFF, 0xFF000000, 0xFF000000, 0xFF000000 },
	/* 2C05-03 */		{ 0xFF6D6D6D, 0xFF002491, 0xFF0000DA, 0xFF6D48DA, 0xFF91006D, 0xFFB6006D, 0xFFB62400, 0xFF914800, 0xFF6D4800, 0xFF244800, 0xFF006D24, 0xFF009100, 0xFF004848, 0xFF000000, 0xFF000000, 0xFF000000, 0xFFB6B6B6, 0xFF006DDA, 0xFF0048FF, 0xFF9100FF, 0xFFB600FF, 0xFFFF0091, 0xFFFF0000, 0xFFDA6D00, 0xFF916D00, 0xFF249100, 0xFF009100, 0xFF00B66D, 0xFF009191, 0xFF000000, 0xFF000000, 0xFF000000, 0xFFFFFFFF, 0xFF6DB6FF, 0xFF9191FF, 0xFFDA6DFF, 0xFFFF00FF, 0xFFFF6DFF, 0xFFFF9100, 0xFFFFB600, 0xFFDADA00, 0xFF6DDA00, 0xFF00FF00, 0xFF48FFDA, 0xFF00FFFF, 0xFF000000, 0xFF000000, 0xFF000000, 0xFFFFFFFF, 0xFFB6DAFF, 0xFFDAB6FF, 0xFFFFB6FF, 0xFFFF91FF, 0xFFFFB6B6, 0xFFFFDA91, 0xFFFFFF48, 0xFFFFFF6D, 0xFFB6FF48, 0xFF91FF6D, 0xFF48FFDA, 0xFF91DAFF, 0xFF000000, 0xFF000000, 0xFF000000 },
	/* 2C05-04 */		{ 0xFF6D6D6D, 0xFF002491, 0xFF0000DA, 0xFF6D48DA, 0xFF91006D, 0xFFB6006D, 0xFFB62400, 0xFF914800, 0xFF6D4800, 0xFF244800, 0xFF006D24, 0xFF009100, 0xFF004848, 0xFF000000, 0xFF000000, 0xFF000000, 0xFFB6B6B6, 0xFF006DDA, 0xFF0048FF, 0xFF9100FF, 0xFFB600FF, 0xFFFF0091, 0xFFFF0000, 0xFFDA6D00, 0xFF916D00, 0xFF249100, 0xFF009100, 0xFF00B66D, 0xFF009191, 0xFF000000, 0xFF000000, 0xFF000000, 0xFFFFFFFF, 0xFF6DB6FF, 0xFF9191FF, 0xFFDA6DFF, 0xFFFF00FF, 0xFFFF6DFF, 0xFFFF9100, 0xFFFFB600, 0xFFDADA00, 0xFF6DDA00, 0xFF00FF00, 0xFF48FFDA, 0xFF00FFFF, 0xFF000000, 0xFF000000, 0xFF000000, 0xFFFFFFFF, 0xFFB6DAFF, 0xFFDAB6FF, 0xFFFFB6FF, 0xFFFF91FF, 0xFFFFB6B6, 0xFFFFDA91, 0xFFFFFF48, 0xFFFFFF6D, 0xFFB6FF48, 0xFF91FF6D, 0xFF48FFDA, 0xFF91DAFF, 0xFF000000, 0xFF000000, 0xFF000000 },
	/* 2C05-05 */		{ 0xFF6D6D6D, 0xFF002491, 0xFF0000DA, 0xFF6D48DA, 0xFF91006D, 0xFFB6006D, 0xFFB62400, 0xFF914800, 0xFF6D4800, 0xFF244800, 0xFF006D24, 0xFF009100, 0xFF004848, 0xFF000000, 0xFF000000, 0xFF000000, 0xFFB6B6B6, 0xFF006DDA, 0xFF0048FF, 0xFF9100FF, 0xFFB600FF, 0xFFFF0091, 0xFFFF0000, 0xFFDA6D00, 0xFF916D00, 0xFF249100, 0xFF009100, 0xFF00B66D, 0xFF009191, 0xFF000000, 0xFF000000, 0xFF000000, 0xFFFFFFFF, 0xFF6DB6FF, 0xFF9191FF, 0xFFDA6DFF, 0xFFFF00FF, 0xFFFF6DFF, 0xFFFF9100, 0xFFFFB600, 0xFFDADA00, 0xFF6DDA00, 0xFF00FF00, 0xFF48FFDA, 0xFF00FFFF, 0xFF000000, 0xFF000000, 0xFF000000, 0xFFFFFFFF, 0xFFB6DAFF, 0xFFDAB6FF, 0xFFFFB6FF, 0xFFFF91FF, 0xFFFFB6B6, 0xFFFFDA91, 0xFFFFFF48, 0xFFFFFF6D, 0xFFB6FF48, 0xFF91FF6D, 0xFF48FFDA, 0xFF91DAFF, 0xFF000000, 0xFF000000, 0xFF000000 }
};