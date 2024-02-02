#include "Stl.h"
/**
 * @brief reads stl file into a polydata.
 * 
 * @param polydata to load file data into it
 */
void STLFileHandler::LoadSTL(vtkSmartPointer<vtkPolyData>& polydata) {
	vtkSmartPointer<vtkSTLReader> reader = vtkSmartPointer<vtkSTLReader>::New();
	reader->SetFileName(input_file_.c_str());
	reader->Update();
	polydata = reader->GetOutput();
}
/**
 * @brief writes stl from polydata into output file.
 * 
 * @param polydata to write file data from it
 */
void STLFileHandler::SaveSTL(vtkSmartPointer<vtkPolyData>& polydata) {
	vtkSmartPointer<vtkSTLWriter> writer = vtkSmartPointer<vtkSTLWriter>::New();
	writer->SetFileName(output_file_.c_str());
	writer->SetInputData(polydata);
	writer->Write();
}