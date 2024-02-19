#pragma once
#include "Model.h"
#include "View.h"
#include<stdio.h>
#include <vtkSmartPointer.h>
#include <vtkAssembly.h>

#include <vtkPropPicker.h>
#include <vtkInteractorStyleTrackballCamera.h>

#include <vtkCellPicker.h>
#include <vtkInteractorStyleTrackballActor.h>
#include <vtkCommand.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>

#include <vtkGPUVolumeRayCastMapper.h>
#include <QMessageBox.h>

class MyCommand : public vtkCommand
{
private:
    vtkActor* actor;
    vtkActor* prevActor = NULL;
public:
    static MyCommand* New()
    {
        return new MyCommand;
    }

    // Add a member variable to store the interactor pointer

    virtual void Execute(vtkObject* caller, unsigned long eventId,
        void* callData)
    {
        vtkRenderWindowInteractor* interactor = static_cast<vtkRenderWindowInteractor*>(caller);
        int* clickPos = interactor->GetEventPosition();
        vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
        vtkRenderer* renderer = interactor->FindPokedRenderer(clickPos[0], clickPos[1]);

        picker->Pick(clickPos[0], clickPos[1], 0, renderer);
        // Get the picked actor


        actor = picker->GetActor();

        if (actor) {
            std::string message = actor->GetObjectName();
            // QMessageBox::information(nullptr, "Title", QString("actor name is : %1").arg(QString::fromStdString(message)));

            actor->GetProperty()->EdgeVisibilityOn();
            actor->GetProperty()->SetEdgeColor(1, 1, 0);
            interactor->Render();
            if (vtkGPUVolumeRayCastMapper::SafeDownCast(actor->GetMapper()))
            {
                QMessageBox::information(nullptr, "Title", QString("volume"));

            }
            if (prevActor) {
                prevActor->GetProperty()->EdgeVisibilityOff();

                if (actor == prevActor) {
                    prevActor = NULL;
                    actor = NULL;
                }
                else {
                    actor->GetProperty()->EdgeVisibilityOn();
                    actor->GetProperty()->SetEdgeColor(1, 1, 0);
                    interactor->Render();
                    prevActor = actor;
                }
            }
            else {
                prevActor = actor;
            }

        }
    }
    vtkActor* getActor() {
        return actor;
    }
};
class Controller {
public:
     vtkSmartPointer<vtkActor> GetActor();
     virtual ~Controller();

protected:

    BaseView* bview;
};
class CubeController : public Controller {
public:
    CubeController(CubeModel* model, CubeView* view);
    void SetLength(double length);

private:
    CubeModel* model;
    CubeView* view;
};

class ConeController : public Controller {
public:
    ConeController(ConeModel* model, ConeView* view);
    void SetRadius(double radius);
    void SetHeight(double height);
private:
    ConeModel* model;
    ConeView* view;
};


class CurvedCylinderController : public Controller {
public:
    CurvedCylinderController(CurvedCylinderModel* model, CurvedCylinderView* view);
    void SetNumberOfPoints(int numberOfPoints);
    void SetPoint(int id, double x, double y, double z);

private:
    CurvedCylinderModel* model;
    CurvedCylinderView* view;
};


class CylinderController : public Controller {
public:
    CylinderController(CylinderModel* model, CylinderView* view);
    void SetRadius(double radius);
    void SetHeight(double height);
private:
    CylinderModel* model;
    CylinderView* view;
};


class DonutController : public Controller {
public:
    DonutController(DonutModel* model, DonutView* view);
    
    void SetRingRadius(double radius);
    void SetCrossSectionRadius(double radius);
    
    
private:
    DonutModel* model;
    DonutView* view;
};


class HemisphereController : public Controller {
public:
    HemisphereController(HemisphereModel* model, HemisphereView* view);
    void SetRadius(double radius);
private:
    HemisphereModel* model;
    HemisphereView* view;
};

class PyramidController : public Controller {
public:
    PyramidController(PyramidModel* model, PyramidView* view);
    void SetPoint(int id, double x, double y, double z);
private:
    PyramidModel* model;
    PyramidView* view;
};
class SphereController : public Controller {
public:
    SphereController(SphereModel* model, SphereView* view);
    void SetRadius(double radius);
private:
    SphereModel* model;
    SphereView* view;
};

class TubeController : public Controller {
public:
    TubeController(TubeModel* model, TubeView* view);
    void SetRadius(double radius);
    void SetNumberOfSides(int numberOfSides);
    void SetPoint1( double x, double y, double z);
    void SetPoint2( double x, double y, double z);
private:
    TubeModel* model;
    TubeView* view;
};

