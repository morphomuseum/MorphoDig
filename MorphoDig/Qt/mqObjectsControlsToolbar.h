/*=========================================================================

   Program: MorphoDig
   Module:    Copied from Paraview pqMainControlsToolbar.h

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
class QReleaseSlider;
class QReleaseSliderValue;
class vtkProp3D;
class vtkMatrix4x4;
//class LMActor;
class vtkLMActor;
#ifndef mqObjectsControlsToolbar_h
#define mqObjectsControlsToolbar_h


#include <QToolBar>
class Ui_mqObjectsControlsToolbar;


class  mqObjectsControlsToolbar : public QToolBar
{
  Q_OBJECT
  typedef QToolBar Superclass;

public:
  mqObjectsControlsToolbar(const QString& title, QWidget* parentObject = 0)
    : Superclass(title, parentObject)
  {
    this->constructor();
  }
  mqObjectsControlsToolbar(QWidget* parentObject = 0)
    : Superclass(parentObject)
  {
    this->constructor();
  }
  void TransformPoint(vtkMatrix4x4* matrix, double pointin[3], double pointout[3]);
  void ChangeAttachmentPoint(vtkMatrix4x4 *NewMat, vtkLMActor *LMActor);
  void Prop3DTransform(vtkProp3D *prop3D,
	  double *boxCenter,
	  int numRotation,
	  double **rotate,
	  double *scale);
  

  public slots :
  virtual void slotDeleteObjects();
  
  virtual void slotXrot(int val);
  virtual void slotYrot(int val);
  virtual void slotZrot(int val);
  virtual void slotXtr(int val);
  virtual void slotYtr(int val);
  virtual void slotZtr(int val);
  virtual void slotZrotPressed();
  virtual void slotYrotPressed();
  virtual void slotXrotPressed();
  virtual void slotZtrPressed();
  virtual void slotYtrPressed();
  virtual void slotXtrPressed();

private:
	Q_DISABLE_COPY(mqObjectsControlsToolbar)
		int oldrotval;
		int oldtrval;
	void SavePositions(int val);
	
	//QReleaseSliderValue * zRot;
	QReleaseSlider * zRot;
	QReleaseSlider *zTr;
	//QReleaseSliderValue * yRot;
	QReleaseSlider * yRot;
	QReleaseSlider *yTr;
	//QReleaseSliderValue * xRot;
	QReleaseSlider * xRot;
	QReleaseSlider *xTr;
  Ui_mqObjectsControlsToolbar *ui;
  void constructor();
  void RotateActors(int axis, int degrees);
  void GetDisplayToWorld(double x, double y, double z, double worldPt[4]);
  void GetWorldToDisplay(double x, double y, double z, double displayPt[3]);
  void PanActors(int axis, int value);
};

#endif
