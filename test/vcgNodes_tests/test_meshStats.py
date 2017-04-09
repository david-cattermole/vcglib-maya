"""
Example and test program for solver.
"""

import maya.cmds

import testUtils

NODE_TYPE = 'vcgMeshStatsNode'

def main():
    print 'Running:', __file__

    print 'Initialize scene...'
    testUtils.reinitializeScene()

    # Ensure it's loaded.
    if not testUtils.hasNodeTypes([NODE_TYPE]):
        assert False

    sph = maya.cmds.polySphere()
    sphShape = maya.cmds.listRelatives(sph[0], shapes=True)[0]
    print 'sphere', sph
    print 'sphereShape', sphShape
    maya.cmds.setAttr(sph[0] + '.translateX', 1.0)
    maya.cmds.polyTriangulate(sph[0])

    inputPath = '/home/davidc/dev/mayaVcgNodes/data/gargoyle.ply'
    readFileNode = maya.cmds.createNode('vcgReadFileNode')
    meshNode = maya.cmds.createNode('mesh')

    # Mesh Stats.
    node = maya.cmds.createNode('vcgMeshStatsNode')
    # node = maya.cmds.vcgMeshStats(meshNode)
    # print 'node:', node
    # if not node:
    #     maya.cmds.error('vcgMeshStats failed to give a node name')
    #     return False
    # node = ['vcgMeshStatsNode1']

    maya.cmds.select(meshNode)
    maya.cmds.sets(edit=True, forceElement='initialShadingGroup')

    maya.cmds.connectAttr(sphShape + '.outMesh', readFileNode + '.inMesh')
    maya.cmds.connectAttr(readFileNode + '.outMesh', node + '.inMesh')
    maya.cmds.connectAttr(node + '.outMesh', meshNode + '.inMesh')

    nodeAttr = readFileNode + '.filePath'
    maya.cmds.setAttr(nodeAttr, inputPath, type='string')

    # create locator
    newTransform = maya.cmds.createNode('transform')
    newLoc = maya.cmds.createNode('locator', parent=newTransform)
    maya.cmds.connectAttr(node+'.outCentreOfMass', newTransform+'.translate')

    # print outputs
    # print 'Out Centre Of Mass:', maya.cmds.getAttr(node[0]+'.outCentreOfMass')
    # print 'Out Mass:', maya.cmds.getAttr(node[0]+'.outMass')
    print maya.cmds.getAttr(node+'.outCentreOfMass')
    print maya.cmds.getAttr(node+'.outMass')

    # Export
    print 'Python: Exporting...'
    path = "/home/davidc/dev/mayaVcgNodes/test/vcgMeshStatsNode_export.ma"
    maya.cmds.select(sph[0], replace=True)
    print maya.cmds.file(path,
                         exportSelected=True,
                         type='mayaAscii',
                         constructionHistory=False,
                         force=True)
    print '=' * 100
    return True

