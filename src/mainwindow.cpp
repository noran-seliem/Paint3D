#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog.h>


#include <qopenglfunctions>
#include <qopenglcontext>
#include <iostream>
#include <qmessagebox>
#include <qmouseevent>


#include <QVTKInteractor.h>
#include <vector>
#include <iostream>
#include <sstream>




vtkStandardNewMacro(customMouseInteractorStyle);

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
	ui->stackedWidget->setCurrentIndex(1);
	mRenderWindow = mainController->GetRenWin();
	mRenderer = mainController->GetRen();

	ui->openGLWidget->setRenderWindow(mRenderWindow);

	mRenderWindow->Render();

	myCommand = MyCommand::New();
	mRenderWindow->GetInteractor()->AddObserver(vtkCommand::LeftButtonPressEvent, myCommand);




	ui->stackedWidget->setCurrentIndex(0);

	ui->openGLWidget_2->setRenderWindow(mRenderWindow);
	ui->openGLWidget_3->setRenderWindow(mainController->mRenderWindow2);
	ui->openGLWidget_4->setRenderWindow(mainController->mRenderWindow3);
	ui->openGLWidget_5->setRenderWindow(mainController->mRenderWindow4);

	mRenderWindow->Render();


	ui->doubleSpinBox->setValue(1.00);
	ui->plainTextEdit->hide();
	ui->comboBox_3->hide();
	ui->editLabel->hide();


	ui->openGLWidget_3->hide();
	ui->openGLWidget_4->hide();
	ui->openGLWidget_5->hide();


	/**
	 * @brief Set the UI connections
	 * 
	 */

	
	QObject::connect(ui->STLButton, &QPushButton::clicked,
		this, &MainWindow::onLoadSTLClick);
		
	QObject::connect(ui->volButton, &QPushButton::clicked,
		this, &MainWindow::onLoadVolClick);

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
void  MainWindow::onLoadVolClick() {

	QString dataDir = QFileDialog::getExistingDirectory(this, "Choose DICOM Directory") + "/";

	ui->openGLWidget_3->show();
	ui->openGLWidget_4->show();
	ui->openGLWidget_5->show();
	mainController->volRayCasting(dataDir);

	mainController->axialView(dataDir);
	mainController->coronalView(dataDir);

	mainController->sagitalView(dataDir);
}
/**
 * @brief draw sphere on button click
 * 
 */
void MainWindow::onDrawSphereClick() {
	mainController->DrawSphere();
	updateUi();
}
void MainWindow::onDrawConeClick() {
	mainController->onDrawConeClick();
	updateUi();


}
void MainWindow::onDrawHemisphereClick() {
	mainController->onDrawHemisphereClick();
	updateUi();

}
void MainWindow::onDrawCubeClick() {
	mainController->onDrawCubeClick();
	updateUi();

}
void MainWindow::onDrawPyramidClick() {
	mainController->onDrawPyramidClick();
	updateUi();

}
void MainWindow::onDrawTubeClick() {
	mainController->onDrawTubeClick();
	updateUi();

}
void MainWindow::onDrawCylinderClick() {
	mainController->onDrawCylinderClick();
	updateUi();

}
void MainWindow::onDrawCurvedCylinderClick() {
	mainController->onDrawCurvedCylinderClick();
	updateUi();

}
void MainWindow::onDrawDonutClick() {
	mainController->onDrawDonutClick();
	updateUi();

}
void MainWindow::updateUi() {
	//objectNames.push_back("cone " + std::to_string(MainController::objectCount));
	mainController->objectNames;
	ui->comboBox->clear();
	ui->comboBox_3->clear();
	for (auto it = mainController->objectNames.begin(); it != mainController->objectNames.end(); ++it)
	{
		QString combo = QString::fromStdString(*it);
		ui->comboBox->addItem(combo);
		ui->comboBox_3->addItem(combo);
	}
}
/**
 * @brief load STL file and renders its content on button click
 * 
 */

void  MainWindow::onLoadSTLClick(){
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath(), tr("STL (*.STL )"));
	mainController->onLoadSTLClick(fileName);
	
	
}

/**
 * @brief save STL file in same directory
 * 
 */
void  MainWindow::onSaveSTLClick(){
	mainController->onSaveSTLClick();
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
	std::string selecteditem2 = "";
	selecteditem2 = ui->comboBox_3->currentText().toStdString();

	mainController->mergeClick(selecteditem1, selecteditem2);
	
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
	vtkActor* selectActorCom = myCommand->getActor();
	MainWindow::shapeType = mainController->getShapeType2(selecteditem, selectActorCom);


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
		

		ui->mergeButton->show();
	}
	if (ui->plainTextEdit->isVisible()) {
		QString text = ui->plainTextEdit->toPlainText();

		ui->pushButton->show();
		ui->nextButton->show();
		ui->nextButton->hide();
		ui->plainTextEdit->hide();
		
		std::string plainEdit = (ui->plainTextEdit->toPlainText()).toStdString();

		vtkActor* selectActorCom = myCommand->getActor();
		mainController->editShape(selectActorCom, plainEdit, ui->spinBox->value(), ui->spinBox_2->value(), ui->spinBox_3->value(), ui->doubleSpinBox->value());

	}
	ui->editLabel->hide();
}
/**
 * @brief apply function chosen from second combobox on the 3D object chosen from first combobox
 * 
 */
void MainWindow::onOkClicked() {
	std::string selecteditem = "";
	selecteditem = ui->comboBox->currentText().toStdString();
	vtkActor* selectActorCom = myCommand->getActor();
	mainController->changeProperty(selecteditem, selectActorCom, ui->comboBox_2->currentIndex(), ui->spinBox->value(), ui->spinBox_2->value(), ui->spinBox_3->value(), ui->doubleSpinBox->value());
	updateUi();

	

	
}
