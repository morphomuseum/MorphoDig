/*=========================================================================

   Program: MorphoDig 

========================================================================*/
#include "mqTransferFunctionWidget.h"

#include "mqCoreUtilities.h"
#include "mqMorphoDigCore.h"
//#include "pqQVTKWidgetBase.h"
//#include "pqTimer.h"
#include <QTimer>
#include <QVTKOpenGLWidget.h>
//#include <QVTKWidget.h>
#include <vtkAxis.h>
#include <vtkBoundingBox.h>
#include <vtkChartXY.h>
#include <vtkColorTransferControlPointsItem.h>
#include <vtkColorTransferFunction.h>
#include <vtkColorTransferFunctionItem.h>
#include <vtkCompositeControlPointsItem.h>
#include <vtkCompositeTransferFunctionItem.h>
#include <vtkContext2D.h>
#include <vtkContextMouseEvent.h>
#include <vtkContextScene.h>
#include <vtkContextView.h>
#include <vtkEventQtSlotConnect.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkNew.h>
#include <vtkObjectFactory.h>
#include <vtkPiecewiseControlPointsItem.h>
#include <vtkPiecewiseFunction.h>
#include <vtkPiecewiseFunctionItem.h>
//#include "vtkSMCoreUtilities.h"
#include <vtkSmartPointer.h>

#include <QColorDialog>
#include <QPointer>
#include <QVBoxLayout>
#include <algorithm>

#if QT_VERSION >= 0x050000
#include <QSurfaceFormat>
#endif

//-----------------------------------------------------------------------------
// We extend vtkChartXY to add logic to reset view bounds automatically. This
// ensures that when LUT changes, we are always showing the complete LUT.
class vtkTransferFunctionChartXY : public vtkChartXY
{
  double XRange[2];
  bool DataValid;

  bool IsDataRangeValid(const double r[2]) const
  {
    double mr[2] = { r[0], r[1] };
    // If vtkSMCoreUtilities::AdjustRange() decided to adjust a valid range, it means the numbers
    // are too close to each other.
	if (r[1]<r[0])
	{
		return false;
	}
	else
	{
		return true;
	}
	
  }

public:
  static vtkTransferFunctionChartXY* New();
  vtkTypeMacro(vtkTransferFunctionChartXY, vtkChartXY);
  vtkWeakPointer<vtkControlPointsItem> ControlPointsItem;

  // Description:
  // Perform any updates to the item that may be necessary before rendering.
  // The scene should take care of calling this on all items before their
  // Paint function is invoked.
  void Update() VTK_OVERRIDE
  {
	//  cout << "mqTransferFunctionWidget Update" << endl;
    if (this->ControlPointsItem)
    {
      // Reset bounds if the control points' bounds have changed.
      double bounds[4];
      this->ControlPointsItem->GetBounds(bounds);
      this->SetVisible(true);
      if (bounds[0] <= bounds[1] && (bounds[0] != this->XRange[0] || bounds[1] != this->XRange[1]))
      {
        this->XRange[0] = bounds[0];
        this->XRange[1] = bounds[1];
        this->DataValid = this->IsDataRangeValid(this->XRange);
        this->RecalculateBounds();
      }
    }
    this->Superclass::Update();
  }

  bool PaintChildren(vtkContext2D* painter) VTK_OVERRIDE
  {
	 // cout << "mqTransferFunctionWidget PaintChildren" << endl;
    if (this->DataValid)
   {
      return this->Superclass::PaintChildren(painter);
    }
    painter->DrawString(5, 5, "Data range too small to render.");
    return true;
  }

  bool MouseEnterEvent(const vtkContextMouseEvent& mouse) VTK_OVERRIDE
  {
    return (this->DataValid ? this->Superclass::MouseEnterEvent(mouse) : false);
	 // return this->Superclass::MouseEnterEvent(mouse);
  }
  bool MouseMoveEvent(const vtkContextMouseEvent& mouse) VTK_OVERRIDE
  {
   return (this->DataValid ? this->Superclass::MouseMoveEvent(mouse) : false);
	 // return  this->Superclass::MouseMoveEvent(mouse);
  }
  bool MouseLeaveEvent(const vtkContextMouseEvent& mouse) VTK_OVERRIDE
  {
    return (this->DataValid ? this->Superclass::MouseLeaveEvent(mouse) : false);
	 // return this->Superclass::MouseLeaveEvent(mouse) ;
  }
  bool MouseButtonPressEvent(const vtkContextMouseEvent& mouse) VTK_OVERRIDE
  {
    return (this->DataValid ? this->Superclass::MouseButtonPressEvent(mouse) : false);
	//return this->Superclass::MouseButtonPressEvent(mouse);
  }
  bool MouseButtonReleaseEvent(const vtkContextMouseEvent& mouse) VTK_OVERRIDE
  {
    return (this->DataValid ? this->Superclass::MouseButtonReleaseEvent(mouse) : false);
	//  return this->Superclass::MouseButtonReleaseEvent(mouse);
  }
  bool MouseWheelEvent(const vtkContextMouseEvent& mouse, int delta) VTK_OVERRIDE
  {
    return (this->DataValid ? this->Superclass::MouseWheelEvent(mouse, delta) : false);
	 // return this->Superclass::MouseWheelEvent(mouse, delta);
  }
  bool KeyPressEvent(const vtkContextKeyEvent& key) VTK_OVERRIDE
  {
	//  return this->Superclass::KeyPressEvent(key);
    return (this->DataValid ? this->Superclass::KeyPressEvent(key) : false);
  }

protected:
  vtkTransferFunctionChartXY()
  {
	//  cout << "mqTransferFunctionWidget ChartXY creation" << endl;
    this->XRange[0] = this->XRange[1] = 0.0;
   this->DataValid = false;
    this->ZoomWithMouseWheelOff();
  }
  ~vtkTransferFunctionChartXY() override {}

private:
  vtkTransferFunctionChartXY(const vtkTransferFunctionChartXY&);
  void operator=(const vtkTransferFunctionChartXY&);
};

vtkStandardNewMacro(vtkTransferFunctionChartXY);

//-----------------------------------------------------------------------------

class mqTransferFunctionWidget::mqInternals
{
  vtkNew<vtkGenericOpenGLRenderWindow> Window;

public:
  //QPointer<QVTKWidget> Widget;
	QPointer<QVTKOpenGLWidget> Widget;
  vtkNew<vtkTransferFunctionChartXY> ChartXY;
  vtkNew<vtkContextView> ContextView;
  vtkNew<vtkEventQtSlotConnect> VTKConnect;

 // pqTimer Timer;
  QTimer Timer;
  vtkSmartPointer<vtkScalarsToColorsItem> TransferFunctionItem;
  vtkSmartPointer<vtkControlPointsItem> ControlPointsItem;
  unsigned long CurrentPointEditEventId;

  mqInternals(mqTransferFunctionWidget* editor)
    : Widget(new QVTKOpenGLWidget(editor))
    , CurrentPointEditEventId(0)
  {
	//  cout << "mqTransferFunctionWidget Internals Creator" << endl;
    this->Timer.setSingleShot(true);
    this->Timer.setInterval(0);

#if QT_VERSION >= 0x050000
    QSurfaceFormat fmt = QSurfaceFormat::defaultFormat();
    fmt.setSamples(8);
    this->Widget->setFormat(fmt);
    this->Widget->setEnableHiDPI(true);
#endif

    this->Widget->setObjectName("1QVTKWidget0");
    this->Widget->SetRenderWindow(this->Window.Get());
    this->ContextView->SetRenderWindow(this->Window.Get());

    this->ChartXY->SetAutoSize(true);
    this->ChartXY->SetShowLegend(false);
    this->ChartXY->SetForceAxesToBounds(true);
    this->ContextView->GetScene()->AddItem(this->ChartXY.GetPointer());
    this->ContextView->SetInteractor(this->Widget->GetInteractor());
    this->ContextView->GetRenderWindow()->SetLineSmoothing(true);

    this->ChartXY->SetActionToButton(vtkChart::PAN, -1);
    this->ChartXY->SetActionToButton(vtkChart::ZOOM, -1);
    this->ChartXY->SetActionToButton(vtkChart::SELECT, vtkContextMouseEvent::RIGHT_BUTTON);
    this->ChartXY->SetActionToButton(vtkChart::SELECT_POLYGON, -1);

    this->Widget->setParent(editor);
    QVBoxLayout* layout = new QVBoxLayout(editor);
    layout->setMargin(0);
    layout->addWidget(this->Widget);

    this->ChartXY->SetAutoAxes(false);
    this->ChartXY->SetHiddenAxisBorder(8);
    for (int cc = 0; cc < 4; cc++)
    {
      this->ChartXY->GetAxis(cc)->SetVisible(false);
      this->ChartXY->GetAxis(cc)->SetBehavior(vtkAxis::AUTO);
    }
  }
  ~mqInternals() { this->cleanup(); }

  void cleanup()
  {
	 // cout << "mqTransferFunctionWidget Cleanup" << endl;
    this->VTKConnect->Disconnect();
    this->ChartXY->ClearPlots();
    if (this->ControlPointsItem && this->CurrentPointEditEventId)
    {
      this->ControlPointsItem->RemoveObserver(this->CurrentPointEditEventId);
      this->CurrentPointEditEventId = 0;
    }
    this->TransferFunctionItem = NULL;
    this->ControlPointsItem = NULL;
  }
};

//-----------------------------------------------------------------------------
mqTransferFunctionWidget::mqTransferFunctionWidget(QWidget* parentObject)
  : Superclass(parentObject)
  , Internals(new mqInternals(this))
{
//	cout << "mqTransferFunctionWidget Widget constructor" << endl;
  QObject::connect(&this->Internals->Timer, SIGNAL(timeout()), this, SLOT(renderInternal()));
}

//-----------------------------------------------------------------------------
mqTransferFunctionWidget::~mqTransferFunctionWidget()
{
  delete this->Internals;
  this->Internals = NULL;
}

//-----------------------------------------------------------------------------
void mqTransferFunctionWidget::initialize(
  vtkScalarsToColors* stc, bool stc_editable, vtkPiecewiseFunction* pwf, bool pwf_editable)
{
	//cout << "mqTransferFunctionWidget Initialize " << endl;
  this->Internals->cleanup();

  // TODO: If needed, we can support vtkLookupTable.
  vtkColorTransferFunction* ctf = vtkColorTransferFunction::SafeDownCast(stc);

  if (ctf != NULL && pwf == NULL)
  {
	//  cout << "mqTransferFunctionWidget Initialize 1" << endl;
    vtkNew<vtkColorTransferFunctionItem> item;
    item->SetColorTransferFunction(ctf);

    this->Internals->TransferFunctionItem = item.GetPointer();

    if (stc_editable)
    {
      vtkNew<vtkColorTransferControlPointsItem> cpItem;
      cpItem->SetColorTransferFunction(ctf);
      cpItem->SetColorFill(true);
      cpItem->SetEndPointsXMovable(false);
      cpItem->SetEndPointsYMovable(false);
      cpItem->SetLabelFormat("%.3f");
      this->Internals->ControlPointsItem = cpItem.GetPointer();

      this->Internals->CurrentPointEditEventId =
        cpItem->AddObserver(vtkControlPointsItem::CurrentPointEditEvent, this,
          &mqTransferFunctionWidget::onCurrentPointEditEvent);
    }
  }
  else if (ctf == NULL && pwf != NULL)
  {
	//  cout << "mqTransferFunctionWidget Initialize 2" << endl;
    vtkNew<vtkPiecewiseFunctionItem> item;
    item->SetPiecewiseFunction(pwf);

    this->Internals->TransferFunctionItem = item.GetPointer();

    if (pwf_editable)
    {
      vtkNew<vtkPiecewiseControlPointsItem> cpItem;
      cpItem->SetPiecewiseFunction(pwf);
      cpItem->SetEndPointsXMovable(false);
      cpItem->SetEndPointsYMovable(true);
      cpItem->SetLabelFormat("%.3f: %.3f");
      this->Internals->ControlPointsItem = cpItem.GetPointer();
    }
  }
  else if (ctf != NULL && pwf != NULL)
  {
	//  cout << "mqTransferFunctionWidget Initialize 3" << endl;
    vtkNew<vtkCompositeTransferFunctionItem> item;
    item->SetOpacityFunction(pwf);
    item->SetColorTransferFunction(ctf);
    item->SetMaskAboveCurve(true);

    this->Internals->TransferFunctionItem = item.GetPointer();
    if (pwf_editable && stc_editable)
    {
      // NOTE: this hasn't been tested yet.
      vtkNew<vtkCompositeControlPointsItem> cpItem;
      cpItem->SetPointsFunction(vtkCompositeControlPointsItem::ColorAndOpacityPointsFunction);
      cpItem->SetOpacityFunction(pwf);
      cpItem->SetColorTransferFunction(ctf);
      cpItem->SetEndPointsXMovable(false);
      cpItem->SetEndPointsYMovable(true);
      cpItem->SetUseOpacityPointHandles(true);
      cpItem->SetLabelFormat("%.3f: %.3f");
      this->Internals->ControlPointsItem = cpItem.GetPointer();
    }
    else if (pwf_editable)
    {
      vtkNew<vtkCompositeControlPointsItem> cpItem;
      cpItem->SetPointsFunction(vtkCompositeControlPointsItem::OpacityPointsFunction);
      cpItem->SetOpacityFunction(pwf);
      cpItem->SetColorTransferFunction(ctf);
      cpItem->SetEndPointsXMovable(false);
      cpItem->SetEndPointsYMovable(true);
      cpItem->SetUseOpacityPointHandles(true);
      cpItem->SetLabelFormat("%.3f: %.3f");
      this->Internals->ControlPointsItem = cpItem.GetPointer();
    }
  }
  else
  {
    return;
  }
 // cout << "mqTransferFunctionWidget Initialize suite" << endl;
  this->Internals->ChartXY->AddPlot(this->Internals->TransferFunctionItem);

  if (this->Internals->ControlPointsItem)
  {
	 // cout << "mqTransferFunctionWidget we have control points item" << endl;

    this->Internals->ChartXY->ControlPointsItem = this->Internals->ControlPointsItem;
    this->Internals->ControlPointsItem->SetEndPointsRemovable(false);
    this->Internals->ControlPointsItem->SetShowLabels(true);
    this->Internals->ChartXY->AddPlot(this->Internals->ControlPointsItem);

	// Have to check the utility of that!
    mqCoreUtilities::connect(this->Internals->ControlPointsItem,
      vtkControlPointsItem::CurrentPointChangedEvent, this, SLOT(onCurrentChangedEvent()));
    mqCoreUtilities::connect(this->Internals->ControlPointsItem, vtkCommand::EndEvent, this,
      SIGNAL(controlPointsModified()));
	//cout << "mqTransferFunctionWidget OK?" << endl;
  }

  // If the transfer functions change, we need to re-render the view. This
  // ensures that.
  if (ctf)
  {
	 // cout << "mqTransferFunctionWidget conect ctf with slot render" << endl;

    this->Internals->VTKConnect->Connect(ctf, vtkCommand::ModifiedEvent, this, SLOT(render()));
  }
  if (pwf)
  {
	 // cout << "mqTransferFunctionWidget conect pwf with slot render" << endl;
    this->Internals->VTKConnect->Connect(pwf, vtkCommand::ModifiedEvent, this, SLOT(render()));
  }
}

//-----------------------------------------------------------------------------
void mqTransferFunctionWidget::onCurrentPointEditEvent()
{
//	cout << "mqTransferFunctionWidget editPT " << endl;
  vtkColorTransferControlPointsItem* cpitem =
    vtkColorTransferControlPointsItem::SafeDownCast(this->Internals->ControlPointsItem);
  if (cpitem == NULL)
  {
    return;
  }

  vtkIdType currentIdx = cpitem->GetCurrentPoint();
  if (currentIdx < 0)
  {
    return;
  }

  vtkColorTransferFunction* ctf = cpitem->GetColorTransferFunction();
  Q_ASSERT(ctf != NULL);

  double xrgbms[6];
  ctf->GetNodeValue(currentIdx, xrgbms);
  QColor color = QColorDialog::getColor(QColor::fromRgbF(xrgbms[1], xrgbms[2], xrgbms[3]), this,
    "Select Color", QColorDialog::DontUseNativeDialog);
  if (color.isValid())
  {
    xrgbms[1] = color.redF();
    xrgbms[2] = color.greenF();
    xrgbms[3] = color.blueF();
    ctf->SetNodeValue(currentIdx, xrgbms);

    emit this->controlPointsModified();
  }
}

//-----------------------------------------------------------------------------
void mqTransferFunctionWidget::onCurrentChangedEvent()
{
	//cout << "mqTransferFunctionWidget Changed " << endl;
  if (this->Internals->ControlPointsItem)
  {
    emit this->currentPointChanged(this->Internals->ControlPointsItem->GetCurrentPoint());
  }
}

//-----------------------------------------------------------------------------
vtkIdType mqTransferFunctionWidget::currentPoint() const
{
	//cout << "mqTransferFunctionWidget currPt" << endl;
  if (this->Internals->ControlPointsItem)
  {
    return this->Internals->ControlPointsItem->GetCurrentPoint();
  }

  return -1;
}

//-----------------------------------------------------------------------------
void mqTransferFunctionWidget::setCurrentPoint(vtkIdType index)
{
	//cout << "mqTransferFunctionWidget setCurrPt " << endl;
  if (this->Internals->ControlPointsItem)
  {
    if (index < -1 || index >= this->Internals->ControlPointsItem->GetNumberOfPoints())
    {
      index = -1;
    }
    this->Internals->ControlPointsItem->SetCurrentPoint(index);
  }
}

//-----------------------------------------------------------------------------
vtkIdType mqTransferFunctionWidget::numberOfControlPoints() const
{
	//cout << "mqTransferFunctionWidget nrCtrlPT" << endl;
  return this->Internals->ControlPointsItem
    ? this->Internals->ControlPointsItem->GetNumberOfPoints()
    : 0;
}

//-----------------------------------------------------------------------------
void mqTransferFunctionWidget::SetLogScaleXAxis(bool logScale)
{

  this->Internals->ChartXY->GetAxis(vtkAxis::BOTTOM)->SetLogScale(logScale);
}

//-----------------------------------------------------------------------------
bool mqTransferFunctionWidget::GetLogScaleXAxis() const
{
  return this->Internals->ChartXY->GetAxis(vtkAxis::BOTTOM)->GetLogScale();
}

//-----------------------------------------------------------------------------
void mqTransferFunctionWidget::render()
{
 //cout << "mqTransferFunctionWidget render" << endl;
  this->Internals->Timer.start();
	/*if (this->isVisible() && this->Internals->ContextView->GetRenderWindow()->IsDrawable())
	{
		this->Internals->ContextView->GetRenderWindow()->Render();
	}*/
}

//-----------------------------------------------------------------------------
void mqTransferFunctionWidget::renderInternal()
{
	//cout << "mqTransferFunctionWidget renderInternal" << endl;
  if (this->isVisible() && this->Internals->ContextView->GetRenderWindow()->IsDrawable())
  {
    this->Internals->ContextView->GetRenderWindow()->Render();
	mqMorphoDigCore::instance()->Render();
  }
}

//-----------------------------------------------------------------------------
void mqTransferFunctionWidget::setCurrentPointPosition(double xpos)
{
	//cout << "mqTransferFunctionWidget currPT Position " << endl;
  vtkIdType currentPid = this->currentPoint();
  if (currentPid < 0)
  {
    return;
  }

  vtkIdType numPts = this->Internals->ControlPointsItem->GetNumberOfPoints();
  if (currentPid >= 0)
  {
    double start_point[4];
    this->Internals->ControlPointsItem->GetControlPoint(0, start_point);
    xpos = std::max(start_point[0], xpos);
  }
  if (currentPid <= (numPts - 1))
  {
    double end_point[4];
    this->Internals->ControlPointsItem->GetControlPoint(numPts - 1, end_point);
    xpos = std::min(end_point[0], xpos);
  }

  double point[4];
  this->Internals->ControlPointsItem->GetControlPoint(currentPid, point);
  if (point[0] != xpos)
  {
    point[0] = xpos;
    this->Internals->ControlPointsItem->SetControlPoint(currentPid, point);
  }
}
