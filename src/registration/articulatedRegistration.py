import vtk
import operator
from src.registration import registration
class ArticulatedRegistration(registration.Registration):
    def __init__(self):
        pass

    def SurfaceXRayRegistration(self, szeLMReader, wrlLMReader, szeReader):

        # Rotate surface because it is not aligned with landmarks
        self.AlignSurfaceLM(szeReader)

        # 1st register the topo to the xray using tps and external landmarks
        thin_plate_trans = vtk.vtkThinPlateSplineTransform()
        thin_plate_trans.SetSourceLandmarks(szeLMReader.points)
        thin_plate_trans.SetTargetLandmarks(wrlLMReader.capteurs_points)
        thin_plate_trans.SetBasisToR2LogR()
        thin_plate_trans.Update()

        # Apply transformation to landmarks
        self.ApplyTransform(szeLMReader.actor, thin_plate_trans)
        print("-----Performance Metrics of Surface Topography to X-Ray using Thin Plate Spline Registration-----")
        self.getMetrics(szeLMReader, wrlLMReader, thin_plate_trans)
        # Apply transformation to Surface
        self.ApplyTransform(szeReader.actorCopy, thin_plate_trans)
        self.ApplyTransform(szeReader.actor, thin_plate_trans)

    def MRIXRayRegistration(self,  mriLMReader, wrlLMReader, mriReader):
        Transrigid = vtk.vtkLandmarkTransform()
        Transrigid.SetSourceLandmarks(mriLMReader.points)
        Transrigid.SetTargetLandmarks(wrlLMReader.vertebrae_points)
        Transrigid.SetModeToRigidBody()
        Transrigid.Update()
        mriReader.actor.SetUserTransform(Transrigid)
        mriLMReader.actor.SetUserTransform(Transrigid)
        print("-----Performance Metrics of MRI to X-Ray using Rigid Registration-----")
        self.getMetrics(mriLMReader, wrlLMReader, Transrigid)








