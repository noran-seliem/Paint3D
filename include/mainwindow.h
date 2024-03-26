#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>



#include "Model.h"
#include "View.h"
#include "Controller.h"
#include "Stl.h"
#include "Volume.h"
#include "MainController.h"



namespace Ui {
    class MainWindow;
}

// Define interaction style.
class customMouseInteractorStyle : public vtkInteractorStyleTrackballCamera
{
private:
  
    vtkActor* actor;
public:
    static customMouseInteractorStyle* New();
    vtkTypeMacro(customMouseInteractorStyle, vtkInteractorStyleTrackballCamera);
    virtual void OnLeftButtonDoubleClick() override
    {
        // Get the current mouse position
        int* clickPos = this->GetInteractor()->GetEventPosition();

        // Pick an actor from the scene using a cell picker
        vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
        // Get the mouse position


        // Find the poked renderer
        vtkRenderer* renderer = this->GetInteractor()->FindPokedRenderer(clickPos[0], clickPos[1]);
        // auto defaultRen = this->GetDefaultRenderer();
        // picker->Pick(clickPos[0], clickPos[1], 0, defaultRen);
        picker->Pick(clickPos[0], clickPos[1], 0, renderer);
        // Get the picked actor
        actor = picker->GetActor();
        // actor = picker->GetActor();
       //  mainWin->pickedActor = picker->GetActor();
         // If an actor is picked, change its color to yellow
        if (actor)
        {
            actor->GetProperty()->EdgeVisibilityOn();
            actor->GetProperty()->SetEdgeColor(1, 1, 0);
            //actor->GetProperty()->SetColor(1.0, 1.0, 0.0); // yellow
            this->GetInteractor()->Render();
        }

        // Forward events
        vtkInteractorStyleTrackballCamera::OnLeftButtonDoubleClick();
    }
    vtkActor* getActor() {
        return actor;
    }

};

//class MyCommand : public vtkCommand
//{
//private:
//    vtkActor* actor;
//    vtkActor* prevActor= NULL;
//public:
//    static MyCommand* New()
//    {
//        return new MyCommand;
//    }
//
//    // Add a member variable to store the interactor pointer
//
//    virtual void Execute(vtkObject* caller, unsigned long eventId,
//        void* callData)
//    {
//        vtkRenderWindowInteractor* interactor = static_cast<vtkRenderWindowInteractor*>(caller);
//        int* clickPos = interactor->GetEventPosition();
//        vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
//        vtkRenderer* renderer = interactor->FindPokedRenderer(clickPos[0], clickPos[1]);
//
//        picker->Pick(clickPos[0], clickPos[1], 0, renderer);
//        // Get the picked actor
//        
//
//        actor = picker->GetActor();
//        if (actor) {
//            actor->GetProperty()->EdgeVisibilityOn();
//            actor->GetProperty()->SetEdgeColor(1, 1, 0);
//            interactor->Render();
//
//            if (prevActor) {
//                prevActor->GetProperty()->EdgeVisibilityOff();
//
//                if (actor == prevActor) {
//                    prevActor = NULL;
//                    actor = NULL;
//                }
//                else {
//                    actor->GetProperty()->EdgeVisibilityOn();
//                    actor->GetProperty()->SetEdgeColor(1, 1, 0);
//                    interactor->Render();
//                    prevActor = actor;
//                }
//            }
//            else {
//                prevActor = actor;
//            }
//           
//        }
//    }
//    vtkActor* getActor() {
//        return actor;
//    }
//};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();



public:
    QSlider* widthSlider;
    QSlider* levelSlider;
    QSlider* cutSlider;
    QLabel* WLlabel;
    Ui::MainWindow* ui;
    //vtkNew<vtkGenericOpenGLRenderWindow> mRenderWindow;
    //vtkNew<vtkRenderer> mRenderer;
    vtkGenericOpenGLRenderWindow* mRenderWindow ;
    vtkRenderer* mRenderer;
    MainController* mainController = new MainController(mRenderWindow, mRenderer);
    ///////////////////////
    vtkGenericOpenGLRenderWindow* mRenderWindow2;
    vtkRenderer* mRenderer2;
    //vtkNew<vtkGenericOpenGLRenderWindow> mRenderWindow2;
    //vtkNew<vtkRenderer> mRenderer2;
    //////////////////////

    int objectCount = 0;
    int objectNumber = 0;

    void updateUi();
    int shapeType = 0;
    std::string inputFile;

    vtkActor* pickedActor = vtkActor::New();
    vtkNew<customMouseInteractorStyle> style;
    MyCommand* myCommand;
public slots:
    void onWSliderValueChanged(int value);
    void onLSliderValueChanged(int value);
    void onCutSliderValueChanged(int value);
    void onDrawConeClick();
    void onDrawSphereClick();
    void onDrawHemisphereClick();
    void onDrawCubeClick();
    void onDrawPyramidClick();
    void onDrawTubeClick();
    void onDrawCylinderClick();
    void onDrawCurvedCylinderClick();
    void onDrawDonutClick();
    void onLoadSTLClick();
    void onSaveSTLClick();
    void onLoadVolClick();

    void onMergeClick();
    void onOkClicked();
    void onEditClick();
    void onNextClicked();


    bool eventFilter(QObject* obj, QEvent* event);


};


#endif // MAINWINDOW_H
