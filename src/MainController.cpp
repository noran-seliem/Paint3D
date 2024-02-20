// A2DD.cpp
#include "MainController.h"


MainController::MainController(vtkRenderWindow* renWin, vtkRenderer* aRenderer) {
	//mRenderer = aRenderer;
	//mRenderWindow = renWin;


	mRenderWindow->AddRenderer(mRenderer);
	mRenderer->ResetCamera();
//	mRenderWindow->Render();
	vtkNew<vtkNamedColors> colors;
	mRenderer->SetBackground(colors->GetColor3d("Cornsilk").GetData());
	myCommand = MyCommand::New();
	iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	//iren->SetRenderWindow(mRenderWindow);
	//iren->AddObserver(vtkCommand::LeftButtonPressEvent, myCommand);

	/////////////////

	//iren->Initialize();
	////renWin->Render();
	//iren->Start();
	///////////////////test///////////////
	mRenderWindow2->AddRenderer(mRenderer2);
	mRenderWindow3->AddRenderer(mRenderer3);
	mRenderWindow4->AddRenderer(mRenderer4);
	mRenderer2->SetBackground(colors->GetColor3d("Cornsilk").GetData());
	////////////////////////////////////

}





void  MainController::volRayCasting(QString dataDir) {


	MainController::volume = new DICOMVolume(dataDir);

	volume->rayCasting(MainController::mRenderWindow, MainController::mRenderer);
	mRenderWindow->Render();


}

void  MainController::axialView(QString dataDir) {


	volume->reslicingDicom(MainController::mRenderWindow2, MainController::mRenderer2, 1);


}


void  MainController::coronalView(QString dataDir) {

	volume->reslicingDicom(MainController::mRenderWindow3, MainController::mRenderer3, 2);

}
void  MainController::sagitalView(QString dataDir) {

	volume->reslicingDicom(MainController::mRenderWindow4, MainController::mRenderer4, 3);

}






void  MainController::loadSTL(QString fileName) {
	if (!fileName.isEmpty()) {

		inputFile = fileName.toStdString();

		STLFileHandler handler(inputFile, "output.STL");

		handler.LoadSTL(MainController::STLpolydata);
		vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		mapper->SetInputData(MainController::STLpolydata);

		vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();


		actor->SetMapper(mapper);
		mRenderer->AddViewProp(actor);
		mRenderer->ResetCamera();
		mRenderWindow->Render();
	}
}

/**
 * @brief save STL file in same directory
 *
 */
void  MainController::saveSTL() {
	STLFileHandler handler(MainController::inputFile, "output.STL");
	handler.SaveSTL(MainController::STLpolydata);


}
vtkGenericOpenGLRenderWindow* MainController::GetRenWin() {
	return mRenderWindow;
}
vtkRenderer* MainController::GetRen() {
	return mRenderer;
}
void MainController::mergeClick(std::string selecteditem1, std::string selecteditem2) {
	int objectNumber1 = 0;
	objectNumber1 = extractInteger(selecteditem1);



	Controller* editableShape1 = new Controller();
	for (auto it = MainController::objectVectorpair.begin(); it != MainController::objectVectorpair.end(); ++it)
	{
		if (it->second == objectNumber1) {
			editableShape1 = it->first;

		}
	}



	int objectNumber2 = 0;
	objectNumber2 = extractInteger(selecteditem2);



	Controller* editableShape2 = new Controller();
	for (auto it2 = MainController::objectVectorpair.begin(); it2 != MainController::objectVectorpair.end(); ++it2)
	{
		if (it2->second == objectNumber2) {
			editableShape2 = it2->first;

		}
	}
	mergeActors(editableShape1->GetActor(), editableShape2->GetActor());
}

int MainController::getShapeType(std::string selecteditem)
{
	
	MainController::objectNumber = extractInteger(selecteditem);
	//Controller* editableShape = new Controller();
	int type = 0;
	for (auto it = MainController::objectVectorpair.begin(); it != MainController::objectVectorpair.end(); ++it)
	{
		if (it->second == MainController::objectNumber) {
			//editableShape = it->first;
			type = it->third;
		}
	}
	return type;
}
int MainController::getShapeType2(std::string selecteditem, vtkActor* selectActorCom)
{


	MainController::shapeType = 0;
	if (selectActorCom) {
		shapeType = getShapeType(selectActorCom->GetObjectName());
	}
	else  {
		shapeType = getShapeType(selecteditem);

	}
	
	return shapeType;
}

void MainController::editShape(vtkActor* selectActorCom, std::string plainEdit, int spinbox, int spinbox2, int spinbox3, double spinbox4) {
	//int typeChoice = 0;


	//typeChoice= getShapeType2(selectActorCom->GetObjectName());
	


	std::vector<int> inputInt = MainController::extractMultiIntegers(plainEdit);
	if (inputInt.empty()) {
		QMessageBox::information(nullptr, "Title", QString("please insert required data and try again"));

	}
	else {
		switch (MainController::shapeType)
		{
		case 1:

			for (auto it2 = MainController::objectVectorpair.begin(); it2 != MainController::objectVectorpair.end(); ++it2) {
				if (it2->third != MainController::shapeType)
					continue;

				if (it2->second == MainController::objectNumber) {

					auto sphereController = dynamic_cast <SphereController*> (it2->first);
					sphereController->SetRadius(inputInt.at(0));

				}

			}

			break;
		case 2:
			for (auto it2 = MainController::objectVectorpair.begin(); it2 != MainController::objectVectorpair.end(); ++it2) {
				if (it2->third != MainController::shapeType)
					continue;
				if (it2->second == MainController::objectNumber) {
					auto cubeShape = dynamic_cast <CubeController*> (it2->first);
					cubeShape->SetLength(inputInt.at(0));

				}

			}


			break;
		case 3:

			for (auto it2 = MainController::objectVectorpair.begin(); it2 != MainController::objectVectorpair.end(); ++it2) {
				if (it2->third != MainController::shapeType)
					continue;
				if (it2->second == MainController::objectNumber) {
					auto coneShape = dynamic_cast <ConeController*> (it2->first);
					coneShape->SetRadius(inputInt.at(0));
					coneShape->SetHeight(inputInt.at(1));
				}

			}

			break;
		case 4:
			for (auto it2 = MainController::objectVectorpair.begin(); it2 != MainController::objectVectorpair.end(); ++it2) {
				if (it2->third != MainController::shapeType)
					continue;
				if (it2->second == MainController::objectNumber) {
					auto cylinderShpe = dynamic_cast <CylinderController*> (it2->first);
					cylinderShpe->SetRadius(inputInt.at(0));
					cylinderShpe->SetHeight(inputInt.at(1));
				}

			}
		
			break;
		case 5:
			for (auto it2 = MainController::objectVectorpair.begin(); it2 != MainController::objectVectorpair.end(); ++it2) {
				if (it2->third != MainController::shapeType)
					continue;
				if (it2->second == MainController::objectNumber) {
					auto donutShape = dynamic_cast <DonutController*> (it2->first);
					donutShape->SetRingRadius(inputInt.at(0));
					donutShape->SetCrossSectionRadius(inputInt.at(1));
				}

			}

		
			break;

		case 7:
			for (auto it2 = MainController::objectVectorpair.begin(); it2 != MainController::objectVectorpair.end(); ++it2) {
				if (it2->third != MainController::shapeType)
					continue;
				if (it2->second == MainController::objectNumber) {
					auto tubeShape = dynamic_cast <TubeController*> (it2->first);
					tubeShape->SetRadius(inputInt.at(0));
					if (spinbox == 0) {
						tubeShape->SetPoint1(spinbox2, spinbox3, spinbox4);
					}
					else if (spinbox == 1) {
						tubeShape->SetPoint2(spinbox2, spinbox3, spinbox4);

					}
				}

			}

			break;
		case 8: 
			for (auto it2 = MainController::objectVectorpair.begin(); it2 != MainController::objectVectorpair.end(); ++it2) {
				if (it2->third != MainController::shapeType)
					continue;
				if (it2->second == MainController::objectNumber) {
					auto pyramidShape = dynamic_cast <PyramidController*> (it2->first);
					pyramidShape->SetPoint(spinbox, spinbox2, spinbox3, spinbox4);
				}

			}

			break;
		case 9:
			for (auto it2 = MainController::objectVectorpair.begin(); it2 != MainController::objectVectorpair.end(); ++it2) {
				if (it2->third != MainController::shapeType)
					continue;
				if (it2->second == MainController::objectNumber) {
					auto hemisphereShape = dynamic_cast <HemisphereController*> (it2->first);
					hemisphereShape->SetRadius(inputInt.at(0));
				}

			}

			break;
		default:
			break;
		}


	}
	switch (MainController::shapeType)
	{
	case 6:
		for (auto it2 = MainController::objectVectorpair.begin(); it2 != MainController::objectVectorpair.end(); ++it2) {
			if (it2->third != MainController::shapeType)
				continue;
			if (it2->second == MainController::objectNumber) {
				auto CurvedCylinderShape = dynamic_cast <CurvedCylinderController*> (it2->first);
				CurvedCylinderShape->SetPoint(spinbox, spinbox2, spinbox3, spinbox4);
			}

		}

		break;

	default:
		break;
	}
	mRenderer->ResetCamera();
	mRenderWindow->Render();

}


/**
 * @brief apply function chosen from second combobox on the 3D object chosen from first combobox or selected by mouse
 *
 */
void MainController::changeProperty(std::string selecteditem, vtkActor* selectActorCom, int property, int spinbox, int spinbox2, int spinbox3, double spinbox4) {


	int objectNumber = 0;
	objectNumber = extractInteger(selecteditem);

	Controller* editableShape = new Controller();
	for (auto it = MainController::objectVectorpair.begin(); it != MainController::objectVectorpair.end(); ++it)
	{
		if (it->second == objectNumber) {
			editableShape = it->first;

		}
	}
	//vtkActor* selectStyleActor = style->getActor();
//	vtkActor* selectActorCom = myCommand->getActor();
	vtkActor* selectActorChoice = NULL;
	if (selectActorCom) {
		selectActorChoice = selectActorCom;
	}
	else if (editableShape) {
		selectActorChoice = editableShape->GetActor();

	}

	if (selectActorChoice) {


		switch (property) {
		case 0:
			MainController::setActorColorAndOpacity(selectActorChoice, spinbox, spinbox2, spinbox3, spinbox4);
			break;
		case 1: {
			MainController::deleteActor(selectActorChoice);

			std::string name = selectActorChoice->GetObjectName();
			for (auto it = objectNames.begin(); it != objectNames.end(); ++it)
			{
				if (name == *it) {
					objectNames.erase(it);
					break;
				}
			}

			break;
		}
			

			
		case 2:
			MainController::rotateActor(selectActorChoice, spinbox, spinbox2, spinbox3);

			break;
		case 3:
			MainController::scaleActor(selectActorChoice, -1, 1, 1);

			break;

		case 4:
			MainController::scaleActor(selectActorChoice, 1, -1, 1);
			break;
		case 5:
			MainController::scaleActor(selectActorChoice, 1, 1, -1);
			break;
		case 6:
			//translateActor(MainController::pickedActor, ui->spinBox->value(), ui->spinBox_2->value(), ui->spinBox_3->value());
			//MainController->translateActor(editableShape->GetActor(), ui->spinBox->value(), ui->spinBox_2->value(), ui->spinBox_3->value());
			MainController::translateActor(selectActorChoice, spinbox, spinbox2, spinbox3);
			break;
		case 7:
			MainController::scaleActor(selectActorChoice, spinbox, spinbox2, spinbox3);
			break;
		}

	}
}
/**
 * @brief extract integer from string
 *
 * @param str
 * @return int
 */

int  MainController::extractInteger(std::string str) {
	std::stringstream ss(str);
	std::string temp;
	int found;
	int result = 0;
	while (!ss.eof()) {
		ss >> temp;
		if (std::stringstream(temp) >> found) {
			result = result * 10 + found;
		}
		temp = "";
	}
	return result;
}
/**
 * @brief extract multiple integers from string and stores them into vector
 *
 * @param str
 * @return std::vector<int>
 */
std::vector<int> MainController::extractMultiIntegers(std::string str) {
	std::stringstream ss(str);
	std::string temp;
	int found;
	std::vector<int> result;
	while (!ss.eof()) {
		ss >> temp;
		if (std::stringstream(temp) >> found) {
			result.push_back(found);
		}
		temp = "";
	}
	return result;
}
void MainController ::drawSphere()
{
	SphereModel* model = new SphereModel();
	SphereView* view = new SphereView(model);
	SphereController* controller = new SphereController(model, view);

	//controller->SetRadius(2.0);
	mRenderer->AddViewProp(view->actor);
	mRenderer->ResetCamera();
	mRenderWindow->Render();
	MainController::objectCount++;

	view->actor->SetObjectName("sphere " + std::to_string(MainController::objectCount));
	objectNames.push_back("sphere " + std::to_string(MainController::objectCount));

	MainController::objectVectorpair.push_back({ controller ,MainController::objectCount, 1 });
}
/**
 * @brief  draw cube on button click
 *
 */
void MainController::drawCube() {

	CubeModel* model = new CubeModel();
	CubeView* view = new CubeView(model);
	CubeController* cube = new CubeController(model, view);
	cube->SetLength(4.0);

	// Add the sphere actor to the OpenGL
	mRenderer->AddViewProp(view->actor);
	mRenderer->ResetCamera();
	mRenderWindow->Render();
	MainController::objectCount++;
	
	view->actor->SetObjectName("cube " + std::to_string(MainController::objectCount));

	MainController::objectVectorpair.push_back({ cube ,MainController::objectCount, 2 });
	objectNames.push_back("cube " + std::to_string(MainController::objectCount));

	//ui->comboBox->addItem(QString("cube %1").arg(MainController::objectCount));
	//ui->comboBox_3->addItem(QString("cube %1").arg(MainController::objectCount));


}
/**
 * @brief  draw cone on button click
 *
 */
void MainController::drawCone() {

	ConeModel* model = new ConeModel();
	ConeView* view = new ConeView(model);
	ConeController* controller = new ConeController(model, view);

	controller->SetRadius(2.0);
	controller->SetHeight(4.0);

	// Add the sphere actor to the OpenGL
	mRenderer->AddViewProp(view->actor);
	mRenderer->ResetCamera();
	mRenderWindow->Render();

	MainController::objectCount++;

	MainController::objectVectorpair.push_back({ controller ,MainController::objectCount, 3 });
	view->actor->SetObjectName("cone " + std::to_string(MainController::objectCount));
	objectNames.push_back("cone " + std::to_string(MainController::objectCount));

	//ui->comboBox->addItem(QString("cone %1").arg(MainController::objectCount));
	//ui->comboBox_3->addItem(QString("cone %1").arg(MainController::objectCount));


}
/**
 * @brief  draw cylinder on button click
 *
 */
void MainController::drawCylinder() {

	CylinderModel* model = new CylinderModel();
	CylinderView* view = new CylinderView(model);
	CylinderController* controller = new CylinderController(model, view);

	controller->SetRadius(2.0);
	controller->SetHeight(4.0);
	// Add the sphere actor to the OpenGL
	mRenderer->AddViewProp(view->actor);
	mRenderer->ResetCamera();
	mRenderWindow->Render();

	MainController::objectCount++;

	MainController::objectVectorpair.push_back({ controller ,MainController::objectCount,4 });
	view->actor->SetObjectName("cylinder " + std::to_string(MainController::objectCount));
	objectNames.push_back("cylinder " + std::to_string(MainController::objectCount));

	//ui->comboBox->addItem(QString("cylinder %1").arg(MainController::objectCount));
	//ui->comboBox_3->addItem(QString("cylinder %1").arg(MainController::objectCount));



}
/**
 * @brief  draw donut on button click
 *
 */
void MainController::drawDonut() {


	DonutModel* model = new DonutModel();
	DonutView* view = new DonutView(model);
	DonutController* controller = new DonutController(model, view);
	controller->SetCrossSectionRadius(1.0);
	controller->SetRingRadius(2.0);


	// Add the sphere actor to the OpenGL
	mRenderer->AddViewProp(view->actor);
	mRenderer->ResetCamera();
	mRenderWindow->Render();


	MainController::objectCount++;

	MainController::objectVectorpair.push_back({ controller ,MainController::objectCount,5 });
	view->actor->SetObjectName("donut " + std::to_string(MainController::objectCount));
	objectNames.push_back("donut " + std::to_string(MainController::objectCount));

	//ui->comboBox->addItem(QString("donut %1").arg(MainController::objectCount));
	//ui->comboBox_3->addItem(QString("donut %1").arg(MainController::objectCount));


}
/**
 * @brief  draw curved cylinder on button click
 *
 */
void MainController::drawCurvedCylinder() {

	CurvedCylinderModel* model = new CurvedCylinderModel();
	CurvedCylinderView* view = new CurvedCylinderView(model);
	CurvedCylinderController* controller = new CurvedCylinderController(model, view);

	controller->SetNumberOfPoints(3);
	controller->SetPoint(0, 0.0, 0.0, 0.0);
	controller->SetPoint(1, 2.0, 1.0, 1.0);
	controller->SetPoint(2, 3.0, 2.0, 2.0);


	// Add the sphere actor to the OpenGL
	mRenderer->AddViewProp(view->actor);
	mRenderer->ResetCamera();
	mRenderWindow->Render();
	MainController::objectCount++;

	MainController::objectVectorpair.push_back({ controller ,MainController::objectCount , 6 });
	view->actor->SetObjectName("curvedcylinder " + std::to_string(MainController::objectCount));
	objectNames.push_back("curvedcylinder " + std::to_string(MainController::objectCount));

	//ui->comboBox->addItem(QString("curved cylinder %1").arg(MainController::objectCount));
	//ui->comboBox_3->addItem(QString("curved cylinder %1").arg(MainController::objectCount));


}
/**
 * @brief  draw tube on button click
 *
 */
void MainController::drawTube() {


	TubeModel* model = new TubeModel();
	TubeView* view = new TubeView(model);
	TubeController* controller = new TubeController(model, view);

	controller->SetRadius(0.1);
	controller->SetNumberOfSides(40);
	controller->SetPoint1(1.0, 0, 0);
	controller->SetPoint2(5.0, 0, 0);




	// Add the sphere actor to the OpenGL
	mRenderer->AddViewProp(view->actor);
	mRenderer->ResetCamera();
	mRenderWindow->Render();

	MainController::objectCount++;

	MainController::objectVectorpair.push_back({ controller ,MainController::objectCount, 7 });
	view->actor->SetObjectName("tube " + std::to_string(MainController::objectCount));
	objectNames.push_back("tube " + std::to_string(MainController::objectCount));


}
/**
 * @brief  draw pyramid on button click
 *
 */
void MainController::drawPyramid() {

	PyramidModel* model = new PyramidModel();
	PyramidView* view = new PyramidView(model);
	PyramidController* controller = new PyramidController(model, view);

	controller->SetPoint(0, -2.0, -2.0, -1.0);
	controller->SetPoint(1, 2.0, -2.0, -1.0);
	controller->SetPoint(2, 2.0, 2.0, -1.0);
	controller->SetPoint(3, -2.0, 2.0, -1.0);
	controller->SetPoint(4, 0.0, 0.0, 1.0);

	// Add the sphere actor to the OpenGL
	mRenderer->AddViewProp(view->actor);
	mRenderer->ResetCamera();
	mRenderWindow->Render();
	MainController::objectCount++;

	MainController::objectVectorpair.push_back({ controller ,MainController::objectCount,8 });
	view->actor->SetObjectName("pyramid " + std::to_string(MainController::objectCount));
	objectNames.push_back("pyramid " + std::to_string(MainController::objectCount));

	//ui->comboBox->addItem(QString("pyramid %1").arg(MainController::objectCount));
	//ui->comboBox_3->addItem(QString("pyramid %1").arg(MainController::objectCount));

}
/**
 * @brief  draw hemisphere on button click
 *
 */
void MainController::drawHemisphere() {


	HemisphereModel* model = new HemisphereModel();
	HemisphereView* view = new HemisphereView(model);
	HemisphereController* controller = new HemisphereController(model, view);

	controller->SetRadius(2.0);
	// Add the sphere actor to the OpenGL
	mRenderer->AddViewProp(view->actor);
	mRenderer->ResetCamera();
	mRenderWindow->Render();


	MainController::objectCount++;

	MainController::objectVectorpair.push_back({ controller ,MainController::objectCount,9 });
	objectNames.push_back("Hemisphere " + std::to_string(MainController::objectCount));
	view->actor->SetObjectName("Hemisphere " + std::to_string(MainController::objectCount));


}
void MainController::setActorColorAndOpacity(vtkActor* actor, double r, double g, double b, double opacity) {
	actor->GetProperty()->SetColor(r, g, b);
	actor->GetProperty()->SetOpacity(opacity);
	mRenderer->AddViewProp(actor);
	mRenderer->ResetCamera();
	mRenderWindow->Render();

}
/**
 * @brief scale actor in x, y, z directions
 *
 * @param actor
 * @param x
 * @param y
 * @param z
 */
void  MainController::scaleActor(vtkActor* actor, double x, double y, double z) {
	actor->SetScale(x, y, z);
	mRenderer->AddViewProp(actor);
	mRenderer->ResetCamera();
	mRenderWindow->Render();
}
/**
 * @brief translate actor
 *
 * @param actor
 * @param x change in x directions
 * @param y change in y directions
 * @param z change in z directions
 */
void MainController::translateActor(vtkActor* actor, double x, double y, double z) {
	actor->AddPosition(x, y, z);
	mRenderer->AddViewProp(actor);
	mRenderer->ResetCamera();
	mRenderWindow->Render();
}
/**
 * @brief rotate actor in x, y, z directions
 *
 * @param actor
 * @param angle rotation angle about x
 * @param angle2 rotation angle about y
 * @param angle3 rotation angle about z
 */
void MainController::rotateActor(vtkActor* actor, double angle, double angle2, double angle3) {

	actor->RotateX(angle);
	actor->RotateY(angle);
	actor->RotateZ(angle);
	mRenderer->AddViewProp(actor);
	mRenderer->ResetCamera();
	mRenderWindow->Render();

}
void MainController::deleteActor(vtkActor* actor) {
	mRenderer->RemoveActor(actor);
	mRenderer->ResetCamera();
	mRenderWindow->Render();

}
/**
 * @brief merge two actors
 *
 * @param actor1
 * @param actor2
 */
void MainController::mergeActors(vtkActor* actor1, vtkActor* actor2) {
	vtkSmartPointer<vtkAssembly> assembly = vtkSmartPointer<vtkAssembly>::New();
	assembly->AddPart(actor1);
	assembly->AddPart(actor2);
	mRenderer->AddViewProp(assembly);
	mRenderer->ResetCamera();
	mRenderWindow->Render();
}