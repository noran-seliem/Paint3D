#pragma once
#include <vtkTubeFilter.h>
#include <vtkSphereSource.h>
#include <vtkPyramid.h>
#include <vtkCellArray.h>
#include <vtkPoints.h>
#include <vtkSphereSource.h>
#include <vtkClipPolyData.h>
#include <vtkPlane.h>
#include <vtkParametricTorus.h>
#include <vtkCylinderSource.h>
#include <vtkParametricFunctionSource.h>
#include <vtkParametricBoy.h>
#include <vtkCubeSource.h>
#include <vtkConeSource.h>
#include <vtkParametricSpline.h>

#include <vtkCellArray.h>
#include <vtkDataSetMapper.h>

#include <vtkPoints.h>
#include <vtkPyramid.h>
#include <vtkUnstructuredGrid.h>
#include <vtkProperty.h>
#include <vtkLineSource.h>
//#include <vtkSource.h>





class BaseModel {
   // BaseModel();
    //vtkSmartPointer<vtkSource> sphereSource;
};

class TubeModel {
public:
    TubeModel();
    vtkSmartPointer<vtkLineSource> lineSource;
    vtkSmartPointer<vtkTubeFilter> tubeFilter;

};

class SphereModel  {
public:
    SphereModel();
    vtkSmartPointer<vtkSphereSource> sphereSource;
};

class PyramidModel   {
public:
    PyramidModel();
    vtkSmartPointer<vtkPyramid> pyramid;
    vtkSmartPointer<vtkCellArray> cells;
    vtkSmartPointer<vtkPoints> points;
};

class HemisphereModel   {
public:
    HemisphereModel();
    vtkSmartPointer<vtkSphereSource> sphereSource;
    vtkSmartPointer<vtkClipPolyData> clipper;
    vtkSmartPointer<vtkPlane> plane;
};

class DonutModel  {
public:
    DonutModel();
    vtkSmartPointer<vtkParametricTorus> torus;
};

class CylinderModel   {
public:
    CylinderModel();
    vtkSmartPointer<vtkCylinderSource> cylinderSource;
};


class CurvedCylinderModel   {
public:
    CurvedCylinderModel();
    vtkSmartPointer<vtkParametricSpline> spline;
    vtkSmartPointer<vtkTubeFilter> tubeFilter;
    
};

class CubeModel  {
public:
    CubeModel();
    vtkSmartPointer<vtkCubeSource> cubeSource;
};


class ConeModel   {
public:
    ConeModel();
    vtkSmartPointer<vtkConeSource> coneSource;
};
