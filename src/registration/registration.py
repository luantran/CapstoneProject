from src.registration import wrlReader

class Registration(object):
    def __init__(self, frame):
        self.mri_directory = ''
        self.xray = ''
        self.surface = ''
        self.frame = frame

    def setMRIDirectory(self, mriDirectory):
        self.mri_directory = mriDirectory

    def setXRay(self, xray):
        self.xray = xray
        self.wrlReader = wrlReader.WRLReader(self.frame)
        self.wrlReader.setWRLFile(self.xray)

    def setSurface(self, surface):
        self.surface = surface

    def removeXray(self):
        self.wrlReader.hide()

    def processXray(self):
        self.wrlReader.initializeRenderer()
        self.wrlReader.importVRMLData()
        self.wrlReader.renderInFrame()
        print("Doing xray stuff")

    def processSurface(self):
        print("Doing surface topography stuff")

