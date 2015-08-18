#include <iostream>
#include <cstdio>
#include <string>
#include <thread>
#include <chrono>

#include "r1htypes.h"
#include "scene.h"
#include "renderer.h"
#include "framebuffer.h"
#include "tonemapper.h"
#include "xmlscene.h"

//
#include "eduptscene.h"
//

#ifdef WIN32
#include <windows.h>
static double gettimeofday_sec() {
	return timeGetTime() / 1000.0;
}
#else
#include <sys/time.h>
static double gettimeofday_sec() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + (double)tv.tv_usec * 1e-6;
}
#endif

///
#define kProgressOutIntervalSec	60.0

///
int main(int argc, char *argv[]) {

	std::cout << "RaRiT" << std::endl;

    using namespace r1h;

    double startTime = gettimeofday_sec();

    // renderer setup
    Renderer *render = new Renderer();
    Renderer::Config renderConf = render->getConfig();
	
    renderConf.width = 1280 / 4;
    renderConf.height = 720 / 4;
	//renderConf.width = 256 / 2;
	//renderConf.height = 256 / 2;
	renderConf.samples = 8;
	renderConf.subSamples = 2;
	renderConf.tileSize = 64;
    renderConf.maxDepth = 12;
	
	// parse render config from arguments?
	
    render->setConfig(renderConf);
	
    printf("renderer configured [%.4f sec]\n", gettimeofday_sec() - startTime);

    // scene setup
    Scene *scene;
	
	bool loaded = false;
	
	scene = new Scene();
	
	//+++++
	{
		XMLSceneLoader loader;
		std::string xmlfile("scenes/edupt_cornelbox.xml");
		loader.load(xmlfile, scene, render);
		return 0;
	}
	//+++++
	
	if(argc > 1) {
		loaded = scene->loadWavefrontObj(argv[1]);
	} else {
		printf("usage:%s [file.obj]\n", argv[0]);
		printf("no obj set. setup default scene.\n");
		loaded = true;
		
		//loaded = scene->loadWavefrontObj("models/manyobjs.obj");
		
		//+++++ edupt cornel box scene +++++
		//EduptScene::load(scene, (double)renderConf.width / renderConf.height);
		EduptScene::load2(scene, (double)renderConf.width / renderConf.height);
	}
	
    printf("scene loaded [%.4f sec]\n", gettimeofday_sec() - startTime);

	if(loaded) {

		// set tone mapper
		ToneMapper *mapper = new ToneMapper();
		
		// render
		double renderStart = gettimeofday_sec();
		render->render(scene, true); // detach
		
		// wait to finish
		int outcount = 0;
		double prevouttime = gettimeofday_sec();
        
        int numcntx = (int)render->getRecderContextCount();
        
		do {
			double progress = render->getRenderProgress();
			
			std::this_thread::sleep_for(std::chrono::seconds(1));
			
			double curtime = gettimeofday_sec();
			if(curtime - prevouttime > kProgressOutIntervalSec) {
				// progress output
				char buf[16];
#ifdef WIN32
				sprintf_s(buf, "%03d.bmp", outcount);
#else
				sprintf(buf, "%03d.bmp", outcount);
#endif
				mapper->exportBMP(render->getFrameBuffer(), buf);
                printf("progress image %s saved\n", buf);
				outcount++;
				prevouttime += kProgressOutIntervalSec;
			}
			
			printf("%.2lf%%:", progress);
            for(int i = 0; i < numcntx; i++) {
                const Renderer::Context *cntx = render->getRenderContext(i);
                printf("[%d:%.1lf]", i, cntx->tileProgress * 100.0);
            }
            printf("    \r");
            fflush(stdout);
            
		} while( !render->isFinished() );

		printf("render finished (%.4f sec) [%.4f sec]\n", gettimeofday_sec() - renderStart, gettimeofday_sec() - startTime);
		
		// final image
        const char *finalname = "final.bmp";
		mapper->exportBMP(render->getFrameBuffer(), finalname);
        printf("%s saved\n", finalname);

		printf("saved [%.4f sec]\n", gettimeofday_sec() - startTime);
		
		delete mapper;
	}

    // cleaning
    delete render;
    delete scene;

    printf("done [%.4f sec]\n", gettimeofday_sec() - startTime);

    return 0;
}
