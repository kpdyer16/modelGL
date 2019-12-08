#include "mgl_parser.h"

MGL_Parser::MGL_Parser()
{

}

void MGL_Parser::parse(const std::string)
{

}

//void readObject()

void MGL_Parser::parse_geo(const std::string filename)
{
    std::list<std::string> tokens = lex(filename);
    while (!tokens.empty())
    {
        std::string front = tokens.front();
        tokens.pop_front();
        if (front == "begin_object")
            begin_object_geo(tokens);
        else if (front == "sphere")
            sphere_geo(tokens);
        else if (front == "assign")
            assign_geo(tokens);
    }
}

void MGL_Parser::begin_object_geo(std::list<std::string> tokens)
{
    // create new TObject?
    std::string name = tokens.front(); tokens.pop_front();
    int numVertices = stoi(tokens.front()); tokens.pop_front();
    MGL_Node *newObject = new MGL_Node();
    GLfloat *vertices = new GLfloat[numVertices*4];
    for (int i = 0; i < numVertices; i++)
    {
        tokens.pop_front(); // remove "v"
        for (int j = 0; j < 3; j++)
        {
            vertices[i*4 + j] = stof(tokens.front());
            tokens.pop_front();
        }
        vertices[i*4 + 3] = 1.0;
    }
    // add triangles to TObject
    newObject->addVerticesByArray4D(vertices,numVertices);
    // TODO: pass TObject back
    tokens.pop_front(); // remove "end_object"
}

void MGL_Parser::sphere_geo(std::list<std::string> tokens)
{
    MGL_SphereNode sphere;
    std::string name = tokens.front(); tokens.pop_front();
    float x = stof(tokens.front()); tokens.pop_front();
    float y = stof(tokens.front()); tokens.pop_front();
    float z = stof(tokens.front()); tokens.pop_front();
    // translate sphere by xyz list (?)

    float radius = stof(tokens.front()); tokens.pop_front();

    // scale sphere by radius

}

void MGL_Parser::assign_geo(std::list<std::string> tokens)
{
    std::string polygon_name = tokens.front(); tokens.pop_front();
    std::string material_name = tokens.front(); tokens.pop_front();

    // add to map in buffer
}


// works on example files
std::list<std::string> MGL_Parser::lex(const std::string filename)
{
    std::list<std::string> tokens;

    std::ifstream inputFile(filename);
    if (!inputFile.is_open())
        return tokens; // return empty tokens list

    for (std::string line; inputFile >> line; )
    {
        tokens.push_back(line);
    }
    inputFile.close();
    return tokens;
}
