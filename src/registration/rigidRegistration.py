import vtk
import operator
from src.registration import registration
class RigidRegistration(registration.Registration):
    def __init__(self):
        pass

    # TODO: refactor or use
    # def register(self, surfaceLMData, xrayLMData, mriLMData, actors):
    #     ST_XRay_Transrigid = self.SurfaceXRayRegistration(surfaceLMData, xrayLMData)
    #     MRI_XRay_Transrigid = self.MRIXRayRegistration(mriLMData, xrayLMData)
    #     return ST_XRay_Transrigid, MRI_XRay_Transrigid

    def SurfaceXRayRegistration(self, surfaceLMData, xrayLMData):

        # Sort Data by name
        surfaceLMData.sort(key=operator.itemgetter('name'))
        xrayLMData.sort(key=operator.itemgetter('name'))

        # matching points extraction
        landmarkXrayPositions = set()
        landmarkSurfacePositions = set()


        for data in xrayLMData:
            landmarkXrayPositions.add(data['name'])

        for data in surfaceLMData:
            landmarkSurfacePositions.add(data['name'])

        matchingPosition = landmarkSurfacePositions.intersection(landmarkXrayPositions)

        # Points to vtkPoints
        xrayLMPoints = vtk.vtkPoints()
        surfaceLMPoints = vtk.vtkPoints()
        for data in surfaceLMData:
            if data['name'] in matchingPosition:
                surfaceLMPoints.InsertNextPoint(data['x'], data['y'], data['z'])

        for data in xrayLMData:
            if data['name'] in matchingPosition:
                xrayLMPoints.InsertNextPoint(data['x'], data['y'], data['z'])


        # 1st register the topo to the xray using tps and external landmarks
        # find the rigid registration using correspondances
        Transrigid = vtk.vtkLandmarkTransform()
        Transrigid.SetSourceLandmarks(surfaceLMPoints)
        Transrigid.SetTargetLandmarks(xrayLMPoints)
        Transrigid.SetModeToRigidBody()
        Transrigid.Update()

        return Transrigid

    def MRIXRayRegistration(self, mriLMData, xrayLMData):
        # Sort Data by name
        mriLMData.sort(key=operator.itemgetter('name'))
        xrayLMData.sort(key=operator.itemgetter('name'))


        # matching points extraction
        landmarkXrayPositions = set()
        landmarkMRIPositions = set()


        for data in xrayLMData:
            landmarkXrayPositions.add(data['name'])

        for data in mriLMData:
            landmarkMRIPositions.add(data['name'])

        matchingPosition = landmarkMRIPositions.intersection(landmarkXrayPositions)

        # Points to vtkPoints
        xrayLMPoints = vtk.vtkPoints()
        surfaceLMPoints = vtk.vtkPoints()
        for data in mriLMData:
            if data['name'] in matchingPosition:
                surfaceLMPoints.InsertNextPoint(data['x'], data['y'], data['z'])

        for data in xrayLMData:
            if data['name'] in matchingPosition:
                xrayLMPoints.InsertNextPoint(data['x'], data['y'], data['z'])


        # 1st register the topo to the xray using tps and external landmarks
        # find the rigid registration using correspondances
        Transrigid = vtk.vtkLandmarkTransform()
        Transrigid.SetSourceLandmarks(surfaceLMPoints)
        Transrigid.SetTargetLandmarks(xrayLMPoints)
        Transrigid.SetModeToRigidBody()
        Transrigid.Update()
        return Transrigid







