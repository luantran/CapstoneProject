class Registration(object):
    def __init__(self):
        self.mri_directory = ''
        self.xray = ''
        self.surface = ''

    def setMRIDirectory(self, mriDirectory):
        self.mri_directory = mriDirectory

    def setXRay(self, xray):
        self.xray = xray

    def setSurface(self, surface):
        self.surface = surface
