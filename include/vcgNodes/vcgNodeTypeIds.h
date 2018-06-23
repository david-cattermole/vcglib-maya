/*
 * Global definition of the MTypeIds used in this Maya plugin.
 *
 * Every ID must be unique, and must not share an id with another external
 * plugin.
 *
 * These plugins are used to differentiate node types when nodes are saved into
 * a Maya binary file, changing these ids will invalidate all saved files.
 */

#define VCG_DECIMATE_NODE_ID 0x00085001
#define VCG_MESH_CUTTER_NODE_ID 0x00085002
#define VCG_MESH_STATS_NODE_ID 0x00085110
#define VCG_READ_FILE_NODE_ID 0x0008510A
#define VCG_WRITE_FILE_NODE_ID 0x0008510B
