/*
 *
 */

// Local
#include <vcgNodes/vcgMeshCutter/vcgMeshCutterFty.h>

// Utils
#include <other/decimateUtils.h>
#include <other/mayaConvertUtils.h>

// STL
#include <unordered_map>

// General Includes
#include <maya/MGlobal.h>
#include <maya/MIOStream.h>
#include <maya/MFloatPointArray.h>
#include <maya/MMatrixArray.h>
#include <maya/MBoundingBox.h>
#include <maya/MFnTransform.h>

// Function Sets
#include <maya/MFnMesh.h>

// Iterators
#include <maya/MItMeshPolygon.h>
#include <maya/MItMeshEdge.h>

// #define CHECK_STATUS(st) if ((st) != MS::kSuccess) { break; }

MStatus vcgMeshCutterFty::doIt()
//
//  Description:
//    Performs the operation on the selected mesh and components
//
{
  MStatus status;
  Timer timer;
  timer.m_total.start();
  const std::string toolHeader = "Mesh Cutter: ";

  // Get access to the mesh's function set
  if (fMesh.isNull())
  {
    ERR(toolHeader << "Mesh Object is invalid");
    return MS::kFailure;
  }
  MFnMesh meshFn(fMesh);

  MPointArray bboxMinArray;
  MPointArray bboxMaxArray;
  MMatrixArray bboxMatrixArray;
  MIntArray bboxShapeArray;

  bboxMinArray.append(fCutterBBoxMin);
  bboxMaxArray.append(fCutterBBoxMax);
  bboxMatrixArray.append(fCutterMatrix);
  bboxShapeArray.append(fCutterShapeType);

  // get DAG path to shape
  MDagPath objectDag;
  MDagPath::getAPathTo(fMesh, objectDag);
  MDagPath objectShapeDag = objectDag;
  objectShapeDag.extendToShape();

  // create mesh polygon iterator
  MItMeshPolygon polyIter(fMesh);

  // create an array for storing components.
  MIntArray components = MIntArray(polyIter.count(), 0);

  // Get current shape node's world-space transform.
  MMatrix objTransformation = fWorldMatrix;

  // loop over object faces, check if they are inside the cutter volume.
  unsigned int selectionCount = 0;
  polyIter.reset();
  int polyNum = polyIter.count();
  for (unsigned int i = 0; i < polyNum; i++)
  {

    MPoint centre = polyIter.center(MSpace::kWorld) * objTransformation;
    if (isInsideCutterVolume(centre,
                             bboxMinArray,
                             bboxMaxArray,
                             bboxMatrixArray,
                             bboxShapeArray))
    {
      selectionCount++;
      unsigned int index = polyIter.index();
      components[index] = 1;
    }
    polyIter.next();
  }

  // get invert selection value
  unsigned int selValue = 1;
  if (fInvert)
  {
    selValue = 0;
  }

  // Delete faces...

  // Nothing needs to be done.
  if (selectionCount == polyNum || (selectionCount == 0))
  {
    if (selValue == 1)
    {
      // delete the mesh
      // TODO: This does not actually seem to be working.
      MFloatPointArray vertexArray;
      MIntArray polygonCounts;
      MIntArray polygonConnects;
      status = meshFn.createInPlace(0, 0, vertexArray, polygonCounts, polygonConnects);
    }
    else
    {
      // nothing needs to be done.
    }
    return MS::kSuccess;
  }

  // old counts and connects
  MIntArray oldCounts;
  MIntArray oldConnects;
  meshFn.getVertices(oldCounts, oldConnects);

  // old verts
  MFloatPointArray oldVertices;
  meshFn.getPoints(oldVertices);

  // create new counts array
  unsigned int newCountsLength = 0;
  if (!fInvert)
  {
    newCountsLength = polyNum - selectionCount;
  }
  else
  {
    newCountsLength = selectionCount;
  }
  MIntArray newCounts = MIntArray(newCountsLength);

  // calculate new mesh values - counts
  unsigned int newConnectsLength = 0;
  for (unsigned int i = 0, j = 0; i < polyNum; i++)
  {
    if (components[i] != selValue)
    {
      unsigned int count = (unsigned int) oldCounts[i];
      newConnectsLength += count;
      newCounts[j++] = count;
    }
  }

  // create new connects array
  MIntArray newConnects = MIntArray(newConnectsLength);

  // create new vertices array
  MFloatPointArray newVertices;

  // calculate new mesh values - connects and vertices
  typedef std::unordered_map<unsigned int, unsigned int> IndexMap;
  typedef IndexMap::iterator IndexMapIt;
  typedef IndexMap::const_iterator IndexMapCIt;
  IndexMap vertexIndexMap;

  unsigned int connectIndex = 0;
  unsigned int foundNum = 0;
  unsigned int notFoundNum = 0;
  polyIter.reset();
  for (unsigned int i = 0; i < polyNum; i++)
  {
    for (unsigned int k = 0; k < oldCounts[i]; k++)
    {
      if (components[i] != selValue)
      {
        int tmpPrev = 0;
        polyIter.setIndex(i, tmpPrev);
        unsigned int oldVertIndex = polyIter.vertexIndex(k);

        IndexMapCIt found = vertexIndexMap.find(oldVertIndex);
        if (found == vertexIndexMap.end())
        {
          ++notFoundNum;

          unsigned int newVertIndex = newVertices.length();
          newVertices.append(oldVertices[oldVertIndex]);

          vertexIndexMap[oldVertIndex] = newVertIndex;
          newConnects[connectIndex] = newVertIndex;
        }
        else
        {
          ++foundNum;
          newConnects[connectIndex] = found->second;
        }

        connectIndex++;
      }
    }
  }

  // create the new mesh
  status = meshFn.createInPlace(newVertices.length(),
                                newCounts.length(),
                                newVertices,
                                newCounts,
                                newConnects);
  if (status != MS::kSuccess)
  {
    ERR(toolHeader << "meshFn.createInPlace failed!" << status.errorString());
  }

  return status;
}

// Distance between two points.
double vcgMeshCutterFty::distance(MPoint p0, MPoint p1)
{
  double dx = (p0.x - p1.x);
  double dy = (p0.y - p1.y);
  double dz = (p0.z - p1.z);
  return std::sqrt((dx * dx) + (dy * dy) + (dz * dz));
}


bool vcgMeshCutterFty::isInsideCutterVolume(MPoint point,
                                         MPointArray &bboxMinArray,
                                         MPointArray &bboxMaxArray,
                                         MMatrixArray &bboxMatrixArray,
                                         MIntArray &bboxShapeArray)
{
  for (unsigned int j = 0; j < bboxMaxArray.length(); j++)
  {
    // convert point from world to object space
    MPoint invPoint = point * bboxMatrixArray[j].inverse();

    // check if the point intersects
    if (bboxShapeArray[j] == kCube) // cube shape type
    {
      // simple min/max bounding box intersection test
      if ((invPoint.x >= bboxMinArray[j].x) &&
          (invPoint.y >= bboxMinArray[j].y) &&
          (invPoint.z >= bboxMinArray[j].z) &&
          (invPoint.x <= bboxMaxArray[j].x) &&
          (invPoint.y <= bboxMaxArray[j].y) &&
          (invPoint.z <= bboxMaxArray[j].z))
      {
        return true;
      }
    }
    else if (bboxShapeArray[j] == kSphere) // sphere shape type
    {
      if (distance(invPoint, MPoint(0.0, 0.0, 0.0)) <= 1.0)
      {
        return true;
      }
    }
  }

  return false;
}


