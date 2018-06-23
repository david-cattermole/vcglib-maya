"""
Example and test program for solver.
"""

import maya.cmds

import testUtils

NODE_TYPE = 'vcgMeshCutterNode'


class MeshCutterTestCase(testUtils.BaseTestCase):

    def runTest(self):
        """
        Read File node test.
        """
        # dataDir = self.getDataRoot()
        self.assertTrue(self.hasNodeTypes([NODE_TYPE]))

        sph = maya.cmds.polySphere()
        sphShape = maya.cmds.listRelatives(sph[0], shapes=True)[0]
        print 'sphere', sph
        print 'sphereShape', sphShape
        maya.cmds.setAttr(sph[0]+'.translateX', 1.0)

        node = maya.cmds.vcgMeshCutter(sph[0])
        print 'node:', node
        if not node:
            maya.cmds.error('vcgMeshCutter failed to give a node name')
            return False
        node = ['vcgMeshCutterNode1']
        maya.cmds.select(sph[0])
        maya.cmds.connectAttr(sph[0]+'.worldMatrix[0]', node[0]+'.worldMatrix')

        cube = maya.cmds.polyCube()
        cubeShape = maya.cmds.listRelatives(cube[0], shapes=True)[0]
        print 'cube', cube
        print 'cubeShape', cubeShape
        maya.cmds.setAttr(cube[0]+'.translateX', 1.5)
        maya.cmds.setAttr(cube[0]+'.scaleX', 1.5)
        maya.cmds.setAttr(cube[0]+'.scaleY', 1.5)
        maya.cmds.setAttr(cube[0]+'.scaleZ', 1.5)
        maya.cmds.setAttr(cubeShape+'.overrideEnabled', 1)
        maya.cmds.setAttr(cubeShape+'.overrideShading', 0)
        maya.cmds.addAttr(cubeShape, longName='shapeType', at='enum', enumName='None=-1:Cube=0:Sphere=1')
        maya.cmds.setAttr(cubeShape+'.shapeType', 0)
        maya.cmds.connectAttr(cubeShape+'.boundingBoxMin', node[0]+'.cutterBBoxMin')
        maya.cmds.connectAttr(cubeShape+'.boundingBoxMax', node[0]+'.cutterBBoxMax')
        maya.cmds.connectAttr(cubeShape+'.worldMatrix[0]', node[0]+'.cutterMatrix')
        maya.cmds.connectAttr(cubeShape+'.shapeType', node[0]+'.cutterShapeType')

        # Export
        print 'Exporting...'
        self.saveMayaAsciiFile(NODE_TYPE, 'test')
        return
