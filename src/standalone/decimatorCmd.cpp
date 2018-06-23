/*
 *
 */

#define WITH_TEXTURE_COORDS

// Local
#include <other/decimateUtils.h>
#include <other/loadModelUtils.h>


// forward declaration.
void printUsage();


// Main executable.
int main(int argc, char **argv)
{
  if (argc < 4)
  {
    printUsage();
  }

  TriEdgeColQuadParam_t qparams;

  // defaults
  bool verbose = false;
  double targetError = std::numeric_limits<float>::max();
  bool cleaningFlag = true;
  double maxDecimTime = 3600.0; // 60 minutes, 3600 seconds.
  qparams.QualityThr = 0.3;
  qparams.NormalCheck = false;
  qparams.NormalThrRad = vcg::math::ToRad(5.0);
  qparams.OptimalPlacement = true;
  qparams.PreserveTopology = false;
  qparams.PreserveBoundary = false;
  qparams.BoundaryWeight = 0.5;
  qparams.UseArea = true;
  qparams.UseVertexWeight = true;
#ifdef WITH_TEXTURE_COORDS
  qparams.ExtraTCoordWeight = 0.0f;
  qparams.QualityQuadric = true;
#else
  qparams.QualityCheck = true;
  qparams.QualityWeight = true;
#endif

  // parse command line.
  for (int i = 4; i < argc; ++i)
  {
    if (argv[i][0] == '-' && argv[i][1] == '-')
    {
      std::string strArg = argv[i];
      if (strArg == "--verbose")
      {
        verbose = true;
      }
      else if (strArg == "--cleanup")
      {
        if (argv[i + 1] == "1")
        {
          cleaningFlag = true;
        }
        else
        {
          cleaningFlag = false;
        }
      }
      else if (strArg == "--max-time")
      {
        maxDecimTime = double(atof(argv[i + 1]));
      }
      else if (strArg == "--max-error")
      {
        targetError = double(atof(argv[i + 1]));
      }
      else if (strArg == "--min-error")
      {
        qparams.QuadricEpsilon = double(atof(argv[i + 1]));
      }
      else if (strArg == "--quality-threshold")
      {
        qparams.QualityThr = atof(argv[i + 1]);
      }
      else if (strArg == "--use-normal-threshold")
      {
        if (argv[i + 1] == "1")
        {
          qparams.NormalCheck = true;
        }
        else
        {
          qparams.NormalCheck = false;
        }
      }
      else if (strArg == "--normal-threshold")
      {
        qparams.NormalThrRad = vcg::math::ToRad(atof(argv[i + 1]));
      }
      else if (strArg == "--boundary-weight")
      {
        qparams.BoundaryWeight = atof(argv[i + 1]);
      }
      else if (strArg == "--preserve-boundary")
      {
        if (argv[i + 1] == "1")
        {
          qparams.PreserveBoundary = true;
        }
        else
        {
          qparams.PreserveBoundary = false;
        }
      }
      else if (strArg == "--preserve-topology")
      {
        if (argv[i + 1] == "1")
        {
          qparams.PreserveTopology = true;
        }
        else
        {
          qparams.PreserveTopology = false;
        }
      }
      else if (strArg == "--use-area")
      {
        if (argv[i + 1] == "1")
        {
          qparams.UseArea = true;
        }
        else
        {
          qparams.UseArea = false;
        }
      }
      else if (strArg == "--optimal-placement")
      {
        if (argv[i + 1] == "1")
        {
          qparams.OptimalPlacement = true;
        }
        else
        {
          qparams.OptimalPlacement = false;
        }
      }
      else if (strArg == "--scale-independent")
      {
        if (argv[i + 1] == "1")
        {
          qparams.ScaleIndependent = true;
        }
        else
        {
          qparams.ScaleIndependent = false;
        }
      }
      else
      {
        ERR("Unknown option '" << strArg << "'");
        exit(0);
      }
    }
  }

  // Print Options
  VRB("Basic Options:");
  VRB(" Verbose: " << verbose);
  VRB(" Cleaning: " << cleaningFlag);
  VRB(" Max Decimation Time: " << maxDecimTime << " seconds"
                               << std::endl);

  VRB("Quality Options:");
  VRB(" Max Error: " << targetError);
  VRB(" Min Error: " << qparams.QuadricEpsilon);
  VRB(" Quality Threshold: " << qparams.QualityThr
                             << std::endl);

  VRB("Normal Options:");
  VRB(" Normal Checking: " << qparams.NormalCheck);
  VRB(" Normal Threshold: " << vcg::math::ToDeg(qparams.NormalThrRad) << " degrees"
                            << std::endl);

  VRB("Miscellaneous Options:");
  VRB(" Boundary Weight: " << qparams.BoundaryWeight);
  VRB(" Preserve Boundary: " << qparams.PreserveBoundary);
  VRB(" Preserve Topology: " << qparams.PreserveTopology);
  VRB(" Use Area: " << qparams.UseArea);
  VRB(" Optimal Placement: " << qparams.OptimalPlacement);
  VRB(" Scale Independent: " << qparams.ScaleIndependent
                             << std::endl);

  Timer timer;
  timer.m_total.start();

  // Load Mesh
  timer.m_meshOpen.start();
  Mesh_t mesh;
  int mask = vcg::tri::io::Mask::IOM_ALL;
  int stat = loadModel(argv[1], mesh, mask, verbose);
  if (stat == EXIT_FAILURE) {return stat;}

  timer.m_meshOpen.stop();
  timer.m_cleanup.start();

  // Compute...
  double percentSize = (double) (atof(argv[3]) / 100.0);
  decimate(
      mesh,
      cleaningFlag,
      percentSize, targetError, maxDecimTime,
      timer,
      qparams,
      verbose);

  // Export
  timer.m_meshSave.start();
  INFO("Saving Mesh: " << argv[2] << std::endl);
  vcg::tri::io::Exporter<Mesh_t>::Save(mesh, argv[2], mask);
  timer.m_meshSave.stop();

  timer.m_total.stop();
  if (verbose)
  {
    timer.printResults();
  }
  else
  {
    timer.m_total.printInSec("Total");
  }
  INFO("Done!");
  return EXIT_SUCCESS;
}


void printUsage()
{
  std::cout
      << "Decimator 1.3" << std::endl
      << __DATE__ << std::endl
      << std::endl
      << "Usage: decimator [file in] [file out] [percent] [arg]" << std::endl
      << "Options and arguments:" << std::endl
      << "     --verbose                      Print out details of program (default no)" << std::endl
      << "     --cleanup [0|1]                Remove duplicate & unreferenced vertices (default yes)" << std::endl
      << "     --max-time [num]               Maximum decimation time in seconds (default 3600 seconds)" << std::endl
      << std::endl
      << "     --max-error [num]              QuadricError threshold  (range [0,inf) default inf)" << std::endl
      << "     --min-error [num]              Minimal admitted quadric value (default 1e-15, must be >0)" << std::endl
      << "     --use-quality-threshold [0|1]  Use or not Quality Threshold (default 1)" << std::endl
      << "     --quality-threshold [num]      Quality threshold (range [0.0, 0.866],  default 0.3 )" << std::endl
      << std::endl
      << "     --use-normal-threshold [0|1]   Use Normal Threshold (default 0)" << std::endl
      << "     --normal-threshold [num]       Normal threshold  (degree range [0,180] default 90)" << std::endl
      << std::endl
      << "     --boundary-weight [num]        Boundary Weight (default 0.5)" << std::endl
      << "     --preserve-boundary [0|1]      Preserve or not mesh boundary (default 0)" << std::endl
      << "     --preserve-topology [0|1]      Preserve or not mesh topology (default 0)" << std::endl
      << "     --use-area [0|1]               Use Area Weighted Quadrics (default 1)" << std::endl
      << "     --optimal-placement [0|1]      Use vertex optimal placement (default 1)" << std::endl
      << "     --scale-independent [0|1]      Use Scale Independent quadric measure (default 1) " << std::endl
      << "     --safe-heap-update [0|1]       Use Safe Heap Update (default 0)" << std::endl
      << std::endl;
  exit(-1);
}
