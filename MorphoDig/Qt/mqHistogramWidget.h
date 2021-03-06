/*=========================================================================

   Program: MorphoDig, taken and Modiied from ParaView
 

========================================================================*/
#ifndef mqHistogramWidget_h
#define mqHistogramWidget_h


#include "vtkType.h"
#include <QWidget>


class vtkImageAccumulate;

/**
* mqHistogramWidget

*/
class mqHistogramWidget : public QWidget
{
  Q_OBJECT
  typedef QWidget Superclass;

public:
  mqHistogramWidget(QWidget* parent = 0);
  ~mqHistogramWidget() override;

  /**
  * Initialize the pqHistogramWidget with the given
  * color-transfer-function and piecewise-function (either of which can be
  * null). The editable flags are used to control if the users should be
  * allowed to edit/change the particular transfer function.
  */
  void initialize(
    vtkImageAccumulate* hist, int numbins, double rangeMin, double rangeMax, double displayMin, double displayMax);
  //int GetNumBins();
  //void SetNumBins(int num_bins);
  double GetDisplayMin();
  void SetDisplayMin(double newmin);
  void SetDisplayMinMax(double newmin, double newmax);
  double GetDisplayMax();
  void SetDisplayMax(double newmax);
  void SetLogScaleXAxis(bool logScale);
  bool GetLogScaleXAxis() const;
  //int numBins;
  double displayMin;

  double displayMax;

  vtkImageAccumulate* mHist;
public slots:
  
  void render();

signals:
  /**
  * signal fired when the \c current selected control point changes.
  */
  void currentThresholdModified();

  

protected slots:
  /**
  * slot called when the internal vtkControlPointsItem fires
  * vtkControlPointsItem::CurrentPointChangedEvent
  */
  //void onCurrentChangedEvent();

  void renderInternal();

protected:
  
  //void onCurrentThresholdModifiedEvent();

private:
  Q_DISABLE_COPY(mqHistogramWidget)

  class mqInternals;
  mqInternals* Internals;
};

#endif
