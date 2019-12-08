#ifndef MGL_OBJECTBUFFER_H
#define MGL_OBJECTBUFFER_H

#include <list>
#include <light.h>
#include <material.h>
#include <unordered_map>
#include <mgl_node.h>

//**********************************
// Stores file generated objects
// for our object managers to
// correctly store in data structures
// with the correct references
//**********************************
struct MGL_ObjectBuffer
{
    std::list<MGL_Node *> objects;
    std::list<Light> lights;
    std::unordered_map<std::string,Material> materials;
};

#endif // MGL_OBJECTBUFFER_H
