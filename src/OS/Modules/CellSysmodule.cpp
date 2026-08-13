#include "CellSysmodule.hpp"
#include "PlayStation3.hpp"


u64 CellSysmodule::cellSysmoduleUnloadModule() {
    const u16 module_id = ARG0;
    const auto module_name = getModule(module_id);
    log("cellSysmoduleUnloadModule(module: %s)\n", module_name.c_str());

    if (module_name == "BAD")
        Helpers::panic("cellSysmoduleUnloadModule(): bad module id %d\n", module_id);

    return CELL_OK;
}

u64 CellSysmodule::cellSysmoduleLoadModule() {
    const u16 module_id = ARG0;
    const auto module_name = getModule(module_id);
    log("cellSysmoduleLoadModule(module: %s)\n", module_name.c_str());

    if (module_name == "BAD") {
        log("WARNING: cellSysmoduleLoadModule unknown module id %d\n", module_id);
        return 0x80012002;  // CELL_SYSMODULE_ERROR_UNKNOWN
    }

    return CELL_OK;
}

std::string CellSysmodule::getModule(u16 id) {
    if (!module_names.contains(id))
        return "BAD";
    return module_names[id];
}
