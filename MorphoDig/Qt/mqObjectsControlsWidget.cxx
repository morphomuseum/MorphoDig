/*=========================================================================

   Program: MorphoDig
   Module:    Copied from Paraview pqMainControlsWidget.cxx

   Copyright (c) 2005,2006 Sandia Corporation, Kitware Inc.
   All rights reserved.

   ParaView is a free software; you can redistribute it and/or modify it
   under the terms of the ParaView license version 1.2.

   See License_v1.2.txt for the full ParaView license.
   A copy of this license can be obtained by contacting
   Kitware Inc.
   28 Corporate Drive
   Clifton Park, NY 12065
   USA

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

========================================================================*/
#include "mqObjectsControlsWidget.h"
#include "ui_mqObjectsControlsWidget.h"

// For later!
#include "QReleaseSlider.h"
#include "QDoubleReleaseSlider.h"
#include "QReleaseSliderValue.h"

#include "mqEditLMKDialogReaction.h"
#include "mqCreateLMKDialogReaction.h"
#include "mqEditFLGDialogReaction.h"
#include "mqEditACTORDialogReaction.h"
#include "mqEditVolumeDialogReaction.h"

#include "mqUndoStack.h"

#include "mqUndoRedoReaction.h"

#include "mqMorphoDigCore.h"

#include "vtkLMActor.h"
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
//#include <vtkRenderWindowInteractor.h>
#include <vtkProp3D.h>
#include <vtkTransform.h>
#include <vtkMatrix4x4.h>
#include <QToolButton>
#include <QHBoxLayout>
#include <QSizePolicy>
#define NORMAL_LMK 0
#define TARGET_LMK 1
#define NODE_LMK 2
#define HANDLE_LMK 3
#define FLAG_LMK 4
#define SPHERE 0
#define ARROW 1


//-----------------------------------------------------------------------------
void mqObjectsControlsWidget::constructor()
{
 // Ui::mqObjectsControlsWidget ui;
 // ui.setupUi(this);
  this->ui = new Ui_mqObjectsControlsWidget;
  
  
  this->oldrotval = 0;
  this->oldtrval = 0;
  this->ui->setupUi(this);
  this->setStyleSheet(

	  "QSlider::groove:vertical{"
	  "border: 1px solid #999999;"
	  "width: 10px;" /* the groove expands to the size of the slider by default. by giving it a width, it has a fixed size */
	  "margin: 0px -4;"
	  "}"

	  "QSlider::handle:vertical{"
	  "background: qlineargradient(x1 : 0, y1 : 0, x2 : 1, y2 : 1, stop : 0 #b4b4b4, stop:1 #8f8f8f);"
	  "border: 1px solid #5c5c5c;"
	  "height: 10px;"
	  "margin: 0px -2;" /* handle is placed by default on the contents rect of the groove. Expand outside the groove */
	  "}"

	  "QSlider::add-page:vertical{"
	  "background:yellow;"
	  "margin: 1px;}"
	  
	  "QSlider::sub-page:vertical{"
	  "background:white;"
	 "margin: 1px;"
	  "}"


	  "QScrollBar{background-color:orange}"

	  "QScrollBar::handle:vertical{background:yellow;max-height:10px;} "

  );
  //this->zTr = new QReleaseSlider;
  this->zTr = new QReleaseSliderValue(Qt::Vertical, tr(""));
  this->zTr->setMaximum(100);
  this->zTr->setMinimum(-100);
  this->zTr->setToolTip(QString("Translate along z viewing axis"));
  QString tz =  QString("tz");
  this->zTr->setLabelText(tz);
  this->zTr->setLabelVisible(1);

  this->zRot = new QReleaseSliderValue(Qt::Vertical, tr(""));
  //this->zRot = new QDoubleReleaseSlider;
  //this->zRot = new QReleaseSlider;
  this->zRot->setMaximum(45);
  this->zRot->setMinimum(-45);
  //this->zRot->setSingleStep(0.1);
  //this->zRot->setSingleStep(0.1);
  this->zRot->setToolTip(QString("Rotation along z viewing axis"));
  QString rz = QString("rz");
  this->zRot->setLabelText(rz);
  this->zRot->setLabelVisible(1);


  this->yTr = new QReleaseSliderValue(Qt::Vertical, tr(""));
  //this->yTr = new QReleaseSlider;
  this->yTr->setMaximum(100);
  this->yTr->setMinimum(-100);
  this->yTr->setToolTip(QString("Translate along y viewing axis"));
  QString ty = QString("ty");
  this->yTr->setLabelText(ty);
  this->yTr->setLabelVisible(1);

  this->yRot = new QReleaseSliderValue(Qt::Vertical, tr(""));
  //this->yRot = new QReleaseSlider;
  this->yRot->setMaximum(45);
  this->yRot->setMinimum(-45);
  this->yRot->setToolTip(QString("Rotation along y viewing axis"));
  QString ry = QString("ry");
  this->yRot->setLabelText(ry);
  this->yRot->setLabelVisible(1);
  
  //this->xTr = new QReleaseSlider;
  this->xTr = new QReleaseSliderValue(Qt::Vertical, tr(""));
  this->xTr->setMaximum(100);
  this->xTr->setMinimum(-100);
  this->xTr->setToolTip(QString("Translate along x viewing axis"));
  QString tx = QString("tx");
  this->xTr->setLabelText(tx);
  this->xTr->setLabelVisible(1);

  this->xRot = new QReleaseSliderValue(Qt::Vertical, tr(""));
  //this->xRot = new QReleaseSlider;
  this->xRot->setMaximum(45);
  this->xRot->setMinimum(-45);
  this->xRot->setToolTip(QString("Rotation along x viewing axis"));
  QString rx = QString("rx");
  this->xRot->setLabelText(rx);
  this->xRot->setLabelVisible(1);

  QHBoxLayout *zlayout = new QHBoxLayout;
  QWidget* zgrid = new QWidget();
  zlayout->setSpacing(1);
  zlayout->setMargin(1);
  //zlayout->set
  zlayout->addWidget(this->zRot);
 // zlayout->addWidget(this->xTr);
  //zlayout->addWidget(this->yTr);
  zlayout->addWidget(this->zTr);

  zgrid->setLayout(zlayout);
  //zgrid->setSizePolicy(QSizePolicy::Policy::Expanding);
  
  this->ui->verticalLayout->addWidget(zgrid);

  QHBoxLayout *ylayout = new QHBoxLayout;
  QWidget* ygrid = new QWidget();
 ylayout->setSpacing(1);
  ylayout->setMargin(1);
  ylayout->addWidget(this->yRot);
  ylayout->addWidget(this->yTr);

  ygrid->setLayout(ylayout);
  this->ui->verticalLayout->addWidget(ygrid);

  QHBoxLayout *xlayout = new QHBoxLayout;
  QWidget* xgrid = new QWidget();
  xlayout->setSpacing(1);
  xlayout->setMargin(1);
 xlayout->addWidget(this->xRot);
 xlayout->addWidget(this->xTr);

  xgrid->setLayout(xlayout);
  this->ui->verticalLayout->addWidget(xgrid);
  /*xgrid->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  ygrid->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  zgrid->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);*/

  

  connect(this->ui->Delete, SIGNAL(pressed()), this, SLOT(slotDeleteObjects()));
  connect(this->ui->Delete, SIGNAL(pressed()), this, SLOT(slotDeleteObjects()));
  connect(this->ui->LandmarkMoveUp, SIGNAL(pressed()), mqMorphoDigCore::instance(), SLOT(slotLandmarkMoveUp()));
  connect(this->ui->LandmarkMoveDown, SIGNAL(pressed()), mqMorphoDigCore::instance(), SLOT(slotLandmarkMoveDown()));

  connect(zRot, SIGNAL(valueChanged(int)), this, SLOT(slotZrot(int)));
  connect(yRot, SIGNAL(valueChanged(int)), this, SLOT(slotYrot(int)));
  connect(xRot, SIGNAL(valueChanged(int)), this, SLOT(slotXrot(int)));
  
  connect(zTr, SIGNAL(valueChanged(int)), this, SLOT(slotZtr(int)));
  connect(yTr, SIGNAL(valueChanged(int)), this, SLOT(slotYtr(int)));
  connect(xTr, SIGNAL(valueChanged(int)), this, SLOT(slotXtr(int)));

  /*connect(zRot->slider, SIGNAL(sliderPressed()), this, SLOT(slotZrotPressed()));
  connect(yRot->slider, SIGNAL(sliderPressed()), this, SLOT(slotYrotPressed()));
  connect(xRot->slider, SIGNAL(sliderPressed()), this, SLOT(slotXrotPressed()));

  connect(zTr->slider, SIGNAL(sliderPressed()), this, SLOT(slotZtrPressed()));
  connect(yTr->slider, SIGNAL(sliderPressed()), this, SLOT(slotYtrPressed()));
  connect(xTr->slider, SIGNAL(sliderPressed()), this, SLOT(slotXtrPressed()));*/
  
  connect(zRot, SIGNAL(sliderPressed()), this, SLOT(slotZrotPressed()));
  connect(yRot, SIGNAL(sliderPressed()), this, SLOT(slotYrotPressed()));
  connect(xRot, SIGNAL(sliderPressed()), this, SLOT(slotXrotPressed()));

  connect(zTr, SIGNAL(sliderPressed()), this, SLOT(slotZtrPressed()));
  connect(yTr, SIGNAL(sliderPressed()), this, SLOT(slotYtrPressed()));
  connect(xTr, SIGNAL(sliderPressed()), this, SLOT(slotXtrPressed()));

  //connect(this->ui->LassoCut, SIGNAL(pressed()), mqMorphoDigCore::instance(), SLOT(slotLassoCutKeepInside()));
  //connect(this->ui->LassoCutKeepOutside, SIGNAL(pressed()), mqMorphoDigCore::instance(), SLOT(slotLassoCutKeepOutside()));
  //connect(this->ui->RubberCut, SIGNAL(pressed()), mqMorphoDigCore::instance(), SLOT(slotRubberCutKeepOutside()));

  QAction* lassoInside = new QAction(tr("&lasso cut: keep inside selected area"), this);
  lassoInside->setToolTip(tr("Lasso cut: keep inside selected surface"));
  this->ui->LassoCut->addAction(lassoInside);
  this->ui->LassoCut->setDefaultAction(lassoInside);
  QIcon icon9;
  icon9.addFile(QStringLiteral(":/Icons/Lasso_keepinside.png"), QSize(), QIcon::Normal, QIcon::Off);
  lassoInside->setIcon(icon9);
  connect(lassoInside, SIGNAL(triggered()), mqMorphoDigCore::instance(), SLOT(slotLassoCutKeepInside()));

  
  QAction* lassoOutside = new QAction(tr("&lasso cut: keep outside selected area"), this);
  lassoOutside->setToolTip(tr("Lasso cut: keep outside selected surface"));
  this->ui->LassoCut->addAction(lassoOutside);
   QIcon icon8;
  icon8.addFile(QStringLiteral(":/Icons/Lasso_keepoutside.png"), QSize(), QIcon::Normal, QIcon::Off);
  lassoOutside->setIcon(icon8);  
  connect(lassoOutside, SIGNAL(triggered()), mqMorphoDigCore::instance(), SLOT(slotLassoCutKeepOutside()));
  
 
  QAction* rubberInside = new QAction(tr("&rubber cut: keep inside selected area"), this);
  rubberInside->setToolTip(tr("Rubber cut: keep inside selected surface"));
  this->ui->RubberCut->addAction(rubberInside);
  this->ui->RubberCut->setDefaultAction(rubberInside);
  QIcon icon10;
  icon10.addFile(QStringLiteral(":/Icons/rubber_mode_keepinside.png"), QSize(), QIcon::Normal, QIcon::Off);
  rubberInside->setIcon(icon10);
  connect(rubberInside, SIGNAL(triggered()), mqMorphoDigCore::instance(), SLOT(slotRubberCutKeepInside()));


  QAction* rubberOutside = new QAction(tr("&rubber cut: keep outside selected area"), this);
  rubberOutside->setToolTip(tr("Rubber cut: keep outside selected surface"));
  this->ui->RubberCut->addAction(rubberOutside);
  QIcon icon11;
  icon11.addFile(QStringLiteral(":/Icons/rubber_mode_keepoutside.png"), QSize(), QIcon::Normal, QIcon::Off);
  rubberOutside->setIcon(icon11);
  connect(rubberOutside, SIGNAL(triggered()), mqMorphoDigCore::instance(), SLOT(slotRubberCutKeepOutside()));


  //TOTO : create an export tag map...
  


  //@@TODO

  QAction* actionEditLandmarks = new QAction(tr("&EditLMK"), this);
  actionEditLandmarks->setToolTip(tr("Edit first selected landmark coordinates."));
  this->ui->EditLandmarks->addAction(actionEditLandmarks);
  this->ui->EditLandmarks->setDefaultAction(actionEditLandmarks);
  QIcon icon;
  icon.addFile(QStringLiteral(":/Icons/Lmk_edit.png"), QSize(), QIcon::Normal, QIcon::Off);
  actionEditLandmarks->setIcon(icon);  
  new mqEditLMKDialogReaction(actionEditLandmarks);


  QAction* actionCreateLandmark = new QAction(tr("&CreateLMK"), this);
  actionCreateLandmark->setToolTip(tr("Create landmark at X,Y,Z."));
  this->ui->CreateLandmark->addAction(actionCreateLandmark);
  this->ui->CreateLandmark->setDefaultAction(actionCreateLandmark);
  QIcon icon2;
  icon2.addFile(QStringLiteral(":/Icons/Lmk_xyz.png"), QSize(), QIcon::Normal, QIcon::Off);
  actionCreateLandmark->setIcon(icon2);
  new mqCreateLMKDialogReaction(actionCreateLandmark);
  
  QAction* actionEditFlags = new QAction(tr("&EditFLG"), this);
  actionEditFlags->setToolTip(tr("Edit first selected flag label, length and color."));
  this->ui->EditFlags->addAction(actionEditFlags);
  this->ui->EditFlags->setDefaultAction(actionEditFlags);
  QIcon icon3;
  icon3.addFile(QStringLiteral(":/Icons/Flag02.png"), QSize(), QIcon::Normal, QIcon::Off);
  actionEditFlags->setIcon(icon3);
  new mqEditFLGDialogReaction(actionEditFlags);


  QAction* actionEditActors = new QAction(tr("&EditActors"), this);
  actionEditActors->setToolTip(tr("Edit first selected surface name, color, matrix and other properties."));
  this->ui->EditActors->addAction(actionEditActors);
  this->ui->EditActors->setDefaultAction(actionEditActors);
  QIcon icon4;
  icon4.addFile(QStringLiteral(":/Icons/actor_edit.png"), QSize(), QIcon::Normal, QIcon::Off);
  actionEditActors->setIcon(icon4);

	new mqEditACTORDialogReaction(actionEditActors);
 
	QAction* actionEditVolumes = new QAction(tr("&EditVolumes"), this);
	actionEditVolumes->setToolTip(tr("Edit first selected volume name, matrix and other properties."));
	this->ui->EditVolumes->addAction(actionEditVolumes);
	this->ui->EditVolumes->setDefaultAction(actionEditVolumes);
	QIcon icon5;
	icon5.addFile(QStringLiteral(":/Icons/volume_edit.png"), QSize(), QIcon::Normal, QIcon::Off);
	actionEditVolumes->setIcon(icon5);

	new mqEditVolumeDialogReaction(actionEditVolumes);

}

/*
void vtkInteractorObserver::ComputeWorldToDisplay(vtkRenderer *ren,
double x,
double y,
double z,
double displayPt[3])
{
ren->SetWorldPoint(x, y, z, 1.0);
ren->WorldToDisplay();
ren->GetDisplayPoint(displayPt);
}


*/



void mqObjectsControlsWidget::PanActors(int axis, int value)
{
	
	//cout << "Pan axis: " << axis << ", value:" << value<< endl;

	// Use initial center as the origin from which to pan
	//double pan_center[3];
	//mqMorphoDigCore::instance()->GetCenterOfMassOfSelectedActors(pan_center);
	//this->GetCenterOfMassOfSelectedActors(pan_center);
	double pan_center[3] = { 0,0,0 };
	mqMorphoDigCore::instance()->GetCenterOfMassOfSelectedActors(pan_center);

	if (value != 0)
	{

		double motion_vector[3] = { 0,0,0 };
		double dPanCenter[3] = { 0,0,0 };
		double dPanAway[3] = { 0,0,0 };

		double view_up[3], view_look[3], view_right[3];

		mqMorphoDigCore::instance()->getCamera()->OrthogonalizeViewUp();
		mqMorphoDigCore::instance()->getCamera()->ComputeViewPlaneNormal();
		mqMorphoDigCore::instance()->getCamera()->GetViewUp(view_up);
		vtkMath::Normalize(view_up);
		mqMorphoDigCore::instance()->getCamera()->GetViewPlaneNormal(view_look);
		vtkMath::Cross(view_up, view_look, view_right);
		vtkMath::Normalize(view_right);
		vtkMath::Normalize(view_look);
		//cout << "View look:" << view_look[0] << "," << view_look[1] << "," << view_look[2] << endl;
		//cout << "View up:" << view_up[0] << "," << view_up[1] << "," << view_up[2] << endl;
		//cout << "View right:" << view_right[0] << "," << view_right[1] << "," << view_right[2] << endl;
		//wrong away!
		double origin[4] = { 0, 0, 1,1 };
		double away[4] = { 0, 0, 2,1 };

	//	cout << "try  DTW" << endl;
		
		//cout << "try  DTW2" << endl;
		int move = 10;
		mqMorphoDigCore::instance()->GetWorldToDisplay(pan_center[0], pan_center[1], pan_center[2], dPanCenter);
		//cout << "dPanCenter:" << dPanCenter[0] << "," << dPanCenter[1] << "," << dPanCenter[2] << endl;
		//cout << "dPanAway:" << dPanAway[0] << "," << dPanAway[1] << "," << dPanAway[2] << endl;
		mqMorphoDigCore::instance()->GetDisplayToWorld(dPanCenter[0], dPanCenter[1], dPanCenter[2], origin);
		mqMorphoDigCore::instance()->GetDisplayToWorld(dPanCenter[0], dPanCenter[1] + 30, dPanCenter[2], away);//example : 30px away from origin!
	
		//cout << "origin:" << origin[0] << "," << origin[1] << "," << origin[2] << endl;
		//cout << "away:" << away[0] << "," << away[1] << "," << away[2] << endl;

		// origin[2] -away[2] will give the amplitude of the movement. Now, let us compute the direction!
		double ampli = -value*(sqrt((origin[0] - away[0])*(origin[0] - away[0])+ (origin[1] - away[1])*(origin[1] - away[1])+ (origin[2] - away[2])*(origin[2] - away[2]))) / 20;
		//cout << "ampli = " << ampli<< endl;
		vtkMath::MultiplyScalar(view_right, ampli);
		vtkMath::MultiplyScalar(view_up, ampli);
		vtkMath::MultiplyScalar(view_look, ampli);

		if (axis==2)
		{
			motion_vector[0] = view_look[0];
			motion_vector[1] = view_look[1];
			motion_vector[2] = view_look[2];
		}
		if (axis ==0)
		{ 
			motion_vector[0] = view_right[0];
			motion_vector[1] = view_right[1];
			motion_vector[2] = view_right[2]; 
		}
			
		if (axis == 1)
		{
			motion_vector[0] = view_up[0];
			motion_vector[1] = view_up[1];
			motion_vector[2] = view_up[2];
		}
	//	cout << "motion vector:"<< motion_vector[0] << "," << motion_vector[1] << "," << motion_vector[2] << endl;


	//	cout << "ok?" << endl;
		mqMorphoDigCore::instance()->getActorCollection()->InitTraversal();
		for (vtkIdType i = 0; i < mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfItems(); i++)
		{
			vtkMDActor *myActor = vtkMDActor::SafeDownCast(mqMorphoDigCore::instance()->getActorCollection()->GetNextActor());
			vtkProp3D *myPropr = vtkProp3D::SafeDownCast(myActor);
			if (myActor->GetSelected() == 1)
			{
				if (myPropr->GetUserMatrix() != NULL)
				{
					vtkTransform *t = vtkTransform::New();
					t->PostMultiply();
					t->SetMatrix(myPropr->GetUserMatrix());
					t->Translate(motion_vector[0], motion_vector[1], motion_vector[2]);
					myPropr->GetUserMatrix()->DeepCopy(t->GetMatrix());
					t->Delete();
				}
				else
				{
					myPropr->AddPosition(motion_vector[0],
						motion_vector[1],
						motion_vector[2]);
				}
				myActor->SetChanged(1);
			}
		}
		mqMorphoDigCore::instance()->getVolumeCollection()->InitTraversal();
		for (vtkIdType i = 0; i < mqMorphoDigCore::instance()->getVolumeCollection()->GetNumberOfItems(); i++)
		{
			vtkMDVolume *myVolume = vtkMDVolume::SafeDownCast(mqMorphoDigCore::instance()->getVolumeCollection()->GetNextVolume());
			vtkProp3D *myPropr = vtkProp3D::SafeDownCast(myVolume);
			if (myVolume->GetSelected() == 1)
			{
				if (myPropr->GetUserMatrix() != NULL)
				{
					vtkTransform *t = vtkTransform::New();
					t->PostMultiply();
					t->SetMatrix(myPropr->GetUserMatrix());
					t->Translate(motion_vector[0], motion_vector[1], motion_vector[2]);
					myPropr->GetUserMatrix()->DeepCopy(t->GetMatrix());
					t->Delete();
				}
				else
				{
					myPropr->AddPosition(motion_vector[0],
						motion_vector[1],
						motion_vector[2]);
				}
				myVolume->SetChanged(1);
			}
		}
		mqMorphoDigCore::instance()->getNormalLandmarkCollection()->InitTraversal();
		for (vtkIdType i = 0; i < mqMorphoDigCore::instance()->getNormalLandmarkCollection()->GetNumberOfItems(); i++)
		{
			vtkLMActor *myActor = vtkLMActor::SafeDownCast(mqMorphoDigCore::instance()->getNormalLandmarkCollection()->GetNextActor());
			vtkProp3D *myPropr = vtkProp3D::SafeDownCast(myActor);
			if (myActor->GetSelected() == 1)
			{
				if (myPropr->GetUserMatrix() != NULL)
				{
					vtkTransform *t = vtkTransform::New();
					t->PostMultiply();
					t->SetMatrix(myPropr->GetUserMatrix());
					t->Translate(motion_vector[0], motion_vector[1], motion_vector[2]);
					myPropr->GetUserMatrix()->DeepCopy(t->GetMatrix());
					t->Delete();
				}
				else
				{
					myPropr->AddPosition(motion_vector[0],
						motion_vector[1],
						motion_vector[2]);
				}
				this->ChangeAttachmentPoint(myPropr->GetMatrix(), myActor);

				myActor->SetChanged(1);
			}
		}
		mqMorphoDigCore::instance()->getTargetLandmarkCollection()->InitTraversal();
		for (vtkIdType i = 0; i < mqMorphoDigCore::instance()->getTargetLandmarkCollection()->GetNumberOfItems(); i++)
		{
			vtkLMActor *myActor = vtkLMActor::SafeDownCast(mqMorphoDigCore::instance()->getTargetLandmarkCollection()->GetNextActor());
			vtkProp3D *myPropr = vtkProp3D::SafeDownCast(myActor);
			if (myActor->GetSelected() == 1)
			{
				if (myPropr->GetUserMatrix() != NULL)
				{
					vtkTransform *t = vtkTransform::New();
					t->PostMultiply();
					t->SetMatrix(myPropr->GetUserMatrix());
					t->Translate(motion_vector[0], motion_vector[1], motion_vector[2]);
					myPropr->GetUserMatrix()->DeepCopy(t->GetMatrix());
					t->Delete();
				}
				else
				{
					myPropr->AddPosition(motion_vector[0],
						motion_vector[1],
						motion_vector[2]);
				}
				this->ChangeAttachmentPoint(myPropr->GetMatrix(), myActor);

				myActor->SetChanged(1);
			}
		}
		mqMorphoDigCore::instance()->getNodeLandmarkCollection()->InitTraversal();
		for (vtkIdType i = 0; i < mqMorphoDigCore::instance()->getNodeLandmarkCollection()->GetNumberOfItems(); i++)
		{
			vtkLMActor *myActor = vtkLMActor::SafeDownCast(mqMorphoDigCore::instance()->getNodeLandmarkCollection()->GetNextActor());
			vtkProp3D *myPropr = vtkProp3D::SafeDownCast(myActor);
			int nchanged = 0;
			if (myActor->GetSelected() == 1)
			{
				if (myPropr->GetUserMatrix() != NULL)
				{
					vtkTransform *t = vtkTransform::New();
					t->PostMultiply();
					t->SetMatrix(myPropr->GetUserMatrix());
					t->Translate(motion_vector[0], motion_vector[1], motion_vector[2]);
					myPropr->GetUserMatrix()->DeepCopy(t->GetMatrix());
					t->Delete();
				}
				else
				{
					myPropr->AddPosition(motion_vector[0],
						motion_vector[1],
						motion_vector[2]);
				}
				this->ChangeAttachmentPoint(myPropr->GetMatrix(), myActor);

				myActor->SetChanged(1);
				nchanged = 1;
			}
			//if (nchanged == 1) { this->NodeLandmarkCollection->SetChanged(1); }
		}
		mqMorphoDigCore::instance()->getHandleLandmarkCollection()->InitTraversal();
		for (vtkIdType i = 0; i < mqMorphoDigCore::instance()->getHandleLandmarkCollection()->GetNumberOfItems(); i++)
		{
			vtkLMActor *myActor = vtkLMActor::SafeDownCast(mqMorphoDigCore::instance()->getHandleLandmarkCollection()->GetNextActor());
			vtkProp3D *myPropr = vtkProp3D::SafeDownCast(myActor);
			if (myActor->GetSelected() == 1)
			{
				if (myPropr->GetUserMatrix() != NULL)
				{
					vtkTransform *t = vtkTransform::New();
					t->PostMultiply();
					t->SetMatrix(myPropr->GetUserMatrix());
					t->Translate(motion_vector[0], motion_vector[1], motion_vector[2]);
					myPropr->GetUserMatrix()->DeepCopy(t->GetMatrix());
					t->Delete();
				}
				else
				{
					myPropr->AddPosition(motion_vector[0],
						motion_vector[1],
						motion_vector[2]);
				}
				this->ChangeAttachmentPoint(myPropr->GetMatrix(), myActor);

				myActor->SetChanged(1);
			}
		}
		mqMorphoDigCore::instance()->getFlagLandmarkCollection()->InitTraversal();
		for (vtkIdType i = 0; i < mqMorphoDigCore::instance()->getFlagLandmarkCollection()->GetNumberOfItems(); i++)
		{
			vtkLMActor *myActor = vtkLMActor::SafeDownCast(mqMorphoDigCore::instance()->getFlagLandmarkCollection()->GetNextActor());
			vtkProp3D *myPropr = vtkProp3D::SafeDownCast(myActor);
			if (myActor->GetSelected() == 1)
			{
				if (myPropr->GetUserMatrix() != NULL)
				{
					vtkTransform *t = vtkTransform::New();
					t->PostMultiply();
					t->SetMatrix(myPropr->GetUserMatrix());
					t->Translate(motion_vector[0], motion_vector[1], motion_vector[2]);
					myPropr->GetUserMatrix()->DeepCopy(t->GetMatrix());
					t->Delete();
				}
				else
				{
					myPropr->AddPosition(motion_vector[0],
						motion_vector[1],
						motion_vector[2]);
				}
				this->ChangeAttachmentPoint(myPropr->GetMatrix(), myActor);

				myActor->SetChanged(1);
			}
		}
	//	cout << "ok???" << endl;

	}
	/*if (this->AutoAdjustCameraClippingRange)
	{
		this->CurrentRenderer->ResetCameraClippingRange();
		mqMorphoDigCore::instance()->ActivateClippingPlane();
	}*/
//	cout << "try to render" << endl;
	mqMorphoDigCore::instance()->Render();
	//cout << "rendering ok" << endl;
}

void mqObjectsControlsWidget::RotateActors(int axis, int degrees)
{
//cout << "Rotate axis: " << axis << ", degrees:" << degrees << endl;
//axis: 0=X, 1=Y, 2=z
	
	double rot_center[3] = { 0,0,0 };

	mqMorphoDigCore::instance()->GetCenterOfMassOfSelectedActors(rot_center);
	//cout << "rotation center: " << rot_centerendl;
	//cout << "Rotation center: " << rot_center[0] << "," << rot_center[1] << "," << rot_center[2] << endl;
	//cout << "bb length...." << endl;
	double boundRadius = mqMorphoDigCore::instance()->GetBoundingBoxLengthOfSelectedActors();
	//cout << "Bound Radius: " << boundRadius << endl;
	if (boundRadius == std::numeric_limits<double>::infinity())
	{
		boundRadius = 60;
	}
	else
	{
		boundRadius *= 0.5;
	}
	// GetLength gets the length of the diagonal of the bounding box
	

	// Get the view up and view right vectors
	double view_up[3], view_look[3], view_right[3];

	mqMorphoDigCore::instance()->getCamera()->OrthogonalizeViewUp();
	mqMorphoDigCore::instance()->getCamera()->ComputeViewPlaneNormal();
	mqMorphoDigCore::instance()->getCamera()->GetViewUp(view_up);
	vtkMath::Normalize(view_up);
	mqMorphoDigCore::instance()->getCamera()->GetViewPlaneNormal(view_look);
	vtkMath::Cross(view_up, view_look, view_right);
	vtkMath::Normalize(view_right);

	// Get the furtherest point from object position+origin
	double outsidept[3];

	outsidept[0] = rot_center[0] + view_right[0] * boundRadius;
	outsidept[1] = rot_center[1] + view_right[1] * boundRadius;
	outsidept[2] = rot_center[2] + view_right[2] * boundRadius;

	// Convert them to display coord
	double disp_obj_center[3];

	
	double radius = sqrt(vtkMath::Distance2BetweenPoints(disp_obj_center,
		outsidept));
	


	if (degrees != 0)
	{
		
		

		double scale[3];
		scale[0] = scale[1] = scale[2] = 1.0;

		double **rotate = new double*[2];

		rotate[0] = new double[4];
		rotate[1] = new double[4];
		
		if (axis == 0)
		{

			rotate[0][0] = degrees;
			rotate[0][1] = view_right[0];
			rotate[0][2] = view_right[1];
			rotate[0][3] = view_right[2];

		}
		else if (axis == 1)
		{
			rotate[0][0] = degrees;
			rotate[0][1] = view_up[0];
			rotate[0][2] = view_up[1];
			rotate[0][3] = view_up[2];

		}
		else
		{

			rotate[0][0] = degrees;
			rotate[0][1] = view_look[0];
			rotate[0][2] = view_look[1];
			rotate[0][3] = view_look[2];
		}
		rotate[1][0] = 0;
		rotate[1][1] = 0;
		rotate[1][2] = 0;
		rotate[1][3] = 1;


		mqMorphoDigCore::instance()->getActorCollection()->InitTraversal();
		for (vtkIdType i = 0; i < mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfItems(); i++)
		{
			vtkMDActor *myActor = vtkMDActor::SafeDownCast(mqMorphoDigCore::instance()->getActorCollection()->GetNextActor());
			vtkProp3D *myPropr = vtkProp3D::SafeDownCast(myActor);
			if (myActor->GetSelected() == 1)
			{
				//cout << "Apply prop3Dtransform" << endl;
				for (vtkIdType j = 0; j < 1; j++)
				{
					for (vtkIdType k = 0; k < 4; k++)
					{
						//cout << "rotate["<<j<<"]"<<"["<<k<<"]="<< rotate[j][k] << endl;

					}
				}

				//cout << "scale:" << scale[0] << ","<< scale[1] << ","<< scale[2] << endl;

				this->Prop3DTransform(myPropr,
					rot_center,
					1,
					rotate,
					scale);
				myActor->SetChanged(1);
			}
		}
		mqMorphoDigCore::instance()->getVolumeCollection()->InitTraversal();
		for (vtkIdType i = 0; i < mqMorphoDigCore::instance()->getVolumeCollection()->GetNumberOfItems(); i++)
		{
			vtkMDVolume *myVolume = vtkMDVolume::SafeDownCast(mqMorphoDigCore::instance()->getVolumeCollection()->GetNextVolume());
			vtkProp3D *myPropr = vtkProp3D::SafeDownCast(myVolume);
			if (myVolume->GetSelected() == 1)
			{
				//cout << "Apply prop3Dtransform" << endl;
				for (vtkIdType j = 0; j < 1; j++)
				{
					for (vtkIdType k = 0; k < 4; k++)
					{
						//cout << "rotate["<<j<<"]"<<"["<<k<<"]="<< rotate[j][k] << endl;

					}
				}

				//cout << "scale:" << scale[0] << ","<< scale[1] << ","<< scale[2] << endl;

				this->Prop3DTransform(myPropr,
					rot_center,
					1,
					rotate,
					scale);
				myVolume->SetChanged(1);
			}
		}
		mqMorphoDigCore::instance()->getNormalLandmarkCollection()->InitTraversal();
		for (vtkIdType i = 0; i < mqMorphoDigCore::instance()->getNormalLandmarkCollection()->GetNumberOfItems(); i++)
		{
			vtkLMActor *myActor = vtkLMActor::SafeDownCast(mqMorphoDigCore::instance()->getNormalLandmarkCollection()->GetNextActor());
			vtkProp3D *myPropr = vtkProp3D::SafeDownCast(myActor);
			if (myActor->GetSelected() == 1)
			{
				this->Prop3DTransform(myPropr,
					rot_center,
					2,
					rotate,
					scale);
				this->ChangeAttachmentPoint(myPropr->GetMatrix(), myActor);
				myActor->SetChanged(1);
			}
		}
		mqMorphoDigCore::instance()->getTargetLandmarkCollection()->InitTraversal();
		for (vtkIdType i = 0; i < mqMorphoDigCore::instance()->getTargetLandmarkCollection()->GetNumberOfItems(); i++)
		{
			vtkLMActor *myActor = vtkLMActor::SafeDownCast(mqMorphoDigCore::instance()->getTargetLandmarkCollection()->GetNextActor());
			vtkProp3D *myPropr = vtkProp3D::SafeDownCast(myActor);
			if (myActor->GetSelected() == 1)
			{
				this->Prop3DTransform(myPropr,
					rot_center,
					2,
					rotate,
					scale);
				this->ChangeAttachmentPoint(myPropr->GetMatrix(), myActor);
				myActor->SetChanged(1);
			}
		}
		mqMorphoDigCore::instance()->getNodeLandmarkCollection()->InitTraversal();
		for (vtkIdType i = 0; i < mqMorphoDigCore::instance()->getNodeLandmarkCollection()->GetNumberOfItems(); i++)
		{
			vtkLMActor *myActor = vtkLMActor::SafeDownCast(mqMorphoDigCore::instance()->getNodeLandmarkCollection()->GetNextActor());
			vtkProp3D *myPropr = vtkProp3D::SafeDownCast(myActor);
			if (myActor->GetSelected() == 1)
			{
				this->Prop3DTransform(myPropr,
					rot_center,
					2,
					rotate,
					scale);
				this->ChangeAttachmentPoint(myPropr->GetMatrix(), myActor);
				myActor->SetChanged(1);
			}
		}
		mqMorphoDigCore::instance()->getHandleLandmarkCollection()->InitTraversal();
		for (vtkIdType i = 0; i < mqMorphoDigCore::instance()->getHandleLandmarkCollection()->GetNumberOfItems(); i++)
		{
			vtkLMActor *myActor = vtkLMActor::SafeDownCast(mqMorphoDigCore::instance()->getHandleLandmarkCollection()->GetNextActor());
			vtkProp3D *myPropr = vtkProp3D::SafeDownCast(myActor);
			if (myActor->GetSelected() == 1)
			{
				this->Prop3DTransform(myPropr,
					rot_center,
					2,
					rotate,
					scale);
				this->ChangeAttachmentPoint(myPropr->GetMatrix(), myActor);
				myActor->SetChanged(1);
			}
		}
		mqMorphoDigCore::instance()->getFlagLandmarkCollection()->InitTraversal();
		for (vtkIdType i = 0; i < mqMorphoDigCore::instance()->getFlagLandmarkCollection()->GetNumberOfItems(); i++)
		{
			vtkLMActor *myActor = vtkLMActor::SafeDownCast(mqMorphoDigCore::instance()->getFlagLandmarkCollection()->GetNextActor());
			vtkProp3D *myPropr = vtkProp3D::SafeDownCast(myActor);
			if (myActor->GetSelected() == 1)
			{
				this->Prop3DTransform(myPropr,
					rot_center,
					2,
					rotate,
					scale);
				this->ChangeAttachmentPoint(myPropr->GetMatrix(), myActor);
				myActor->SetChanged(1);
			}
		}

		delete[] rotate[0];
		delete[] rotate[1];
		delete[] rotate;
		//mqMorphoDigCore::instance()->getRenderer()->GetRenderWindow()->GetInteractor();
		/*if (mqMorphoDigCore::instance()->getRenderer()->inteAutoAdjustCameraClippingRange)
		{
			this->CurrentRenderer->ResetCameraClippingRange();
			mqMorphoDigCore::instance()->ActivateClippingPlane();
		}*/

		mqMorphoDigCore::instance()->Render();
	}
	
}
void mqObjectsControlsWidget::slotDeleteObjects()
{
	mqMorphoDigCore::instance()->DeleteSelectedActors();
	mqMorphoDigCore::instance()->Render();
		
	
}


void mqObjectsControlsWidget::SavePositions(int val)
{

	std::string action = "Action!";
	cout << "State = " << this->State << endl;
	switch (val)
	{
	case 0:
		action = "Rotate selected actors along X viewing axis";
		//cout << "VTK_IS_ROTATE: action=" << action.c_str() << endl;
		break;
	case 1:
		action = "Rotate selected actors along Y viewing axis";
		//cout << "VTK_IS_PAN: action=" << action.c_str() << endl;
		break;
	case 2:
		action = "Rotate selected actors along Z viewing axis";
		//cout << "VTK_IS_SPIN: action=" << action.c_str() << endl;
		break;
	case 3:
		action = "Translate selected actors along X viewing axis";
		//cout << "VTK_IS_SPIN: action=" << action.c_str() << endl;
		break;
	case 4:
		action = "Translate selected actors along Y viewing axis";
		//cout << "VTK_IS_SPIN: action=" << action.c_str() << endl;
		break;
	case 5:
		action = "Translate selected actors along Z viewing axis";
		//cout << "VTK_IS_SPIN: action=" << action.c_str() << endl;
		break;
	}

	int Count = BEGIN_UNDO_SET(action);

	//cout << action.c_str() << endl;
	mqMorphoDigCore::instance()->getActorCollection()->InitTraversal();
	for (vtkIdType i = 0; i < mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfItems(); i++)
	{
		vtkMDActor *myActor = vtkMDActor::SafeDownCast(mqMorphoDigCore::instance()->getActorCollection()->GetNextActor());
		if (myActor->GetSelected() == 1)
		{
			//cout << "Call myActor Save Position with count"<<Count << endl;
			myActor->SaveState(Count);
		}
	}
	mqMorphoDigCore::instance()->getVolumeCollection()->InitTraversal();
	for (vtkIdType i = 0; i < mqMorphoDigCore::instance()->getVolumeCollection()->GetNumberOfItems(); i++)
	{
		vtkMDVolume *myVolume = vtkMDVolume::SafeDownCast(mqMorphoDigCore::instance()->getVolumeCollection()->GetNextVolume());
		if (myVolume->GetSelected() == 1)
		{
			//cout << "Call myActor Save Position with count"<<Count << endl;
			myVolume->SaveState(Count);
		}
	}
	mqMorphoDigCore::instance()->getNormalLandmarkCollection()->InitTraversal();
	for (vtkIdType i = 0; i < mqMorphoDigCore::instance()->getNormalLandmarkCollection()->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(mqMorphoDigCore::instance()->getNormalLandmarkCollection()->GetNextActor());
		if (myActor->GetSelected() == 1)
		{
		//	cout << "Call myLMActor Save Position with count" << Count << endl;
			myActor->SaveState(Count);
		}
	}
	mqMorphoDigCore::instance()->getTargetLandmarkCollection()->InitTraversal();
	for (vtkIdType i = 0; i < mqMorphoDigCore::instance()->getTargetLandmarkCollection()->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(mqMorphoDigCore::instance()->getTargetLandmarkCollection()->GetNextActor());
		if (myActor->GetSelected() == 1)
		{
		//	cout << "Call myLMActor Save Position with count" << Count << endl;
			myActor->SaveState(Count);
		}
	}
	mqMorphoDigCore::instance()->getNodeLandmarkCollection()->InitTraversal();
	for (vtkIdType i = 0; i < mqMorphoDigCore::instance()->getNodeLandmarkCollection()->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(mqMorphoDigCore::instance()->getNodeLandmarkCollection()->GetNextActor());
		if (myActor->GetSelected() == 1)
		{
		//	cout << "Call myLMActor Save Position with count" << Count << endl;
			myActor->SaveState(Count);
		}
	}
	mqMorphoDigCore::instance()->getHandleLandmarkCollection()->InitTraversal();
	for (vtkIdType i = 0; i < mqMorphoDigCore::instance()->getHandleLandmarkCollection()->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(mqMorphoDigCore::instance()->getHandleLandmarkCollection()->GetNextActor());
		if (myActor->GetSelected() == 1)
		{
		//	cout << "Call myLMActor Save Position with count" << Count << endl;
			myActor->SaveState(Count);
		}
	}

	mqMorphoDigCore::instance()->getFlagLandmarkCollection()->InitTraversal();
	for (vtkIdType i = 0; i < mqMorphoDigCore::instance()->getFlagLandmarkCollection()->GetNumberOfItems(); i++)
	{
		vtkLMActor *myActor = vtkLMActor::SafeDownCast(mqMorphoDigCore::instance()->getFlagLandmarkCollection()->GetNextActor());
		if (myActor->GetSelected() == 1)
		{
			//cout << "Call myLMActor Save Position with count" << Count << endl;
			myActor->SaveState(Count);
		}
	}
	//vtkMorphoDigCore::instance()->getUndoStack()->

	END_UNDO_SET();

}

void mqObjectsControlsWidget::slotZrotPressed()
{
	this->SavePositions(2);
}

void mqObjectsControlsWidget::slotYrotPressed()
{
	this->SavePositions(1);
}

void mqObjectsControlsWidget::slotXrotPressed()
{
	this->SavePositions(0);
}



void mqObjectsControlsWidget::slotZtrPressed()
{
	this->SavePositions(5);
}

void mqObjectsControlsWidget::slotYtrPressed()
{
	this->SavePositions(4);
}

void mqObjectsControlsWidget::slotXtrPressed()
{
	this->SavePositions(3);
}

void mqObjectsControlsWidget::slotZtr(int val)
{
	if (val == 0) {
		this->oldtrval = 0;
		return;
	}
	this->PanActors(2,   val- this->oldtrval);
	this->oldtrval = val;
	//cout << "hereZ!" << endl;
}

void mqObjectsControlsWidget::slotYtr(int val)
{
	if (val == 0) {
		this->oldtrval = 0;
		return;
	}
	this->PanActors(1,  this->oldtrval-val);
	this->oldtrval = val;
	//cout << "hereY!" << endl;
}

void mqObjectsControlsWidget::slotXtr(int val)
{
	if (val == 0) {
		this->oldtrval = 0;
		return;
	}
	this->PanActors(0, this->oldtrval-val);
	this->oldtrval = val;
	//cout << "hereX!" << endl;
}

void mqObjectsControlsWidget::slotZrot(int val)
{	
	if (val == 0) {
		this->oldrotval = 0;
		// here the actors should be updated for good!
		return; }
	this->RotateActors(2, val-this->oldrotval);
	this->oldrotval = val;
}
void mqObjectsControlsWidget::slotXrot(int val)
{	
	if (val == 0) {
		this->oldrotval = 0;
		// here the actors should be updated for good!
		return;
	}

	this->RotateActors(0, this->oldrotval-val);
	this->oldrotval = val;

}
void mqObjectsControlsWidget::slotYrot(int val)
{
	if (val == 0) {
		this->oldrotval = 0;
		// here the actors should be updated for good!
		return;
	}

	this->RotateActors(1, val-this->oldrotval);
	this->oldrotval = val;

}
void mqObjectsControlsWidget::Prop3DTransform(vtkProp3D *prop3D,
	double *boxCenter,
	int numRotation,
	double **rotate,
	double *scale)
{
	vtkMatrix4x4 *oldMatrix = vtkMatrix4x4::New();
	prop3D->GetMatrix(oldMatrix);

	double orig[3];
	prop3D->GetOrigin(orig);

	vtkTransform *newTransform = vtkTransform::New();
	newTransform->PostMultiply();
	if (prop3D->GetUserMatrix() != NULL)
	{
		newTransform->SetMatrix(prop3D->GetUserMatrix());
	}
	else
	{
		newTransform->SetMatrix(oldMatrix);
	}

	newTransform->Translate(-(boxCenter[0]), -(boxCenter[1]), -(boxCenter[2]));

	for (int i = 0; i < numRotation; i++)
	{
		newTransform->RotateWXYZ(rotate[i][0], rotate[i][1],
			rotate[i][2], rotate[i][3]);
	}

	if ((scale[0] * scale[1] * scale[2]) != 0.0)
	{
		newTransform->Scale(scale[0], scale[1], scale[2]);
	}

	newTransform->Translate(boxCenter[0], boxCenter[1], boxCenter[2]);

	// now try to get the composit of translate, rotate, and scale
	newTransform->Translate(-(orig[0]), -(orig[1]), -(orig[2]));
	newTransform->PreMultiply();
	newTransform->Translate(orig[0], orig[1], orig[2]);

	if (prop3D->GetUserMatrix() != NULL)
	{
		// apply new transform to prop3D
		newTransform->GetMatrix(prop3D->GetUserMatrix());
	}
	else
	{
		prop3D->SetPosition(newTransform->GetPosition());
		prop3D->SetScale(newTransform->GetScale());
		prop3D->SetOrientation(newTransform->GetOrientation());
	}
	oldMatrix->Delete();
	newTransform->Delete();
}
void mqObjectsControlsWidget::ChangeAttachmentPoint(vtkMatrix4x4 *NewMat, vtkLMActor *LMActor)
{
	vtkBillboardTextActor3D *LMLabel = LMActor->GetLMLabelActor3D();
	double init_pos = 1;
	double mult = 1;
	if (LMActor->GetLMType() == TARGET_LMK || LMActor->GetLMType() == HANDLE_LMK)
	{
		mult = 1.1;
	}
	if (LMActor->GetLMBodyType() == SPHERE)
	{
		init_pos = 0.5*1.1*mult*LMActor->GetLMSize();
	}
	else
	{
		if (LMActor->GetLMType() == FLAG_LMK)
		{
			init_pos = 1.1*mult*LMActor->GetLMSize();
		}
		else
		{
			init_pos = 3 * 1.1*mult*LMActor->GetLMSize();
		}
	}

	double ap[3] = { init_pos,
		0,
		0 };

	double apt[3];

	this->TransformPoint(NewMat, ap, apt);
	LMLabel->SetPosition(apt);
}
void mqObjectsControlsWidget::TransformPoint(vtkMatrix4x4* matrix, double pointin[3], double pointout[3]) {
	double pointPred[4]; double pointNew[4] = { 0, 0, 0, 0 };
	pointPred[0] = pointin[0];
	pointPred[1] = pointin[1];
	pointPred[2] = pointin[2];
	pointPred[3] = 1;

	matrix->MultiplyPoint(pointPred, pointNew);
	pointout[0] = pointNew[0];
	pointout[1] = pointNew[1];
	pointout[2] = pointNew[2];
}