#pragma once

#include <common.hpp>
#include <logger.hpp>
#include <BEField.hpp>

#include <CellTypes.hpp>


// Circular dependency
class PlayStation3;

using namespace CellTypes;

static constexpr u32 CELL_SAVEDATA_DIRNAME_SIZE     = 32;
static constexpr u32 CELL_SAVEDATA_FILENAME_SIZE    = 13;

static constexpr u32 CELL_SAVEDATA_SYSP_TITLE_SIZE      = 128;
static constexpr u32 CELL_SAVEDATA_SYSP_SUBTITLE_SIZE   = 128;
static constexpr u32 CELL_SAVEDATA_SYSP_DETAIL_SIZE     = 1024;
static constexpr u32 CELL_SAVEDATA_SYSP_LPARAM_SIZE     = 8;

static constexpr u32 CELL_SAVEDATA_FILETYPE_SECUREFILE      = 0;
static constexpr u32 CELL_SAVEDATA_FILETYPE_NORMALFILE      = 1;
static constexpr u32 CELL_SAVEDATA_FILETYPE_CONTENT_ICON0   = 2;
static constexpr u32 CELL_SAVEDATA_FILETYPE_CONTENT_ICON1   = 3;
static constexpr u32 CELL_SAVEDATA_FILETYPE_CONTENT_PIC1    = 4;
static constexpr u32 CELL_SAVEDATA_FILETYPE_CONTENT_SND0    = 5;

static constexpr u32 CELL_SAVEDATA_FILEOP_READ          = 0;
static constexpr u32 CELL_SAVEDATA_FILEOP_WRITE         = 1;
static constexpr u32 CELL_SAVEDATA_FILEOP_DELETE        = 2;
static constexpr u32 CELL_SAVEDATA_FILEOP_WRITE_NOTRUNC = 3;

static constexpr u32 CELL_SAVEDATA_CBRESULT_OK_LAST_NOCONFIRM   =  2;
static constexpr u32 CELL_SAVEDATA_CBRESULT_OK_LAST             =  1;
static constexpr u32 CELL_SAVEDATA_CBRESULT_OK_NEXT             =  0;
static constexpr u32 CELL_SAVEDATA_CBRESULT_ERR_NOSPACE         = -1;
static constexpr u32 CELL_SAVEDATA_CBRESULT_ERR_FAILURE         = -2;
static constexpr u32 CELL_SAVEDATA_CBRESULT_ERR_BROKEN          = -3;
static constexpr u32 CELL_SAVEDATA_CBRESULT_ERR_NODATA          = -4;
static constexpr u32 CELL_SAVEDATA_CBRESULT_ERR_INVALID         = -5;

class CellSaveData {
public:
    CellSaveData(PlayStation3* ps3) : ps3(ps3) {}
    PlayStation3* ps3;

    struct CellSaveDataDirStat {
        BEField<s64> atime;
        BEField<s64> mtime;
        BEField<s64> ctime;
        char dir_name[CELL_SAVEDATA_DIRNAME_SIZE];
    };

    struct CellSaveDataSystemFileParam {
        char title[CELL_SAVEDATA_SYSP_TITLE_SIZE];
        char sub_title[CELL_SAVEDATA_SYSP_SUBTITLE_SIZE];
        char detail[CELL_SAVEDATA_SYSP_DETAIL_SIZE];
        BEField<u32> attribute;
        BEField<u32> parental_level;
        char list_param[CELL_SAVEDATA_SYSP_LPARAM_SIZE];
        char reserved[256];
    };

    struct CellSaveDataFileStat {
        BEField<u32> filetype;
        char reserved1[4];
        BEField<u64> size;
        BEField<s64> atime;
        BEField<s64> mtime;
        BEField<s64> ctime;
        char file_name[CELL_SAVEDATA_FILENAME_SIZE];
        char reserved2[3];
    };

    struct CellSaveDataSetBuf {
        BEField<u32> dir_list_max;
        BEField<u32> file_list_max;
        BEField<u32> reserved[6];
        BEField<u32> buf_size;
        BEField<u32> buf_ptr;
    };

    struct CellSaveDataAutoIndicator {
        BEField<u32> disp_pos;
        BEField<u32> disp_mode;
        BEField<u32> disp_msg_ptr;
        BEField<u32> pic_buf_size;
        BEField<u32> pic_buf_ptr;
        BEField<u32> reserved;
    };

    struct CellSaveDataStatGet {
        BEField<s32> hdd_free_size_kb;
        BEField<u32> is_new_data;
        CellSaveDataDirStat dir;
        CellSaveDataSystemFileParam get_param;
        BEField<u32> bind;
        BEField<s32> size_kb;
        BEField<s32> sys_size_kb;
        BEField<u32> file_num;
        BEField<u32> file_list_num;
        BEField<u32> file_list_ptr; // file_list is array of CellSaveDataFileStat
        char reserved[64];
    };

    struct CellSaveDataStatSet {
        BEField<u32> set_param_ptr; // set_param is CellSaveDataSystemFileParam
        BEField<u32> recreate_mode;
        BEField<u32> indicator;     // indicator is CellSaveDataAutoIndicator
    };

    struct CellSaveDataFileGet {
        BEField<u32> exc_size;
        char reserved[64];
    };

    struct CellSaveDataFileSet {
        BEField<u32> file_op;
        BEField<u32> reserved;
        BEField<u32> filetype;
        u8 secure_file_id[16];
        BEField<u32> file_name;
        BEField<u32> file_offset;
        BEField<u32> filesize;
        BEField<u32> file_buf_size;
        BEField<u32> file_buf;
    };

    struct CellSaveDataCBResult {
        BEField<s32> result;
        BEField<u32> progress_bar_inc;
        BEField<s32> err_need_size_kb;
        BEField<u32> invalid_msg_ptr;
        BEField<u32> userdata_ptr;
    };

    struct CellSaveDataDirList {
        char dir_name[CELL_SAVEDATA_DIRNAME_SIZE];
        char list_param[CELL_SAVEDATA_SYSP_LPARAM_SIZE];
        char reserved[8];
    };

    struct CellSaveDataListGet {
        BEField<u32> dir_num;
        BEField<u32> dir_list_num;
        BEField<u32> dir_list_ptr;  // dir_list is CellSaveDataDirList
        char reserved[64];
    };
    
    u64 handleSaveDataOperation(fs::path savedata_path, u32 stat_cb_ptr, u32 file_cb_ptr, u32 set_buf_ptr, u32 userdata_ptr);

    u64 cellSaveDataUserListAutoLoad();
    u64 cellSaveDataUserAutoSave();
    u64 cellSaveDataAutoSave2();
    u64 cellSaveDataUserAutoLoad();
    u64 cellSaveDataAutoLoad2();

private:
    MAKE_LOG_FUNCTION(log, cellSaveData);
};
