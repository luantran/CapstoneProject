import vtk
import operator
from src.registration import registration
class RigidRegistration(registration.Registration):
    def __init__(self):
        pass

    def register(self, reference, source):

        pass

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







