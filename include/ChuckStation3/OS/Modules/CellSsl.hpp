#pragma once

#include <common.hpp>
#include <logger.hpp>
#include <BEField.hpp>

#include <CellTypes.hpp>


// Circular dependency
class PlayStation3;

using namespace CellTypes;

class CellSsl {
public:
    CellSsl(PlayStation3* ps3) : ps3(ps3) {}
    PlayStation3* ps3;

    const fs::path cert_dir = "/dev_flash/data/cert/";
    fs::path getCertificatePathByID(u32 id);
    std::string getCertificateByID(u32 id);

    u64 cellSslCertificateLoader();

private:
    MAKE_LOG_FUNCTION(log, cellSsl);
};