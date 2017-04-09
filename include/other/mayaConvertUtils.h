/*
 *
 */

#ifndef VCG_NODES_MAYA_CONVERT_UTILS_H
#define VCG_NODES_MAYA_CONVERT_UTILS_H

// VCG
#include <vcg/complex/complex.h>

// Maya
#include <maya/MIntArray.h>
#include <maya/MFnMesh.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnDagNode.h>
#include <maya/MFloatPointArray.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MDagPath.h>
#include <maya/MSelectionList.h>

inline
void convertMayaToVCG(const MObject &inputObj,
                      const MFnMesh &inputMesh,
                      Mesh_t &outputMesh)
{
  DBG("convertMayaToVCG: start");
  typedef Mesh_t::VertexIterator VertexIterator;
  typedef Mesh_t::VertexPointer VertexPointer;
  typedef Mesh_t::FaceIterator FaceIterator;
  typedef Mesh_t::CoordType CoordType;

//  // Compact vertices and faces memory
//  vcg::tri::Allocator<Mesh_t>::CompactVertexVector(outputMesh);
//  vcg::tri::Allocator<Mesh_t>::CompactFaceVector(outputMesh);

  // Get number of verts and faces.
  MStatus status = MS::kSuccess;
  std::size_t numVerts = (size_t) inputMesh.numVertices();
  std::size_t numFaces = (size_t) inputMesh.numPolygons();
//  DBG("convertMayaToVCG: numVerts: " << numVerts);
//  DBG("convertMayaToVCG: numFaces: " << numFaces);

  // Get Points
  // DBG("convertMayaToVCG: 1");
  MFloatPointArray points;
  inputMesh.getPoints(points);
  // DBG("convertMayaToVCG: numPoints: " << points.length());

  // Get Points
  // DBG("convertMayaToVCG: 1b");
  MFloatVectorArray normals;
  inputMesh.getNormals(normals);

  // Get triangle indices
  // DBG("convertMayaToVCG: 2");
  MIntArray triCounts;
  MIntArray triVertIDs;
  inputMesh.getTriangles(triCounts, triVertIDs);
  std::size_t numTriCounts = triCounts.length();
  std::size_t numTriVerts = triVertIDs.length();
  std::size_t numTriFaces = numTriVerts / 3;
  DBG("convertMayaToVCG: numTriCounts: " << numTriCounts);
  DBG("convertMayaToVCG: numTriFaces: " << numTriFaces);
  DBG("convertMayaToVCG: numTriVerts: " << numTriVerts);
  DBG("convertMayaToVCG: verts size: " << outputMesh.vert.size());
  DBG("convertMayaToVCG: faces size: " << outputMesh.face.size());
  DBG("convertMayaToVCG: VN: " << outputMesh.VN());
  DBG("convertMayaToVCG: FN: " << outputMesh.FN());

  outputMesh.Clear();
  vcg::tri::Allocator<Mesh_t>::AddVertices(outputMesh, numVerts);
  vcg::tri::Allocator<Mesh_t>::CompactVertexVector(outputMesh);

  // Add vertices
//  DBG("convertMayaToVCG: 3");
  DBG("convertMayaToVCG: verts size: " << outputMesh.vert.size());
  DBG("convertMayaToVCG: VN: " << outputMesh.VN());
  VertexIterator vi = outputMesh.vert.begin();
  VertexIterator vi_end = outputMesh.vert.end();
  // DBG("convertMayaToVCG: begin: " << &vi);
  // DBG("convertMayaToVCG: end:   " << &vi_end);
  for (unsigned int i = 0; vi != vi_end; ++vi)
  {
    if (!vi->IsD())
    {
      vi->P() = CoordType(points[i][0], points[i][1], points[i][2]);
      vi->N() = CoordType(normals[i][0], normals[i][1], normals[i][2]);
      ++i;
    }
    else
    {
      // DBG("IsD: " << i);
    }
  }

  // Add faces
  DBG("convertMayaToVCG: 4");
  vcg::tri::Allocator<Mesh_t>::AddFaces(outputMesh, numTriFaces);
  DBG("convertMayaToVCG: faces size: " << outputMesh.face.size());
  DBG("convertMayaToVCG: FN: " << outputMesh.FN());
  unsigned int m = 0;
  unsigned int n = 0;
  FaceIterator fi = outputMesh.face.begin();
  for (unsigned int i = 0; i < numTriCounts; ++i)
  {
//    DBG("i=" << i);
//    if((*fi).IsD())
//    {
//      DBG("delete");
//    }
    for (unsigned int j = 0; j < triCounts[i]; ++j)
    {
//      DBG("j=" << j);
//      DBG("n=" << n);
      const float vertNum = 3;
      outputMesh.face[n].Alloc(vertNum);
      for (unsigned int k = 0; k < vertNum; ++k)
      {
//        DBG("k=" << k);
        int vertexIdx = triVertIDs[m++];
        fi->V(k) = &(outputMesh.vert[vertexIdx]);
      }
      ++fi;
      ++n;
    }
  }
  DBG("convertMayaToVCG: 5");
  vcg::tri::Allocator<Mesh_t>::CompactFaceVector(outputMesh);

  // Force compactness of data structure.
  DBG("convertMayaToVCG: 6");
//  vcg::tri::Allocator<Mesh_t>::CompactVertexVector(outputMesh);
//  vcg::tri::Allocator<Mesh_t>::CompactFaceVector(outputMesh);
//  vcg::tri::Allocator<Mesh_t>::CompactEveryVector(outputMesh);

  DBG("convertMayaToVCG: 7");
  vcg::tri::UpdateBounding<Mesh_t>::Box(outputMesh);
  DBG("convertMayaToVCG: 7a");
  vcg::tri::UpdateNormal<Mesh_t>::NormalizePerVertex(outputMesh);
  vcg::tri::UpdateNormal<Mesh_t>::NormalizePerFace(outputMesh);
  vcg::tri::UpdateNormal<Mesh_t>::PerFaceNormalized(outputMesh);
  vcg::tri::UpdateNormal<Mesh_t>::PerVertexNormalized(outputMesh);
  DBG("convertMayaToVCG: 7b");
  vcg::tri::UpdateTopology<Mesh_t>::FaceFace(outputMesh);
  vcg::tri::UpdateTopology<Mesh_t>::TestFaceFace(outputMesh);
  vcg::tri::UpdateTopology<Mesh_t>::VertexFace(outputMesh);
  vcg::tri::UpdateTopology<Mesh_t>::VertexEdge(outputMesh);
  vcg::tri::UpdateTopology<Mesh_t>::EdgeEdge(outputMesh);
  vcg::tri::UpdateTopology<Mesh_t>::AllocateEdge(outputMesh);
  DBG("convertMayaToVCG: 7c");
  vcg::tri::UpdateFlags<Mesh_t>::Clear(outputMesh);
  vcg::tri::UpdateFlags<Mesh_t>::FaceBorderFromFF(outputMesh);
  vcg::tri::UpdateFlags<Mesh_t>::VertexBorderFromFaceBorder(outputMesh);
  vcg::tri::UpdateFlags<Mesh_t>::FaceBorderFromFF(outputMesh);
  DBG("convertMayaToVCG: 7d");
  vcg::tri::Allocator<Mesh_t>::CompactEveryVector(outputMesh);

  assert(outputMesh.vert.size() == outputMesh.VN());
  assert(outputMesh.face.size() == outputMesh.FN());
  DBG("convertMayaToVCG: end");
}


inline
void convertVCGToMaya(MObject &inputObj,
                      Mesh_t &inputMesh,
                      MFnMesh &outputMesh)
{
  DBG("convertVCGToMaya: start");
  typedef Mesh_t::VertexIterator VertexIterator;
  typedef Mesh_t::FaceIterator FaceIterator;
  typedef Mesh_t::ConstVertexIterator ConstVertexIterator;
  typedef Mesh_t::ConstFaceIterator ConstFaceIterator;

  // Force compactness of data structure.
//  vcg::tri::Allocator<Mesh_t>::CompactFaceVector(inputMesh);
//  vcg::tri::Allocator<Mesh_t>::CompactVertexVector(inputMesh);
  vcg::tri::Allocator<Mesh_t>::CompactEveryVector(inputMesh);

//  DBG("convertVCGToMaya: 1");
  MStatus status = MS::kSuccess;
  unsigned int numVertices = (unsigned int) inputMesh.VN();
  unsigned int numFaces = (unsigned int) inputMesh.FN();

  assert(inputMesh.vert.size() == inputMesh.VN());
  assert(inputMesh.face.size() == inputMesh.FN());

  // Vertices
  //
  // FIXME: Some how we cannot pre-allocate the vertex array,
  // otherwise we get incorrect results. So we must 'append'
  // each point, which is likely slow.
  //
//  DBG("convertVCGToMaya: 2");
  MFloatPointArray vertexArray;
  VertexIterator cvi = inputMesh.vert.begin();
  VertexIterator cvi_end = inputMesh.vert.end();
  for (int i = 0; cvi != cvi_end; ++cvi)
  {
    MFloatPoint p(cvi->P()[0],
                  cvi->P()[1],
                  cvi->P()[2],
                  1.0);
    vertexArray.append(p);
  }

  // Faces Counts
//  DBG("convertVCGToMaya: 3");
  unsigned int faceTotal = 0;
  FaceIterator fi;
  MIntArray polygonCounts(numFaces);
  fi = inputMesh.face.begin();
  for (int i = 0; i < inputMesh.face.size(); ++i)
  {
    int num = fi->VN();
    polygonCounts[i] = num;
    // polygonCounts.append(num);
    faceTotal += num;
  }

  // Faces Connects
  //
  // FIXME: Some how we cannot pre-allocate the polygon
  // counts array, otherwise we get incorrect results.
  // So we must 'append' each point, which is likely slow.
  //
//  DBG("convertVCGToMaya: 4");
  MIntArray polygonConnects;
  fi = inputMesh.face.begin();
  for (int i = 0, k = 0, m = 0; i < inputMesh.face.size(); ++i)
  {
    int num = polygonCounts[i];

    for (int j = 0; j < num; ++j)
    {
      Mesh_t::VertexPointer vp = fi->V(j);
      int index = (int) vcg::tri::Index(inputMesh, vp);
      polygonConnects.append(index);
    }
    ++fi;
  }
  assert(vertexArray.length() == numVertices);
  assert(polygonCounts.length() == numFaces);

  // Create the Maya mesh.
//  DBG("convertVCGToMaya: 5");
  MObject outputObj = outputMesh.create(
      numVertices,
      numFaces,
      vertexArray,
      polygonCounts,
      polygonConnects,
      inputObj,
      &status);
  CHECK_MSTATUS(status);

  DBG("convertVCGToMaya: end");
  return;
}

#endif // VCG_NODES_MAYA_CONVERT_UTILS_H