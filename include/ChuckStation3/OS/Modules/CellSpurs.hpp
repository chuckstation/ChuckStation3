#pragma once

#include <common.hpp>
#include <logger.hpp>
#include <BEField.hpp>

#include <CellTypes.hpp>


// Circular dependency
class PlayStation3;

using namespace CellTypes;

static constexpr size_t CELL_SPURS_WORKLOAD_ATTRIBUTE_SIZE = 512;

class CellSpurs {
public:
    CellSpurs(PlayStation3* ps3) : ps3(ps3) {}
    PlayStation3* ps3;

    struct alignas(8) CellSpursTasksetAttribute {
        BEField<u32> revision;           
        BEField<u32> sdk_version;        
        BEField<u64> args;               
        u8 priority[8];               
        BEField<u32> max_contention;     
        BEField<u32> name_ptr;         
        BEField<u32> taskset_size;       
        BEField<s32> enable_clear_ls;    
        u8 reserved[472];
    };

    struct alignas(128) CellSpursTaskset {
        v128 running;                           // 0x00
        v128 ready;                             // 0x10
        v128 pending_ready;                     // 0x20
        v128 enabled;                           // 0x30
        v128 signalled;                         // 0x40
        v128 waiting;                           // 0x50
        BEField<u64> spurs_ptr;                 // 0x60
        BEField<u64> args;                      // 0x68
        u8 enable_clear_ls;                     // 0x70
        u8 x71;                                 // 0x71
        u8 wkl_flag_wait_task;                  // 0x72
        u8 last_scheduled_task;                 // 0x73
        BEField<u32> wid;                       // 0x74
        BEField<u64> x78;                       // 0x78
        u8 task_info[128 * 48];                 // 0x80 (48 == sizeof(TaskInfo)
        BEField<u64> exception_handler_ptr;     // 0x1880
        BEField<u64> exception_handler_arg_ptr; // 0x1888
        BEField<u32> size;                      // 0x1890
        u32 unk2;                               // 0x1894
        u32 event_flag_id1;                     // 0x1898
        u32 event_flag_id2;                     // 0x189C
        u8 unk3[0x60];                          // 0x18A0
    };

    struct CellSpursWorkloadAttribute {
        BEField<u32> revision;
        BEField<u32> sdk_ver;
        BEField<u32> pm_ptr;
        BEField<u32> size;
        BEField<u64> data;
        u8 prio[8];
        BEField<u32> min_contention;
        BEField<u32> max_contention;
        BEField<u32> name_class_ptr;
        BEField<u32> name_instance_ptr;
        BEField<u32> hook_ptr;
        BEField<u32> hook_arg_ptr;
        u8 padding[456];
    };
    static_assert(sizeof(CellSpursWorkloadAttribute) == CELL_SPURS_WORKLOAD_ATTRIBUTE_SIZE);

    struct CellSpursWorkloadFlag {
        BEField<u64> unused0;
        BEField<u32> unused1;
        BEField<u32> flag;
    };

    u32 spurs_thread_id = 0;

    struct Task {
        u32 handle = 0;
        u32 elf_ptr = 0;
        u32 arg_ptr = 0;
    };

    struct Taskset {
        u32 handle = 0;
        std::vector<Task> tasks;

        Task* getTask(u32 handle) {
            for (auto& i : tasks) {
                if (i.handle == handle) return &i;
            }
            return nullptr;
        }
    };

    std::vector<Taskset> tasksets;
    Taskset* getTaskset(u32 handle) {
        for (auto& i : tasksets) {
            if (i.handle == handle) return &i;
        }
        return nullptr;
    }

    u64 _cellSpursLFQueueInitialize();
    u64 cellSpursAttributeSetNamePrefix();
    u64 cellSpursAttributeEnableSpuPrintfIfAvailable();
    u64 _cellSpursTasksetAttributeInitialize();
    u64 cellSpursLFQueueAttachLv2EventQueue();
    u64 cellSpursRequestIdleSpu();
    u64 cellSpursCreateTaskWithAttribute();
    u64 cellSpursGetInfo();
    u64 cellSpursInitializeWithAttribute2();
    u64 cellSpursWorkloadAttributeSetName();
    u64 cellSpursCreateTaskset2();
    u64 cellSpursEventFlagClear();
    u64 cellSpursCreateTaskset();
    u64 _cellSpursEventFlagInitialize();
    u64 cellSpursTasksetAttributeSetName();
    u64 cellSpursAttributeSetMemoryContainerForSpuThread();
    u64 cellSpursEventFlagAttachLv2EventQueue();
    u64 _cellSpursLFQueuePushBody();
    u64 _cellSpursAttributeInitialize();
    u64 cellSpursAttributeEnableSystemWorkload();
    u64 _cellSpursWorkloadFlagReceiver();
    u64 cellSpursAttributeSetSpuThreadGroupType();
    u64 cellSpursInitializeWithAttribute();
    u64 cellSpursInitialize();
    u64 _cellSpursTaskAttributeInitialize();
    u64 cellSpursAttachLv2EventQueue();
    u64 cellSpursCreateTask();
    u64 cellSpursAddWorkloadWithAttribute();
    u64 cellSpursCreateTasksetWithAttribute();
    u64 _cellSpursTasksetAttribute2Initialize();
    u64 cellSpursGetWorkloadFlag();
    u64 cellSpursSetExceptionEventHandler();
    u64 _cellSpursWorkloadAttributeInitialize();

    // ***** SPU SIDE *****

    enum SpursTasksetRequest : s32 {
        SPURS_TASKSET_REQUEST_POLL_SIGNAL = -1,
        SPURS_TASKSET_REQUEST_DESTROY_TASK = 0,
        SPURS_TASKSET_REQUEST_YIELD_TASK = 1,
        SPURS_TASKSET_REQUEST_WAIT_SIGNAL = 2,
        SPURS_TASKSET_REQUEST_POLL = 3,
        SPURS_TASKSET_REQUEST_WAIT_WKL_FLAG = 4,
        SPURS_TASKSET_REQUEST_SELECT_TASK = 5,
        SPURS_TASKSET_REQUEST_RECV_WKL_FLAG = 6,
    };

    std::string requestToString(s32 request) {
        switch (request) {
            case SPURS_TASKSET_REQUEST_POLL_SIGNAL:     return "SPURS_TASKSET_REQUEST_POLL_SIGNAL";     break;
            case SPURS_TASKSET_REQUEST_DESTROY_TASK:    return "SPURS_TASKSET_REQUEST_DESTROY_TASK";    break;
            case SPURS_TASKSET_REQUEST_YIELD_TASK:      return "SPURS_TASKSET_REQUEST_YIELD_TASK";      break;
            case SPURS_TASKSET_REQUEST_WAIT_SIGNAL:     return "SPURS_TASKSET_REQUEST_WAIT_SIGNAL";     break;
            case SPURS_TASKSET_REQUEST_POLL:            return "SPURS_TASKSET_REQUEST_POLL";            break;
            case SPURS_TASKSET_REQUEST_WAIT_WKL_FLAG:   return "SPURS_TASKSET_REQUEST_WAIT_WKL_FLAG";   break;
            case SPURS_TASKSET_REQUEST_SELECT_TASK:     return "SPURS_TASKSET_REQUEST_SELECT_TASK";     break;
            case SPURS_TASKSET_REQUEST_RECV_WKL_FLAG:   return "SPURS_TASKSET_REQUEST_RECV_WKL_FLAG";   break;
            default: return "UNKNOWN";
        }
    }

    // This struct is at 0x2700
    struct SpursTasksetContext
    {
        u8 tempAreaTaskset[0x80];                   // 0x2700
        u8 tempAreaTaskInfo[0x30];                  // 0x2780
        BEField<u64> x27B0;                         // 0x27B0
        BEField<u64> taskset_ptr;                   // 0x27B8
        BEField<u32> kernelMgmtAddr;                // 0x27C0
        BEField<u32> syscallAddr;                   // 0x27C4
        BEField<u32> x27C8;                         // 0x27C8
        BEField<u32> spuNum;                        // 0x27CC
        BEField<u32> dmaTagId;                      // 0x27D0
        BEField<u32> taskId;                        // 0x27D4
        u8 x27D8[0x2840 - 0x27D8];                  // 0x27D8
        u8 moduleId[16];                            // 0x2840
        u8 stackArea[0x2C80 - 0x2850];              // 0x2850
        v128 savedContextLr;                        // 0x2C80
        v128 savedContextSp;                        // 0x2C90
        v128 savedContextR80ToR127[48];             // 0x2CA0
        v128 savedContextFpscr;                     // 0x2FA0
        BEField<u32> savedWriteTagGroupQueryMask;   // 0x2FB0
        BEField<u32> savedSpuWriteEventMask;        // 0x2FB4
        BEField<u32> tasksetMgmtAddr;               // 0x2FB8
        BEField<u32> guidAddr;                      // 0x2FBC
        BEField<u64> x2FC0;                         // 0x2FC0
        BEField<u64> x2FC8;                         // 0x2FC8
        BEField<u32> taskExitCode;                  // 0x2FD0
        BEField<u32> x2FD4;                         // 0x2FD4
        u8 x2FD8[0x3000 - 0x2FD8];                  // 0x2FD8
    };

    static_assert(sizeof(SpursTasksetContext) == 0x900);

    void spursTasksetProcessRequest();

private:
    MAKE_LOG_FUNCTION(log, cellSpurs);
};