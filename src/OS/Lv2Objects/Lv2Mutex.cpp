#include "Lv2Mutex.hpp"
#include "PlayStation3.hpp"


bool Lv2Mutex::isFree() {
    return owner == -1;
}

bool Lv2Mutex::lock() {
    Thread* curr_thread = ps3->thread_manager.getCurrentThread();
    if (isFree()) {
        owner = curr_thread->id;
        if (recursive) recursive_cnt++;
    }
    else if (owner != curr_thread->id) {
        curr_thread->wait(std::format("mutex {:d} owned by {:s}", handle(), ps3->thread_manager.getThreadByID(owner)->name));
        wait_list.push(curr_thread->id);
        //printf("\"%s\" tried to lock mutex owned by \"%s\", waiting...\n", curr_thread->name.c_str(), ps3->thread_manager.getThreadByID(owner)->name.c_str());
    }
    else if (recursive) {
        recursive_cnt++;
    }
    else return false;

    return true;
}

bool Lv2Mutex::tryLock() {
    Thread* curr_thread = ps3->thread_manager.getCurrentThread();
    if (isFree()) {
        owner = curr_thread->id;
        if (recursive) recursive_cnt++;
        return true;
    }
    else if (owner != curr_thread->id)
        return false;
    else if (recursive) {
        recursive_cnt++;
        return true;
    }
    else Helpers::panic("tryLock twice on a non-recursive mutex\n");
}


void Lv2Mutex::unlock() {
    Thread* curr_thread = ps3->thread_manager.getCurrentThread();
    if (owner == curr_thread->id) {
        if (recursive) {
            if (recursive_cnt > 0) {
                recursive_cnt--;
                if (recursive_cnt != 0) return; // Don't release the mutex if we still have to recursively unlock it more times
            } else Helpers::panic("Invalid recursive_cnt in mutex (%d)\n", recursive_cnt);
        }
        
        if (wait_list.size()) {
            // Wake up thread
            Thread* t = ps3->thread_manager.getThreadByID(wait_list.front());
            t->wakeUp();
            wait_list.pop();
            owner = t->id;
            if (recursive)
                recursive_cnt++;
        } else
            owner = -1; // free
    }
    else if (isFree()) { /* Do nothing */ }
    else
        Helpers::panic("Tried to unlock a mutex, but the current thread is not the mutex's owner (owner was thread %d \"%s\")\n", owner, ps3->thread_manager.getThreadByID(owner)->name.c_str());
}
