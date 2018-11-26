from abc import ABC, abstractmethod
import vtk
from src.landmarks_readers.szeLandmarksReader import SZEReaderLM
from src.landmarks_readers.mriLandmarksReader import MRIReaderLM
import numpy
class Registration(ABC):
    def __init__(self):
        super().__init__()

    @abstractmethod
    def SurfaceXRayRegistration(self, szeLMReader, wrlLMReader, szeReader):
        pass

    @abstractmethod
    def MRIXRayRegistration(self, mriLMReader, wrlLMReader, mriReader):
        pass

    def getMetrics(self, otherReader, wrlLMReader, Transrigid):
        LMAfter = []
        points = self.getTransformedLandmarksPoints(otherReader.points, Transrigid)
        for i in range(0, points.GetNumberOfPoints()):
            LMAfter.append((points.GetPoint(i)[0], points.GetPoint(i)[1], points.GetPoint(i)[2]))

        wrlLM = []
        if isinstance(otherReader, SZEReaderLM):
            for i in range(0, wrlLMReader.capteurs_points.GetNumberOfPoints()):
                wrlLM.append((wrlLMReader.capteurs_points.GetPoint(i)[0], wrlLMReader.capteurs_points.GetPoint(i)[1],
                              wrlLMReader.capteurs_points.GetPoint(i)[2]))
        elif isinstance(otherReader, MRIReaderLM):
            for i in range(0, wrlLMReader.vertebrae_points.GetNumberOfPoints()):
                wrlLM.append((wrlLMReader.vertebrae_points.GetPoint(i)[0], wrlLMReader.vertebrae_points.GetPoint(i)[1],
                              wrlLMReader.vertebrae_points.GetPoint(i)[2]))
        if len(wrlLM) != len(LMAfter):
            print("Error")
            return None
        print("---------------------------------")
        self.getMeanSquaredError(wrlLM, LMAfter)
        print("---------------------------------")
        self.getAccuracy(wrlLM, LMAfter)
        print("---------------------------------")

    def getMeanSquaredError(self, target, result):
        sumX=0
        sumY=0
        sumZ=0
        sumOverall=0
        length=len(target)
        for i in range(0, length):
            sumX+=(target[i][0] - result[i][0])**2
            sumY+=(target[i][1] - result[i][1])**2
            sumZ+=(target[i][2] - result[i][2])**2
            a_target = numpy.array((target[i][0], target[i][1], target[i][2]))
            b_result = numpy.array((result[i][0], result[i][1], result[i][2]))
            distance = numpy.linalg.norm(a_target-b_result)
            sumOverall+=(distance**2)
        # print("MSE on X coordinates: "+str(float(sumX)/float(length)))
        # print("MSE on Y coordinates: "+str(float(sumY)/float(length)))
        # print("MSE on Z coordinates: "+str(float(sumZ)/float(length)))
        print("MSE overall of coordinates: "+str(float(sumOverall)/float(length)))

    def getAccuracy(self, target, result):
        accX = 0
        accY = 0
        accZ = 0
        accOverall = 0
        length = len(target)
        count = 0
        for i in range(0, length):

            accX = ((target[i][0] - result[i][0])/target[i][0]) * 100
            accY = ((target[i][1] - result[i][1])/target[i][1]) * 100
            accZ = ((target[i][2] - result[i][2])/target[i][2]) * 100
            accOverall += float(accX + accY + accZ)/float(3)
        # print("Accuracy on X coordinates: " + str(float(accX) / float(length)))
        # print("Accuracy on Y coordinates: " + str(float(accY) / float(length)))
        # print("Accuracy on Z coordinates: " + str(float(accZ) / float(length)))
        print("Accuracy overall of coordinates: " + str(100 - abs(float(accOverall) / float(length))))

    def AlignSurfaceLM(self, szeReader):
        # Rotate surface because it is not aligned with landmarks
        gen_trans = vtk.vtkGeneralTransform()
        gen_trans.RotateZ(-90)
        gen_trans.RotateX(90)

        gen_trans_filter = vtk.vtkTransformPolyDataFilter()
        gen_trans_filterCopy = vtk.vtkTransformPolyDataFilter()
        gen_trans_filter.SetInputData(szeReader.actor.GetMapper().GetInput())
        gen_trans_filterCopy.SetInputData(szeReader.actorCopy.GetMapper().GetInput())
        gen_trans_filter.SetTransform(gen_trans)
        gen_trans_filterCopy.SetTransform(gen_trans)

        szeReader.actorCopy.GetMapper().SetInputConnection(gen_trans_filterCopy.GetOutputPort())
        szeReader.actor.GetMapper().SetInputConnection(gen_trans_filter.GetOutputPort())
        szeReader.actorCopy.GetMapper().Update()
        szeReader.actor.GetMapper().Update()

    def ApplyTransform(self, actor, trans):
        lm_trans = vtk.vtkTransformPolyDataFilter()
        lm_trans.SetInputData(actor.GetMapper().GetInput())
        lm_trans.SetTransform(trans)
        actor.GetMapper().SetInputConnection(lm_trans.GetOutputPort())
        actor.GetMapper().Update()

    def getTransformedLandmarksPoints(self, points, transform):
        actor = vtk.vtkActor()

        polydata = vtk.vtkPolyData()
        polydata.SetPoints(points)

        lm_trans = vtk.vtkTransformPolyDataFilter()
        lm_trans.SetInputData(polydata)
        lm_trans.SetTransform(transform)

        mapper = vtk.vtkPolyDataMapper()
        mapper.SetInputConnection(lm_trans.GetOutputPort())
        actor.SetMapper(mapper)
        actor.GetMapper().Update()

        transformed_points = vtk.vtkPoints()
        return actor.GetMapper().GetInput().GetPoints()


