#pragma once

#include <common.hpp>
#include <logger.hpp>
#include <BEField.hpp>

#include <unordered_map>

#include <Filesystem/Filesystem.hpp>


class SFOLoader {
public:
    SFOLoader(Filesystem& fs) : fs(fs) {}
    Filesystem& fs;

    struct SFOData {
        std::unordered_map<std::string, std::u8string> strings;
        std::unordered_map<std::string, u32> ints;
    };

    SFOData parse(fs::path guest_path);
    void save(fs::path guest_path, SFOData& sfo_data);

    struct SFOHeader {
        u32 magic; // Always " PSF"
        u32 version;
        u32 key_table_start;
        u32 data_table_start;
        u32 n_entries;
    };

    struct SFOIndexEntry {
        u16 key_offset;
        u16 data_fmt;
        u32 data_len;
        u32 data_max_len;
        u32 data_offset;
    };

private:
    MAKE_LOG_FUNCTION(log, loader_sfo);
};
