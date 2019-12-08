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

void MGL_Node::translate(float factor, int axis)
{
    switch(axis)
    {
    case 0: xform->mat_translate.translate(factor,0.0,0.0);
        break;
    case 1: xform->mat_translate.translate(0.0,factor,0.0);
        break;
    case 2: xform->mat_translate.translate(0.0,0.0,factor);
        break;
    }
}

//- rotate ( modifies ttransform)
void MGL_Node::rotateOverX(float x)
{
    static QVector3D x_axis(1.0,0.0,0.0);
    xform->mat_rotateX.rotate(2*x,x_axis);
    xform->calculateRotationMatrix();
}
void MGL_Node::rotateOverY(float y)
{
    static QVector3D y_axis(0.0,1.0,0.0);
    xform->mat_rotateY.rotate(2*y,y_axis);
    xform->calculateRotationMatrix();
}
void MGL_Node::rotateOverZ(float z)
{
    static QVector3D z_axis(0.0,0.0,1.0);
    xform->mat_rotateY.rotate(2*z,z_axis);
    xform->calculateRotationMatrix();
}

void MGL_Node::rotate(float factor, int axis)
{
    switch(axis)
    {
    case 0: rotateOverX(factor);
        break;
    case 1: rotateOverY(factor);
        break;
    case 2: rotateOverZ(factor);
        break;
    }
}

//- scale ( modifies ttransform)
void MGL_Node::scaleAll(float x, float y, float z)
{
    xform->scaleFactorX *= x;
    xform->scaleFactorY *= y;
    xform->scaleFactorZ *= z;
}

void MGL_Node::scale(float factor, int axis)
{
    switch(axis)
    {
    case 0: xform->scaleFactorX *= factor;
        break;
    case 1: xform->scaleFactorY *= factor;
        break;
    case 2: xform->scaleFactorZ *= factor;
        break;
    }
}

void MGL_Node::scaleLinearAll(float x, float y, float z)
{
    xform->scaleFactorX += x;
    xform->scaleFactorY += y;
    xform->scaleFactorZ += z;
}

void MGL_Node::scaleLinear(float factor, int axis)
{
    switch(axis)
    {
    case 0: xform->scaleFactorX += factor;
        break;
    case 1: xform->scaleFactorY += factor;
        break;
    case 2: xform->scaleFactorZ += factor;
        break;
    }
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
int MGL_Node::getVertexCountLocal() const
{
    return obj->vertexCount;
}

int MGL_Node::getVertexCountRecursive() const
{
    int count = 0;
    for (auto node: children)
    {
        count += node->getVertexCountRecursive();
    }
    return count + getVertexCountLocal();
}

int MGL_Node::fillRaw(float *arr) const
{
    float *vt = obj->vertices;
    int vcount = getVertexCountLocal();

    for (int i = 0; i < vcount; i++)
    {
        QVector4D old = QVector4D(vt[i*4],vt[i*4+1],
                                vt[i*4+2],vt[i*4+3]);
        QVector3D raw = (getXform() * old).toVector3DAffine(); // apply transformations
        arr[i*3] = raw.x();
        arr[i*3+1] = raw.y();
        arr[i*3+2] = raw.z();
    }

    for (auto &obj: children)
    {
        vcount += obj->fillRaw(arr+(vcount*3));
    }
    return vcount;
}

//- return local transform (ttransform)
QMatrix4x4 MGL_Node::getXform() const
{
    return xform->transformMatrix();
}

int getCenterHelper(const MGL_Node *node, float &xavg, float &yavg,
                    float &zavg, int numVertices)
{

}

QVector3D MGL_Node::getCenter() const
{
    // calculate average for each point
    float x,y,z;

}

float MGL_Node::getRadius() const
{

}
