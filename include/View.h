
#pragma once
#include <Model.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>

#include <vtkPolyDataMapper.h>
#include <vtkActor.h>

#include <vtkPolyDataMapper.h>
#include <vtkActor.h>

#include <vtkPolyDataMapper.h>
#include <vtkActor.h>

#include <vtkPolyDataMapper.h>
#include <vtkActor.h>

#include <vtkPolyDataMapper.h>
#include <vtkActor.h>

#include <vtkPolyDataMapper.h>
#include <vtkActor.h>

#include <vtkPolyDataMapper.h>
#include <vtkActor.h>

#include <vtkPolyDataMapper.h>
#include <vtkActor.h>

class BaseView {
public:
   //View(BaseModel* model);
 
    vtkSmartPointer<vtkActor> bactor;
};

class CubeView : public BaseView {
public:
    CubeView(CubeModel* model);
    vtkSmartPointer<vtkPolyDataMapper> mapper;
    vtkSmartPointer<vtkActor> actor;
};


class TubeView : public BaseView {
public:
    TubeView(TubeModel* model);
    vtkSmartPointer<vtkPolyDataMapper> mapper;
    vtkSmartPointer<vtkActor> actor;
};

class SphereView : public BaseView {
public:
    SphereView(SphereModel* model);
    vtkSmartPointer<vtkPolyDataMapper> mapper;
    vtkSmartPointer<vtkActor> actor;
};

class PyramidView : public BaseView {
public:
    PyramidView(PyramidModel* model);
    vtkSmartPointer<vtkDataSetMapper> mapper ;
    vtkSmartPointer<vtkActor> actor;
};

class HemisphereView : public BaseView {
public:
    HemisphereView(HemisphereModel* model);
    vtkSmartPointer<vtkPolyDataMapper> mapper;
    vtkSmartPointer<vtkActor> actor;
};

class DonutView : public BaseView {
public:
    DonutView(DonutModel* model);
    vtkSmartPointer<vtkPolyDataMapper> mapper;
    vtkSmartPointer<vtkActor> actor;
};

class CylinderView : public BaseView {
public:
    CylinderView(CylinderModel* model);
    vtkSmartPointer<vtkPolyDataMapper> mapper;
    vtkSmartPointer<vtkActor> actor;
};

class CurvedCylinderView : public BaseView {
public:
    CurvedCylinderView(CurvedCylinderModel* model);
    vtkSmartPointer<vtkPolyDataMapper> mapper;
    vtkSmartPointer<vtkActor> actor;
};




class ConeView : public BaseView {
public:
    ConeView(ConeModel* model);
    vtkSmartPointer<vtkPolyDataMapper> mapper;
    vtkSmartPointer<vtkActor> actor;
};
