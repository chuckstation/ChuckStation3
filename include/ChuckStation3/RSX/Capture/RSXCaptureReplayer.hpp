#pragma once

#include <common.hpp>
#include <logger.hpp>


class PlayStation3;

class RSXCaptureReplayer {
public:
    RSXCaptureReplayer(PlayStation3* ps3) : ps3(ps3) {};
    PlayStation3* ps3;

    void load(fs::path capture_dir);

    static constexpr char CSCF_MAGIC[4] = { 'C', 'S', 'C', 'F' };
    static constexpr char CSCM_MAGIC[4] = { 'C', 'S', 'C', 'M' };

private:
    MAKE_LOG_FUNCTION(log, rsx_capture_replayer);
};