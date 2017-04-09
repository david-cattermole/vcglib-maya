/*
 *
 */

#include <vcgNodes/vcgReadFile/vcgReadFileCmd.h>
#include <vcgNodes/vcgReadFile/vcgReadFileNode.h>

#include <vcgNodes/vcgMeshCutter/vcgMeshCutterCmd.h>
#include <vcgNodes/vcgMeshCutter/vcgMeshCutterNode.h>

#include <vcgNodes/vcgMeshStats/vcgMeshStatsCmd.h>
#include <vcgNodes/vcgMeshStats/vcgMeshStatsNode.h>

#include <vcgNodes/vcgDecimate/vcgDecimateCmd.h>
#include <vcgNodes/vcgDecimate/vcgDecimateNode.h>

#include <maya/MFnPlugin.h>

MStatus initializePlugin(MObject obj)
//
//  Description:
//    this method is called when the plug-in is loaded into Maya.  It 
//    registers all of the services that this plug-in provides with 
//    Maya.
//
//  Arguments:
//    obj - a handle to the plug-in object (use MFnPlugin to access it)
//
{
  MStatus status;
  MFnPlugin plugin(obj, PLUGIN_COMPANY, "5.0", "Any");

  // VCG Read File
  {
    status = plugin.registerCommand("vcgReadFile", vcgReadFile::creator);
    if (!status)
    {
      status.perror("VCG Read File: registerCommand");
      return status;
    }

    status = plugin.registerNode("vcgReadFileNode",
                                 vcgReadFileNode::id,
                                 vcgReadFileNode::creator,
                                 vcgReadFileNode::initialize);
    if (!status)
    {
      status.perror("VCG Read File: registerNode");
      return status;
    }
  }

  // VCG Mesh Stats
  {
    status = plugin.registerCommand("vcgMeshStats", vcgMeshStats::creator);
    if (!status)
    {
      status.perror("VCG Mesh Stats: registerCommand");
      return status;
    }

    status = plugin.registerNode("vcgMeshStatsNode",
                                 vcgMeshStatsNode::id,
                                 vcgMeshStatsNode::creator,
                                 vcgMeshStatsNode::initialize);
    if (!status)
    {
      status.perror("VCG Mesh Stats: registerNode");
      return status;
    }
  }

  // VCG Mesh Cutter
  {
    status = plugin.registerCommand("vcgMeshCutter", vcgMeshCutter::creator);
    if (!status)
    {
      status.perror("VCG Mesh Cutter: registerCommand");
      return status;
    }

    status = plugin.registerNode("vcgMeshCutterNode",
                                 vcgMeshCutterNode::id,
                                 vcgMeshCutterNode::creator,
                                 vcgMeshCutterNode::initialize);
    if (!status)
    {
      status.perror("VCG Mesh Cutter: registerNode");
      return status;
    }
  }

  // VCG Decimate
  {
    status = plugin.registerCommand("vcgDecimate", vcgDecimate::creator);
    if (!status)
    {
      status.perror("VCG Decimate: registerCommand");
      return status;
    }

    status = plugin.registerNode("vcgDecimateNode",
                                 vcgDecimateNode::id,
                                 vcgDecimateNode::creator,
                                 vcgDecimateNode::initialize);
    if (!status)
    {
      status.perror("VCG Decimate: registerNode");
      return status;
    }
  }

  return status;
}

MStatus uninitializePlugin(MObject obj)
//
//  Description:
//    this method is called when the plug-in is unloaded from Maya. It 
//    deregisters all of the services that it was providing.
//
//  Arguments:
//    obj - a handle to the plug-in object (use MFnPlugin to access it)
//
{
  MStatus status;
  MFnPlugin plugin(obj);

  // VCG Read File
  {
    status = plugin.deregisterCommand("vcgReadFile");
    if (!status)
    {
      status.perror("VCG Read File: deregisterCommand");
      return status;
    }

    status = plugin.deregisterNode(vcgReadFileNode::id);
    if (!status)
    {
      status.perror("VCG Read File: deregisterNode");
      return status;
    }
  }

  // VCG Read File
  {
    status = plugin.deregisterCommand("vcgMeshStats");
    if (!status)
    {
      status.perror("VCG Read File: deregisterCommand");
      return status;
    }

    status = plugin.deregisterNode(vcgMeshStatsNode::id);
    if (!status)
    {
      status.perror("VCG Read File: deregisterNode");
      return status;
    }
  }

  // VCG Mesh Cutter
  {
    status = plugin.deregisterCommand("vcgMeshCutter");
    if (!status)
    {
      status.perror("VCG Mesh Cutter: deregisterCommand");
      return status;
    }

    status = plugin.deregisterNode(vcgMeshCutterNode::id);
    if (!status)
    {
      status.perror("VCG Mesh Cutter: deregisterNode");
      return status;
    }
  }

  // VCG Decimate
  {
    status = plugin.deregisterCommand("vcgDecimate");
    if (!status)
    {
      status.perror("VCG Decimate: deregisterCommand");
      return status;
    }

    status = plugin.deregisterNode(vcgDecimateNode::id);
    if (!status)
    {
      status.perror("VCG Decimate: deregisterNode");
      return status;
    }
  }

  return status;
}
