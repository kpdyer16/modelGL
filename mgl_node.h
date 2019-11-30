#ifndef MGL_NODE_H
#define MGL_NODE_H

#include <tobject.h>
#include <ttransform.h>

class MGL_Node
{
public:
    MGL_Node();
    ~MGL_Node();

    //- add triangles (tobject)
    void addVerticesByArray3D(GLfloat *vertices, int numVertices); // converts to 4D
    void addVerticesByArray4D(GLfloat *vertices, int numVertices);
    void addVerticesByVector4D(QVector4D *vertices, int numVertices);

    //- clear triangles (tobject)
    void clearTriangles();

    //- translate ( modifies ttransform)
    void translateOnAllAxes(float x, float y, float z);
    void translateAlongX(float x);
    void translateAlongY(float y);
    void translateAlongZ(float z);

    //- rotate ( modifies ttransform)
    void rotateOverX(float x);
    void rotateOverY(float y);
    void rotateOverZ(float z);

    //- scale ( modifies ttransform)
    void scaleAll(float x, float y, float z);
    void scaleX(float x);
    void scaleY(float y);
    void scaleZ(float z);
    void scaleLinearAll(float x, float y, float z);
    void scaleLinearX(float x);
    void scaleLinearY(float y);
    void scaleLinearZ(float z);

    //- set transform order ( modifies ttransform)
    void setXformOrder(int order);

    //- set rotation axis order ( modifies ttransform)
    void setRotationAxisOrder(int order);

    // operations on tree
    void spawnChild(); // creates new child
    void addChild(MGL_Node *child); // add existing node to this node's children
    void removeChild(std::list<MGL_Node*>::iterator refToChild); // memory unsafe (for now)
    void deleteChild(std::list<MGL_Node*>::iterator refToChild);
    void deleteChildren();

    //- iterators
    std::list<MGL_Node *>::iterator begin();
    std::list<MGL_Node *>::iterator end();

    //- take an iterator pointing to a node, and make it a child of a different node
    void makeXChildOfY(std::list<MGL_Node *>::iterator x,
                       std::list<MGL_Node *>::iterator y);

    //- return constant triangle array (tobject)
    GLfloat* getTriangleArray();
    int getVertexCount();

    //- return local transform (ttransform)
    QMatrix4x4 getXform(); // returns multiplied matrix

    size_t size() { return children.size(); }
    std::list<MGL_Node *> getChildren() const { return children; }
private:
    TObject *obj;
    TTransform *xform;
    std::list<MGL_Node *> children;
};

#endif // MGL_NODE_H
