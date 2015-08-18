#ifndef R1H_RENDERER_H
#define R1H_RENDERER_H

#include <thread>
#include <vector>
#include <queue>
#include <string>

#include "r1htypes.h"
#include "random.h"
#include "framebuffer.h"
#include "ray.h"

namespace r1h {

class Scene;
class RenderCommandQueue;

class Renderer {
public:
	struct Config {
        int width;
        int height;
        int subSamples;
		int samples;
        int minDepth;
        int maxDepth;
        int defaultThreads;
        int tileSize;
		std::string outputFile;
		
        Config(): // make default values
		width(640),
		height(360),
		subSamples(2),
		samples(2),
        minDepth(3),
		maxDepth(32),
		defaultThreads(4),
		tileSize(32),
		outputFile("output.bmp")
        {}
    };
	
    enum RenderState {
        kAwake,
        kWorking,
        kDone
    };
    
	struct Context {
		Random random;
		std::vector<Ray> rayVector1;
		std::vector<Ray> rayVector2;
		std::vector<Ray> workVector;
		
		const Config *config;
		int state;
        double tileProgress;
        
		Context() : random(0), state(kAwake) {}
		void init(const unsigned int seed, const Config *conf) {
			random.setSeed(seed);
			rayVector1.reserve(128);
			rayVector2.reserve(128);
			workVector.reserve(32);
			config = conf;
		}
	};
	
public:
    Renderer();
    ~Renderer();
    
    void render(Scene *scene, bool isdetach=false);
    
    Config getConfig() const { return config; };
    void setConfig(const Config &conf) { config = conf; };
    FrameBuffer* getFrameBuffer() const { return frameBuffer; };
    
	double getRenderProgress() const;
    
    size_t getRecderContextCount() const;
    const Context* getRenderContext(int cntxid) const;
	
    bool isFinished() const;
    
	///
	void workerJob(int workerId, Scene *scene);
private:
    FrameBuffer *frameBuffer;
    Config config;
    
	std::vector<Context> *renderContexts;
	
	int pushedCommandCount;
    RenderCommandQueue *renderQueue;
    std::vector<std::thread> workers;
	void renderTile(Context *cntx, Scene *scene, FrameBuffer::Tile tile);
	
	static void startWorker(Renderer *rndr, int workerId, Scene *scene);
};

}
#endif
