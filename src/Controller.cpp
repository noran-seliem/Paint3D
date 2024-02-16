

#include "Controller.h"

/**
 * @brief this function gets the actor from the controller 
 * 
 * @return vtkSmartPointer<vtkActor> 
 */
vtkSmartPointer<vtkActor> Controller::GetActor() {
    vtkSmartPointer<vtkActor> actor = this->bview->bactor;
    return actor;
}

Controller::~Controller()
{
}


/**
 * @brief Construct a new Tube Controller:: Tube Controller object
 * 
 * @param model pointer to TubeModel
 * @param view pointer to TubeView
 */
TubeController::TubeController(TubeModel* model, TubeView* view) {
    this->model = model;
    this->view = view;
    this->bview = view;
}
/**
 * @brief set radius of tube
 * 
 * @param radius 
 */
void TubeController::SetRadius(double radius) {
    model->tubeFilter->SetRadius(radius);
    model->tubeFilter->Update();
    view->mapper->Update();

}
/**
 * @brief set number of sides of tube
 * 
 * @param numberOfSides 
 */
void TubeController::SetNumberOfSides(int numberOfSides) {
    model->tubeFilter->SetNumberOfSides(numberOfSides);
    model->tubeFilter->Update();
    view->mapper->Update();
}
/**
 * @brief set start point of tube
 * 
 * @param x 
 * @param y 
 * @param z 
 */
void TubeController::SetPoint1( double x, double y, double z) {
    model->lineSource->SetPoint1(x, y, z);

    view->mapper->Update();
}
/**
 * @brief set end point of tube 
 * 
 * @param x 
 * @param y 
 * @param z 
 */
void TubeController::SetPoint2( double x, double y, double z) {
    model->lineSource->SetPoint2(x, y, z);

    view->mapper->Update();
}
/**
 * @brief Construct a new Sphere Controller:: Sphere Controller object
 * 
 * @param model pointer to SphereModel
 * @param view pointer to SphereView
 */
SphereController::SphereController(SphereModel* model, SphereView* view) {
    this->model = model;
    this->view = view;
    this->bview = view;
}
/**
 * @brief set radius of sphere
 * 
 * @param radius 
 */
void SphereController::SetRadius(double radius) {
    model->sphereSource->SetRadius(radius);
    model->sphereSource->Update();
    view->mapper->Update();
}
/**
 * @brief Construct a new Pyramid Controller:: Pyramid Controller object
 * 
 * @param model pointer to PyramidModel
 * @param view pointer to PyramidView
 */
PyramidController::PyramidController(PyramidModel* model, PyramidView* view) {
    this->model = model;
    this->view = view;
    this->bview = view;
}
/**
 * @brief set point in pyramid 
 * 
 * @param id 
 * @param x 
 * @param y 
 * @param z 
 */
void PyramidController::SetPoint(int id, double x, double y, double z) {
    model->points->SetPoint(id, x, y, z);
    model->pyramid->Modified();
    view->mapper->Update();
}


/**
 * @brief Construct a new Hemisphere Controller:: Hemisphere Controller object
 * 
 * @param model pointer to HemisphereModel
 * @param view pointer to HemisphereView
 */
HemisphereController::HemisphereController(HemisphereModel* model, HemisphereView* view) {
    this->model = model;
    this->view = view;
    this->bview = view;
}
/**
 * @brief set radius of hemisphere
 * 
 * @param radius 
 */
void HemisphereController::SetRadius(double radius) {
    model->sphereSource->SetRadius(radius);
    model->sphereSource->Update();
    view->mapper->Update();
}
/**
 * @brief Construct a new Donut Controller:: Donut Controller object
 * 
 * @param model pointer to DonutModel
 * @param view pointer to DonutView
 */
DonutController::DonutController(DonutModel* model, DonutView* view) {
    this->model = model;
    this->view = view;
    this->bview = view;
}
/**
 * @brief set ring radius of donut
 * 
 * @param radius ring radius of donut
 */
void DonutController::SetRingRadius(double radius) {
    model->torus->SetRingRadius(radius);
    view->mapper->Update();
}
/**
 * @brief set cross section radius of donut 
 * 
 * @param radius cross section radius of donut 
 */
void DonutController::SetCrossSectionRadius(double radius) {
    model->torus->SetCrossSectionRadius(radius);
    view->mapper->Update();
}
/**
 * @brief Construct a new Cylinder Controller:: Cylinder Controller object
 * 
 * @param model pointer to CylinderModel
 * @param view pointer to CylinderView
 */
CylinderController::CylinderController(CylinderModel* model, CylinderView* view) {
    this->model = model;
    this->view = view;
    this->bview = view;
}
/**
 * @brief set cylinder radius
 * 
 * @param radius cylinder radius
 */
void CylinderController::SetRadius(double radius) {
    model->cylinderSource->SetRadius(radius);
    model->cylinderSource->Update();
    view->mapper->Update();
}
/**
 * @brief set height of cylinder 
 * 
 * @param height height of cylinder 
 */
void CylinderController::SetHeight(double height) {
    model->cylinderSource->SetHeight(height);
    model->cylinderSource->Update();
    view->mapper->Update();
}
/**
 * @brief Construct a new Curved Cylinder Controller:: Curved Cylinder Controller object
 * 
 * @param model pointer to CurvedCylinderModel
 * @param view pointer to CurvedCylinderView
 */
CurvedCylinderController::CurvedCylinderController(CurvedCylinderModel* model, CurvedCylinderView* view) {
    this->model = model;
    this->view = view;
    this->bview = view;
}
/**
 * @brief Set Number Of Points of spline to construct the curved cylinder around
 * 
 * @param numberOfPoints Number Of Points of spline
 */
void CurvedCylinderController::SetNumberOfPoints(int numberOfPoints) {
    model->spline->SetNumberOfPoints(numberOfPoints);
    
    view->mapper->Update();
}
/**
 * @brief set points of spline with id 
 * 
 * @param id must be within the Number Of Points
 * @param x 
 * @param y 
 * @param z 
 */
void CurvedCylinderController::SetPoint(int id, double x, double y, double z) {
    model->spline->SetPoint(id, x, y, z);
    
    view->mapper->Update();
}

/**
 * @brief Construct a new Cube Controller:: Cube Controller object
 * 
 * @param model pointer to CubeModel
 * @param view pointer to CubeView
 */
CubeController::CubeController(CubeModel* model, CubeView* view) {
    this->model = model;
    this->view = view;
    this->bview = view;
}
/**
 * @brief set length of cube
 * 
 * @param length 
 */
void CubeController::SetLength(double length) {
    model->cubeSource->SetXLength(length);
    model->cubeSource->SetYLength(length);
    model->cubeSource->SetZLength(length);
    model->cubeSource->Update();
    view->mapper->Update();

}
/**
 * @brief Construct a new Cone Controller:: Cone Controller object
 * 
 * @param model pointer to ConeModel
 * @param view pointer to ConeView
 */
ConeController::ConeController(ConeModel* model, ConeView* view) {
    this->model = model;
    this->view = view;
    this->bview = view;
}
/**
 * @brief set radius of cone
 * 
 * @param radius 
 */
void ConeController::SetRadius(double radius) {
    model->coneSource->SetRadius(radius);
    model->coneSource->Update();
    view->mapper->Update();
}
/**
 * @brief set height of cone 
 * 
 * @param height 
 */
void ConeController::SetHeight(double height) {
    model->coneSource->SetHeight(height);
    model->coneSource->Update();
    view->mapper->Update();
}
