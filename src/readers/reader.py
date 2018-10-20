from abc import ABC, abstractmethod


class Reader(ABC):
    def __init__(self):
        self.filepath = ''
        super().__init__()

    @abstractmethod
    def setFilePath(self, filepath):
        self.filepath = filepath

    @abstractmethod
    def getPolyData(self):
        pass

    @abstractmethod
    def getVTKActor(self):
        pass

    @abstractmethod
    def getLandmarks(self):
        pass
