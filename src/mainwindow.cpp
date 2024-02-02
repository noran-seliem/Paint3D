#include "mainwindow.h"
#include "ui_mainwindow.h"

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

#include <vtkSmartPointer.h>
#include <vtkAssembly.h>

#include <vtkPropPicker.h>

/**
 * @brief Construct a new Main Window:: Main Window object
 * 
 * @param parent 
 */

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	/**
	 * @brief connect rendering to UI
	 * 
	 */
	mRenderWindow->AddRenderer(mRenderer);

	ui->openGLWidget->setRenderWindow(mRenderWindow);
	ui->openGLWidget->installEventFilter(this);

	
	mRenderer->ResetCamera();
	mRenderWindow->Render();

/**
 * @brief Set the background color
 * 
 */
 
	vtkNew<vtkNamedColors> colors;
	mRenderer->SetBackground(colors->GetColor3d("Cornsilk").GetData());


	////////////////////////////////////////////////////////////////////////////

	ui->doubleSpinBox->setValue(1.00);
	ui->plainTextEdit->hide();
	ui->comboBox_3->hide();
	/**
	 * @brief Set the UI connections
	 * 
	 */
 
	
	QObject::connect(ui->STLButton, &QPushButton::clicked,
		this, &MainWindow::onLoadSTLClick);
	QObject::connect(ui->saveSTLButton , &QPushButton::clicked,
		this, &MainWindow::onSaveSTLClick);
	QObject::connect(ui->sphereButton, &QPushButton::clicked,
		this, &MainWindow::onDrawSphereClick);

	QObject::connect(ui->coneButton, &QPushButton::clicked,
		this, &MainWindow::onDrawConeClick);
	QObject::connect(ui->cubeButton, &QPushButton::clicked,
		this, &MainWindow::onDrawCubeClick);
	QObject::connect(ui->curvedCylButton, &QPushButton::clicked,
		this, &MainWindow::onDrawCurvedCylinderClick);
	QObject::connect(ui->cylinderButton, &QPushButton::clicked,
		this, &MainWindow::onDrawCylinderClick);
	QObject::connect(ui->donutButton, &QPushButton::clicked,
		this, &MainWindow::onDrawDonutClick);
	QObject::connect(ui->HemisphereButton, &QPushButton::clicked,
		this, &MainWindow::onDrawHemisphereClick);
	QObject::connect(ui->pyramidButton, &QPushButton::clicked,
		this, &MainWindow::onDrawPyramidClick);
	QObject::connect(ui->tubeButton, &QPushButton::clicked,
		this, &MainWindow::onDrawTubeClick);
	QObject::connect(ui->mergeButton, &QPushButton::clicked,
		this, &MainWindow::onMergeClick);
	QObject::connect(ui->editButton, &QPushButton::clicked,
		this, &MainWindow::onEditClick);
	QObject::connect(ui->pushButton, &QPushButton::clicked,
		this, &MainWindow::onOkClicked);
	QObject::connect(ui->nextButton, &QPushButton::clicked,
		this, &MainWindow::onNextClicked);
	


}
/**
 * @brief Destroy the Main Window:: Main Window object
 * 
 */
MainWindow::~MainWindow()
{
	delete ui;
}

bool MainWindow::eventFilter(QObject* obj, QEvent* event)
{
	// Capturing keyboard events for moving
	if (event->type() == QEvent::MouseButtonDblClick)
	{
		vtkSmartPointer<vtkPropPicker> picker = vtkSmartPointer<vtkPropPicker>::New();

		int x, y;
		mRenderWindow->GetInteractor()->GetEventPosition(x, y);
		picker->Pick(x, y, 0, mRenderer);
		MainWindow::pickedActor = picker->GetActor();

		if (MainWindow::pickedActor)
		{
			MainWindow::pickedActor->GetProperty()->EdgeVisibilityOn();
			MainWindow::pickedActor->GetProperty()->SetEdgeColor(1, 1, 0);
			mRenderer->ResetCamera();
			mRenderWindow->Render();
		}
		
	}
	return QObject::eventFilter(obj, event);
}
/**
 * @brief set Actor Color And Opacity 
 * 
 * @param actor 
 * @param r 
 * @param g 
 * @param b 
 * @param opacity 
 */

void MainWindow::setActorColorAndOpacity(vtkActor* actor, double r, double g, double b, double opacity) {
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
void  MainWindow::scaleActor(vtkActor* actor, double x, double y, double z) {
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
void MainWindow::translateActor(vtkActor* actor, double x, double y, double z) {
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
void MainWindow::rotateActor(vtkActor* actor, double angle, double angle2, double angle3) {

	actor->RotateX(angle);
	actor->RotateY(angle);
	actor->RotateZ(angle);
	mRenderer->AddViewProp(actor);
	mRenderer->ResetCamera();
	mRenderWindow->Render();

}
void MainWindow::deleteActor(vtkActor* actor) {
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
void MainWindow::mergeActors(vtkActor* actor1, vtkActor* actor2) {
	vtkSmartPointer<vtkAssembly> assembly = vtkSmartPointer<vtkAssembly>::New();
	assembly->AddPart(actor1);
	assembly->AddPart(actor2);
	mRenderer->AddViewProp(assembly);
	mRenderer->ResetCamera();
	mRenderWindow->Render();
}
/**
 * @brief draw sphere on button click
 * 
 */
void MainWindow::onDrawSphereClick() {


	SphereModel* model = new SphereModel();
	SphereView* view = new SphereView(model);
	SphereController* controller = new SphereController(model, view);

	controller->SetRadius(2.0);
	// Add the sphere actor to the OpenGL
	mRenderer->AddViewProp(view->actor);
	mRenderer->ResetCamera();
	mRenderWindow->Render();

	MainWindow::objectCount++;
	MainWindow::spherePair.push_back({ controller ,MainWindow::objectCount });
	MainWindow::objectVectorpair.push_back({ controller ,MainWindow::objectCount, 1 });
	ui->comboBox->addItem(QString("sphere %1").arg(MainWindow::objectCount));
	ui->comboBox_3->addItem(QString("sphere %1").arg(MainWindow::objectCount));

}
/**
 * @brief  draw cube on button click
 * 
 */
void MainWindow::onDrawCubeClick() {

	CubeModel* model = new CubeModel();
	CubeView* view = new CubeView(model);
	CubeController* cube = new CubeController(model, view);
	cube->SetLength(4.0);

	// Add the sphere actor to the OpenGL
	mRenderer->AddViewProp(view->actor);
	mRenderer->ResetCamera();
	mRenderWindow->Render();
	MainWindow::objectCount++;
	MainWindow::cubePair.push_back({ cube ,MainWindow::objectCount });
	MainWindow::objectVectorpair.push_back({ cube ,MainWindow::objectCount, 2 });
	ui->comboBox->addItem(QString("cube %1").arg(MainWindow::objectCount));
	ui->comboBox_3->addItem(QString("cube %1").arg(MainWindow::objectCount));

	
}
/**
 * @brief  draw cone on button click
 * 
 */
void MainWindow::onDrawConeClick() {

	ConeModel* model = new ConeModel();
	ConeView* view = new ConeView(model);
	ConeController* controller = new ConeController(model, view);

	controller->SetRadius(2.0);
	controller->SetHeight(4.0);

	// Add the sphere actor to the OpenGL
	mRenderer->AddViewProp(view->actor);
	mRenderer->ResetCamera();
	mRenderWindow->Render();

	MainWindow::objectCount++;
	MainWindow::conePair.push_back({ controller ,MainWindow::objectCount });

	MainWindow::objectVectorpair.push_back({ controller ,MainWindow::objectCount, 3 });
	ui->comboBox->addItem(QString("cone %1").arg(MainWindow::objectCount));
	ui->comboBox_3->addItem(QString("cone %1").arg(MainWindow::objectCount));


}
/**
 * @brief  draw cylinder on button click
 * 
 */
void MainWindow::onDrawCylinderClick() {

	CylinderModel* model = new CylinderModel();
	CylinderView* view = new CylinderView(model);
	CylinderController* controller = new CylinderController(model, view);

	controller->SetRadius(2.0);
	controller->SetHeight(4.0);
	// Add the sphere actor to the OpenGL
	mRenderer->AddViewProp(view->actor);
	mRenderer->ResetCamera();
	mRenderWindow->Render();

	MainWindow::objectCount++;
	MainWindow::cylinderPair.push_back({ controller ,MainWindow::objectCount });

	MainWindow::objectVectorpair.push_back({ controller ,MainWindow::objectCount,4 });
	ui->comboBox->addItem(QString("cylinder %1").arg(MainWindow::objectCount));
	ui->comboBox_3->addItem(QString("cylinder %1").arg(MainWindow::objectCount));



}
/**
 * @brief  draw donut on button click
 * 
 */
void MainWindow::onDrawDonutClick() {


	DonutModel* model = new DonutModel();
	DonutView* view = new DonutView(model);
	DonutController* controller = new DonutController(model, view);
	controller->SetCrossSectionRadius(1.0);
	controller->SetRingRadius(2.0);


	// Add the sphere actor to the OpenGL
	mRenderer->AddViewProp(view->actor);
	mRenderer->ResetCamera();
	mRenderWindow->Render();


	MainWindow::objectCount++;
	MainWindow::donutPair.push_back({ controller ,MainWindow::objectCount });

	MainWindow::objectVectorpair.push_back({ controller ,MainWindow::objectCount,5 });
	ui->comboBox->addItem(QString("donut %1").arg(MainWindow::objectCount));
	ui->comboBox_3->addItem(QString("donut %1").arg(MainWindow::objectCount));


}
/**
 * @brief  draw curved cylinder on button click
 * 
 */
void MainWindow::onDrawCurvedCylinderClick() {

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
	MainWindow::objectCount++;
	MainWindow::curvedCylPair.push_back({ controller ,MainWindow::objectCount });

	MainWindow::objectVectorpair.push_back({ controller ,MainWindow::objectCount , 6});
	ui->comboBox->addItem(QString("curved cylinder %1").arg(MainWindow::objectCount));
	ui->comboBox_3->addItem(QString("curved cylinder %1").arg(MainWindow::objectCount));


}
/**
 * @brief  draw tube on button click
 * 
 */
void MainWindow::onDrawTubeClick() {


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

	MainWindow::objectCount++;
	MainWindow::tubePair.push_back({ controller ,MainWindow::objectCount });

	MainWindow::objectVectorpair.push_back({ controller ,MainWindow::objectCount, 7 });
	ui->comboBox->addItem(QString("tube %1").arg(MainWindow::objectCount));
	ui->comboBox_3->addItem(QString("tube %1").arg(MainWindow::objectCount));

}
/**
 * @brief  draw pyramid on button click
 * 
 */
void MainWindow::onDrawPyramidClick() {

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
	MainWindow::objectCount++;
	MainWindow::pyramidPair.push_back({ controller ,MainWindow::objectCount });

	MainWindow::objectVectorpair.push_back({ controller ,MainWindow::objectCount,8 });
	ui->comboBox->addItem(QString("pyramid %1").arg(MainWindow::objectCount));
	ui->comboBox_3->addItem(QString("pyramid %1").arg(MainWindow::objectCount));

}
/**
 * @brief  draw hemisphere on button click
 * 
 */
void MainWindow::onDrawHemisphereClick() {


	HemisphereModel* model = new HemisphereModel();
	HemisphereView* view = new HemisphereView(model);
	HemisphereController* controller = new HemisphereController(model, view);

	controller->SetRadius(2.0);
	// Add the sphere actor to the OpenGL
	mRenderer->AddViewProp(view->actor);
	mRenderer->ResetCamera();
	mRenderWindow->Render();


	MainWindow::objectCount++;
	MainWindow::hemispherePair.push_back({ controller ,MainWindow::objectCount });

	MainWindow::objectVectorpair.push_back({ controller ,MainWindow::objectCount,9 });
	ui->comboBox->addItem(QString("Hemisphere %1").arg(MainWindow::objectCount));
	ui->comboBox_3->addItem(QString("Hemisphere %1").arg(MainWindow::objectCount));

}
/**
 * @brief load STL file and renders its content on button click
 * 
 */

void  MainWindow::onLoadSTLClick(){
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath(), tr("STL (*.STL )"));
	if (!fileName.isEmpty()) {
		QMessageBox::information(nullptr, "Title", QString("file is empty"));
	}
	 inputFile = fileName.toStdString();
	
	STLFileHandler handler(inputFile, "output.STL");
	
	handler.LoadSTL(MainWindow::STLpolydata);
	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputData(MainWindow::STLpolydata);

	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	

	actor->SetMapper(mapper);
	mRenderer->AddViewProp(actor);
	mRenderer->ResetCamera();
	mRenderWindow->Render();
	
}
/**
 * @brief save STL file in same directory
 * 
 */
void  MainWindow::onSaveSTLClick(){
	STLFileHandler handler(MainWindow::inputFile, "output.STL");
	handler.SaveSTL(MainWindow::STLpolydata);
}

/**
 * @brief changes interface to recieve input of merge.
 * 
 */
void MainWindow::onMergeClick() {
	ui->comboBox_2->setCurrentIndex(-1);
	ui->comboBox_3->show();
	ui->nextButton->show();
	ui->pushButton->hide();
	ui->mergeButton->hide();
	std::string selecteditem1 = "";
	selecteditem1 = ui->comboBox->currentText().toStdString();


	int objectNumber1 = 0;
	objectNumber1 = extractInteger(selecteditem1);



	Controller* editableShape1 = new Controller();
	for (auto it = MainWindow::objectVectorpair.begin(); it != MainWindow::objectVectorpair.end(); ++it)
	{
		if (it->second == objectNumber1) {
			editableShape1 = it->first;

		}
	}
	std::string selecteditem2 = "";
	selecteditem2 = ui->comboBox->currentText().toStdString();


	int objectNumber2 = 0;
	objectNumber2 = extractInteger(selecteditem2);



	Controller* editableShape2 = new Controller();
	for (auto it2 = MainWindow::objectVectorpair.begin(); it2 != MainWindow::objectVectorpair.end(); ++it2)
	{
		if (it2->second == objectNumber2) {
			editableShape2 = it2->first;

		}
	}
	mergeActors(editableShape1->GetActor(), editableShape2->GetActor());
}
/**
 * @brief changes interface based on the 3D object edited
 * 
 */
void MainWindow::onEditClick() {
	ui->pushButton->hide();
	ui->comboBox_3->hide();
	ui->nextButton->hide();
	ui->nextButton->show();
	ui->plainTextEdit->show();
	ui->editLabel->show();



	//ui->label->setText(text);

	std::string selecteditem = "";
	selecteditem = ui->comboBox->currentText().toStdString();
	MainWindow::objectNumber = extractInteger(selecteditem);
	//Controller* editableShape = new Controller();

	for (auto it = MainWindow::objectVectorpair.begin(); it != MainWindow::objectVectorpair.end(); ++it)
	{
		if (it->second == MainWindow::objectNumber) {
			//editableShape = it->first;
			MainWindow::shapeType = it->third;
		}
	}
	switch (MainWindow::shapeType)
	{
	case 1:
		ui->editLabel->setText("change radius of sphere");
		break;
	case 2:
		ui->editLabel->setText("change cube length");
		break;	
	case 3:
		ui->editLabel->setText("insert radius and hight");
			break;
	case 4:
		ui->editLabel->setText("insert radius and hight");
		break;
	case 5:
		ui->editLabel->setText("insert ring radius and cross radius");
		break;
	case 6:
		ui->editLabel->setText("insert (0:2), x, y, z use spinboxes for points");
		
		break;
	case 7:
		ui->editLabel->setText("insert radius and id(0:1), x, y, z use spinboxes for points");
		break;
	case 8:
		ui->editLabel->setText("insert (0:4), x, y, z use spinboxes for points");
		

		break;
	case 9:
		ui->editLabel->setText("insert radius");
		break;
	}
	
}


/**
 * @brief apply edit or merge 
 * 
 */

void  MainWindow::onNextClicked() {
	if (ui->comboBox_3->isVisible()) {
		onMergeClick();
		ui->nextButton->hide();
		ui->pushButton->show();
		ui->comboBox_3->hide();
		ui->editLabel->hide();
		ui->mergeButton->show();
	}
	if (ui->plainTextEdit->isVisible()) {
		QString text = ui->plainTextEdit->toPlainText();
		QMessageBox::information(nullptr, "Title", QString("text:%1").arg(text));
		ui->pushButton->show();
		ui->nextButton->show();
		ui->nextButton->hide();
		ui->plainTextEdit->hide();

		SphereModel* sphereModel = new SphereModel();
		SphereView* sphereView = new SphereView(sphereModel);
		SphereController* sphereShape = new SphereController(sphereModel, sphereView);

		CubeModel* cubeModel = new CubeModel();
		CubeView* cubeView = new CubeView(cubeModel);
		CubeController* cubeShape = new CubeController(cubeModel, cubeView);

		ConeModel* coneModel = new ConeModel();
		ConeView* coneView = new ConeView(coneModel);
		ConeController* coneShape = new ConeController(coneModel, coneView);

		CylinderModel* cylinderModel = new CylinderModel();
		CylinderView* cylinderView = new CylinderView(cylinderModel);
		CylinderController* cylinderShpe = new CylinderController(cylinderModel, cylinderView);

		DonutModel* donutModel = new DonutModel();
		DonutView* donutView = new DonutView(donutModel);
		DonutController* donutShape = new DonutController(donutModel, donutView);

		CurvedCylinderModel* curvedCylinderModel = new CurvedCylinderModel();
		CurvedCylinderView* curvedCylinderView = new CurvedCylinderView(curvedCylinderModel);
		CurvedCylinderController* CurvedCylinderShape = new CurvedCylinderController(curvedCylinderModel, curvedCylinderView);

		TubeModel* tubeModel = new TubeModel();
		TubeView* tubeView = new TubeView(tubeModel);
		TubeController* tubeShape = new TubeController(tubeModel, tubeView);

		PyramidModel* pyramidModel = new PyramidModel();
		PyramidView* pyramidView = new PyramidView(pyramidModel);
		PyramidController* pyramidShape = new PyramidController(pyramidModel, pyramidView);

		HemisphereModel* hemisphereModel = new HemisphereModel();
		HemisphereView* hemisphereView = new HemisphereView(hemisphereModel);
		HemisphereController* hemisphereShape = new HemisphereController(hemisphereModel, hemisphereView);
		std::string plainEdit = (ui->plainTextEdit->toPlainText()).toStdString();
		
		std::vector<int> inputInt = MainWindow::extractMultiIntegers(plainEdit);
		if (inputInt.empty()) {
			QMessageBox::information(nullptr, "Title", QString("please insert required data and try again"));
			
		} else{
			switch (MainWindow::shapeType)
			{
			case 1:

				for (auto it2 = MainWindow::spherePair.begin(); it2 != MainWindow::spherePair.end(); ++it2) {
					if (it2->second == MainWindow::objectNumber) {
						sphereShape = it2->first;
					}

				}

				sphereShape->SetRadius(inputInt.at(0));
				
				break;
			case 2:

				for (auto it = MainWindow::cubePair.begin(); it != MainWindow::cubePair.end(); ++it) {
					if (it->second == MainWindow::objectNumber) {
						cubeShape = it->first;
					}

				}
				cubeShape->SetLength(inputInt.at(0));
				
				break;
			case 3:

				for (auto it2 = MainWindow::conePair.begin(); it2 != MainWindow::conePair.end(); ++it2) {
					if (it2->second == MainWindow::objectNumber) {
						coneShape = it2->first;
					}

				}
				coneShape->SetRadius(inputInt.at(0));
				coneShape->SetHeight(inputInt.at(1));
				break;
			case 4:

				for (auto it2 = MainWindow::cylinderPair.begin(); it2 != MainWindow::cylinderPair.end(); ++it2) {
					if (it2->second == MainWindow::objectNumber) {
						cylinderShpe = it2->first;
					}
				}
				cylinderShpe->SetRadius(inputInt.at(0));
				cylinderShpe->SetHeight(inputInt.at(1));
				break;
			case 5:

				for (auto it2 = MainWindow::donutPair.begin(); it2 != MainWindow::donutPair.end(); ++it2) {
					if (it2->second == MainWindow::objectNumber) {
						donutShape = it2->first;
					}
				}
				donutShape->SetRingRadius(inputInt.at(0));
				donutShape->SetCrossSectionRadius(inputInt.at(1));
				break;

			case 7:

				for (auto it2 = MainWindow::tubePair.begin(); it2 != MainWindow::tubePair.end(); ++it2) {
					if (it2->second == MainWindow::objectNumber) {
						tubeShape = it2->first;
					}
				}
				tubeShape->SetRadius(inputInt.at(0));
				if (ui->spinBox->value() == 0) {
					tubeShape->SetPoint1(ui->spinBox_2->value(), ui->spinBox_3->value(), ui->doubleSpinBox->value());
				}
				else if (ui->spinBox->value() == 1) {
					tubeShape->SetPoint2(ui->spinBox_2->value(), ui->spinBox_3->value(), ui->doubleSpinBox->value());

				}
				break;

				pyramidShape->SetPoint(ui->spinBox->value(), ui->spinBox_2->value(), ui->spinBox_3->value(), ui->doubleSpinBox->value());

				break;
			case 9:

				for (auto it2 = MainWindow::hemispherePair.begin(); it2 != MainWindow::hemispherePair.end(); ++it2) {
					if (it2->second == MainWindow::objectNumber) {
						hemisphereShape = it2->first;
					}
					
				}
				hemisphereShape->SetRadius(inputInt.at(0));
				break;
			default:
				break;
			}


		}
		switch (MainWindow::shapeType)
		{
		case 6:

			for (auto it2 = MainWindow::curvedCylPair.begin(); it2 != MainWindow::curvedCylPair.end(); ++it2) {
				if (it2->second == MainWindow::objectNumber) {
					CurvedCylinderShape = it2->first;
				}
			}
			CurvedCylinderShape->SetPoint(ui->spinBox->value(), ui->spinBox_2->value(), ui->spinBox_3->value(), ui->doubleSpinBox->value());
			break;
		case 8:

			for (auto it2 = MainWindow::pyramidPair.begin(); it2 != MainWindow::pyramidPair.end(); ++it2) {
				if (it2->second == MainWindow::objectNumber) {
					pyramidShape = it2->first;
				}

			}
		default:
			break;
		}
		mRenderer->ResetCamera();
		mRenderWindow->Render();

	}
}
/**
 * @brief apply function chosen from second combobox on the 3D object chosen from first combobox
 * 
 */
void MainWindow::onOkClicked() {
	std::string selecteditem = "";
	selecteditem = ui->comboBox->currentText().toStdString();
	
	int objectNumber = 0;
	objectNumber = extractInteger(selecteditem);

	
	Controller* editableShape = new Controller();
	for (auto it = MainWindow::objectVectorpair.begin(); it != MainWindow::objectVectorpair.end(); ++it)
	{
		if (it->second == objectNumber) {
			editableShape = it->first;

		}
	}
	switch (ui->comboBox_2->currentIndex()) {
	case 0:
		setActorColorAndOpacity(MainWindow::pickedActor, ui->spinBox->value(), ui->spinBox_2->value(), ui->spinBox_3->value(), ui->doubleSpinBox->value());
		break;
	case 1:
		deleteActor(MainWindow::pickedActor);
		ui->comboBox->removeItem(objectNumber-1);
		ui->comboBox_3->removeItem(objectNumber - 1);
		break;

		break;
	case 2:
		rotateActor(MainWindow::pickedActor, ui->spinBox->value(), ui->spinBox_2->value(), ui->spinBox_3->value());

		break;
	case 3:
		scaleActor(MainWindow::pickedActor, -1, 1, 1);

		break;
		
	case 4:
		scaleActor(MainWindow::pickedActor, 1, -1, 1);
		break;
	case 5:
		scaleActor(MainWindow::pickedActor, 1, 1, -1);
		break;
	case 6:
		translateActor(MainWindow::pickedActor, ui->spinBox->value(), ui->spinBox_2->value(), ui->spinBox_3->value());
		break;
	case 7:
		scaleActor(MainWindow::pickedActor, ui->spinBox->value(), ui->spinBox_2->value(), ui->spinBox_3->value());
		break;
	}


}
/**
 * @brief extract integer from string
 * 
 * @param str 
 * @return int 
 */

int  MainWindow::extractInteger(std::string str) {
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
std::vector<int> MainWindow::extractMultiIntegers(std::string str) {
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