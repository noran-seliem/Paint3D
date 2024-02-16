// A2DD.h
#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <vtkActor.h>
#include <vtkPolyData.h> 
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>

#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkSmartPointer.h>
#include <vtkAssembly.h>
#include <vtkProperty.h>
#include <vtkCellPicker.h>

//////////////////////////////////////////////////////////
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
#include <vtkSmartPointer.h>
#include <vtkAssembly.h>

#include <vtkPropPicker.h>
#include <vtkInteractorStyleTrackballCamera.h>

#include <vtkCellPicker.h>
#include <vtkInteractorStyleTrackballActor.h>

#include <QMessageBox.h>

#include <QFileDialog.h>

#include <vtkspheresource.h>
#include <vtkconesource.h>
#include <vtkcubesource.h>
#include <vtkcylindersource.h>

#include <vtkpyramid.h>
#include <vtktubefilter.h>

#include <vtkactor.h>
#include <vtkpolydatamapper.h>

#include <vtknamedcolors.h>
#include <vtkelevationfilter.h>
#include <vtkdatasetmapper.h>


#include <vtkpolydatamapper.h>
#include <vtkactor.h>
#include <vtkrenderer.h>
#include <vtkrenderwindow.h>

#include <vtkspheresource.h>
#include <vtkcylindersource.h>
#include <vtktransform.h>
#include <vtktransformpolydatafilter.h>
#include <vtkappendpolydata.h>

#include <vtkproperty.h>

#include <qopenglfunctions>
#include <qopenglcontext>
#include <iostream>
#include <qmessagebox>
#include <qmouseevent>


#include <vtklinesource.h>


#include <vtkActor.h>
#include <vtkNamedColors.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>

#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderer.h>
#include <QVTKInteractor.h>
#include <vector>
#include <iostream>
#include <sstream>





////////////////////////////////////////////////////////////////

#include "Model.h"
#include "View.h"
#include "Controller.h"
#include "Stl.h"
#include "Volume.h"

class MainController {
private:


public:
    MainController(vtkRenderWindow* renWin, vtkRenderer* aRenderer);
    vtkNew<vtkGenericOpenGLRenderWindow> mRenderWindow;
    vtkNew<vtkRenderer> mRenderer;
    vtkSmartPointer<vtkRenderWindowInteractor> iren;
    vtkNew<vtkGenericOpenGLRenderWindow> mRenderWindow2;
    vtkNew<vtkRenderer> mRenderer2;
    vtkNew<vtkGenericOpenGLRenderWindow> mRenderWindow3;
    vtkNew<vtkRenderer> mRenderer3;
    vtkNew<vtkGenericOpenGLRenderWindow> mRenderWindow4;
    vtkNew<vtkRenderer> mRenderer4;

    struct Triplet {
        Controller* first;
        int second;
        int third;
    };
    std::vector<Triplet> objectVectorpair;
    std::vector<std:: string> objectNames;


    int objectCount = 0;
    int objectNumber = 0;
    int extractInteger(std::string str);
    std::vector<int> extractMultiIntegers(std::string str);
    int shapeType = 0;
    std::string inputFile;
    vtkSmartPointer<vtkPolyData> STLpolydata;
    vtkActor* pickedActor = vtkActor::New();

    //vtkRenderWindow*  mRenderWindow;
    //vtkRenderer* mRenderer;
    MyCommand* myCommand;
    vtkGenericOpenGLRenderWindow* GetRenWin();
    vtkRenderer* GetRen();

   // void onOkClicked();
    void changeProperty(std::string selecteditem, vtkActor* selectActorCom, int property, int spinbox, int spinbox2, int spinbox3, double spinbox4);
    void editShape(vtkActor* selectActorCom, std::string plainEdit, int spinbox, int spinbox2, int spinbox3, double spinbox4);
    int getShapeType(std::string selecteditem);
    int getShapeType2(std::string selecteditem, vtkActor* selectActorCom);

    void mergeClick (std::string selecteditem1,std::string selecteditem2);


    void onLoadSTLClick(QString fileName);
    void  volRayCasting(QString dataDir);


    void  axialView(QString dataDir);
    void  coronalView(QString dataDir);
    void  sagitalView(QString dataDir);
    void onSaveSTLClick();
    void DrawSphere();
    void onDrawHemisphereClick();
    void onDrawCubeClick();
    void onDrawConeClick();
    void onDrawPyramidClick();
    void onDrawTubeClick();
    void onDrawCylinderClick();
    void onDrawCurvedCylinderClick();
    void onDrawDonutClick();

    void setActorColorAndOpacity(vtkActor* actor, double r, double g, double b, double opacity);
    void rotateActor(vtkActor* actor, double angle, double angle2, double angle3);
    void scaleActor(vtkActor* actor, double x, double y, double z);
    void translateActor(vtkActor* actor, double x, double y, double z);
    void mergeActors(vtkActor* actor1, vtkActor* actor2);
    void deleteActor(vtkActor* actor);
};

#endif
