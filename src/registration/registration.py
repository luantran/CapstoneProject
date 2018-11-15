from abc import ABC, abstractmethod

class Registration(ABC):
    def __init__(self):
        super().__init__()

    @abstractmethod
    def SurfaceXRayRegistration(self, surfaceLMData, xrayLMData):
        pass

    @abstractmethod
    def MRIXRayRegistration(self, mriLMData, xrayLMData):
        pass