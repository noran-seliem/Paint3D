#include "Model.h"

/**
 * @brief Construct a new Cone Model:: Cone Model object
 * 
 */
ConeModel::ConeModel() {
    coneSource = vtkSmartPointer<vtkConeSource>::New();
    coneSource->SetRadius(1.0);
    coneSource->SetHeight(2.0);
    coneSource->SetCenter(0.0, 0.0, 0.0);
    coneSource->SetResolution(30);
}


/**
 * @brief Construct a new Cube Model:: Cube Model object
 * 
 */
CubeModel::CubeModel() {
    cubeSource = vtkSmartPointer<vtkCubeSource>::New();
    cubeSource->SetCenter(0.0, 0.0, 0.0);
    cubeSource->SetXLength(1.0);
    cubeSource->SetYLength(1.0);
    cubeSource->SetZLength(1.0);
}


/**
 * @brief Construct a new Curved Cylinder Model:: Curved Cylinder Model object
 * 
 */

CurvedCylinderModel::CurvedCylinderModel() {
    spline = vtkSmartPointer<vtkParametricSpline>::New();
    vtkSmartPointer<vtkParametricFunctionSource> functionSource = vtkSmartPointer<vtkParametricFunctionSource>::New();
    functionSource->SetParametricFunction(spline);

     tubeFilter = vtkSmartPointer<vtkTubeFilter>::New();
    tubeFilter->SetInputConnection(functionSource->GetOutputPort());
    tubeFilter->SetRadius(0.5);
    tubeFilter->SetNumberOfSides(30);


}


/**
 * @brief Construct a new Cylinder Model:: Cylinder Model object
 * 
 */
CylinderModel::CylinderModel() {
    cylinderSource = vtkSmartPointer<vtkCylinderSource>::New();
    cylinderSource->SetRadius(1.0);
    cylinderSource->SetHeight(2.0);
    cylinderSource->SetCenter(0.0, 0.0, 0.0);
    cylinderSource->SetResolution(30);
}
/**
 * @brief Construct a new Donut Model:: Donut Model object
 * 
 */
DonutModel::DonutModel() {
    
    torus = vtkSmartPointer<vtkParametricTorus>::New();
    torus->SetRingRadius(1.0);
    torus->SetCrossSectionRadius(0.5);
    
}
/**
 * @brief Construct a new Pyramid Model:: Pyramid Model object. it needs 5 points 
 * 
 */
PyramidModel::PyramidModel() {
    pyramid = vtkSmartPointer<vtkPyramid>::New();
    pyramid->GetPointIds()->SetId(0, 0);
    pyramid->GetPointIds()->SetId(1, 1);
    pyramid->GetPointIds()->SetId(2, 2);
    pyramid->GetPointIds()->SetId(3, 3);
    pyramid->GetPointIds()->SetId(4, 4);

    cells = vtkSmartPointer<vtkCellArray>::New();
    cells->InsertNextCell(pyramid);

    points = vtkSmartPointer<vtkPoints>::New();
    points->InsertNextPoint(-1.0, -1.0, -1.0);
    points->InsertNextPoint(1.0, -1.0, -1.0);
    points->InsertNextPoint(1.0, 1.0, -1.0);
    points->InsertNextPoint(-1.0, 1.0, -1.0);
    points->InsertNextPoint(0.0, 0.0, 1.0);
}
/**
 * @brief Construct a new Sphere Model:: Sphere Model object
 * 
 */
SphereModel::SphereModel() {
    sphereSource = vtkSmartPointer<vtkSphereSource>::New();
    sphereSource->SetRadius(1.0);
    sphereSource->SetCenter(0.0, 0.0, 0.0);
    sphereSource->SetPhiResolution(30);
    sphereSource->SetThetaResolution(30);
}
/**
 * @brief Construct a new Tube Model:: Tube Model object
 * 
 */
TubeModel::TubeModel() {
    tubeFilter = vtkSmartPointer<vtkTubeFilter>::New();
    lineSource= vtkSmartPointer<vtkLineSource>::New();
    lineSource->SetPoint1(2.0, 0.0, 0.0);
    lineSource->SetPoint2(1.0, 0.0, 0.0);
    lineSource->Update();

    
    tubeFilter->SetInputConnection(lineSource->GetOutputPort());
    tubeFilter->SetRadius(0.1);
    tubeFilter->SetNumberOfSides(50);
    tubeFilter->Update();
}
/**
 * @brief Construct a new Hemisphere Model:: Hemisphere Model object
 * 
 */
HemisphereModel::HemisphereModel() {
    sphereSource = vtkSmartPointer<vtkSphereSource>::New();
    sphereSource->SetRadius(1.0);
    sphereSource->SetCenter(0.0, 0.0, 0.0);
    sphereSource->SetPhiResolution(30);
    sphereSource->SetThetaResolution(30);

    plane = vtkSmartPointer<vtkPlane>::New();
    plane->SetOrigin(0.0, 0.0, 0.0);
    plane->SetNormal(0.0, 0.0, 1.0);

    clipper = vtkSmartPointer<vtkClipPolyData>::New();
    clipper->SetInputConnection(sphereSource->GetOutputPort());
    clipper->SetClipFunction(plane);
    clipper->InsideOutOn();
}
