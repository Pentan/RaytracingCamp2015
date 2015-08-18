#ifndef R1H_COMMANDQUEUE_H
#define R1H_COMMANDQUEUE_H

#include <queue>
#include <mutex>

#include "framebuffer.h"

namespace r1h {

class RenderCommandQueue {
public:
    RenderCommandQueue();
    ~RenderCommandQueue();
    
    enum CommandType {
        kNoOp   = 0,
        kTile,
        kSleep,
		kYield,
        kFinish
    };
    
    struct Command {
        CommandType type;
        union {
            FrameBuffer::Tile tile;
            long usec;
        };
        Command():type(kNoOp) {}
    };
    
    void pushTileCommand(FrameBuffer::Tile tile);
    void pushSleepCommand(unsigned long usec);
    Command popCommand();
    
	bool isQueueEmpty();
	size_t getRemainCommandCount();
	
private:
    std::queue<Command> cmdQueue;
    std::mutex cmdMutex;
};

}

#endif
