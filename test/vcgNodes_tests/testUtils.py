"""
Utilities for creating tests.
"""

import sys
import os
import unittest

PLUGIN_NAME = 'vcgNodes'


class BaseTestCase(unittest.TestCase):

    def setUp(self):
        path = os.path.dirname(__file__)
        path = os.path.join(path, '..', '..')
        self.__projectRoot = os.path.abspath(path)
        print 'Running:', self.id()
        self.reinitializeScene()

    def tearDown(self):
        pass

    def getProjectRoot(self):
        return self.__projectRoot

    def getDataRoot(self):
        root = self.getProjectRoot()
        assert root and os.path.isdir(root)
        return os.path.join(root, 'data')

    def getTestRoot(self):
        root = self.getProjectRoot()
        assert root and os.path.isdir(root)
        return os.path.join(root, 'test')

    def getTestTempRoot(self):
        root = self.getProjectRoot()
        assert root and os.path.isdir(root)
        return os.path.join(root, 'test', 'temp')

    def getTempFilePath(self, className, fileName, fileExt):
        root = self.getTestTempRoot()
        name = '{className}_{fileName}_output.{ext}'.format(className=className,
                                                            fileName=fileName,
                                                            ext=fileExt)
        path = os.path.join(root, name)
        return path

    def saveMayaAsciiFile(self, className, fileName,
                          selectedOnly=False):
        fileExt = 'ma'
        path = self.getTempFilePath(className, fileName, fileExt)
        print 'Save:', path

        import maya.cmds
        if selectedOnly:
            maya.cmds.file(path,
                           exportSelected=True,
                           type='mayaAscii',
                           constructionHistory=False,
                           force=True)
        else:
            maya.cmds.file(rename=path)
            maya.cmds.file(save=True,
                           type='mayaAscii',
                           constructionHistory=True,
                           force=True)
        return

    def loadPlugin(self, pluginName):
        """Loads a plugin."""
        import maya.cmds
        result = True
        try:
            maya.cmds.loadPlugin(pluginName)
        except RuntimeError, e:
            print 'LoadPlugin Error:', e
            result = False
        return result

    def reloadPlugins(self):
        """Reloads all plugins needed for vcgNodes."""
        import maya.cmds
        maya.cmds.unloadPlugin(PLUGIN_NAME, force=True)
        print 'Plugins loaded...', self.loadPlugin(PLUGIN_NAME)

    def hasNodeType(self, pluginNode):
        """
        Checks if a node type is available.

        :param pluginNode:
        :type pluginNode: str
        """
        import maya.cmds
        allTypes = maya.cmds.allNodeTypes()
        return pluginNode in allTypes

    def hasNodeTypes(self, pluginNodes):
        import maya.cmds
        for pluginNode in pluginNodes:
            if not self.hasNodeType(pluginNode):
                print 'Node Type %s has not loaded.' % pluginNode
                return False
        return True

    def reinitializeScene(self):
        import maya.cmds
        maya.cmds.file(new=True, force=True)
        self.reloadPlugins()


CALLBACK_ID = None


def printToConsoleStart():
    """
    Begin writing to terminal.
    """
    import maya.api.OpenMaya as om

    global CALLBACK_ID

    if CALLBACK_ID is None:
        CALLBACK_ID = om.MCommandMessage.addCommandOutputFilterCallback(
            writeToTerminal)

    # # force maya to print MEL print statments into the terminal.
    # def callback(nativeMsg, messageType, data):
    #     print nativeMsg, messageType, data
    # CALLBACK_ID = om.MCommandMessage.addCommandOutputCallback(callback, None)

    # force Maya to print out into the terminal.
    sys.stdout = sys.__stdout__
    # dir(sys.stdout)
    print "hello, world"


def printToConsoleEnd():
    """
    Stop writing to terminal
    """
    import maya.api.OpenMaya as om

    global CALLBACK_ID

    if not CALLBACK_ID is None:
        om.MMessage.removeCallback(CALLBACK_ID)
        CALLBACK_ID = None


def writeToTerminal(msg, msgType, filterOutput, clientData):
    """
    This is the callback function that gets called when Maya wants to print something.
    It will take the msg and output it to the terminal rather than the Maya Script Editor
    """
    import maya.api.OpenMaya as om
    om.MScriptUtil.setBool(filterOutput, True)
    sys.__stdout__.write(str(msg))
    sys.__stdout__.flush()
