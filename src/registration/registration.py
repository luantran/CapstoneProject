from abc import ABC, abstractmethod
import vtk

class Registration(ABC):
    def __init__(self):
        super().__init__()

    @abstractmethod
    def SurfaceXRayRegistration(self, szeLMReader, wrlLMReader, szeReader):
        pass

    @abstractmethod
    def MRIXRayRegistration(self, mriLMReader, wrlLMReader, mriReader):
        pass

    def AlignSurfaceLM(self, szeReader):
        # Rotate surface because it is not aligned with landmarks
        gen_trans = vtk.vtkGeneralTransform()
        gen_trans.RotateZ(-90)
        gen_trans.RotateX(90)

        gen_trans_filter = vtk.vtkTransformPolyDataFilter()
        gen_trans_filter.SetInputData(szeReader.actor.GetMapper().GetInput())
        gen_trans_filter.SetTransform(gen_trans)

        szeReader.actor.GetMapper().SetInputConnection(gen_trans_filter.GetOutputPort())
        szeReader.actor.GetMapper().Update()

    def ApplyTransform(self, actor, trans):
        lm_trans = vtk.vtkTransformPolyDataFilter()
        lm_trans.SetInputData(actor.GetMapper().GetInput())
        lm_trans.SetTransform(trans)
        actor.GetMapper().SetInputConnection(lm_trans.GetOutputPort())
        actor.GetMapper().Update()
