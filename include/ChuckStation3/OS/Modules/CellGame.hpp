#pragma once

#include <common.hpp>
#include <logger.hpp>
#include <BEField.hpp>

#include <unordered_map>

#include <CellTypes.hpp>


// Circular dependency
class PlayStation3;

using namespace CellTypes;

static constexpr u64 CELL_GAME_ERROR_NOTPATCH           = 0x8002cb27;
static constexpr u64 CELL_DISCGAME_ERROR_NOT_DISCBOOT   = 0x8002bd02;
static constexpr u64 CELL_GAME_RET_NONE                 = 2;
static constexpr u64 CELL_GAME_ERROR_PARAM              = 0x8002cb07;

static constexpr u32 CELL_GAMEDATA_CBRESULT_OK_CANCEL   = 1;
static constexpr u32 CELL_GAMEDATA_CBRESULT_OK          = 0;
static constexpr u32 CELL_GAMEDATA_CBRESULT_ERR_NOSPACE = -1;
static constexpr u32 CELL_GAMEDATA_CBRESULT_ERR_BROKEN  = -3;
static constexpr u32 CELL_GAMEDATA_CBRESULT_ERR_NODATA  = -4;
static constexpr u32 CELL_GAMEDATA_CBRESULT_ERR_INVALID = -5;

static constexpr u32 CELL_GAMEDATA_SIZEKB_NOTCALC = -1;

static constexpr u32 CELL_GAMEDATA_PATH_MAX             = 1055;
static constexpr u32 CELL_GAMEDATA_SYSP_LANGUAGE_NUM    = 20;
static constexpr u32 CELL_GAMEDATA_SYSP_TITLE_SIZE      = 128;
static constexpr u32 CELL_GAMEDATA_SYSP_TITLEID_SIZE    = 10;
static constexpr u32 CELL_GAMEDATA_SYSP_VERSION_SIZE    = 6;

class CellGame {
public:
    CellGame(PlayStation3* ps3) : ps3(ps3) {}
    PlayStation3* ps3;

    enum CellGameType : u32 {
        CELL_GAME_GAMETYPE_SYS,
        CELL_GAME_GAMETYPE_DISC,
        CELL_GAME_GAMETYPE_HDD,
        CELL_GAME_GAMETYPE_GAMEDATA,
        CELL_GAME_GAMETYPE_HOME,
    };

    enum CellGameParamID : u32{
        // Strings
        CELL_GAME_PARAMID_TITLE = 0,
        CELL_GAME_PARAMID_TITLE_DEFAULT = 1,
        CELL_GAME_PARAMID_TITLE_JAPANESE = 2,
        CELL_GAME_PARAMID_TITLE_ENGLISH = 3,
        CELL_GAME_PARAMID_TITLE_FRENCH = 4,
        CELL_GAME_PARAMID_TITLE_SPANISH = 5,
        CELL_GAME_PARAMID_TITLE_GERMAN = 6,
        CELL_GAME_PARAMID_TITLE_ITALIAN = 7,
        CELL_GAME_PARAMID_TITLE_DUTCH = 8,
        CELL_GAME_PARAMID_TITLE_PORTUGUESE = 9,
        CELL_GAME_PARAMID_TITLE_RUSSIAN = 10,
        CELL_GAME_PARAMID_TITLE_KOREAN = 11,
        CELL_GAME_PARAMID_TITLE_CHINESE_T = 12,
        CELL_GAME_PARAMID_TITLE_CHINESE_S = 13,
        CELL_GAME_PARAMID_TITLE_FINNISH = 14,
        CELL_GAME_PARAMID_TITLE_SWEDISH = 15,
        CELL_GAME_PARAMID_TITLE_DANISH = 16,
        CELL_GAME_PARAMID_TITLE_NORWEGIAN = 17,
        CELL_GAME_PARAMID_TITLE_POLISH = 18,
        CELL_GAME_PARAMID_TITLE_PORTUGUESE_BRAZIL = 19,
        CELL_GAME_PARAMID_TITLE_ENGLISH_UK = 20,
        CELL_GAME_PARAMID_TITLE_TURKISH = 21,
        CELL_GAME_PARAMID_TITLE_ID = 100,
        CELL_GAME_PARAMID_VERSION = 101,
        CELL_GAME_PARAMID_PS3_SYSTEM_VER = 105,
        CELL_GAME_PARAMID_APP_VER = 106,

        // Ints
        CELL_GAME_PARAMID_PARENTAL_LEVEL = 102,
        CELL_GAME_PARAMID_RESOLUTION = 103,
        CELL_GAME_PARAMID_SOUND_FORMAT = 104,
    };

    std::unordered_map<u32, std::string> id_to_param {
        // Strings
        { CELL_GAME_PARAMID_TITLE,                    "TITLE"          }, 
        { CELL_GAME_PARAMID_TITLE_DEFAULT,            "TITLE"          },
        { CELL_GAME_PARAMID_TITLE_JAPANESE,           "TITLE_00"       },
        { CELL_GAME_PARAMID_TITLE_ENGLISH,            "TITLE_01"       },
        { CELL_GAME_PARAMID_TITLE_FRENCH,             "TITLE_02"       },
        { CELL_GAME_PARAMID_TITLE_SPANISH,            "TITLE_03"       },
        { CELL_GAME_PARAMID_TITLE_GERMAN,             "TITLE_04"       },
        { CELL_GAME_PARAMID_TITLE_ITALIAN,            "TITLE_05"       },
        { CELL_GAME_PARAMID_TITLE_DUTCH,              "TITLE_06"       },
        { CELL_GAME_PARAMID_TITLE_PORTUGUESE,         "TITLE_07"       },
        { CELL_GAME_PARAMID_TITLE_RUSSIAN,            "TITLE_08"       },
        { CELL_GAME_PARAMID_TITLE_KOREAN,             "TITLE_09"       },
        { CELL_GAME_PARAMID_TITLE_CHINESE_T,          "TITLE_10"       },
        { CELL_GAME_PARAMID_TITLE_CHINESE_S,          "TITLE_11"       },
        { CELL_GAME_PARAMID_TITLE_FINNISH,            "TITLE_12"       },
        { CELL_GAME_PARAMID_TITLE_SWEDISH,            "TITLE_13"       },
        { CELL_GAME_PARAMID_TITLE_DANISH,             "TITLE_14"       },
        { CELL_GAME_PARAMID_TITLE_NORWEGIAN,          "TITLE_15"       },
        { CELL_GAME_PARAMID_TITLE_POLISH,             "TITLE_16"       },
        { CELL_GAME_PARAMID_TITLE_PORTUGUESE_BRAZIL,  "TITLE_17"       },
        { CELL_GAME_PARAMID_TITLE_ENGLISH_UK,         "TITLE_18"       },
        { CELL_GAME_PARAMID_TITLE_TURKISH,            "TITLE_19"       },
        { CELL_GAME_PARAMID_TITLE_ID,                 "TITLE_ID"       },
        { CELL_GAME_PARAMID_VERSION,                  "VERSION"        },
        { CELL_GAME_PARAMID_PS3_SYSTEM_VER,           "PS3_SYSTEM_VER" },
        { CELL_GAME_PARAMID_APP_VER,                  "APP_VER"        },

        // Ints
        { CELL_GAME_PARAMID_PARENTAL_LEVEL,           "PARENTAL_LEVEL" },
        { CELL_GAME_PARAMID_RESOLUTION,               "RESOLUTION"     },
        { CELL_GAME_PARAMID_SOUND_FORMAT,             "SOUND_FORMAT"   },
    };

    struct CellGameContentSize {    // All sizes are in KB
        BEField<s32> hdd_free;
        BEField<s32> size;
        BEField<s32> sys_size;
    };

    struct CellGameDataSystemFileParam {
        char title[CELL_GAMEDATA_SYSP_TITLE_SIZE];
        char title_lang[CELL_GAMEDATA_SYSP_LANGUAGE_NUM][CELL_GAMEDATA_SYSP_TITLE_SIZE];
        char title_id[CELL_GAMEDATA_SYSP_TITLEID_SIZE];
        char reserved0[2];
        char data_ver[CELL_GAMEDATA_SYSP_VERSION_SIZE];
        char reserved1[2];
        BEField<u32> parental_level;
        BEField<u32> attribute;
        BEField<u32> resolution;
        BEField<u32> sound_format;
        char reserved2[248];
    };

    struct CellGameDataStatGet {
        BEField<s32> hdd_free_size_kb;
        BEField<u32> is_new_data;
        char content_info_path[CELL_GAMEDATA_PATH_MAX];
        char game_data_path[CELL_GAMEDATA_PATH_MAX];
        char reserved0[2];
        BEField<s64> st_atime;
        BEField<s64> st_mtime;
        BEField<s64> st_ctime;
        CellGameDataSystemFileParam get_param;
        BEField<s32> size_kb;
        BEField<s32> sys_size_kb;
        char reserved1[68];
    };

    struct CellGameDataStatSet
    {
        BEField<u32> set_param_ptr;
        BEField<u32> reserved;
    };

    struct CellGameDataCBResult {
        BEField<s32> result;
        BEField<s32> err_need_size_kb;
        BEField<u32> invalid_msg_ptr;
        BEField<u32> reserved;
    };

    u32 stat_get_ptr  = 0;
    u32 stat_set_ptr  = 0;
    u32 cb_result_ptr = 0;

    std::string content_path = "/dev_hdd0/game/STUB12345\0\0";
    void setContentPath(fs::path path);

    u64 cellGameGetParamString();
    u64 cellGameContentPermit();
    u64 cellGameContentErrorDialog();
    u64 cellGameGetParamInt();
    u64 cellGameDataCheckCreate2();
    u64 cellGamePatchCheck();
    u64 cellGameDataCheck();
    u64 cellGameBootCheck();

    u64 cellHddGameCheck();
    
    u64 cellDiscGameGetBootDiscInfo();

private:
    MAKE_LOG_FUNCTION(log, cellGame);
};
