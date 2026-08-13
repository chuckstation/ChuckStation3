#pragma once

#include <common.hpp>
#include <logger.hpp>
#include <BEField.hpp>

#include <CellTypes.hpp>

#include <lodepng.h>

// Circular dependency
class PlayStation3;

using namespace CellTypes;

constexpr static u32 PNG_CODEC_VERSION = 0x00420000;

class CellPngDec {
public:
    CellPngDec(PlayStation3* ps3) : ps3(ps3) {}
    PlayStation3* ps3;


    struct CellPngDecSrc {
        BEField<u32> src_sel;
        BEField<u32> filename_ptr;
        BEField<s64> file_offs;
        BEField<u32> file_size;
        BEField<u32> stream_ptr;
        BEField<u32> stream_size;
        BEField<u32> spu_thread_enable;
    };

    struct CellPngDecThreadOutParam {
        BEField<u32> codec_version;
    };

    struct CellPngDecSrcLE {
        u32 src_sel;
        u8* filename;
        s64 file_offs;
        u32 file_size;
        u8* stream;
        u32 stream_size;
        u32 spu_thread_enable;
    };

    struct CellPngDecInfo {
        BEField<u32> width;
        BEField<u32> height;
        BEField<u32> n_components;
        BEField<s32> color_space;
        BEField<u32> bit_depth;
        BEField<s32> interlace_method;
        BEField<u32> chunk_info;
    };

    struct CellPngDecInfoLE {
        u32 width;
        u32 height;
        u32 n_components;
        s32 color_space;
        u32 bit_depth;
        s32 interlace_method;
        u32 chunk_info;
    };

    struct CellPngDecInParam {
        BEField<u32> cmd_ptr;
        BEField<u32> output_mode;
        BEField<s32> output_color_space;
        BEField<u32> output_bit_depth;
        BEField<u32> output_pack_flag;
        BEField<u32> output_alpha_sel;
        BEField<u32> output_col_alpha;
    };

    struct CellPngDecOutParam {
        BEField<u64> output_width_byte;
        BEField<u32> output_width;
        BEField<u32> output_height;
        BEField<u32> output_components;
        BEField<u32> output_bit_depth;
        BEField<s32> output_mode;
        BEField<s32> output_color_space;
        BEField<u32> use_mem_space;
    };

    struct CellPngDecOutParamLE {
        u64 output_width_byte;
        u32 output_width;
        u32 output_height;
        u32 output_components;
        u32 output_bit_depth;
        s32 output_mode;
        s32 output_color_space;
        u32 use_mem_space;
    };

    struct CellPngDecDataOutInfo {
        BEField<u32> chunkInformation;
        BEField<u32> numText;
        BEField<u32> numUnknownChunk;
        BEField<s32> status;
    };

    enum CellPngDecStreamSrcSel : u32 {
        CELL_PNGDEC_FILE = 0,
        CELL_PNGDEC_BUFFER = 1,
    };

    struct CellPngDecDataCtrlParam {
        BEField<u64> output_bytes_per_line;
    };

    enum CellPngDecColorSpace : s32 {
        CELL_PNGDEC_GRAYSCALE = 1,
        CELL_PNGDEC_RGB = 2,
        CELL_PNGDEC_PALETTE = 4,
        CELL_PNGDEC_GRAYSCALE_ALPHA = 9,
        CELL_PNGDEC_RGBA = 10,
        CELL_PNGDEC_ARGB = 20,
    };

    CellPngDecSrcLE curr_src;
    CellPngDecInfoLE curr_info;
    CellPngDecOutParamLE out_param;
    fs::path curr_file;
    std::vector<u8> buf;

    void displayPNGInfo(const LodePNGInfo& info);

    u64 cellPngDecDecodeData();
    u64 cellPngDecClose();
    u64 cellPngDecDestroy();
    u64 cellPngDecReadHeader();
    u64 cellPngDecCreate();
    u64 cellPngDecOpen();
    u64 cellPngDecSetParameter();

private:
    MAKE_LOG_FUNCTION(log, cellPngDec);
};