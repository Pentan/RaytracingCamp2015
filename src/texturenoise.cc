#include <iostream>

#include "random.h"
#include "texturenoise.h"

using namespace r1h;

/// noise
struct NoiseSortComparator {
    unsigned int num[512];
    //NoiseSortComparator() {}
    //~NoiseSortComparator() {}
    bool operator() (const int a, const int b) {
        return num[a] < num[b];
    }
};

NoiseTexture::NoiseTexture(const IpoType ipo, const int seed): noise(0), ipotype(ipo) {
    noise = new int[512];
    setSeed(seed);
}
NoiseTexture::~NoiseTexture() {
    if(noise) { delete [] noise; }
}

void NoiseTexture::setSeed(const int seed) {
    NoiseSortComparator cmp;
    Random rnd(seed);
    for(int i = 0; i < 256; i++) {
        cmp.num[i] = rnd.next();
        noise[i] = i;
    }
    std::sort(noise, noise + 256, cmp);
    
    for(int i = 0; i < 256; i++) {
        noise[i + 256] = noise[i];
    }
}

// ref. http://mrl.nyu.edu/~perlin/noise/
Color NoiseTexture::sample(const Vector3 &p) const {
    Vector3 lp = applyTransform(p);
    
    // integer
    int ix = int(floor(lp.x)) & 255;
    int iy = int(floor(lp.y)) & 255;
    int iz = int(floor(lp.z)) & 255;
    
    // fraction
    double fx = lp.x - floor(lp.x);
    double fy = lp.y - floor(lp.y);
    double fz = lp.z - floor(lp.z);
    // fade
    double tx = fade(fx);
    double ty = fade(fy);
    double tz = fade(fz);
    
    int A  = noise[ix] + iy;
    int AA = noise[A] + iz;
    int AB = noise[A + 1] + iz;
    
    int B  = noise[ix + 1] + iy;
    int BA = noise[B] + iz;
    int BB = noise[B + 1] + iz;
    
    double val;
    
    switch(ipotype) {
        case Cell:
            val = noise[AA] / 255.0; //grad(noise[AA], fx, fy, fz);
            break;
        case Smooth:
            val = lerp(tz, lerp(ty, lerp(tx, grad(noise[AA  ], fx    , fy    , fz),
                                             grad(noise[BA  ], fx-1.0, fy    , fz)),
                                    lerp(tx, grad(noise[AB  ], fx    , fy-1.0, fz),
                                             grad(noise[BB  ], fx-1.0, fy-1.0, fz))
                           ),
                           lerp(ty, lerp(tx, grad(noise[AA+1], fx    , fy    , fz-1.0),
                                             grad(noise[BA+1], fx-1.0, fy    , fz-1.0)),
                                    lerp(tx, grad(noise[AB+1], fx    , fy-1.0, fz-1.0),
                                             grad(noise[BB+1], fx-1.0, fy-1.0, fz-1.0))
                           )
                  );
            break;
    }
    
    return Color(val);
}

double NoiseTexture::fade(const double t) const {
    return t * t * t * (t * (t * 6.0 - 15.0) + 10.0);
}
double NoiseTexture::lerp(const double t, const double a, const double b) const {
    return a + t * (b - a);
}
double NoiseTexture::grad(const int hash, const double x, const double y, const double z) const {
    int h = hash & 15;
    double u = (h < 8)? x : y;
    double v = (h < 4)? y : ((h == 12 || h == 14)? x : z);
    return (((h & 1) == 0)? u : -u) + (((h & 2) == 0)? v : -v);
}

///// voronoi
VoronoiTexture::VoronoiTexture(const double j, const int seed) : jitter(j) {
    for(int i = 0; i < 3; i++) {
        noiseTexs[i] = new NoiseTexture(NoiseTexture::Cell, seed + i);
        noiseTexs[i]->setIsUseTransform(false);
    }
}

VoronoiTexture::~VoronoiTexture() {
    for(int i = 0; i < 3; i++) {
        delete noiseTexs[i];
    }
}

Color VoronoiTexture::sample(const Vector3 &p) const {
    // local point
    Vector3 lp = applyTransform(p);
    // stepping point
    Vector3 sp = Vector3(floor(lp.x), floor(lp.y), floor(lp.z));
    
    double minll = 1e10;
    for(int iz = -1; iz <= 1; iz++) {
        for(int iy = -1; iy <= 1; iy++) {
            for(int ix = -1; ix <= 1; ix++) {
                // sampling point
                Vector3 csp = sp + Vector3(ix, iy, iz);
                Color jx = noiseTexs[0]->sample(csp);
                Color jy = noiseTexs[1]->sample(csp);
                Color jz = noiseTexs[2]->sample(csp);
                // voronoi sphere's position = csp + jitter
                Vector3 jv = Vector3(
                    (jx.r - 0.5) * jitter,
                    (jy.r - 0.5) * jitter,
                    (jz.r - 0.5) * jitter
                );
                Vector3 pp = (csp + jv) - lp;
                double tmpll = Vector3::dot(pp, pp); // square len
                if(tmpll < minll) {
                    minll = tmpll;
                }
            }
        }
    }
    //return Vector3(1.0 - sqrt(minll));
    return Vector3(sqrt(minll));
}

///// fractal noise
FractalNoiseTexture::FractalNoiseTexture(const NoiseType noisetype, const int d, const int seed):
    depth(0), noiseSeed(seed), voroJitter(0.8), noiseType(noisetype)
{
    textures.reserve(d);
    setDepth(d);
}

FractalNoiseTexture::~FractalNoiseTexture() {
    for(int i = 0; i < (int)textures.size(); i++) {
        delete textures[i];
    }
}

void FractalNoiseTexture::setDepth(const int d) {
    depth = d;
    
    int texnum = (int)textures.size();
    if(depth < texnum) {
        return;
    }
    
    for(int i = texnum; i < depth; i++) {
        Texture *tex=0;
        switch(noiseType) {
            case CellNoise:
                tex = new NoiseTexture(NoiseTexture::Cell, noiseSeed + i);
                break;
            case SmoothNoise:
                tex = new NoiseTexture(NoiseTexture::Smooth, noiseSeed + i);
                break;
            case Voronoi:
                tex = new VoronoiTexture(voroJitter, noiseSeed + i);
                break;
        }
        tex->setIsUseTransform(false);
        textures.push_back(tex);
        //printf("nt:%d, %p\n", noiseType, tex);
    }
}

void FractalNoiseTexture::setJitter(const double j) {
    voroJitter = j;
    if(noiseType == Voronoi) {
        int texnum = (int)textures.size();
        for(int i = 0; i < texnum; i++) {
            VoronoiTexture *tex = dynamic_cast<VoronoiTexture*>(textures[i]);
            tex->setJitter(voroJitter);
        }
    }
}

Color FractalNoiseTexture::sample(const Vector3 &p) const {
    Vector3 lp = applyTransform(p);
    Color ret = Color(0.0);
    double weight = 1.0;
    double wtotal = 0.0;
    
    for(int i = 0; i < depth; i++) {
        Texture *tex = textures[i];
        Color col = tex->sample(lp);
        ret += col * weight;
        
        wtotal += weight;
        lp = lp * 2.0;
        weight *= 0.5;
    }
    
    return ret / wtotal;
};
