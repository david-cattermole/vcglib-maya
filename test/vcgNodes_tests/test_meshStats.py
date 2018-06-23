"""
Example and test program for solver.
"""

import maya.cmds

import testUtils

NODE_TYPE = 'vcgMeshStatsNode'


class MeshStatsTestCase(testUtils.BaseTestCase):

    def runTest(self):
        """
        Read File node test.
        """
        dataDir = self.getDataRoot()
        self.assertTrue(self.hasNodeTypes([NODE_TYPE]))

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
        print 'Out Centre Of Mass:', maya.cmds.getAttr(node[0]+'.outCentreOfMass')
        print 'Out Mass:', maya.cmds.getAttr(node[0]+'.outMass')
        print maya.cmds.getAttr(node+'.outCentreOfMass')
        print maya.cmds.getAttr(node+'.outMass')

        self.saveMayaAsciiFile(NODE_TYPE, 'test')
        return