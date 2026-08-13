#pragma once

#include <common.hpp>
#include <logger.hpp>
#include <BEField.hpp>

#include <unordered_map>

#include <CellTypes.hpp>


// Circular dependency
class PlayStation3;

using namespace CellTypes;

class CellSysmodule {
public:
    CellSysmodule(PlayStation3* ps3) : ps3(ps3) {}
    PlayStation3* ps3;

    u64 cellSysmoduleUnloadModule();
    u64 cellSysmoduleLoadModule();

    std::string getModule(u16 id);
    std::unordered_map<u16, std::string> module_names {
        { 0x0000, "sys_net" },
        { 0x0001, "cellHttp" },
        { 0x0002, "cellHttpUtil" },
        { 0x0003, "cellSsl" },
        { 0x0004, "cellHttps" },
        { 0x0005, "libvdec" },
        { 0x0006, "cellAdec" },
        { 0x0007, "cellDmux" },
        { 0x0008, "cellVpost" },
        { 0x0009, "cellRtc" },
        { 0x000a, "cellSpurs" },
        { 0x000b, "cellOvis" },
        { 0x000c, "cellSheap" },
        { 0x000d, "cellSync" },
        { 0x000e, "sys_fs" },
        { 0x000f, "cellJpgDec" },
        { 0x0010, "cellGcmSys" },
        { 0x0011, "cellAudio" },
        { 0x0012, "cellPamf" },
        { 0x0013, "cellAtrac" },
        { 0x0014, "cellNetCtl" },
        { 0x0015, "cellSysutil" },
        { 0x0016, "sceNp" },
        { 0x0017, "sys_io" },
        { 0x0018, "cellPngDec" },
        { 0x0019, "cellFont" },
        { 0x001a, "cellFontFT" },
        { 0x001b, "cell_FreeType2" },
        { 0x001c, "cellUsbd" },
        { 0x001d, "cellSail" },
        { 0x001e, "cellL10n" },
        { 0x001f, "cellResc" },
        { 0x0020, "cellDaisy" },
        { 0x0021, "cellKey2char" },
        { 0x0022, "cellMic" },
        { 0x0023, "cellCamera" },
        { 0x0024, "cellVdecMpeg2" },
        { 0x0025, "cellVdecAvc" },
        { 0x0026, "cellAdecLpcm" },
        { 0x0027, "cellAdecAc3" },
        { 0x0028, "cellAdecAtx" },
        { 0x0029, "cellAdecAt3" },
        { 0x002a, "cellDmuxPamf" },
        { 0x002b, "BAD" },
        { 0x002c, "BAD" },
        { 0x002d, "BAD" },
        { 0x002e, "sys_lv2dbg" },
        { 0x002f, "cellSysutilAvcExt" },
        { 0x0030, "cellUsbPspcm" },
        { 0x0031, "cellSysutilAvconfExt" },
        { 0x0032, "cellUserInfo" },
        { 0x0033, "cellSaveData" },
        { 0x0034, "cellSubDisplay" },
        { 0x0035, "cellRec" },
        { 0x0036, "cellVideoExportUtility" },
        { 0x0037, "cellGameExec" },
        { 0x0038, "sceNp2" },
        { 0x0039, "cellSysutilAp" },
        { 0x003a, "sceNpClans" },
        { 0x003b, "cellOskExtUtility" },
        { 0x003c, "cellVdecDivx" },
        { 0x003d, "cellJpgEnc" },
        { 0x003e, "cellGame" },
        { 0x003f, "cellBGDLUtility" },
        { 0x0040, "cell_FreeType2" },
        { 0x0041, "cellVideoUpload" },
        { 0x0042, "cellSysconfExtUtility" },
        { 0x0043, "cellFiber" },
        { 0x0044, "sceNpCommerce2" },
        { 0x0045, "sceNpTus" },
        { 0x0046, "cellVoice" },
        { 0x0047, "cellAdecCelp8" },
        { 0x0048, "cellCelp8Enc" },
        { 0x0049, "cellSysutilMisc" },
        { 0x004a, "cellMusicUtility" },
        { 0x004b, "libad_core" },
        { 0x004c, "libad_async" },
        { 0x004d, "libad_billboard_util" },
        { 0x004e, "cellScreenShotUtility" },
        { 0x004f, "cellMusicDecodeUtility" },
        { 0x0050, "cellSpursJq" },
        { 0x0052, "cellPngEnc" },
        { 0x0053, "cellMusicDecodeUtility" },
        { 0x0054, "libmedi" },
        { 0x0055, "cellSync2" },
        { 0x0056, "sceNpUtil" },
        { 0x0057, "cellRudp" },
        { 0x0059, "sceNpSns" },
        { 0x005a, "libgem" },
        { 0x005c, "cellCrossController" },
        { 0xf00a, "cellCelpEnc" },
        { 0xf010, "cellGifDec" },
        { 0xf019, "cellAdecCelp" },
        { 0xf01b, "cellAdecM2bc" },
        { 0xf01d, "cellAdecM4aac" },
        { 0xf01e, "cellAdecMp3" },
        { 0xf023, "cellImeJpUtility" },
        { 0xf028, "cellMusicUtility" },
        { 0xf029, "cellPhotoUtility" },
        { 0xf02a, "cellPrintUtility" },
        { 0xf02b, "cellPhotoImportUtil" },
        { 0xf02c, "cellMusicExportUtility" },
        { 0xf02e, "cellPhotoDecodeUtil" },
        { 0xf02f, "cellSearchUtility" },
        { 0xf030, "cellSysutilAvc2" },
        { 0xf034, "cellSailRec" },
        { 0xf035, "sceNpTrophy" },
        { 0xf044, "cellSysutilNpEula" },
        { 0xf053, "cellAdecAt3multi" },
        { 0xf054, "cellAtracMulti" }
    };

private:
    MAKE_LOG_FUNCTION(log, cellSysmodule);
};