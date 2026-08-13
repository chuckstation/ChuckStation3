#pragma once

#include <common.hpp>
#include <logger.hpp>
#include <opengl.hpp>

#include <unordered_map>

#include <xxhash.h>


class RSXCache {
public:
    RSXCache() {
        shader_cache.clear();
        program_cache.clear();
        texture_cache.clear();
    }

    struct ProgramHash {
        u64 hash_vertex;
        u64 hash_fragment;

        bool operator==(const ProgramHash& other) {
            return hash_vertex == other.hash_vertex && hash_fragment == other.hash_fragment;
        }
    };

    struct CachedShader {
        OpenGL::Shader shader;
    };
    
    bool getShader(u64 hash, CachedShader& shader) {
        if (shader_cache.contains(hash)) {
            shader = shader_cache[hash];
            //log("Got cached shader: %016x\n", hash);
            return true;
        }
        return false;
    }

    bool getProgram(u64 hash, OpenGL::Program& program) {
        if (program_cache.contains(hash)) {
            program = program_cache[hash];
            //log("Got cached program: %016x\n", hash);
            return true;
        }
        return false;
    }

    void cacheShader(u64 hash, CachedShader shader) {
        shader_cache[hash] = shader;
        log("Cached new shader: %016x\n", hash);
    }

    void cacheProgram(u64 hash, OpenGL::Program& program) {
        program_cache[hash] = program;
        log("Cached new program: %016x\n", hash);
    }

    u64 computeHash(u8* ptr, size_t size) {
        return XXH3_64bits(ptr, size);
    }

    u64 computeProgramHash(u64 hash_vertex, u64 hash_fragment) {
        u64 hashes[2] = { hash_vertex, hash_fragment };
        return computeHash((u8*)&hashes[0], sizeof(u64) * 2);
    }

    u64 computeTextureHash(u8* ptr, u64 width, u64 height, u64 n_components) {
        const size_t size = width * height * n_components;
        return computeHash(ptr, size);
    }

    bool getTexture(u64 hash, OpenGL::Texture& texture) {
        if (texture_cache.contains(hash)) {
            texture = texture_cache[hash];
            //log("Got cached texture: %016x\n", hash);
            return true;
        }
        return false;
    }

    void cacheTexture(u64 hash, OpenGL::Texture& texture) {
        texture_cache[hash] = texture;
        log("Cached new texture: %016x\n", hash);
    }

    bool getFramebuffer(u32 addr, OpenGL::Texture& framebuffer) {
        if (framebuffer_cache.contains(addr)) {
            framebuffer = framebuffer_cache[addr];
            //log("Got cached framebuffer: %032x\n", addr);
            return true;
        }
        return false;
    }

    void cacheFramebuffer(u32 addr, OpenGL::Texture& framebuffer) {
        framebuffer_cache[addr] = framebuffer;
        log("Cached new framebuffer: 0x%08x\n", addr);
    }

private:
    MAKE_LOG_FUNCTION(log, rsx_cache);

    // TODO: Might change this to an std::vector of "CachedShader" structs or something
    std::unordered_map<u64, CachedShader> shader_cache;
    std::unordered_map<u64, OpenGL::Program> program_cache;
    std::unordered_map<u64, OpenGL::Texture> texture_cache;
    std::unordered_map<u32, OpenGL::Texture> framebuffer_cache;
};
