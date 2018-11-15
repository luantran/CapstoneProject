import vtk
import operator
from src.registration import registration
class ArticulatedRegistration(registration.Registration):
    def __init__(self):
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
        thin_plate_trans = vtk.vtkThinPlateSplineTransform()
        thin_plate_trans.SetSourceLandmarks(surfaceLMPoints)
        thin_plate_trans.SetTargetLandmarks(xrayLMPoints)
        thin_plate_trans.SetBasisToR2LogR()
        thin_plate_trans.Update()

        return thin_plate_trans

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
        mriLMPoints = vtk.vtkPoints()
        for data in mriLMData:
            if data['name'] in matchingPosition:
                mriLMPoints.InsertNextPoint(data['x'], data['y'], data['z'])

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







