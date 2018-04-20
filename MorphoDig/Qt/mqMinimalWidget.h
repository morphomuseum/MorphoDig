/*=========================================================================

   Program: MorphoDig, taken and Modiied from ParaView
 

========================================================================*/
#ifndef mqMinimalWidget_h
#define mqMinimalWidget_h


#include "vtkType.h"
#include <QWidget>

class vtkScalarsToColors;
class vtkPiecewiseFunction;

/**
* mqMinimalWidget, heavily copied from mqMinimalWidget, provides a widget that can edit the control points

*/
class mqMinimalWidget : public QWidget
{
  Q_OBJECT
  typedef QWidget Superclass;

public:
  mqMinimalWidget(QWidget* parent = 0);
  ~mqMinimalWidget() override;

  /**
  * Initialize the pqTransferFunctionWidget with the given
  * color-transfer-function and piecewise-function (either of which can be
  * null). The editable flags are used to control if the users should be
  * allowed to edit/change the particular transfer function.
  */
  void initialize(
    vtkScalarsToColors* stc, bool stc_editable, vtkPiecewiseFunction* pwf, bool pwf_editable);

  /**
  * Returns the current point index. -1 is none is selected.
  */
  vtkIdType currentPoint() const;

  /**
  * Returns the number of control points in the editor widget.
  */
  vtkIdType numberOfControlPoints() const;

  /**
   * Switches the chart to use a log scaled X axis.
   */
  void SetLogScaleXAxis(bool logScale);
  bool GetLogScaleXAxis() const;

public slots:
  /**
  * Set the current point. Set to -1 clear the current point.
  */
  void setCurrentPoint(vtkIdType index);

  /**
  * Set the X-position for the current point (without changing the Y position,
  * if applicable. We ensure that the point doesn't move past neighbouring
  * points. Note this will not change the end points i.e. start and end points.
  */
  void setCurrentPointPosition(double xpos);

  /**
  * re-renders the transfer function view. This doesn't render immediately,
  * schedules a render.
  */
  void render();

signals:
  /**
  * signal fired when the \c current selected control point changes.
  */
  void currentPointChanged(vtkIdType index);

  /**
  * signal fired to indicate that the control points changed i.e. either they
  * were moved, orone was added/deleted, or edited to change color, etc.
  */
  void controlPointsModified();

protected slots:
  /**
  * slot called when the internal vtkControlPointsItem fires
  * vtkControlPointsItem::CurrentPointChangedEvent
  */
  void onCurrentChangedEvent();

  void renderInternal();

protected:
  /**
  * callback called when vtkControlPointsItem fires
  * vtkControlPointsItem::CurrentPointEditEvent.
  */
  void onCurrentPointEditEvent();

private:
  Q_DISABLE_COPY(mqMinimalWidget)

  class mqInternals;
  mqInternals* Internals;
};

#endif
