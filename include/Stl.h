#pragma once

#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkSTLReader.h>
#include <vtkSTLWriter.h>

class STLFileHandler {
public:
    STLFileHandler(const std::string& input_file, const std::string& output_file)
        : input_file_(input_file), output_file_(output_file) {}

    void LoadSTL(vtkSmartPointer<vtkPolyData>& polydata);
    void SaveSTL(vtkSmartPointer<vtkPolyData>& polydata);

private:
    std::string input_file_;
    std::string output_file_;
};


