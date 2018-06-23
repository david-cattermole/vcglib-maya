/*
 *
 */

// Local
#include <vcgNodes/vcgWriteFile/vcgWriteFileFty.h>

// Utils
#include <utilities/debugUtils.h>

// STL
#include <limits>

vcgWriteFileFty::vcgWriteFileFty() :
    fMesh()
{}

vcgWriteFileFty::~vcgWriteFileFty()
{}


// Sets the mesh object for the factory to operate on
void vcgWriteFileFty::setMesh(MObject &mesh)
{
  fMesh = mesh;
}


// Set file path.
void vcgWriteFileFty::setFilePath(MString &value)
{
  fFilePath = value;
}


// Set whether to clean the mesh prior to writing data.
void vcgWriteFileFty::setCleanMesh(bool value)
{
  fCleanMesh = value;
}


void vcgWriteFileFty::setVerbose(bool value)
{
  fVerbose = value;
}
