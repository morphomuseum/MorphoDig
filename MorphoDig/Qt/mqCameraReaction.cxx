/*=========================================================================

   Program: MorphoDig
   Module:    mqCameraReaction.cxx

========================================================================*/
#include "mqCameraReaction.h"

#include "mqMorphoDigCore.h"
#include <vtkRenderer.h>



//-----------------------------------------------------------------------------
mqCameraReaction::mqCameraReaction(QAction* parentObject,  int _mode)
  : Superclass(parentObject)
{
  this->Mode = _mode;
  this->MainWindow = mqMorphoDigCore::instance()->GetMainWindow();
  
}

//@@TODO
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void mqCameraReaction::CameraFront()
{
	//@@TODO
	cout << "Camera Front!" << endl;
	double cameracentre[3] = { 0, 0, 0 };
	if (mqMorphoDigCore::instance()->Getmui_CameraCentreOfMassAtOrigin() == 0)
	{
		mqMorphoDigCore::instance()->getActorCollection()->GetCenterOfMass(cameracentre);
		mqMorphoDigCore::instance()->getGridActor()->SetGridOrigin(cameracentre);
	}
	double multfactor = 1 / tan(mqMorphoDigCore::instance()->getCamera()->GetViewAngle() *  vtkMath::Pi() / 360.0);
	// = 3.73 when viewing angle = 30°
	double camdist = multfactor*mqMorphoDigCore::instance()->getCamera()->GetParallelScale();
	cout << "Camdist=" <<camdist<< endl;
	mqMorphoDigCore::instance()->getCamera()->SetPosition(camdist + cameracentre[0], cameracentre[1], cameracentre[2]);
	mqMorphoDigCore::instance()->getCamera()->SetFocalPoint(cameracentre);
	mqMorphoDigCore::instance()->getCamera()->SetViewUp(0, 0, 1);
	mqMorphoDigCore::instance()->getRenderer()->ResetCameraClippingRange();
	mqMorphoDigCore::instance()->ActivateClippingPlane();
	//this->ReplaceCamera();

	mqMorphoDigCore::instance()->getGridActor()->SetGridOrigin(cameracentre);
	mqMorphoDigCore::instance()->getGridActor()->SetOutlineMode(mqMorphoDigCore::instance()->Getmui_CameraCentreOfMassAtOrigin());

	mqMorphoDigCore::instance()->getGridActor()->SetGridType(2);
	mqMorphoDigCore::instance()->Render();

	
}

void mqCameraReaction::CameraBack() {
	cout << "mqMorphoDigCore::instance()->Getmui_CameraCentreOfMassAtOrigin()=" << mqMorphoDigCore::instance()->Getmui_CameraCentreOfMassAtOrigin() << endl;
	//this->ReplaceCamera();
	double cameracentre[3] = { 0, 0, 0 };
	if (mqMorphoDigCore::instance()->Getmui_CameraCentreOfMassAtOrigin() == 0)
	{
		mqMorphoDigCore::instance()->getActorCollection()->GetCenterOfMass(cameracentre);
		mqMorphoDigCore::instance()->getGridActor()->SetGridOrigin(cameracentre);
	}
	double multfactor = 1 / tan(mqMorphoDigCore::instance()->getCamera()->GetViewAngle() *  vtkMath::Pi() / 360.0);
	// = 3.73 when viewing angle = 30°
	double camdist = multfactor*mqMorphoDigCore::instance()->getCamera()->GetParallelScale();
	mqMorphoDigCore::instance()->getCamera()->SetPosition(-camdist + cameracentre[0], cameracentre[1], cameracentre[2]);
	mqMorphoDigCore::instance()->getCamera()->SetFocalPoint(cameracentre);
	mqMorphoDigCore::instance()->getCamera()->SetViewUp(0, 0, 1);
	mqMorphoDigCore::instance()->getRenderer()->ResetCameraClippingRange();
	mqMorphoDigCore::instance()->ActivateClippingPlane();
	mqMorphoDigCore::instance()->getGridActor()->SetGridOrigin(cameracentre);
	mqMorphoDigCore::instance()->getGridActor()->SetOutlineMode(mqMorphoDigCore::instance()->Getmui_CameraCentreOfMassAtOrigin());
	mqMorphoDigCore::instance()->getGridActor()->SetGridType(2);
	mqMorphoDigCore::instance()->Render();
}//1
void mqCameraReaction::CameraLeft() {
	cout << "mqMorphoDigCore::instance()->Getmui_CameraCentreOfMassAtOrigin()=" << mqMorphoDigCore::instance()->Getmui_CameraCentreOfMassAtOrigin() << endl;
	//this->ReplaceCamera
	double cameracentre[3] = { 0, 0, 0 };
	if (mqMorphoDigCore::instance()->Getmui_CameraCentreOfMassAtOrigin() == 0)
	{
		mqMorphoDigCore::instance()->getActorCollection()->GetCenterOfMass(cameracentre);
		mqMorphoDigCore::instance()->getGridActor()->SetGridOrigin(cameracentre);
	}
	double multfactor = 1 / tan(mqMorphoDigCore::instance()->getCamera()->GetViewAngle() *  vtkMath::Pi() / 360.0);
	// = 3.73 when viewing angle = 30°
	double camdist = multfactor*mqMorphoDigCore::instance()->getCamera()->GetParallelScale();
	mqMorphoDigCore::instance()->getCamera()->SetPosition(cameracentre[0], camdist + cameracentre[1], cameracentre[2]);
	mqMorphoDigCore::instance()->getCamera()->SetFocalPoint(cameracentre);
	mqMorphoDigCore::instance()->getRenderer()->ResetCameraClippingRange();
	mqMorphoDigCore::instance()->ActivateClippingPlane();

	mqMorphoDigCore::instance()->getCamera()->SetViewUp(0, 0, 1);
	mqMorphoDigCore::instance()->getGridActor()->SetGridOrigin(cameracentre);
	mqMorphoDigCore::instance()->getGridActor()->SetOutlineMode(mqMorphoDigCore::instance()->Getmui_CameraCentreOfMassAtOrigin());
	mqMorphoDigCore::instance()->getGridActor()->SetGridType(1);
	mqMorphoDigCore::instance()->Render();
}//2
void mqCameraReaction::CameraRight() 
{
	cout << "mqMorphoDigCore::instance()->Getmui_CameraCentreOfMassAtOrigin()=" << mqMorphoDigCore::instance()->Getmui_CameraCentreOfMassAtOrigin() << endl;
	//this->ReplaceCamera();
	double cameracentre[3] = { 0, 0, 0 };
	if (mqMorphoDigCore::instance()->Getmui_CameraCentreOfMassAtOrigin() == 0)
	{
		mqMorphoDigCore::instance()->getActorCollection()->GetCenterOfMass(cameracentre);
		mqMorphoDigCore::instance()->getGridActor()->SetGridOrigin(cameracentre);
	}
	double multfactor = 1 / tan(mqMorphoDigCore::instance()->getCamera()->GetViewAngle() *  vtkMath::Pi() / 360.0);
	// = 3.73 when viewing angle = 30°
	double camdist = multfactor*mqMorphoDigCore::instance()->getCamera()->GetParallelScale();
	mqMorphoDigCore::instance()->getCamera()->SetPosition(cameracentre[0], -camdist + cameracentre[1], cameracentre[2]);
	mqMorphoDigCore::instance()->getCamera()->SetFocalPoint(cameracentre);
	mqMorphoDigCore::instance()->getCamera()->SetViewUp(0, 0, 1);
	mqMorphoDigCore::instance()->getRenderer()->ResetCameraClippingRange();
	mqMorphoDigCore::instance()->ActivateClippingPlane();

	mqMorphoDigCore::instance()->getGridActor()->SetGridOrigin(cameracentre);
	mqMorphoDigCore::instance()->getGridActor()->SetOutlineMode(mqMorphoDigCore::instance()->Getmui_CameraCentreOfMassAtOrigin());
	mqMorphoDigCore::instance()->getGridActor()->SetGridType(1);
	mqMorphoDigCore::instance()->Render();
}//3
void mqCameraReaction::CameraAbove() {
	cout << "mqMorphoDigCore::instance()->Getmui_CameraCentreOfMassAtOrigin()=" << mqMorphoDigCore::instance()->Getmui_CameraCentreOfMassAtOrigin() << endl;
	//this->ReplaceCamera();
	double cameracentre[3] = { 0, 0, 0 };
	if (mqMorphoDigCore::instance()->Getmui_CameraCentreOfMassAtOrigin() == 0)
	{
		mqMorphoDigCore::instance()->getActorCollection()->GetCenterOfMass(cameracentre);
		mqMorphoDigCore::instance()->getGridActor()->SetGridOrigin(cameracentre);
	}
	double multfactor = 1 / tan(mqMorphoDigCore::instance()->getCamera()->GetViewAngle() *  vtkMath::Pi() / 360.0);
	// = 3.73 when viewing angle = 30°
	double camdist = multfactor*mqMorphoDigCore::instance()->getCamera()->GetParallelScale();
	mqMorphoDigCore::instance()->getCamera()->SetPosition(cameracentre[0], cameracentre[1], camdist + cameracentre[2]);
	mqMorphoDigCore::instance()->getCamera()->SetFocalPoint(cameracentre);


	mqMorphoDigCore::instance()->getCamera()->SetViewUp(-1, 0, 0);
	mqMorphoDigCore::instance()->getRenderer()->ResetCameraClippingRange();
	mqMorphoDigCore::instance()->ActivateClippingPlane();
	mqMorphoDigCore::instance()->getGridActor()->SetGridOrigin(cameracentre);
	mqMorphoDigCore::instance()->getGridActor()->SetOutlineMode(mqMorphoDigCore::instance()->Getmui_CameraCentreOfMassAtOrigin());
	mqMorphoDigCore::instance()->getGridActor()->SetGridType(0);
	mqMorphoDigCore::instance()->Render();
}//4
void mqCameraReaction::CameraBelow() {
	cout << "mqMorphoDigCore::instance()->Getmui_CameraCentreOfMassAtOrigin()=" << mqMorphoDigCore::instance()->Getmui_CameraCentreOfMassAtOrigin() << endl;
	double cameracentre[3] = { 0, 0, 0 };
	if (mqMorphoDigCore::instance()->Getmui_CameraCentreOfMassAtOrigin() == 0)
	{
		mqMorphoDigCore::instance()->getActorCollection()->GetCenterOfMass(cameracentre);
		mqMorphoDigCore::instance()->getGridActor()->SetGridOrigin(cameracentre);
	}
	double multfactor = 1 / tan(mqMorphoDigCore::instance()->getCamera()->GetViewAngle() *  vtkMath::Pi() / 360.0);
	// = 3.73 when viewing angle = 30°
	double camdist = multfactor*mqMorphoDigCore::instance()->getCamera()->GetParallelScale();
	mqMorphoDigCore::instance()->getCamera()->SetPosition(cameracentre[0], cameracentre[1], -camdist + cameracentre[2]);
	mqMorphoDigCore::instance()->getCamera()->SetFocalPoint(cameracentre);
	mqMorphoDigCore::instance()->getCamera()->SetViewUp(1, 0, 0);
	mqMorphoDigCore::instance()->getRenderer()->ResetCameraClippingRange();
	mqMorphoDigCore::instance()->ActivateClippingPlane();
	//mqMorphoDigCore::instance()->getCamera()->SetParallelScale(120);
	mqMorphoDigCore::instance()->getGridActor()->SetGridOrigin(cameracentre);
	mqMorphoDigCore::instance()->getGridActor()->SetOutlineMode(mqMorphoDigCore::instance()->Getmui_CameraCentreOfMassAtOrigin());
	mqMorphoDigCore::instance()->getGridActor()->SetGridType(0);
	mqMorphoDigCore::instance()->Render();
}//5


void mqCameraReaction::CameraCentreOfMassToggle() {
	if (mqMorphoDigCore::instance()->Getmui_CameraCentreOfMassAtOrigin() == 1)
	{
		mqMorphoDigCore::instance()->Setmui_CameraCentreOfMassAtOrigin(0);
	}
	else
	{
		mqMorphoDigCore::instance()->Setmui_CameraCentreOfMassAtOrigin(1);
	}

	mqMorphoDigCore::instance()->ReplaceCameraAndGrid();

}
void mqCameraReaction::CameraOrthoPerspectiveToggle() {
	if (mqMorphoDigCore::instance()->Getmui_CameraOrtho() == 1)
	{
		mqMorphoDigCore::instance()->Setmui_CameraOrtho(0);
	}
	else
	{
		mqMorphoDigCore::instance()->Setmui_CameraOrtho(1);

	}

	mqMorphoDigCore::instance()->signal_projectionModeChanged(); 
	mqMorphoDigCore::instance()->ResetCameraOrthoPerspective();
	
}

