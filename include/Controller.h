#pragma once
#include "Model.h"
#include "View.h"
#include<stdio.h>

class Controller {
public:
     vtkSmartPointer<vtkActor> GetActor();

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

