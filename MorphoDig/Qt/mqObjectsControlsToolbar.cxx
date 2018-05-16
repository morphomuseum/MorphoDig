/*=========================================================================

   Program: MorphoDig
   Module:    Copied from Paraview pqMainControlsToolbar.cxx

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
#include "mqObjectsControlsToolbar.h"
#include "ui_mqObjectsControlsToolbar.h"

// For later!
#include "QReleaseSlider.h"
//#include "QReleaseSliderValue.h"

#include "mqEditLMKDialogReaction.h"
#include "mqCreateLMKDialogReaction.h"
#include "mqEditFLGDialogReaction.h"
#include "mqEditACTORDialogReaction.h"

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
#define NORMAL_LMK 0
#define TARGET_LMK 1
#define NODE_LMK 2
#define HANDLE_LMK 3
#define FLAG_LMK 4
#define SPHERE 0
#define ARROW 1


//-----------------------------------------------------------------------------
void mqObjectsControlsToolbar::constructor()
{
 // Ui::mqObjectsControlsToolbar ui;
 // ui.setupUi(this);
  this->ui = new Ui_mqObjectsControlsToolbar;
  
  
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
  this->zTr = new QReleaseSlider;
  this->zTr->setMaximum(100);
  this->zTr->setMinimum(-100);
  this->zTr->setToolTip(QString("Translate along z viewing axis"));


  //this->zRot = new QReleaseSliderValue(Qt::Vertical, tr(""));
  this->zRot = new QReleaseSlider;
  this->zRot->setMaximum(90);
  this->zRot->setMinimum(-90);
  this->zRot->setToolTip(QString("Rotation along z viewing axis"));


  this->yTr = new QReleaseSlider;
  this->yTr->setMaximum(100);
  this->yTr->setMinimum(-100);
  this->yTr->setToolTip(QString("Translate along y viewing axis"));

  //this->yRot = new QReleaseSliderValue(Qt::Vertical, tr(""));
  this->yRot = new QReleaseSlider;
  this->yRot->setMaximum(90);
  this->yRot->setMinimum(-90);
  this->yRot->setToolTip(QString("Rotation along y viewing axis"));

  
  this->xTr = new QReleaseSlider;
  this->xTr->setMaximum(100);
  this->xTr->setMinimum(-100);
  this->xTr->setToolTip(QString("Translate along x viewing axis"));

  //this->xRot = new QReleaseSliderValue(Qt::Vertical, tr(""));
  this->xRot = new QReleaseSlider;
  this->xRot->setMaximum(90);
  this->xRot->setMinimum(-90);
  this->xRot->setToolTip(QString("Rotation along x viewing axis"));

  QHBoxLayout *zlayout = new QHBoxLayout;
  QWidget* zgrid = new QWidget();
  zlayout->setSpacing(1);
  zlayout->setMargin(5);
  zlayout->addWidget(this->zRot);
 // zlayout->addWidget(this->xTr);
  //zlayout->addWidget(this->yTr);
  zlayout->addWidget(this->zTr);

  zgrid->setLayout(zlayout);
  this->addWidget(zgrid);

  QHBoxLayout *ylayout = new QHBoxLayout;
  QWidget* ygrid = new QWidget();
 ylayout->setSpacing(1);
  ylayout->setMargin(5);
  ylayout->addWidget(this->yRot);
  ylayout->addWidget(this->yTr);

  ygrid->setLayout(ylayout);
  this->addWidget(ygrid);

  QHBoxLayout *xlayout = new QHBoxLayout;
  QWidget* xgrid = new QWidget();
  xlayout->setSpacing(1);
  xlayout->setMargin(5);
 xlayout->addWidget(this->xRot);
 xlayout->addWidget(this->xTr);

  xgrid->setLayout(xlayout);
  this->addWidget(xgrid);

  connect(this->ui->actionDelete, SIGNAL(triggered()), this, SLOT(slotDeleteObjects()));
  connect(this->ui->actionDelete, SIGNAL(triggered()), this, SLOT(slotDeleteObjects()));
  connect(this->ui->actioLandmarkMoveUp, SIGNAL(triggered()), mqMorphoDigCore::instance(), SLOT(slotLandmarkMoveUp()));
  connect(this->ui->actioLandmarkMoveDown, SIGNAL(triggered()), mqMorphoDigCore::instance(), SLOT(slotLandmarkMoveDown()));

  connect(zRot, SIGNAL(valueChanged(int)), this, SLOT(slotZrot(int)));
  connect(yRot, SIGNAL(valueChanged(int)), this, SLOT(slotYrot(int)));
  connect(xRot, SIGNAL(valueChanged(int)), this, SLOT(slotXrot(int)));
  
  connect(zTr, SIGNAL(valueChanged(int)), this, SLOT(slotZtr(int)));
  connect(yTr, SIGNAL(valueChanged(int)), this, SLOT(slotYtr(int)));
  connect(xTr, SIGNAL(valueChanged(int)), this, SLOT(slotXtr(int)));

  connect(zRot, SIGNAL(sliderPressed()), this, SLOT(slotZrotPressed()));
  connect(yRot, SIGNAL(sliderPressed()), this, SLOT(slotYrotPressed()));
  connect(xRot, SIGNAL(sliderPressed()), this, SLOT(slotXrotPressed()));

  connect(zTr, SIGNAL(sliderPressed()), this, SLOT(slotZtrPressed()));
  connect(yTr, SIGNAL(sliderPressed()), this, SLOT(slotYtrPressed()));
  connect(xTr, SIGNAL(sliderPressed()), this, SLOT(slotXtrPressed()));
  
  connect(this->ui->actionLassoCutKeepInside, SIGNAL(triggered()), mqMorphoDigCore::instance(), SLOT(slotLassoCutKeepInside()));
  connect(this->ui->actionLassoCutKeepOutside, SIGNAL(triggered()), mqMorphoDigCore::instance(), SLOT(slotLassoCutKeepOutside()));

  new mqEditLMKDialogReaction(this->ui->actionEditLandmarks);
  new mqCreateLMKDialogReaction(this->ui->actionCreateLandmark);
  new mqEditFLGDialogReaction(this->ui->actionEditFlags);
  new mqEditACTORDialogReaction(this->ui->actionEditActors);
 
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



void mqObjectsControlsToolbar::PanActors(int axis, int value)
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


		double origin[4] = { 0, 0, 1,1 };
		double away[4] = { 0, 0, 2,1 };

	//	cout << "try  DTW" << endl;
		
		//cout << "try  DTW2" << endl;
		int move = 10;
		mqMorphoDigCore::instance()->GetWorldToDisplay(pan_center[0], pan_center[1], pan_center[2], dPanCenter);
		mqMorphoDigCore::instance()->GetDisplayToWorld(dPanCenter[0], dPanCenter[1], dPanCenter[2], origin);
		mqMorphoDigCore::instance()->GetDisplayToWorld(dPanCenter[0], dPanCenter[1]+30, dPanCenter[2], away); //example : 10px away from origin!
		
		// origin[2] -away[2] will give the amplitude of the movement. Now, let us compute the direction!
		double ampli = value*(origin[2] - away[2]) / 12;
		double view_up[3], view_look[3], view_right[3];

		mqMorphoDigCore::instance()->getCamera()->OrthogonalizeViewUp();
		mqMorphoDigCore::instance()->getCamera()->ComputeViewPlaneNormal();
		mqMorphoDigCore::instance()->getCamera()->GetViewUp(view_up);
		vtkMath::Normalize(view_up);
		mqMorphoDigCore::instance()->getCamera()->GetViewPlaneNormal(view_look);
		vtkMath::Cross(view_up, view_look, view_right);
		vtkMath::Normalize(view_right);
		vtkMath::Normalize(view_look);
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

void mqObjectsControlsToolbar::RotateActors(int axis, int degrees)
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

	/*this->ComputeWorldToDisplay(rot_center[0], rot_center[1], rot_center[2],
		disp_obj_center);

	this->ComputeWorldToDisplay(outsidept[0], outsidept[1], outsidept[2],
		outsidept);
		*/
	double radius = sqrt(vtkMath::Distance2BetweenPoints(disp_obj_center,
		outsidept));
	/*double nxf = (rwi->GetEventPosition()[0] - disp_obj_center[0]) / radius;

	double nyf = (rwi->GetEventPosition()[1] - disp_obj_center[1]) / radius;

	double oxf = (rwi->GetLastEventPosition()[0] - disp_obj_center[0]) / radius;

	double oyf = (rwi->GetLastEventPosition()[1] - disp_obj_center[1]) / radius;*/


	if (degrees != 0)
	{
		double newXAngle = 0;
		if (axis == 0)
		{
			newXAngle = degrees;
		}

		double newYAngle = 0; 
		if (axis == 0)
		{
			newYAngle = degrees;
		}

		double newZAngle = 0;
		if (axis == 0)
		{
			newZAngle = degrees;
		}
		

		double scale[3];
		scale[0] = scale[1] = scale[2] = 1.0;

		double **rotate = new double*[2];

		rotate[0] = new double[4];
		rotate[1] = new double[4];
		
		if (axis == 0)
		{
			rotate[0][0] = degrees;
			rotate[0][1] = view_up[0];
			rotate[0][2] = view_up[1];
			rotate[0][3] = view_up[2];
		}
		else if (axis == 1)
		{

			rotate[0][0] = degrees;
			rotate[0][1] = view_right[0];
			rotate[0][2] = view_right[1];
			rotate[0][3] = view_right[2];
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
void mqObjectsControlsToolbar::slotDeleteObjects()
{
	mqMorphoDigCore::instance()->DeleteSelectedActors();
	mqMorphoDigCore::instance()->Render();
		
	
}


void mqObjectsControlsToolbar::SavePositions(int val)
{

	std::string action = "Action!";
	//cout << "State = " << this->State << endl;
	switch (val)
	{
	case 0:
		action = "Rotate selected actors along X";
		//cout << "VTK_IS_ROTATE: action=" << action.c_str() << endl;
		break;
	case 1:
		action = "Rotate selected actors along Y";
		//cout << "VTK_IS_PAN: action=" << action.c_str() << endl;
		break;
	case 2:
		action = "Rotate selected actors along Z";
		//cout << "VTK_IS_SPIN: action=" << action.c_str() << endl;
		break;
	case 3:
		action = "Translate selected actors along X";
		//cout << "VTK_IS_SPIN: action=" << action.c_str() << endl;
		break;
	case 4:
		action = "Translate selected actors along Y";
		//cout << "VTK_IS_SPIN: action=" << action.c_str() << endl;
		break;
	case 5:
		action = "Translate selected actors along Z";
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

void mqObjectsControlsToolbar::slotZrotPressed()
{
	this->SavePositions(2);
}

void mqObjectsControlsToolbar::slotYrotPressed()
{
	this->SavePositions(1);
}

void mqObjectsControlsToolbar::slotXrotPressed()
{
	this->SavePositions(0);
}



void mqObjectsControlsToolbar::slotZtrPressed()
{
	this->SavePositions(5);
}

void mqObjectsControlsToolbar::slotYtrPressed()
{
	this->SavePositions(4);
}

void mqObjectsControlsToolbar::slotXtrPressed()
{
	this->SavePositions(3);
}

void mqObjectsControlsToolbar::slotZtr(int val)
{
	if (val == 0) {
		this->oldtrval = 0;
		return;
	}
	this->PanActors(2,   val- this->oldtrval);
	this->oldtrval = val;
	//cout << "hereZ!" << endl;
}

void mqObjectsControlsToolbar::slotYtr(int val)
{
	if (val == 0) {
		this->oldtrval = 0;
		return;
	}
	this->PanActors(1,  this->oldtrval-val);
	this->oldtrval = val;
	//cout << "hereY!" << endl;
}

void mqObjectsControlsToolbar::slotXtr(int val)
{
	if (val == 0) {
		this->oldtrval = 0;
		return;
	}
	this->PanActors(0, this->oldtrval-val);
	this->oldtrval = val;
	cout << "hereX!" << endl;
}

void mqObjectsControlsToolbar::slotZrot(int val)
{	
	if (val == 0) {
		this->oldrotval = 0;
		// here the actors should be updated for good!
		return; }
	this->RotateActors(2, val-this->oldrotval);
	this->oldrotval = val;
}
void mqObjectsControlsToolbar::slotXrot(int val)
{	
	if (val == 0) {
		this->oldrotval = 0;
		// here the actors should be updated for good!
		return;
	}

	this->RotateActors(0, val- this->oldrotval);
	this->oldrotval = val;

}
void mqObjectsControlsToolbar::slotYrot(int val)
{
	if (val == 0) {
		this->oldrotval = 0;
		// here the actors should be updated for good!
		return;
	}

	this->RotateActors(1, this->oldrotval-val);
	this->oldrotval = val;

}
void mqObjectsControlsToolbar::Prop3DTransform(vtkProp3D *prop3D,
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
void mqObjectsControlsToolbar::ChangeAttachmentPoint(vtkMatrix4x4 *NewMat, vtkLMActor *LMActor)
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
void mqObjectsControlsToolbar::TransformPoint(vtkMatrix4x4* matrix, double pointin[3], double pointout[3]) {
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