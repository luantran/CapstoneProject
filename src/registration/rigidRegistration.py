import vtk
import operator
from src.registration import registration
class RigidRegistration(registration.Registration):
    def __init__(self):
        pass

    def SurfaceXRayRegistration(self, szeLMReader, wrlLMReader, szeReader):

        # Rotate surface becsause it is not aligned with landmarks
        self.AlignSurfaceLM(szeReader)

        # find the rigid registration using correspondences
        Transrigid = vtk.vtkLandmarkTransform()
        Transrigid.SetSourceLandmarks(szeLMReader.points)
        Transrigid.SetTargetLandmarks(wrlLMReader.capteurs_points)
        Transrigid.SetModeToRigidBody()
        Transrigid.Update()
        # Apply transformation to landmarks
        self.ApplyTransform(szeLMReader.actor, Transrigid)
        print("-----Performance Metrics of Surface Topography to X-Ray using Rigid Registration-----")
        self.getMetrics(szeLMReader, wrlLMReader, Transrigid)
        # Apply transformation to surface
        self.ApplyTransform(szeReader.actor, Transrigid)

    def MRIXRayRegistration(self, mriLMReader, wrlLMReader, mriReader):
        Transrigid = vtk.vtkLandmarkTransform()
        Transrigid.SetSourceLandmarks(mriLMReader.points)
        Transrigid.SetTargetLandmarks(wrlLMReader.vertebrae_points)
        Transrigid.SetModeToRigidBody()
        Transrigid.Update()
        mriReader.actor.SetUserTransform(Transrigid)
        mriLMReader.actor.SetUserTransform(Transrigid)
        print("-----Performance Metrics of MRI to X-Ray using Rigid Registration-----")
        self.getMetrics(mriLMReader, wrlLMReader, Transrigid)







