#include "Volume.h"
#include "Controller.h"

//forward declaration 
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2);


#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkImplicitPlaneWidget2.h"
#include "vtkImplicitPlaneRepresentation.h"
#include "vtkCommand.h"
#include "vtkPlane.h"
#include "vtkImageMapToColors.h"
#include "vtkSmartPointer.h"
#include "vtkPolyData.h"
#include "vtkPoints.h"
#include "vtkCellArray.h"
#include "vtkFloatArray.h"
#include "vtkImageReslice.h"
#include "vtkPiecewiseFunction.h"
#include "vtkColorTransferFunction.h"
#include "vtkGPUVolumeRayCastMapper.h"
#include "vtkVolumeProperty.h"
#include "vtkTexture.h"
#include "vtkLookupTable.h"
#include "vtkImageMarchingCubes.h"
#include "vtkPointData.h"
vtkSmartPointer<vtkPolyData> DICOMVolume::MyCreateSimplePlane(const double* corners)
{
    vtkSmartPointer<vtkPolyData>  ret = vtkSmartPointer<vtkPolyData>::New();
    vtkSmartPointer< vtkFloatArray > tcoords = vtkSmartPointer< vtkFloatArray >::New();
    tcoords->SetNumberOfComponents(2);
    tcoords->InsertNextTuple2(0, 0);
    tcoords->InsertNextTuple2(1, 0);
    tcoords->InsertNextTuple2(1, 1);
    tcoords->InsertNextTuple2(0, 1);
    ret->GetPointData()->SetTCoords(tcoords);

    vtkSmartPointer<vtkFloatArray> floatArray = vtkSmartPointer<vtkFloatArray>::New();
    floatArray->SetNumberOfComponents(3);
    for (int i = 0; i < 4; ++i)
    {
        floatArray->InsertNextTuple3(corners[3 * i], corners[3 * i + 1], corners[3 * i + 2]);
    }

    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    points->SetData(floatArray);
    ret->SetPoints(points);

    vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();
    cells->InsertNextCell(4);
    cells->InsertCellPoint(0);
    cells->InsertCellPoint(1);
    cells->InsertCellPoint(2);
    cells->InsertCellPoint(3);
    ret->SetPolys(cells);
    return ret;
}


/**
 * @brief constructor 
 *
 * @param dicom directory
 *
 */
DICOMVolume::DICOMVolume(QString dataDir)
{
    DICOMVolume::dataDir = dataDir;
}
void DICOMVolume::initialize() {
    // Create and initialize the vtk objects.
    reader = vtkSmartPointer<vtkDICOMImageReader>::New();
    reader->SetDirectoryName(dataDir.toStdString().c_str());
    reader->Update();
    imageData = vtkSmartPointer<vtkImageData>::New();
    imageData = reader->GetOutput();
    style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    volumeMapper = vtkSmartPointer<vtkGPUVolumeRayCastMapper>::New();
    volumeMapperMain = vtkGPUVolumeRayCastMapper::New();
    volumeProperty1 = vtkVolumeProperty::New();
   cutplane = vtkSmartPointer<vtkPlane>::New();
   volumCut = vtkVolume::New();

    opacityTransferFunction = vtkSmartPointer<vtkPiecewiseFunction>::New();
    colorTransferFunction = vtkSmartPointer<vtkColorTransferFunction>::New();
    volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
    volume = vtkSmartPointer<vtkVolume>::New();
    myLookupTable = vtkSmartPointer<vtkWindowLevelLookupTable>::New();
}

DICOMVolume::~DICOMVolume()
{
    // Delete the vtk objects.
    reader->Delete();
    style->Delete();
    iren->Delete();
    volumeMapper->Delete();
    opacityTransferFunction->Delete();
    colorTransferFunction->Delete();
    volumeProperty->Delete();
    volume->Delete();
}

/**
 * @brief render volume 
 *
 * @param vtkRenderWindow
 * @param vtkRenderer
 *
 */

void DICOMVolume::rayCasting(vtkRenderWindow* renWin, vtkRenderer* aRenderer) {
   
    renWin->AddRenderer(aRenderer);
    vtkRenderWindowInteractor* iren = vtkRenderWindowInteractor::New();
    iren->SetRenderWindow(renWin);
    // define cutplane early on
    //-1 shows the upper half, 1 shows lower half
    cutplane->SetNormal(0, 0, -1);

    volumeMapperMain->SetInputData(imageData);
    volumeMapperMain->SetBlendModeToComposite();

    vtkColorTransferFunction* volumeColor = vtkColorTransferFunction::New();
    volumeColor->AddRGBPoint(0, 0.0, 0.0, 0.0);
    volumeColor->AddRGBPoint(500, 1.0, 0.0, 0.3);
    volumeColor->AddRGBPoint(1000, 1.0, 0.5, 0.3);
    volumeColor->AddRGBPoint(1150, 1.0, 1.0, 0.9);

    vtkPiecewiseFunction* volumeScalarOpacity = vtkPiecewiseFunction::New();
    volumeScalarOpacity->AddPoint(0, 0.00);
    volumeScalarOpacity->AddPoint(500, 0.15);
    volumeScalarOpacity->AddPoint(1000, 0.15);
    volumeScalarOpacity->AddPoint(1150, 0.85);

    vtkPiecewiseFunction* volumeGradientOpacity = vtkPiecewiseFunction::New();
    volumeGradientOpacity->AddPoint(0, 0.0);
    volumeGradientOpacity->AddPoint(90, 0.5);
    volumeGradientOpacity->AddPoint(100, 1.0);

    volumeProperty1->SetColor(volumeColor);
    volumeProperty1->SetScalarOpacity(volumeScalarOpacity);
    volumeProperty1->SetGradientOpacity(volumeGradientOpacity);
    volumeProperty1->SetInterpolationTypeToLinear();
    volumeProperty1->ShadeOn();
    volumeProperty1->SetAmbient(0.4);
    volumeProperty1->SetDiffuse(0.6);
    volumeProperty1->SetSpecular(0.2);

    volumCut->SetMapper(volumeMapperMain);
    volumCut->SetProperty(volumeProperty1);

    aRenderer->AddViewProp(volumCut);
//edits reslicing here
    vCamera = aRenderer->GetActiveCamera();


    double  c[3];
    c[0] = volumCut->GetCenter()[0];
    c[1] = volumCut->GetCenter()[1];
    c[2] = volumCut->GetCenter()[2];
    vCamera->SetFocalPoint(c[0], c[1], c[2]);
    vCamera->SetPosition(c[0] + 500, c[1], c[2]);
    vCamera->SetViewUp(0, 0, -1);

    aRenderer->SetActiveCamera(vCamera);
    iren->SetRenderWindow(renWin);
    iren->SetInteractorStyle(vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New());
    myCommand = MyCommand::New();

    iren->AddObserver(vtkCommand::LeftButtonPressEvent, myCommand);

    renWin->Render();
   // volumeSlicer(10);
    vRenderer = aRenderer;
    vRenWin = renWin;

    int extent[6];
    imageData->GetExtent(extent);
    volLength = (extent[5] - extent[4]) ;
    std::cout <<" volume length :"<< volLength;
    volumeMapperMain->AddClippingPlane(cutplane);

}

void DICOMVolume::volumeSlicer(int z) {


    double origin[3];
    double spacing[3];
    imageData->GetOrigin(origin);
    imageData->GetSpacing(spacing);
    double corner[3] = { origin[0], origin[1], origin[2] + (double)z * spacing[2] };
    cutplane->SetOrigin(corner);
    vRenWin->Render();

}
/**
 * @brief reslicing volume to get axial, coronal sagital views
 *
 * @param int 1:4
 * 
 */


void DICOMVolume::reslicingDicom(vtkRenderWindow* renWin, vtkRenderer* aRenderer, int view) {


   // Calculate the center of the volume

    int extent[6];
    double spacing[3];
    double origin[3];

    reader->GetOutputInformation(0)->Get(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(), extent);
    reader->GetOutput()->GetSpacing(spacing);
    reader->GetOutput()->GetOrigin(origin);

    double center[3];
    center[0] = origin[0] + spacing[0] * 0.5 * (extent[0] + extent[1]);
    center[1] = origin[1] + spacing[1] * 0.5 * (extent[2] + extent[3]);
    center[2] = origin[2] + spacing[2] * 0.5 * (extent[4] + extent[5]);

   //  Matrices for axial, coronal, sagittal, oblique view orientations
    static double viewElements[16];
 
    switch (view)
    {
    case(1): {
        static double axialElements[16] = {
                                              1, 0, 0, 0,
                                              0, 1, 0, 0,
                                              0, 0, 1, 0,
                                              0, 0, 0, 1 };
        for (int i = 0; i < 16; i++)
        {
            viewElements[i] = axialElements[i];
        }
        break;
    }


    case(2): {
        static double coronalElements[16] = {
        1, 0, 0, 0,
        0, 0, 1, 0,
        0,-1, 0, 0,
        0, 0, 0, 1 };
        for (int i = 0; i < 16; i++)
        {
            viewElements[i] = coronalElements[i];
        }
        break;
    }
      

    case(3): {
        static double sagittalElements[16] = {
           0, 0, -1, 0,
           1, 0, 0, 0,
           0, -1, 0, 0,
           0, 0, 0, 1
        };
        for (int i = 0; i < 16; i++)
        {
            viewElements[i] = sagittalElements[i];
        }
        break;
    }
       
     
    case(4): {
        static double obliqueElements[16] = {
          1, 0, 0, 0,
          0, 0.866025, -0.5, 0,
          0, 0.5, 0.866025, 0,
          0, 0, 0, 1 };
        for (int i = 0; i < 16; i++)
        {
            viewElements[i] = obliqueElements[i];
        }
        break;
    }


    default:
        break;
    }
    // Set the slice orientation
    vtkSmartPointer<vtkMatrix4x4> resliceAxes = vtkSmartPointer<vtkMatrix4x4>::New();
    //choose element view
    resliceAxes->DeepCopy(viewElements);
    // Set the point through which to slice
    resliceAxes->SetElement(0, 3, center[0]);
    resliceAxes->SetElement(1, 3, center[1]);
    resliceAxes->SetElement(2, 3, center[2]);

    // Extract a slice in the desired orientation
    vtkSmartPointer<vtkImageReslice> reslice = vtkSmartPointer<vtkImageReslice>::New();

    reslice->SetInputData(imageData);
    reslice->SetOutputDimensionality(2);
    reslice->SetResliceAxes(resliceAxes);
    reslice->SetInterpolationModeToLinear();

    // Create a greyscale lookup table


    if (WindowWidth == 0) {
        WindowWidth = imageData->GetScalarRange()[1] - imageData->GetScalarRange()[0];
        WindowLevel = (imageData->GetScalarRange()[1] + imageData->GetScalarRange()[0]) / 2.0;
    }


    myLookupTable->SetWindow(WindowWidth);
    myLookupTable->SetLevel(WindowLevel);

    // Map the image through the lookup table
    vtkSmartPointer<vtkImageMapToColors> color = vtkSmartPointer<vtkImageMapToColors>::New();
    color->SetLookupTable(myLookupTable);
    color->SetInputConnection(reslice->GetOutputPort());

    // Display the image
    vtkSmartPointer<vtkImageActor> actor = vtkSmartPointer<vtkImageActor>::New();
    actor->GetMapper()->SetInputConnection(color->GetOutputPort());

    //camera settings
    
    //vtkCamera* camera = aRenderer->GetActiveCamera();
    vtkSmartPointer<vtkCamera> camera = vtkSmartPointer<vtkCamera>::New();
    aRenderer->SetActiveCamera(camera);
    int* imageSize = imageData->GetDimensions();
    //double tanAngle = std::tan(0.5*camera->GetViewAngle());
    // double distance = (imageSize[1] )/( 2* tanAngle);

    double angleRad = vtkMath::RadiansFromDegrees(camera->GetViewAngle()) ;
 //   double distance = imageSize[2] / angleRad;

    int iw = 0;
    int ih = 0;

    switch (view)
    {
    case 1:
        iw = imageSize[0];
        ih = imageSize[1];
        break;
    case 2:
        iw = imageSize[0];
        ih = imageSize[2];
        break;
    case 3:
        iw = imageSize[1];
        ih = imageSize[2];
        break;
    default:
        break;
    }
    double distance = ih / angleRad;

    actor->SetOrigin(0,0,0);
    aRenderer->AddActor(actor);
   // aRenderer->ResetCamera();
    //set actor center as focal point

    camera->SetFocalPoint(0, 0, 0);
    camera->SetPosition(0, 0 ,distance);

    int* rWindowSize = renWin->GetActualSize();
 // QMessageBox::information(nullptr, "Title", QString("w: %1 h:%2, ww:%3, wh/w:%4, viewAngle %5").arg(imageSize[0]).arg(imageSize[1]).arg(rWindowSize[0]).arg((rWindowSize[1]*1.0) / rWindowSize[0]).arg(camera->GetViewAngle()));

    if (!((rWindowSize[0] >= rWindowSize[1]) && (iw <= ih))) {
        camera->Dolly(1 - ((rWindowSize[0] * 1.0) / (rWindowSize[1] * ((imageSize[1] * 1.0) / imageSize[2]))));
    }
    aRenderer->ResetCameraClippingRange();
    renWin->Render();
    renWin->AddRenderer(aRenderer);
    aRenderer->SetBackground(0,0,0);
    // Set up the interaction
    vtkSmartPointer<vtkInteractorStyleImage> imageStyle = vtkSmartPointer<vtkInteractorStyleImage>::New();
    vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    interactor->SetInteractorStyle(imageStyle);
    renWin->SetInteractor(interactor);
    renWin->Render();

    vtkSmartPointer<vtkImageInteractionCallback> callback = vtkSmartPointer<vtkImageInteractionCallback>::New();
    callback->SetImageReslice(reslice);
    callback->SetInteractor(interactor);

    imageStyle->AddObserver(vtkCommand::MouseMoveEvent, callback);
    imageStyle->AddObserver(vtkCommand::LeftButtonPressEvent, callback);
    imageStyle->AddObserver(vtkCommand::LeftButtonReleaseEvent, callback);


}

void DICOMVolume::setWindowWidth(vtkRenderWindow* renWin, double WindowWidth) {
    myLookupTable->SetWindow(WindowWidth);
    renWin->Render();
}
void DICOMVolume::setWindowLevel(vtkRenderWindow* renWin,  double WindowLevel) {
    myLookupTable->SetLevel(WindowLevel);
    renWin->Render();
}



