import vtk
import operator
from src.registration import registration
class ArticulatedRegistration(registration.Registration):
    def __init__(self):
        pass

    def SurfaceXRayRegistration(self, szeLMReader, wrlLMReader, szeReader):
        # 1st register the topo to the xray using tps and external landmarks
        thin_plate_trans = vtk.vtkThinPlateSplineTransform()
        thin_plate_trans.SetSourceLandmarks(szeLMReader.points)
        thin_plate_trans.SetTargetLandmarks(wrlLMReader.capteurs_points)
        thin_plate_trans.SetBasisToR2LogR()
        thin_plate_trans.Update()

        lm_trans = vtk.vtkTransformPolyDataFilter()
        lm_trans.SetInputData(szeLMReader.actor.GetMapper().GetInput())
        lm_trans.SetTransform(thin_plate_trans)
        szeLMReader.actor.GetMapper().SetInputConnection(lm_trans.GetOutputPort())
        szeLMReader.actor.GetMapper().Update()

        sze_trans = vtk.vtkTransformPolyDataFilter()
        sze_trans.SetInputData(szeReader.actor.GetMapper().GetInput())
        sze_trans.SetTransform(thin_plate_trans)
        szeReader.actor.GetMapper().SetInputConnection(sze_trans.GetOutputPort())
        szeReader.actor.GetMapper().Update()

        return szeReader.actor, szeLMReader.actor

    def MRIXRayRegistration(self,  mriLMReader, wrlLMReader, mriReader):
        pass







