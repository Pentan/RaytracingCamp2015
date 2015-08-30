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
#define kProgressOutIntervalSec	30.0

///
int main(int argc, char *argv[]) {

	std::cout << "twi" << std::endl;

    using namespace r1h;

    double startTime = gettimeofday_sec();

    // renderer setup
    Renderer *render = new Renderer();

	/*
	Renderer::Config renderConf = render->getConfig();
	
    //renderConf.width = 1280 / 4;
    //renderConf.height = 720 / 4;
	renderConf.width = 640 / 4;
	renderConf.height = 480 / 4;
	renderConf.samples = 16;
	renderConf.subSamples = 2;
	renderConf.tileSize = 64;
	renderConf.minDepth = 2;
    renderConf.maxDepth = 4;
	
	// parse render config from arguments?
	
    render->setConfig(renderConf);
	
    printf("renderer configured [%.4f sec]\n", gettimeofday_sec() - startTime);
	 */

    // scene setup
    Scene *scene = new Scene();
	bool loaded = false;
	
#if 1
	{
		XMLSceneLoader loader;
		//std::string xmlfile("scenes/edupt_cornelbox.xml");
		//std::string xmlfile("scenes/mesh_cube.xml");
		//std::string xmlfile("scenes/textest.xml");
		//std::string xmlfile("scenes/bsdftest.xml");
		//std::string xmlfile("scenes/mesh_ref.xml");
		//std::string xmlfile("scenes/mesh_cat.xml");
		std::string xmlfile("scenes/scene.xml");

		loader.load(xmlfile, scene, render);
		loaded = true;
		//return 0;
	}
#else
	if(argc > 1) {
		loaded = loader.load(argv[1], scene, render);
		
	} else {
		printf("usage:%s [file.obj]\n", argv[0]);
		printf("no obj set. setup default scene.\n");
		loaded = true;
		
		EduptScene::load(scene, (double)renderConf.width / renderConf.height);
		//EduptScene::load2(scene, (double)renderConf.width / renderConf.height);
	}
#endif

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
				sprintf(buf, "%03d.bmp", outcount);

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
		Renderer::Config renderConf = render->getConfig();
		const char *finalname = renderConf.outputFile.c_str();//"final.bmp";
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
