
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
    materialsky.o \
    mesh.o \
    renderer.o \
    scene.o \
    scenesupport.o \
    scene2013.o \
    scene2014.o \
    sceneobject.o \
    sphere.o \
    texture.o \
    tonemapper.o \
    wavefrontobj.o

EXT_OBJS = \
    $(EXT_DIR)/tinyexr/tinyexr.o \
    $(EXT_DIR)/tinyxml/tinyxml.o

TESTOBJ = main.o
MAINOBJ = main.o

MAINEXE = RaRiT

MAINTARGET = rarity
TESTTARGET = r15m

# main
$(MAINTARGET) : $(MAINOBJ) $(OBJS) $(EXT_OBJS)
	$(CC) -o "$(MAINEXE)" $(MAINOBJ) $(OBJS) $(LDFLAGS)

# test
$(TESTTARGET) : $(TESTOBJ) $(OBJS) $(EXT_OBJS)
	$(CC) -o "$(TESTTARGET)" $(TESTOBJ) $(OBJS) $(LDFLAGS)

clean :
	rm -f $(OBJS) $(TESTOBJ) $(MAINOBJ) "$(MAINEXE)" "$(TESTTARGET)"
