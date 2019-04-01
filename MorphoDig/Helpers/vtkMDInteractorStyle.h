/*=========================================================================

  Program:   MorphoDig

  Copied from:    vtkInteractorStyleRubberBandPick.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/


#ifndef vtkMDInteractorStyle_h
#define vtkMDInteractorStyle_h
#include "vtkMDActorCollection.h"
#include "vtkLMActorCollection.h"

#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkSmartPointer.h>


class vtkUnsignedCharArray;

class  vtkMDInteractorStyle : public vtkInteractorStyleTrackballCamera
{
public:
  static vtkMDInteractorStyle *New();
  vtkTypeMacro(vtkMDInteractorStyle, vtkInteractorStyleTrackballCamera);
  void PrintSelf(ostream& os, vtkIndent indent);

  /*void StartSelect();
  void StartTagPencil();*/

  //@{
  /**
   * Event bindings
   */
  virtual void OnMouseMove();
  virtual void OnLeftButtonDown();
  virtual void OnLeftButtonUp();
  virtual void OnRightButtonDown();
  virtual void OnRightButtonUp();
  virtual void OnMiddleButtonDown();
  virtual void OnMiddleButtonUp();
  virtual void OnChar();
  virtual void RubberStart();
  virtual void RubberStop();
  virtual void TagPencilStart();
  virtual void TagPencilStop();
  virtual void Tag(int mode);
  virtual void OnKeyPress();
  virtual void OnKeyRelease();
  virtual void EndRotate();
  virtual void EndSpin();
  virtual void EndPan();
 
  //@}
  //virtual void SaveSelectedActorPositions();
  virtual void RotateActors();
  virtual void SpinActors();
  virtual void PanActors();
  void EndLandmarkMovements(vtkLMActor *myActor);
  void EndLandmarkMovements();
  void SetActorCollection(vtkSmartPointer<vtkMDActorCollection>ActColl);
  void SetNormalLandmarkCollection(vtkSmartPointer<vtkLMActorCollection>LmkColl);
  void SetTargetLandmarkCollection(vtkSmartPointer<vtkLMActorCollection>LmkColl);
  void SetNodeLandmarkCollection(vtkSmartPointer<vtkLMActorCollection>LmkColl);
  void SetHandleLandmarkCollection(vtkSmartPointer<vtkLMActorCollection>LmkColl);
  void SetFlagLandmarkCollection(vtkSmartPointer<vtkLMActorCollection>LmkColl);

protected:
  
  vtkMDInteractorStyle();
  ~vtkMDInteractorStyle();
  virtual void Dolly(double factor);
  void Prop3DTransform(vtkProp3D *prop3D,

	  double *boxCenter,
	  int NumRotation,
	  double **rotate,
	  double *scale);
  //void AttachmentPointTransform(double *ap, vtkProp3D *prop3D, double apt[3]);
/*void  AttachmentPointTransform(double* attachmentPoint, double *boxCenter,
  
	  int numRotation,
	  double **rotate,
	  double *scale, 
	double newAttachmentPoint[3]);*/
  virtual void Pick();
  void RedrawRubberBand();
  void RedrawTagPencilCircle();
  int StartPosition[2];
  int TagPencilCenterPosition[2];
  int TagPencilStarted;
  int EndPosition[2];
  int MoveWhat;
  int Moving;
  int ActorsPositionsSaved;
  void SaveSelectedActorsPositions();
  void DeleteSelectedActors();
  int NumberOfSelectedActors;
  int getNumberOfSelectedActors();
  double GetBoundingBoxLengthOfSelectedActors();
  void GetCenterOfMassOfSelectedActors(double com[3]);
  vtkUnsignedCharArray *PixelArray;
  vtkUnsignedCharArray *PixelArray2;
  void ChangeAttachmentPoint(vtkMatrix4x4 *NewMat, vtkLMActor *LMActor);
  void ResetMoveWhat();
  int CurrentMode;
  int Ctrl;
  int Shift;
  int L;
  int T;
  int Alt;
  int LM_Button;
  int RM_Button;
  //vtkProp3D *InteractionProp;
  vtkSmartPointer<vtkMDActorCollection> ActorCollection;
  vtkSmartPointer<vtkLMActorCollection> NormalLandmarkCollection;
  vtkSmartPointer<vtkLMActorCollection> TargetLandmarkCollection;
  vtkSmartPointer<vtkLMActorCollection> NodeLandmarkCollection;
  vtkSmartPointer<vtkLMActorCollection> HandleLandmarkCollection;
  vtkSmartPointer<vtkLMActorCollection> FlagLandmarkCollection;
private:

	void TransformPoint(vtkMatrix4x4* matrix, double pointin[3], double pointout[3]);
#if VTK_MINOR_VERSION >= 1
	vtkMDInteractorStyle(const vtkMDInteractorStyle&) VTK_DELETE_FUNCTION;
	void operator=(const vtkMDInteractorStyle&) VTK_DELETE_FUNCTION;
#else
	vtkMDInteractorStyle(const vtkMDInteractorStyle&);  // Not implemented
	void operator=(const vtkMDInteractorStyle&);  // Not implemented
#endif

  
};

#endif
