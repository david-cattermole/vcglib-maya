/*
 *
 */

#ifndef VCG_NODES_LOAD_MODEL_UTILS_H
#define VCG_NODES_LOAD_MODEL_UTILS_H

// Utilities
#include <debugUtils.h>

// STL
#include <ctime>
#include <cmath>

// VCG
#include <vcg/complex/complex.h>
#include <wrap/io_trimesh/import.h>
#include <wrap/io_trimesh/export.h>

template<class MESH_TYPE>
int loadModel(const char *filePath, MESH_TYPE &mesh, int mask, bool verbose)
{
  INFO("Loading Mesh: " << filePath);
  int err = vcg::tri::io::Importer<MESH_TYPE>::Open(mesh, filePath, mask);
  if (vcg::tri::io::Importer<Mesh_t>::ErrorCritical(err))
  {
    ERR("Unable to open mesh "
            << "'" << filePath << "' "
            << " : '" << vcg::tri::io::Importer<MESH_TYPE>::ErrorMsg(err) << "'");
    return EXIT_FAILURE;
  }
   INFO(" Mesh vertices (vn): " << mesh.VN());
   INFO(" Mesh faces (fn):    " << mesh.FN());
  return EXIT_SUCCESS;
}


#endif // VCG_NODES_LOAD_MODEL_UTILS_H
