import vtk

class SZEWriter():
    def __init__(self):
        pass
    def save(self, dirName, actor):
        # Get surface polydata from actor
        actor.GetMapper().Update()
        surfacePolyData = actor.GetMapper().GetInput()
        numOfPoints = surfacePolyData.GetNumberOfPoints()
        numOfPolygons = surfacePolyData.GetPolys().GetNumberOfCells()
        numofVertex = surfacePolyData.GetCell(0).GetPointIds().GetNumberOfIds()
        # open file
        fileName = dirName + "/registered.sze"
        print(fileName)
        with open(fileName , 'w') as szeFile:

            # writing polygon
            szeFile.write("NbPolygon={}\n".format(numOfPolygons))
            szeFile.write("SizePolygon={}\n".format(numOfPolygons * (numofVertex + 1)))
            for polyIndex in range(numOfPolygons):
                poly = surfacePolyData.GetCell(polyIndex)
                szeFile.write("{}\n".format(numofVertex))

                for idIndex in range(numofVertex):
                    id = poly.GetPointIds().GetId(idIndex)
                    szeFile.write("{} ".format(id))

                szeFile.write("\n")

            szeFile.write("END\n")

            # writing coordinates
            szeFile.write("XPos YPos Zpos Row = 1 Col = 72971 Zmin = 339999995214436420000000000000000000000.000000 File = 0\n")
            for pointIndex in range(numOfPoints):
                point = surfacePolyData.GetPoint(pointIndex)
                szeFile.write("{} {} {}\n".format(point[0], point[1], point[2]))

            szeFile.write("END\n")