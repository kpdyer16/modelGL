#ifndef MGL_OBJECTBUFFER_H
#define MGL_OBJECTBUFFER_H

#include <list>
#include <tobject.h>
#include <light.h>
#include <material.h>
#include <unordered_map>

//**********************************
// Stores file generated objects
// for our object managers to
// correctly store in data structures
// with the correct references
//**********************************
struct MGL_ObjectBuffer
{
    std::list<TObject> tobjects;
    std::list<Light> lights;
    std::list<Material> materials;
};

#endif // MGL_OBJECTBUFFER_H
