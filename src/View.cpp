#include "View.h"
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>


/**
 * @brief Construct a new Tube View:: Tube View object.
 * 
 * @param model 
 */
TubeView::TubeView(TubeModel* model) {
    mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(model->tubeFilter->GetOutputPort());

    actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    this->bactor = actor;
}
/**
 * @brief Construct a new Sphere View:: Sphere View object.
 * 
 * @param model 
 */
SphereView::SphereView(SphereModel* model) {
    mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(model->sphereSource->GetOutputPort());

    actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    this->bactor = actor;
}
/**
 * @brief Construct a new Cylinder View:: Cylinder View object
 * 
 * @param model 
 */
CylinderView::CylinderView(CylinderModel* model) {
    mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(model->cylinderSource->GetOutputPort());

    actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    this->bactor = actor;
}
/**
 * @brief Construct a new Curved Cylinder View:: Curved Cylinder View object
 * 
 * @param model 
 */
CurvedCylinderView::CurvedCylinderView(CurvedCylinderModel* model) {



    mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(model->tubeFilter->GetOutputPort());

    actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    this->bactor = actor;
}
/**
 * @brief Construct a new Cube View:: Cube View object
 * 
 * @param model 
 */
CubeView::CubeView(CubeModel* model) {

    mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(model->cubeSource->GetOutputPort());

    actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    this->bactor = actor;
}
/**
 * @brief Construct a new Cone View:: Cone View object
 * 
 * @param model 
 */
ConeView::ConeView(ConeModel* model) {
    mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(model->coneSource->GetOutputPort());

    actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    this->bactor = actor;
}
/**
 * @brief Construct a new Pyramid View:: Pyramid View object
 * 
 * @param model 
 */
PyramidView::PyramidView(PyramidModel* model) {


    vtkSmartPointer<vtkUnstructuredGrid> unstructuredGrid =
        vtkSmartPointer<vtkUnstructuredGrid>::New();

    unstructuredGrid->SetPoints(model->points);
    unstructuredGrid->InsertNextCell(model->pyramid->GetCellType(), model->pyramid->GetPointIds());

    // Create an actor and mapper
     mapper = vtkSmartPointer<vtkDataSetMapper>::New();
    mapper->SetInputData(unstructuredGrid);
     actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    this->bactor = actor;

}

/**
 * @brief Construct a new Hemisphere View:: Hemisphere View object
 * 
 * @param model 
 */
HemisphereView::HemisphereView(HemisphereModel* model) {
    mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(model->clipper->GetOutputPort());

    actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    this->bactor = actor;
}

/**
 * @brief Construct a new Donut View:: Donut View object
 * 
 * @param model 
 */
DonutView::DonutView(DonutModel* model) {
    
    vtkSmartPointer<vtkParametricFunctionSource> functionSource = vtkSmartPointer<vtkParametricFunctionSource>::New();
    functionSource->SetParametricFunction(model->torus);

    mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(functionSource->GetOutputPort());

    actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    this->bactor = actor;
    
}
