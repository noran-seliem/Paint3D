// DICOMVolume.cpp
// The implementation of the DICOMVolume class.

#include "Volume.h"
#include <vtkAutoInit.h>
#include <vtkImageProperty.h>

#include "Controller.h"
//forward declaration 
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2);
/**
 * @brief constructor 
 *
 * @param dicom directory
 *
 */
DICOMVolume::DICOMVolume(QString dataDir)
{
 

    // Create and initialize the vtk objects.
    reader = vtkSmartPointer<vtkDICOMImageReader>::New();
    reader->SetDirectoryName(dataDir.toStdString().c_str());
    reader->Update();
    //readerOutput = reader->GetOutputPort();

    style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    volumeMapper = vtkSmartPointer<vtkGPUVolumeRayCastMapper>::New();
    opacityTransferFunction = vtkSmartPointer<vtkPiecewiseFunction>::New();
    colorTransferFunction = vtkSmartPointer<vtkColorTransferFunction>::New();
    volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
    volume = vtkSmartPointer<vtkVolume>::New();


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

    vtkGPUVolumeRayCastMapper* volumeMapper = vtkGPUVolumeRayCastMapper::New();
    volumeMapper->SetInputConnection(reader->GetOutputPort());
    volumeMapper->SetBlendModeToComposite();

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

    vtkVolumeProperty* volumeProperty = vtkVolumeProperty::New();
    volumeProperty->SetColor(volumeColor);
    volumeProperty->SetScalarOpacity(volumeScalarOpacity);
    volumeProperty->SetGradientOpacity(volumeGradientOpacity);
    volumeProperty->SetInterpolationTypeToLinear();
    volumeProperty->ShadeOn();
    volumeProperty->SetAmbient(0.4);
    volumeProperty->SetDiffuse(0.6);
    volumeProperty->SetSpecular(0.2);

    vtkVolume* volume = vtkVolume::New();
    volume->SetMapper(volumeMapper);
    volume->SetProperty(volumeProperty);

    aRenderer->AddViewProp(volume);

    vtkCamera* camera = aRenderer->GetActiveCamera();


    double  c[3];
    c[0] =    volume->GetCenter()[0];
    c[1] =    volume->GetCenter()[1];
    c[2] =    volume->GetCenter()[2];
    camera->SetFocalPoint(c[0], c[1], c[2]);
    camera->SetPosition(c[0] + 500, c[1], c[2]);
    camera->SetViewUp(0, 0, -1);

    aRenderer->SetActiveCamera(camera);
    iren->SetRenderWindow(renWin);
    iren->SetInteractorStyle(vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New());
    myCommand = MyCommand::New();

    iren->AddObserver(vtkCommand::LeftButtonPressEvent, myCommand);

    renWin->Render();
}
/**
 * @brief reslicing volume to get axial, coronal sagital views
 *
 * @param int 1:4
 * 
 */


void DICOMVolume::reslicingDicom(vtkRenderWindow* renWin, vtkRenderer* aRenderer,  int view) {


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
    //vtkSmartPointer<vtkImageData> imageData = vtkSmartPointer<vtkImageData>::New();
    //imageData = reader->GetOutput();
    reslice->SetInputConnection(reader->GetOutputPort());
    reslice->SetOutputDimensionality(2);
    reslice->SetResliceAxes(resliceAxes);
    reslice->SetInterpolationModeToLinear();

    // Create a greyscale lookup table
    vtkSmartPointer<vtkLookupTable> table = vtkSmartPointer<vtkLookupTable>::New();
    table->SetRange(0, 2000);            // image intensity range
    table->SetValueRange(0.0, 1.0);      // from black to white
    table->SetSaturationRange(0.0, 0.0); // no color saturation
    table->SetRampToLinear();
    table->Build();

    // Map the image through the lookup table
    vtkSmartPointer<vtkImageMapToColors> color = vtkSmartPointer<vtkImageMapToColors>::New();
    color->SetLookupTable(table);
    color->SetInputConnection(reslice->GetOutputPort());

    // Display the image
    vtkSmartPointer<vtkImageActor> actor = vtkSmartPointer<vtkImageActor>::New();
    actor->GetMapper()->SetInputConnection(color->GetOutputPort());

    aRenderer->AddActor(actor);
   
    renWin->AddRenderer(aRenderer);
    aRenderer->ResetCamera();
    aRenderer->ResetCameraClippingRange();
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
