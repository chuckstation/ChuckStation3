#include "RSXCaptureReplayer.hpp"
#include "PlayStation3.hpp"


void RSXCaptureReplayer::load(fs::path capture_dir) {
    log("Loading capture %s\n", capture_dir.generic_string().c_str());
    Helpers::debugAssert(fs::is_directory(capture_dir), "RSXCaptureReplayer::load: path %s is not a directory\n", capture_dir.generic_string().c_str());
    Helpers::debugAssert(fs::is_directory(capture_dir / "memblocks"), "RSXCaptureReplayer::load: capture %s has no memblocks\n", capture_dir.filename().generic_string().c_str());
    Helpers::debugAssert(fs::exists(capture_dir / "capture.cscf"), "RSXCaptureReplayer::load: path %s is not a valid capture (missing capture.cscf)\n");

    auto fifo = Helpers::readBinary(capture_dir / "capture.cscf");
    Helpers::debugAssert(*(u32*)&fifo[0] == *(u32*)CSCF_MAGIC, "RSXCaptureReplayer::load: capture.cscf is not valid\n");

    // Allocate some space for local variables
    auto vars_entry = ps3->mem.alloc(1_MB);
    u32 curr_var_ptr = vars_entry->vaddr;

    // Init cellGcm, but don't init IO (by passing 0 as IO size) (we will do that manually later)
    u32 ctx_ptr = curr_var_ptr;
    curr_var_ptr += sizeof(u32);

    log("* Initializing cellGcm\n");
    ARG0 = ctx_ptr;
    ARG1 = 0;
    ARG2 = 0;
    ARG3 = 0;
    ps3->module_manager.cellGcmSys.cellGcmInitBody();
    ps3->module_manager.cellGcmSys.gcm_config.io_addr = 0x10000000;
    CellGcmSys::CellGcmContextData* ctx = (CellGcmSys::CellGcmContextData*)ps3->mem.getPtr(ps3->mem.read<u32>(ctx_ptr));

    // Allocate FIFO data
    log("* Initializing RSX FIFO data\n");
    const auto io_size = fifo.size() + 1_MB;
    auto fifo_entry = ps3->mem.alloc(io_size, 0x30000000);  // Must be aligned to 1 MB (1 << 20) boundary, we can just place it at 0x30000000
    // Copy FIFO data
    std::memcpy(ps3->mem.getPtr(fifo_entry->vaddr), fifo.data() + 8, fifo.size() - 8);

    // Map RSX IO
    log("* Initializing RSX IO memory\n");
    const u32 start_offs = 0x10000000;
    for (u32 i = 0; i < io_size >> 20; i++)
        ps3->module_manager.cellGcmSys.mapEaIo(fifo_entry->vaddr + (i << 20), start_offs + (i << 20));

    // Initialize fifo control
    const u32 end_offs = *(u32*)&fifo[4];
    CellGcmSys::CellGcmControl* ctrl = (CellGcmSys::CellGcmControl*)ps3->mem.getPtr(ps3->module_manager.cellGcmSys.cellGcmGetControlRegister());
    ctrl->get = start_offs;
    ctrl->put = end_offs;
    //ps3->mem.write<u32>(ps3->rsx.ioToEa(end_offs), 0);

    // Setup memory blocks
    log("* Loading memory blocks\n");
    for (auto& i : fs::directory_iterator(capture_dir / "memblocks")) {
        // Load CSCM file
        auto memblock = Helpers::readBinary(i.path());
        Helpers::debugAssert(*(u32*)&memblock[0] == *(u32*)CSCM_MAGIC, "RSXCaptureReplayer::load: memblock %s is not valid\n", i.path().filename().generic_string().c_str());
        const u32 addr = *(u32*)&memblock[4];
        const u32 size = memblock.size() - 8;
        // Allocate memory
        if (addr >= RSX_VIDEO_MEM_START) {
            // Allocate new memory if it wasn't already mapped
            if (!ps3->mem.rsx.isMapped(addr).first) {
                const auto block = ps3->mem.rsx.allocPhys(size);
                ps3->mem.rsx.mmap(addr, block->start, size);
            }
        }
        else {
            if (!ps3->mem.isMapped(addr).first) {
                const auto block = ps3->mem.allocPhys(size);
                ps3->mem.mmap(addr, block->start, size);
            }
        }
        // Copy the block
        std::memcpy(ps3->mem.getPtr(addr), memblock.data() + 8, size);
        log("* Loaded memblock at 0x%08x with size 0x%08x\n", addr, size);
    }

    // Execute
    log("Done, executing...\n");
    ps3->rsx.runCommandList();
    ps3->flip();
    ctrl->get = start_offs;
    ps3->rsx.runCommandList();
    ps3->flip();
}