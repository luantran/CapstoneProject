from abc import ABC, abstractmethod

class Registration(ABC):
    def __init__(self):
        super().__init__()

    @abstractmethod
    def SurfaceXRayRegistration(self, szeLMReader, wrlLMReader, szeReader):
        pass

    @abstractmethod
    def MRIXRayRegistration(self, mriLMReader, wrlLMReader, mriReader):
        pass