#pragma once

#include <elfio/elfio.hpp>
#include <filesystem>
#include <fstream>
#include <map>
#include <optional>
#include <string>

#include <common.hpp>


class ElfSymbolParser {
    struct Symbol {
        std::string name;
    };

    bool loaded = false;

public:
    std::map<u32, Symbol> symbolMap;
    ElfSymbolParser(const std::filesystem::path& path) {
        using namespace ELFIO;

        elfio reader;
        std::ifstream file(path, std::ios::binary);

        if (!reader.load(file)) {
            //printf("ELF Symbol parser failed to load file");
            return;
        }

        auto sectionCount = reader.sections.size();

        for (int i = 0; i < sectionCount; ++i) {
            ELFIO::section* section = reader.sections[i];

            // Check section type
            if (section->get_type() == ELFIO::SHT_SYMTAB) {
                const symbol_section_accessor symbols(reader, section);
                for (u32 j = 0; j < symbols.get_symbols_num(); ++j) {
                    std::string name;
                    Elf64_Addr address;
                    Elf_Xword size;
                    unsigned char bind;
                    unsigned char type;
                    Elf_Half sectionIndex;
                    unsigned char other;

                    symbols.get_symbol(j, name, address, size, bind, type, sectionIndex, other);

                    Symbol symbol{
                        .name = name,
                    };

                    symbolMap[u32(address)] = symbol;
                }
            }
        }

        loaded = true;
    }

    std::optional<Symbol> getSymbol(u32 address) {
        if (!loaded || !symbolMap.contains(address)) {
            return std::nullopt;
        }

        return symbolMap[address];
    }
};