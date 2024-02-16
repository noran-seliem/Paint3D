// DICOMVolume.h
// A class that represents a DICOM volume.

#ifndef DICOM_VOLUME_H
#define DICOM_VOLUME_H

#include <vtkSmartPointer.h>
#include <vtkDICOMImageReader.h>
#include <vtkContourFilter.h>
#include <vtkPolyDataNormals.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkGPUVolumeRayCastMapper.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkVolumeProperty.h>
#include <vtkVolume.h>


#include "vtkCommand.h"
#include "vtkImageActor.h"
#include "vtkImageData.h"
#include "vtkImageMapToColors.h"
#include "vtkImageMapper3D.h"
#include "vtkImageReader2.h"
#include "vtkImageReslice.h"
#include "vtkInformation.h"
#include "vtkInteractorStyleImage.h"
#include "vtkLookupTable.h"
#include "vtkMatrix4x4.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkSmartPointer.h"
#include "vtkStreamingDemandDrivenPipeline.h"

#include <QApplication.h>
#include <QMainWindow.h>
#include <QFileDialog.h>
#include <QSlider.h>
#include <QPushButton.h>
#include <QComboBox.h>


class MyCommand;


// The mouse motion callback, to turn "Slicing" on and off
class vtkImageInteractionCallback : public vtkCommand
{
public:
    static vtkImageInteractionCallback* New() { return new vtkImageInteractionCallback; }

    vtkImageInteractionCallback()
    {
        this->Slicing = 0;
        this->ImageReslice = nullptr;
        this->Interactor = nullptr;
    }
    void vtk_reslicing(vtkRenderWindow* renWin, vtkRenderer* aRenderer, QString dataDir, int view);
    void SetImageReslice(vtkImageReslice* reslice) { this->ImageReslice = reslice; }

    vtkImageReslice* GetImageReslice() { return this->ImageReslice; }

    void SetInteractor(vtkRenderWindowInteractor* interactor) { this->Interactor = interactor; }

    vtkRenderWindowInteractor* GetInteractor() { return this->Interactor; }

    void Execute(vtkObject*, unsigned long event, void*) override
    {
        vtkRenderWindowInteractor* interactor = this->GetInteractor();

        int lastPos[2];
        interactor->GetLastEventPosition(lastPos);
        int currPos[2];
        interactor->GetEventPosition(currPos);

        if (event == vtkCommand::LeftButtonPressEvent)
        {
            this->Slicing = 1;
        }
        else if (event == vtkCommand::LeftButtonReleaseEvent)
        {
            this->Slicing = 0;
        }
        else if (event == vtkCommand::MouseMoveEvent)
        {
            if (this->Slicing)
            {
                vtkImageReslice* reslice = this->ImageReslice;

                // Increment slice position by deltaY of mouse
                int deltaY = lastPos[1] - currPos[1];

                reslice->Update();
                double sliceSpacing = reslice->GetOutput()->GetSpacing()[2];
                vtkMatrix4x4* matrix = reslice->GetResliceAxes();
                // move the center point that we are slicing through
                double point[4];
                double center[4];
                point[0] = 0.0;
                point[1] = 0.0;
                point[2] = sliceSpacing * deltaY;
                point[3] = 1.0;
                matrix->MultiplyPoint(point, center);
                matrix->SetElement(0, 3, center[0]);
                matrix->SetElement(1, 3, center[1]);
                matrix->SetElement(2, 3, center[2]);
                interactor->Render();
            }
            else
            {
                vtkInteractorStyle* style =
                    vtkInteractorStyle::SafeDownCast(interactor->GetInteractorStyle());
                if (style)
                {
                    style->OnMouseMove();
                }
            }
        }
    }

private:
    // Actions (slicing only, for now)
    int Slicing;

    // Pointer to vtkImageReslice
    vtkImageReslice* ImageReslice;

    // Pointer to the interactor
    vtkRenderWindowInteractor* Interactor;
};


class DICOMVolume
{
	//Q_OBJECT // Macro for Qt signals and slots

public:
	DICOMVolume();

	~DICOMVolume();

public:

	void  vtk_surface_rendering_UI(vtkRenderWindow* renWin, vtkRenderer* aRenderer, QString dataDir);
	void vtk_rayCasting(vtkRenderWindow* renWin, vtkRenderer* aRenderer, QString dataDir);
   // void vtk_reslicing(vtkRenderWindow* renWin, vtkRenderer* aRenderer, QString dataDir) {


	MyCommand* myCommand;

	vtkSmartPointer<vtkDICOMImageReader> reader;
	vtkSmartPointer<vtkContourFilter> surfaceExtractor;
	vtkSmartPointer<vtkPolyDataNormals> surfaceNormals;
	vtkSmartPointer<vtkPolyDataMapper> surfaceMapper;
	vtkSmartPointer<vtkActor> surface;
	vtkSmartPointer<vtkCamera> aCamera;
	vtkSmartPointer<vtkInteractorStyleTrackballCamera> style;
	vtkSmartPointer<vtkRenderWindowInteractor> iren;
	vtkSmartPointer<vtkGPUVolumeRayCastMapper> volumeMapper;
	vtkSmartPointer<vtkPiecewiseFunction> opacityTransferFunction;
	vtkSmartPointer<vtkColorTransferFunction> colorTransferFunction;
	vtkSmartPointer<vtkVolumeProperty> volumeProperty;
	vtkSmartPointer<vtkVolume> volume;
};


#endif // DICOM_VOLUME_H
