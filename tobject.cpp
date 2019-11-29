#include "tobject.h"

int TObject::universal_id = 0;

TObject::TObject()
{
    id = universal_id++;
}

TObject::~TObject()
{

}
