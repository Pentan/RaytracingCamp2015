
#include "commandqueue.h"

using namespace r1h;

RenderCommandQueue::RenderCommandQueue() {};
RenderCommandQueue::~RenderCommandQueue() {};

void RenderCommandQueue::pushTileCommand(FrameBuffer::Tile tile) {
    std::lock_guard<std::mutex> lock(cmdMutex);
    Command cmd;
    cmd.type = kTile;
    cmd.tile = tile;
    cmdQueue.push(cmd);
}

void RenderCommandQueue::pushSleepCommand(unsigned long usec) {
    std::lock_guard<std::mutex> lock(cmdMutex);
    Command cmd;
    cmd.type = kSleep;
    cmd.usec = usec;
    cmdQueue.push(cmd);
}

RenderCommandQueue::Command RenderCommandQueue::popCommand() {
    std::lock_guard<std::mutex> lock(cmdMutex);
    if(cmdQueue.size() > 0) {
        Command cmd = cmdQueue.front();
        cmdQueue.pop();
        return cmd;
    } else {
        Command cmd;
        cmd.type = kFinish;
        return cmd;
    }
}

bool RenderCommandQueue::isQueueEmpty() {
	return (getRemainCommandCount() <= 0);
}

size_t RenderCommandQueue::getRemainCommandCount() {
	return cmdQueue.size();
}
