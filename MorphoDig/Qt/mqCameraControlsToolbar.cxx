/*=========================================================================

   Program: MorphoDig
  

========================================================================*/
#include "mqCameraControlsToolbar.h"
#include "ui_mqCameraControlsToolbar.h"

// For later!

#include "mqUndoRedoReaction.h"
#include "mqMorphoDigCore.h"
#include "mqCameraReaction.h"
#include <vtkRenderer.h>
#include "QDoubleSlider.h"
#include "QReleaseDial.h"
#include "QReleaseSlider.h"
#include "QDoubleReleaseSlider.h"
#include "QReleaseSliderValue.h"
#include "QReleaseScrollBarValue.h"
//#include <QDial>
#include <QToolButton>
#include <QComboBox>
#include <QSlider>
#include <QVBoxLayout>
#include <QHBoxLayout>


//-----------------------------------------------------------------------------
void mqCameraControlsToolbar::constructor()
{
	this->lastzoom = 0;
	this->lastcp = 0;
 // Ui::mqCameraControlsToolbar ui;
 // ui.setupUi(this);
  this->ui = new Ui_mqCameraControlsToolbar;


  this->ui->setupUi(this);
  this->setStyleSheet(
	  
	  "QSlider::groove:vertical{"
  "border: 1px solid #999999;"
  "width: 10px;" /* the groove expands to the size of the slider by default. by giving it a width, it has a fixed size */
  "margin: 0px -5;"
  "}"

	"QSlider::handle:vertical{"
  "background: qlineargradient(x1 : 0, y1 : 0, x2 : 1, y2 : 1, stop : 0 #b4b4b4, stop:1 #8f8f8f);"
  "border: 1px solid #5c5c5c;"
  "height: 10px;"
  "margin: 0px -2;" /* handle is placed by default on the contents rect of the groove. Expand outside the groove */
	  
  "}"
	  "QSlider::add-page:vertical{background:blue;} QSlider::sub-page:vertical{background:orange;}"


	  "QScrollBar{background-color:orange}"

	  "QScrollBar::handle:vertical{background:yellow;max-height:10px;} "  

  );
  /*
  
  "QSlider::handle:vertical{background:qlineargradient(x1 : 0, y1 : 0, x2 : 1, y2 : 1, stop : 0 #b4b4b4, stop:1 #8f8f8f);"
	  "border: 1px solid #5c5c5c;  width: 18px;  margin: -2px 0; "
  	  "border - radius: 3px;}"

  "QSlider::add-page:vertical{background:yellow;} QSlider::sub-page:vertical{background:orange;}"

  */

  /*
  
    "QSlider::groove:vertical{border: 1px solid #999999;height:100px;}"
	
  */
  new mqCameraReaction(this->ui->actionCameraFront, 0); //0 = camera Front
  new mqCameraReaction(this->ui->actionCameraBack, 1); //1 = camera Back
  new mqCameraReaction(this->ui->actionCameraLeft, 2); //2 = camera Left
 new mqCameraReaction(this->ui->actionCameraRight, 3); //3 = camera Right
  new mqCameraReaction(this->ui->actionCameraAbove, 4); //4 = camera Above
  new mqCameraReaction(this->ui->actionCameraBelow, 5); //5 = camera Below

  if (mqMorphoDigCore::instance()->Getmui_CameraCentreOfMassAtOrigin() == 0)
  {

	  this->ui->actionCameraCentreOfMassToggle->setChecked(true);
  }



  if (mqMorphoDigCore::instance()->Getmui_CameraOrtho() == 0)
  {

	  this->ui->actionCameraOrthoPerspectiveToggle->setChecked(true);
  }

  new mqCameraReaction(this->ui->actionCameraCentreOfMassToggle, 6); //6 = camera COM toggle
  new mqCameraReaction(this->ui->actionCameraOrthoPerspectiveToggle, 7); //7 = camera OrthoPerspective toggle



 /* */
  // this->cP = new QDoubleSlider;
  //this->cP = new QReleaseSliderValue(Qt::Vertical, tr("")); 
  this->cP = new QReleaseSlider;
  //this->cP->set
   this->cP->setToolTip(QString("Clipping plane"));
   
   this->cP->setMaximum(100);
   this->cP->setMinimum(-100);
  this->zoom = new QDoubleReleaseSlider;
  this->zoom->setToolTip(QString("Zoom"));
  
  this->zoom->setDoubleMinimum(-100);
  this->zoom->setDoubleMaximum(100);
  this->zoom->setDoubleValue(0);
  //this->zRot = new QReleaseSlider;
  //this->zRot = new QReleaseSliderValue(Qt::Vertical, tr(""));
  this->zRot = new QReleaseSliderValue(Qt::Vertical, tr(""));
  //this->zRot = new QReleaseDial;
  //this->zRot = new QDial;
  //this->zRot->setFixedSize(20, 300);
  this->zRot->setMaximum(90);
  this->zRot->setMinimum(-90);
  
  //this->zRot->setFixedSize(30, 30);
  this->zRot->setObjectName("zRot");
 // this->setFixedWidth(200);
  //this->zRot->setStyleSheet("background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #b4b4b4, stop:1 #8f8f8f);"    );
  
  /*this->setStyleSheet("QDial#zRot{ background-color: blue }"
	  "QDial#zRot:pressed {"
					"background-color: rgb(224, 0, 0);"
					"border-style: inset;"
		"}"
	   );
	   */
  /*QSlider::groove:vertical{
  background: red;
  position: absolute; 
  left: 4px; right: 4px;
  }

	  QSlider::handle:vertical{
  height: 10px;
  background: green;
  margin: 0 - 4px; 
  }

	  QSlider::addpage:vertical{
  background: white;
  }

	  QSlider::sub - page : vertical{
  background: pink;
  }*/

  

  this->zRot->setToolTip(QString("Rotate camera along viewing axis"));
  
  QHBoxLayout *layout = new QHBoxLayout;
  
  
  QWidget* grid = new QWidget();
 layout->addWidget(this->zRot);
  layout->setSpacing(1);
  layout->setMargin(1);
 //layout->addWidget(this->cP);
 //layout->addWidget(this->zoom);

  grid->setLayout(layout);
  this->addWidget(grid);

  // Add values in the combo box
  /*this->addWidget(this->zRot);
  this->addWidget(this->cP);
  this->addWidget(this->zoom);*/
  QHBoxLayout *layout2 = new QHBoxLayout;
  layout2->setSpacing(1);
  layout2->setMargin(1);

  QWidget* grid2 = new QWidget();
  layout2->addWidget(this->cP);


  grid2->setLayout(layout2);
  this->addWidget(grid2);

 

  /*QHBoxLayout *layout2 = new QHBoxLayout;
  QWidget* grid2 = new QWidget();
  layout2->addWidget(this->cP);
  grid2->setLayout(layout2);
  this->addWidget(grid2);*/
  
  //this->addWidget(this->cP);
  

  QWidget* spacer = new QWidget();
  spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  this->addWidget(spacer);

  QHBoxLayout *layout3 = new QHBoxLayout;
  layout3->setSpacing(1);
  layout3->setMargin(1);
  QWidget* grid3 = new QWidget();
  layout3->addWidget(this->zoom);
  grid3->setLayout(layout3);
  this->addWidget(grid3);

  //this->setLayout(layout);
  //this->ui->actionTagEdit->setDisabled(true);
  //this->ui->actionClippingPlaneOnOff->setDisabled(true);
  //connect(mqMorphoDigCore::instance(), SIGNAL(clippingPlaneChanged(int)), this, SLOT(slotClippingPlaneChanged(int)));

  
 
  connect(cP, SIGNAL(valueChanged(int)), this, SLOT(slotCp(int)));
  connect(zRot, SIGNAL(valueChanged(int)), this, SLOT(slotZrot(int)));
  connect(zoom, SIGNAL(valueChanged(int)), this, SLOT(slotZoom()));
  
}

void mqCameraControlsToolbar::slotZoom()
{
	double val = this->zoom->doubleValue();
	if (val == 0) { this->lastzoom = 0;  return; }
	if (this->lastzoom>val)
	{
		mqMorphoDigCore::instance()->getCamera()->Zoom(1.1);
		this->lastzoom = val;
	}
	else if (this->lastzoom<val)
	{
		this->lastzoom = val;
		mqMorphoDigCore::instance()->getCamera()->Zoom(0.9);
		
	}
	mqMorphoDigCore::instance()->signal_zoomChanged();
	mqMorphoDigCore::instance()->Render();
	
}
void mqCameraControlsToolbar::slotCp(int val)
{
	if (val == 0) { this->lastcp = 0;  return; }

	double cr[2];
	double cameracentre[3];
	double camerafocalpoint[3];
	mqMorphoDigCore::instance()->getRenderer()->GetActiveCamera()->GetPosition(cameracentre);
	mqMorphoDigCore::instance()->getRenderer()->GetActiveCamera()->GetFocalPoint(camerafocalpoint);
	/*double dist = sqrt((cameracentre[0] - camerafocalpoint[0])*(cameracentre[0] - camerafocalpoint[0])
		+ (cameracentre[1] - camerafocalpoint[1])*(cameracentre[1] - camerafocalpoint[1])
		+ (cameracentre[2] - camerafocalpoint[2])*(cameracentre[2] - camerafocalpoint[2])
	);
	*/
	
	mqMorphoDigCore::instance()->getRenderer()->GetActiveCamera()->GetClippingRange(cr);
	double dist = cr[1] - cr[0];
	//this->getRenderer()->GetActiveCamera()->SetClippingRange(dist, cr[1]);
	if (val == 0) { this->lastcp = 0;  return; }
	double newnear = cr[0]; // current near clipping plane.
	double newfar = cr[1];
	if (lastcp < val)
	{
		newnear = newnear + (cr[1] - cr[0]) *(val - lastcp) / 1000;
	}
	else
	{
		newnear = newnear - cr[0] * (lastcp-val) / 1000;
	}
	if (newnear <= 0) { return; }
	if (newnear > cr[1]) {
		return;
	}
	//cout << "old near=" << cr[0] << endl;
	//cout << "new near=" << newnear << endl;
	//cout << "old far=" << cr[1] << endl;
	newfar = newnear + dist;
	//cout << "new far=" << newfar << endl;
	mqMorphoDigCore::instance()->getRenderer()->GetActiveCamera()->SetClippingRange(newnear, newfar);
	mqMorphoDigCore::instance()->Render();
	this->lastcp = val;
}

void mqCameraControlsToolbar::slotZrot(int val)
{
	cout << "Rot val=" << val << endl;
	if (val == 0) { this->lastzrot = 0;  return; }
	
		
		mqMorphoDigCore::instance()->getCamera()->Roll(this->lastzrot -val);
		mqMorphoDigCore::instance()->getCamera()->OrthogonalizeViewUp();					
	
	mqMorphoDigCore::instance()->Render();
	this->lastzrot = val;
	/*
	
	  double *center = this->CurrentRenderer->GetCenter();

  double newAngle =
    vtkMath::DegreesFromRadians( atan2( rwi->GetEventPosition()[1] - center[1],
                                        rwi->GetEventPosition()[0] - center[0] ) );

  double oldAngle =
    vtkMath::DegreesFromRadians( atan2( rwi->GetLastEventPosition()[1] - center[1],
                                        rwi->GetLastEventPosition()[0] - center[0] ) );

  vtkCamera *camera = this->CurrentRenderer->GetActiveCamera();
  camera->Roll( newAngle - oldAngle );
  camera->OrthogonalizeViewUp();
	*/
	/*if (val > 1)
	{
		mqMorphoDigCore::instance()->getCamera()->Zoom(1.1);
	}
	else
	{
		mqMorphoDigCore::instance()->getCamera()->Zoom(0.9);
	}
	mqMorphoDigCore::instance()->Render();*/
}


	

	




