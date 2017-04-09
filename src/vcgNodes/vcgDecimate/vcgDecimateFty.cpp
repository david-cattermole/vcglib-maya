/*
 *
 */

// Local
#include <vcgNodes/vcgDecimate/vcgDecimateFty.h>

// Utils
#include <debugUtils.h>

// STL
#include <limits>

vcgDecimateFty::vcgDecimateFty() :
    fPercent(100.0),
    fTargetError(std::numeric_limits<double>::max()),
    fMaxTime(3600.0), // 3600 seconds = 1 hour.
    fUseQualityThreshold(true),
    fQualityThreshold(0.3),
    fUseNormalThreshold(false),
    fNormalThreshold(5.0),
    fBoundaryWeight(0.5),
    fPreserveBoundary(false),
    fVerbose(false)
{}

vcgDecimateFty::~vcgDecimateFty()
{}

// Sets the mesh object for the factory to operate on
void vcgDecimateFty::setMesh(MObject &mesh)
{
  fMesh = mesh;
}

void vcgDecimateFty::setPercent(double value)
{
  fPercent = value;
}

void vcgDecimateFty::setTargetError(double value)
{
  fTargetError = value;
}


void vcgDecimateFty::setMaxTime(double value)
{
  fMaxTime = value;
}


void vcgDecimateFty::setUseQualityThreshold(bool value)
{
  fUseQualityThreshold = value;
}


void vcgDecimateFty::setQualityThreshold(double value)
{
  fQualityThreshold = value;
}


void vcgDecimateFty::setUseNormalThreshold(bool value)
{
  fUseNormalThreshold = value;
}


void vcgDecimateFty::setNormalThreshold(double value)
{
  fNormalThreshold = value;
}


void vcgDecimateFty::setBoundaryWeight(double value)
{
  fBoundaryWeight = value;
}


void vcgDecimateFty::setPreserveBoundary(bool value)
{
  fPreserveBoundary = value;
}


void vcgDecimateFty::setVerbose(bool value)
{
  fVerbose = value;
}


MObject &vcgDecimateFty::getMesh()
{
  return fMesh;
}

