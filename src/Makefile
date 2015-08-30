
FULL_OPTIMIZE = 1

ifdef FULL_OPTIMIZE
    CC = cc
    CXX = c++
    EXTRAFLAGS = -O3
else
    CC = cc
    CXX = c++
    EXTRAFLAGS = -g
endif

EXT_DIR = ext
INCLUDES = -I$(EXT_DIR)

CFLAGS = -Wall $(EXTRAFLAGS) $(INCLUDES)
CXXFLAGS = -std=c++11 -stdlib=libc++ -Wall $(EXTRAFLAGS) $(INCLUDES)
LDFLAGS = -lc++ $(EXTRAFLAGS)

OBJS = \
    main.o \
    aabb.o \
    aabbgeometry.o \
    abstracttonemapper.o \
    bsdfdiffuse.o \
    bsdfrefraction.o \
    bsdfspecular.o \
    bsdfpaint.o \
    bsdftranslucent.o \
    bvhnode.o \
    camera.o \
    commandqueue.o \
    eduptscene.o \
    framebuffer.o \
    intersection.o \
    materialsinglebsdf.o \
    materialsky.o \
    mesh.o \
    renderer.o \
    scene.o \
    scenesupport.o \
    sceneobject.o \
    sphere.o \
    texturebase.o \
    textureimage.o \
    texturenoise.o \
    tonemapper.o \
    wavefrontobj.o \
    xmlscene.o \
    xmlobjloader.o

EXT_OBJS = \
    $(EXT_DIR)/tinyxml/tinyxml2.o

#    $(EXT_DIR)/tinyexr/tinyexr.o \

MAINEXE = twi
MAINTARGET = twi

# main
$(MAINTARGET) : $(OBJS) $(EXT_OBJS)
	$(CC) -o "$(MAINEXE)" $(OBJS) $(EXT_OBJS) $(LDFLAGS)

clean :
	rm -f $(OBJS) $(EXT_OBJS) "$(MAINEXE)"
