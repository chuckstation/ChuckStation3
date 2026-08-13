#pragma once

#include <common.hpp>
#include <logger.hpp>
#include <BEField.hpp>

#include <CellTypes.hpp>


// Circular dependency
class PlayStation3;

using namespace CellTypes;

class CellMsgDialog {
public:
    CellMsgDialog(PlayStation3* ps3) : ps3(ps3) {}
    PlayStation3* ps3;

    u64 cellMsgDialogOpen2();

private:
    MAKE_LOG_FUNCTION(log, cellMsgDialog);
};