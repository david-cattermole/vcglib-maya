"""
Example and test program for solver.
"""

import os
import maya.cmds
import testUtils

NODE_TYPE = 'vcgDecimateNode'


class DecimateTestCase(testUtils.BaseTestCase):
    def runTest(self):
        """
        Test decimate node and command.
        """
        self.assertTrue(self.hasNodeTypes([NODE_TYPE]))

        sph = maya.cmds.polySphere()

        node = maya.cmds.vcgDecimate(sph[0], percent=20)
        if not node:
            maya.cmds.error('vcgDecimate failed to give a node name')
            return False
        node = ['vcgDecimateNode1']
        maya.cmds.select(sph[0])
        maya.cmds.setAttr(node[0] + '.verbose', 1)

        newTransform = maya.cmds.createNode('transform')
        newMesh = maya.cmds.createNode('mesh', parent=newTransform)
        maya.cmds.connectAttr(node[0] + '.outMesh', newMesh + '.inMesh')
        maya.cmds.setAttr(newTransform + '.translateX', 2.0)
        maya.cmds.sets(edit=True, forceElement='initialShadingGroup')

        if not maya.cmds.about(batch=True):
            maya.cmds.viewFit()

        maya.cmds.select(sph[0], replace=True)
        self.saveMayaAsciiFile('vcgDecimateNode', 'mayaObjExport',
                               selectedOnly=True)
        return
