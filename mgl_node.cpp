#include "mgl_node.h"

MGL_Node::MGL_Node()
{
    obj = new TObject();
    xform = new TTransform();
}

MGL_Node::~MGL_Node()
{
    delete obj;
    delete xform;
}

//- add triangles (tobject)
void MGL_Node::addVerticesByArray3D(GLfloat *vertices, int numVertices)
{
    if (numVertices + obj->vertexCount > obj->max_vertices)
    {
        std::cout << "Too many vertices being added to object" << std::endl;
    }
    else
    {
        int vertexCount = obj->vertexCount;
        // Next available vertex slot           vvv
        GLfloat *overtices = obj->vertices + (vertexCount*4);

        for (int i = 0; i < numVertices; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                overtices[i*4+j] = vertices[i*3+j];
            }
            overtices[i*4+3] = 1.0;
        }
    }
}
void MGL_Node::addVerticesByArray4D(GLfloat *vertices, int numVertices)
{
    if (numVertices + obj->vertexCount > obj->max_vertices)
    {
        std::cout << "Too many vertices being added to object" << std::endl;
    }
    else
    {
        int vertexCount = obj->vertexCount;
        // Next available vertex slot           vvv
        GLfloat *overtices = obj->vertices + (vertexCount*4);
        for (int i = 0; i < numVertices * 4; i++)
        {
            overtices[i] = vertices[i];
        }
    }
}

void MGL_Node::addVerticesByVector4D(QVector4D *vertices, int numVertices)
{
    if (numVertices + obj->vertexCount > obj->max_vertices)
    {
        std::cout << "Too many vertices being added to object" << std::endl;
    }
    else
    {
        int vertexCount = obj->vertexCount;
        // Next available vertex slot           vvv
        GLfloat *overtices = obj->vertices + (vertexCount*4);
        for (int i = 0; i < numVertices; i++)
        {
            // for each vertex in vertices, input 4 values into overtices
            overtices[i*4]   = vertices[i].x();
            overtices[i*4+1] = vertices[i].y();
            overtices[i*4+2] = vertices[i].z();
            overtices[i*4+3] = vertices[i].w();
        }
        obj->vertexCount += numVertices;
//        for (int i = 0; i < obj->vertexCount; i++)
//        {
//            for (int j = 0; j < 4; j++)
//            {
//                std::cout << obj->vertices[i*4+j] << " ";
//            }
//            std::cout << std::endl;
//        }
    }
}

//- clear triangles (tobject)
void MGL_Node::clearTriangles()
{
    obj->vertexCount = 0;
}

//- translate ( modifies ttransform)
void MGL_Node::translateOnAllAxes(float x, float y, float z)
{
    xform->mat_translate.translate(x,y,z);
}
void MGL_Node::translateAlongX(float x)
{
    xform->mat_translate.translate(x,0.0,0.0);
}
void MGL_Node::translateAlongY(float y)
{
    xform->mat_translate.translate(0.0,y,0.0);
}
void MGL_Node::translateAlongZ(float z)
{
    xform->mat_translate.translate(0.0,0.0,z);
}

//- rotate ( modifies ttransform)
void MGL_Node::rotateOverX(float x)
{

}
void MGL_Node::rotateOverY(float y)
{

}
void MGL_Node::rotateOverZ(float z)
{

}

//- scale ( modifies ttransform)
void MGL_Node::scaleX(float x)
{

}
void MGL_Node::scaleY(float y)
{

}
void MGL_Node::scaleZ(float z)
{

}

//- set transform order ( modifies ttransform)
void MGL_Node::setXformOrder(int order)
{
    xform->setTransformOrder(order);
}

//- set rotation axis order ( modifies ttransform)
void MGL_Node::setRotationAxisOrder(int order)
{
    xform->setRotationAxisOrder(order);
}

// operations on tree
void MGL_Node::spawnChild()
{
    MGL_Node *newNode = new MGL_Node();
    addChild(newNode);
}


// TODO: decide whether these two methods should be
//       private. potential mem loss and dangling ptrs

// child must not have a parent. otherwise,
// we would have two references to this node
void MGL_Node::addChild(MGL_Node *child)
{
    children.push_back(child);
}
// memory unsafe (for now)
void MGL_Node::removeChild(std::list<MGL_Node*>::iterator refToChild)
{
    children.erase(refToChild);
}
void MGL_Node::deleteChild(std::list<MGL_Node*>::iterator refToChild)
{
    MGL_Node *node = *refToChild;
    delete node;
    children.erase(refToChild);
}
void MGL_Node::deleteChildren()
{
    for (auto &child: children)
    {
        delete child;
    }
    children.clear();
}

//- iterators
std::list<MGL_Node *>::iterator MGL_Node::begin()
{
    return children.begin();
}
std::list<MGL_Node *>::iterator MGL_Node::end()
{
    return children.end();
}

//- take an iterator pointing to a node, and make it a child of a different node
void MGL_Node::makeXChildOfY(std::list<MGL_Node *>::iterator x,
                   std::list<MGL_Node *>::iterator y)
{
    MGL_Node *tmp = *x;
    removeChild(x);
    (*y)->addChild(tmp);
}

//- return constant triangle array (tobject)
GLfloat* MGL_Node::getTriangleArray()
{
    return obj->vertices;
}
int MGL_Node::getVertexCount()
{
    return obj->vertexCount;
}

//- return local transform (ttransform)
QMatrix4x4 MGL_Node::getXform()
{
    return xform->transformMatrix();
}

