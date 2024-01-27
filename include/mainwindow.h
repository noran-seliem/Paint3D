#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <vtkActor.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSphereSource.h>

#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderer.h>
#include <QVTKInteractor.h>
#include <vector>
#include <iostream>
#include <sstream>
#include <vtkSmartPointer.h>

#include "Model.h"
#include "View.h"
#include "Controller.h"
#include "Stl.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow* ui;
    vtkNew<vtkGenericOpenGLRenderWindow> mRenderWindow;
    vtkNew<vtkRenderer> mRenderer;
    struct Triplet {
        Controller* first;
        int second;
        int third;
    };
    std::vector<Triplet> objectVectorpair;
    std::vector<std::pair<SphereController*, int>> spherePair;
    std::vector<std::pair<CubeController*, int>> cubePair;
    std::vector<std::pair<ConeController*, int>> conePair;
    std::vector<std::pair<CurvedCylinderController*, int>> curvedCylPair;
    std::vector<std::pair<CylinderController*, int>> cylinderPair;
    std::vector<std::pair<PyramidController*, int>> pyramidPair;
    std::vector<std::pair<HemisphereController*, int>> hemispherePair;
    std::vector<std::pair<TubeController*, int>> tubePair;
    std::vector<std::pair<DonutController*, int>> donutPair;
    int objectCount = 0;
    int objectNumber = 0;
    int extractInteger(std::string str);
    std::vector<int> extractMultiIntegers(std::string str);
    int shapeType = 0;
    std::string inputFile;
    vtkSmartPointer<vtkPolyData> STLpolydata;

public slots:
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
    void onMergeClick();
    void onOkClicked();
    void onEditClick();
    void onNextClicked();

    void setActorColorAndOpacity(vtkActor* actor, double r, double g, double b, double opacity);
    void rotateActor(vtkActor* actor, double angle, double angle2, double angle3);
    void scaleActor(vtkActor* actor, double x, double y, double z);
    void translateActor(vtkActor* actor, double x, double y, double z);
    void mergeActors(vtkActor* actor1, vtkActor* actor2);
    void deleteActor(vtkActor* actor);

   
};

#endif // MAINWINDOW_H
