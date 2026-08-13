#include "CellSsl.hpp"
#include "PlayStation3.hpp"


fs::path CellSsl::getCertificatePathByID(u32 id) {
    fs::path cert_path = cert_dir / std::format("CA{:02d}.cer", id);
    Helpers::debugAssert(ps3->fs.exists(cert_path), "Could not find required certificate %s\n", cert_path.generic_string().c_str());

    return cert_path;
}

std::string CellSsl::getCertificateByID(u32 id) {
    const auto path = getCertificatePathByID(id);
    const auto local_path = ps3->fs.guestPathToHost(path);
    std::string cer = "";

    cer.reserve(2_KB);  // Size of almost all certificates
    for (const auto& i : Helpers::readBinary(local_path))
        cer += i;

    return cer;
}

// This function will be called once with req_ptr set, and once with buf_ptr set.
// The first time, we need to return in req_ptr the amount of bytes needed to store all the certificates
// required by flag, the second time we actually store them in the buffer.
u64 CellSsl::cellSslCertificateLoader() {
    const u64 flag = ARG0;
    const u32 buf_ptr = ARG1;
    const u32 size = ARG2;
    const u32 req_ptr = ARG3;
    log("cellSslCertificateLoader(flag: 0x%016llx, buf_ptr: 0x%08x, size: %d, req_ptr: 0x%08x)\n", flag, buf_ptr, size, req_ptr);

    if (!buf_ptr) {
        size_t req = 0;

        for (int i = 0; i < 64; i++) {
            if ((flag >> i) & 1)
                req += getCertificateByID(i + 1).length();
        }
        ps3->mem.write<u32>(req_ptr, req);
    }
    else {
        std::string buf = "";
        buf.reserve(2_KB * 60);

        for (int i = 0; i < 64; i++) {
            if ((flag >> i) & 1)
                buf += getCertificateByID(i + 1);
        }

        Helpers::debugAssert(size >= buf.length(), "cellSslCertificateLoader(): buf is not big enough to fit certificates\n");
        std::memset(ps3->mem.getPtr(buf_ptr), 0, size);
        std::memcpy(ps3->mem.getPtr(buf_ptr), buf.c_str(), buf.size());
    }

    return CELL_OK;
}