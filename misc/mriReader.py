#

# This example demonstrates how to read a series of dicom images

# and how to scroll with the up/down keys

# through all slices

#

# standard vtk headers
import vtk


def example1():
    # Dicm files are in the directory MyHead.
    folder = "MyHead"

    # Read all the DICOM files in the specified directory.
    reader = vtk.vtkDICOMImageReader()
    reader.SetDirectoryName(folder)
    reader.Update()

    # Visualize
    imageViewer = vtk.vtkImageViewer2()
    imageViewer.SetInputConnection(reader.GetOutputPort())

    # Slice status message
    sliceTextProp = vtk.vtkTextProperty()
    sliceTextProp.SetFontFamilyToCourier()
    sliceTextProp.SetFontSize(20)
    sliceTextProp.SetVerticalJustificationToBottom()
    sliceTextProp.SetJustificationToLeft()
    sliceTextMapper = vtk.vtkTextMapper()
    msg = "Slice {} out of {}".format(imageViewer.GetSliceMin(),imageViewer.GetSliceMax())
    sliceTextMapper.SetInput(msg)
    sliceTextMapper.SetTextProperty(sliceTextProp)
    sliceTextActor = vtk.vtkActor2D()
    sliceTextActor.SetMapper(sliceTextMapper)
    sliceTextActor.SetPosition(15, 10)

    # Usage hint message
    usageTextProp = vtk.vtkTextProperty()
    usageTextProp.SetFontFamilyToCourier()
    usageTextProp.SetFontSize(14)
    usageTextProp.SetVerticalJustificationToTop()
    usageTextProp.SetJustificationToLeft()
    usageTextMapper = vtk.vtkTextMapper()
    usageTextMapper.SetInput("Press up and down key to scroll through MRI slices")
    usageTextMapper.SetTextProperty(usageTextProp)
    usageTextActor = vtk.vtkActor2D()
    usageTextActor.SetMapper(usageTextMapper)
    usageTextActor.GetPositionCoordinate().SetCoordinateSystemToNormalizedDisplay()
    usageTextActor.GetPositionCoordinate().SetValue(0.05, 0.95)

    # Create an interactor
    # And assign interactor to render
    renderWindowInteractor = vtk.vtkRenderWindowInteractor()
    InteractorStyle = vtk.vtkInteractorStyleImage()
    InteractorStyle.SetInteractionModeToImage3D()
    imageViewer.SetupInteractor(renderWindowInteractor)
    renderWindowInteractor.SetInteractorStyle(InteractorStyle)

    # Add slice status message and usage hint message to the renderer
    imageViewer.GetRenderer().AddActor2D(sliceTextActor)
    imageViewer.GetRenderer().AddActor2D(usageTextActor)

    # Initially show the first slice
    imageViewer.SetSlice(0);

    # Initialize rendering and interaction
    imageViewer.Render()
    imageViewer.GetRenderer().ResetCamera()

    # Scroll through slices with up and down keys
    def keyEventListener(obj, event):
        key = renderWindowInteractor.GetKeySym()
        currenSlice = imageViewer.GetSlice()
        requestedSlice = currenSlice
        if key == 'Down':
            requestedSlice = (currenSlice - 1) % imageViewer.GetSliceMax()
            imageViewer.SetSlice(requestedSlice)
            print("Key pressed: " + key)
            print("Get the current slice: " + str(currenSlice))
            print("showing slice: " + str(requestedSlice))
        elif key == 'Up':
            requestedSlice = (currenSlice + 1) % imageViewer.GetSliceMax()
            imageViewer.SetSlice(requestedSlice)
            print("Key pressed: " + key)
            print("Get the current slice: " + str(currenSlice))
            print("showing slice: " + str(requestedSlice))
        # Display the slice number
        print(requestedSlice)
        msg = "Slice {} out of {}".format(requestedSlice, imageViewer.GetSliceMax())
        sliceTextMapper.SetInput(msg)
        sliceTextMapper.SetTextProperty(sliceTextProp)
        sliceTextActor = vtk.vtkActor2D()
        sliceTextActor.SetMapper(sliceTextMapper)
        sliceTextActor.SetPosition(15, 10)

    renderWindowInteractor.AddObserver('KeyPressEvent', keyEventListener, 1.0)

    # Show render window.
    renderWindowInteractor.Start()


if __name__ == '__main__':
    example1()