/*
 *
 */

// Local
#include <vcgNodes/vcgReadFile/vcgReadFileFty.h>

// Utils
#include <debugUtils.h>

// STL
#include <limits>

vcgReadFileFty::vcgReadFileFty() :
    fMesh()
{}

vcgReadFileFty::~vcgReadFileFty()
{}

// Sets the mesh object for the factory to operate on
void vcgReadFileFty::setMesh(MObject &mesh)
{
  fMesh = mesh;
}


// Set file path.
void vcgReadFileFty::setFilePath(MString &value)
{
  fFilePath = value;
}
