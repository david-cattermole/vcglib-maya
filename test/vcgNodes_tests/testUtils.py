"""
Utilities for creating tests.
"""

import sys
import os
import random
import maya.api.OpenMaya as om
import maya.cmds

PLUGIN_NAME = 'vcgNodes'

IMAGE_DIR1 = "/data/Tutorials/Footage/fxphdLidarShoot/clip04_85mm/"
IMAGE_DIR2 = "/data/Tutorials/Footage/distortion/lensDistortion/A009_C006_0618FM/"
IMAGE_PATH1 = os.path.join(IMAGE_DIR1, "clip04_85mm.0430.png")
IMAGE_PATH2 = os.path.join(IMAGE_DIR2, "A009_C006_0618FM.0000302.exr")
IMAGE_INDEX_NUM = 2

CALLBACK_ID = None
def printToConsoleStart():
    """
    Begin writing to terminal.
    """
    global CALLBACK_ID

    if CALLBACK_ID is None:
        CALLBACK_ID = om.MCommandMessage.addCommandOutputFilterCallback(writeToTerminal)

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
    global CALLBACK_ID

    if not CALLBACK_ID is None:
        om.MMessage.removeCallback(CALLBACK_ID)
        CALLBACK_ID = None


def writeToTerminal(msg, msgType, filterOutput, clientData):
    """
    This is the callback function that gets called when Maya wants to print something.
    It will take the msg and output it to the terminal rather than the Maya Script Editor
    """
    om.MScriptUtil.setBool(filterOutput, True)
    sys.__stdout__.write(str(msg))
    sys.__stdout__.flush()


def loadPlugin(pluginName):
    """Loads a plugin."""
    result = True
    try:
        maya.cmds.loadPlugin(pluginName)
    except RuntimeError, e:
        print 'LoadPlugin Error:', e
        result = False
    return result

    
def reloadPlugins():
    """Reloads all plugins needed for opNodes."""
    maya.cmds.unloadPlugin(PLUGIN_NAME, force=True)
    print 'Plugins loaded...', loadPlugin(PLUGIN_NAME)

    
def hasNodeType(pluginNode):
    """
    Checks if a node type is available.

    :param pluginNode:
    :type pluginNode: str
    """
    allTypes = maya.cmds.allNodeTypes()
    return pluginNode in allTypes


def hasNodeTypes(pluginNodes):
    for pluginNode in pluginNodes:
        if not hasNodeType(pluginNode):
            print 'Node Type %s has not loaded.' % pluginNode
            return False
    return True


def reinitializeScene():
    maya.cmds.file(new=True, force=True)
    reloadPlugins()


# def getTestImage(index):
#     result = None
#     if index == 0:
#         result = IMAGE_PATH1
#     elif index == 1:
#         result = IMAGE_PATH2
#     return result
#
#
# def getRandomTestImage():
#     result = None
#
#     # pick a random directory
#     # dirs = [IMAGE_DIR1, IMAGE_DIR2]
#     dirs = [IMAGE_DIR1, IMAGE_DIR1]
#     imageDir = random.choice(dirs)
#
#     # pick a random image from a random directory.
#     if os.path.isdir(imageDir):
#         result = os.path.join(imageDir, random.choice(os.listdir(imageDir)))
#
#     return result