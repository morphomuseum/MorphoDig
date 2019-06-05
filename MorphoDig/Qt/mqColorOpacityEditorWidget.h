/*=========================================================================

   Program: ParaView
   Module:    $RCSfile$

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
#ifndef mqColorOpacityEditorWidget_h
#define mqColorOpacityEditorWidget_h


#include <QList>
#include <QVariant>
#include <QWidget>
#include <vtkDiscretizableColorTransferFunction.h>
class vtkPiecewiseFunction;
class mqColorMapModel;
class vtkImageAccumulate;
//class vtkImageData;

class  mqColorOpacityEditorWidget : public QWidget
{
  Q_OBJECT
  Q_PROPERTY(QList<QVariant> xrgbPoints READ xrgbPoints WRITE setXrgbPoints)
  Q_PROPERTY(QList<QVariant> xvmsPoints READ xvmsPoints WRITE setXvmsPoints)
  
	  typedef QWidget Superclass;


public:
  //mqColorOpacityEditorWidget(vtkDiscretizableColorTransferFunction* stc, QWidget* parent = 0, int mapSurfaces=1);
  mqColorOpacityEditorWidget(vtkDiscretizableColorTransferFunction* stc, QWidget* parent = 0, int mapSurfaces = 1);
  ~mqColorOpacityEditorWidget() override;
  void reInitialize(vtkDiscretizableColorTransferFunction *stc, int keepMinMax=0);
  //void reInitializeHIST(vtkImageAccumulate *hist, vtkImageData *data);
  void reInitializeHIST(vtkImageAccumulate *hist, int numbins, double rangeMin, double rangeMax, double displayMin, double displayMax);
  /**
  * Returns the current list of control points for the color transfer
  * function. This a list of 4-tuples.
  */
  vtkDiscretizableColorTransferFunction* getSTC();
  int hasSTC();

  QList<QVariant> xrgbPoints() const;
  void SetCTFMin(double newMin);
  void SetCTFMax(double newMax);

  /**
  * Returns the current list of control points for the opacity
  * function. This a list of 4-tuples.
  */
  QList<QVariant> xvmsPoints() const;

  
signals:
  void changeFinished();
  void minmaxChanged();
  void shiftOrSlideStarted();
  void shiftOrSlideStopped();
public slots:
  /**
  * Sets the xvmsPoints that control the opacity transfer function.
  */
  void setXvmsPoints(const QList<QVariant>&);

  /**
  * Sets the xrgbPoints that control the color transfer function.
  */
  void setXrgbPoints(const QList<QVariant>&);

  
  /**
  * Reset the transfer function ranges to active data source.
  */
  void invertRGB();
  void invertOpacity();
  void resetRangeToData();
  void changedEnableOpacity();
  void changeDiscretize();
  void  changedDiscretizeValue(int value);
  void UpdateLookupTableRange();
  void UpdateHistogram();
  double getSTCMin();
  double getSTCMax();

  /**
  * save current transfer function as preset.
  */
  void saveAsCustom();

signals:
  /**
  * Signal fired when the xrgbPoints change.
  */
  void xrgbPointsChanged();

  /**
  * Signal fired when the xvmsPoints change.
  */
  void xvmsPointsChanged();

  /**
  * Signal fired when useLogScale changes.
  */
  void useLogScaleChanged();

  /**
  * Used to rescale the STC
  */
  
 

protected slots:
  /**
  * slots called when the current point changes on the two internal
  * pqTransferFunctionWidget widgets.
  */
  void opacityCurrentChanged(vtkIdType);
  void colorCurrentChanged(vtkIdType);
  virtual void slotCurrentMinEdited();
  virtual void slotCurrentMaxEdited();
  virtual void slotShiftSlider(int shift);
  virtual void slotSliderStart();
  virtual void slotSlideMin(int slideMin);
  virtual void slotSlideMax(int slideMax);
  virtual void slotSliderStop();
  virtual void slotExportCTF();

  /**
  * updates the panel to show/hide advanced settings
  */
  void updatePanel();

  /**
  * updates the text shown in the "current data" input.
  */
  void updateCurrentData();

  /**
  * update the transfer function with the text in CurrentDataValue text field.
  */
  void currentDataEdited();



protected:
  /**
   * Validate and adjust the current range before converting to a log range.
   */
  //void prepareRangeForLogScaling();

  /**
   * Initialize the opacity editor.
   */
  void initializeOpacityEditor(vtkPiecewiseFunction* pwf);

private:
	Q_DISABLE_COPY(mqColorOpacityEditorWidget)
	vtkDiscretizableColorTransferFunction *STC;
	vtkImageAccumulate *HIST;
	//vtkImageData *IMGDATA;
	int _mapSurfaces;
	double maxShiftAmplitude;
	double slideMin;
	double slideMax;
	double ctfMin;
	double ctfMax;
  class mqInternals;
  mqInternals* Internals;
};

#endif
