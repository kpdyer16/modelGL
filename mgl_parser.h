#ifndef MGL_PARSER_H
#define MGL_PARSER_H

#include <string>
#include <vector>
#include <fstream>
#include <QVector4D>
#include <mgl_node.h>
#include <mgl_spherenode.h>

class MGL_Parser
{
private:

    void parse_geo(const std::string);
    void begin_object_geo(std::list<std::string>);
    void sphere_geo(std::list<std::string>);
    void assign_geo(std::list<std::string>);

    std::list<std::string> lex(const std::string);
public:
    MGL_Parser();

    void parse(const std::string);
};

#endif // MGL_PARSER_H
