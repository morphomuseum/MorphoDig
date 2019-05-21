/*=========================================================================

   Program: MorphoDig 

========================================================================*/
#include "mqHistogramWidget.h"

#include "mqCoreUtilities.h"
#include "mqMorphoDigCore.h"
//#include "pqQVTKWidgetBase.h"
//#include "pqTimer.h"
#include <QTimer>
#if VTK_MAJOR_VERSION<8
#include <QVTKWidget.h>
#elseif VTK_MAJOR_VERSION == 8 && VTK_MINOR_VERSION < 2
#include <QVTKOpenGLWidget.h>
#else
#include <QVTKOpenGLNativeWidget.h>
#endif
#include <vtkAxis.h>
#include <vtkBoundingBox.h>
#include <vtkChartXY.h>
#include <vtkPlot.h>
#include <vtkDoubleArray.h>
#include <vtkIntArray.h>
#include <vtkTextProperty.h>
#include <vtkTable.h>
#include <vtkImageAccumulate.h>
#include <vtkContext2D.h>
#include <vtkContextMouseEvent.h>
#include <vtkContextScene.h>
#include <vtkContextView.h>
#include <vtkEventQtSlotConnect.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkNew.h>
#include <vtkObjectFactory.h>

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

class mqHistogramWidget::mqInternals
{
  vtkNew<vtkGenericOpenGLRenderWindow> Window;

public:
  	
#if VTK_MAJOR_VERSION<8	  	
	QPointer<QVTKWidget> Widget;
#elseif VTK_MAJOR_VERSION == 8 && VTK_MINOR_VERSION < 2
	QPointer<QVTKOpenGLWidget> Widget;
#else
	QPointer<QVTKOpenGLNativeWidget> Widget; 	
#endif


  vtkNew<vtkChartXY> ChartXY;
  vtkNew<vtkContextView> ContextView;
  vtkNew<vtkEventQtSlotConnect> VTKConnect;
  vtkImageAccumulate *Hist;

  
  unsigned long CurrentPointEditEventId;

  mqInternals(mqHistogramWidget* editor)
#if VTK_MAJOR_VERSION<8
	  : Widget(new QVTKWidget(editor))
	  
#elseif VTK_MAJOR_VERSION == 8 && VTK_MINOR_VERSION < 2
	  : Widget(new QVTKOpenGLWidget(editor))
#else
	  : Widget(new QVTKOpenGLNativeWidget(editor))	 
#endif    
    , CurrentPointEditEventId(0)
  {


#if QT_VERSION >= 0x050000
    QSurfaceFormat fmt = QSurfaceFormat::defaultFormat();
    fmt.setSamples(8);
   //@@ this->Widget->setFormat(fmt);
    //@@this->Widget->setEnableHiDPI(true);
#endif
	this->Hist = NULL;
    this->Widget->setObjectName("1QVTKWidget0");
    this->Widget->SetRenderWindow(this->Window.Get());
    this->ContextView->SetRenderWindow(this->Window.Get());

    this->ChartXY->SetAutoSize(true);
    this->ChartXY->SetShowLegend(true);
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
  void reinit(vtkImageAccumulate *hist)
  {
	  if (hist !=NULL)
	  {
		  cout << "Histogram Widget being reinitialized" << endl;
		  this->Hist = hist;
		  this->Hist->Update();
		  vtkSmartPointer<vtkDoubleArray> bins =
			  vtkSmartPointer<vtkDoubleArray>::New();
		  bins->SetNumberOfComponents(1);
		  bins->SetNumberOfTuples(1000);
		  bins->SetName("Bins");
		  vtkSmartPointer<vtkIntArray> frequencies =
			  vtkSmartPointer<vtkIntArray>::New();
		  frequencies->SetNumberOfComponents(1);
		  frequencies->SetNumberOfTuples(1000);
		  frequencies->SetName("Frequency");
		  int* output = static_cast<int*>(this->Hist->GetOutput()->GetScalarPointer());
		  double spacing = this->Hist->GetComponentSpacing()[0];
		  double bin = this->Hist->GetComponentOrigin()[0];
		  for (unsigned int j = 0; j < 1000; ++j)
		  {
			  bins->SetTuple1(j, bin);
			  bin += spacing;
			  frequencies->SetTuple1(j, *output++);
		  }

		  vtkSmartPointer<vtkTable> table =
			  vtkSmartPointer<vtkTable>::New();
		  table->AddColumn(bins);
		  table->AddColumn(frequencies);
		  vtkPlot * line = this->ChartXY->AddPlot(vtkChart::BAR);

		  line->SetInputData(table, 0, 1);
		  line->GetXAxis()->SetTitle("Bin");
		  vtkTextProperty *textProp = line->GetXAxis()->GetLabelProperties();
		  textProp->SetColor(1.0, 1.0, 1.0);
		  textProp = line->GetXAxis()->GetTitleProperties();
		  textProp->SetColor(1.0, 1.0, 1.0);
		  line->GetYAxis()->SetTitle("Count");
		  line->SetColor(0.1, 0.3, 0.9);
		  /*

   

   

    vtkPlot * line = chart->AddPlot( vtkChart::BAR );
#if VTK_MAJOR_VERSION < 6
    line->SetInput( table, 0, 1 );
#else
    line->SetInputData( table, 0, 1 );
#endif
    line->GetXAxis()->SetTitle( "Bin" );
    vtkTextProperty *textProp = line->GetXAxis()->GetLabelProperties();
    textProp->SetColor( 1.0, 1.0, 1.0 );
    textProp = line->GetXAxis()->GetTitleProperties();
    textProp->SetColor( 1.0, 1.0, 1.0 );
    line->GetYAxis()->SetTitle( "Count" );
line->SetColor( 0.1, 0.3, 0.9 );*/

	  }

  }
  void cleanup()
  {
	 // cout << "mqHistogramWidget Cleanup" << endl;
    this->VTKConnect->Disconnect();
    this->ChartXY->ClearPlots();
   
  }
};

//-----------------------------------------------------------------------------
mqHistogramWidget::mqHistogramWidget(QWidget* parentObject)
  : Superclass(parentObject)
  , Internals(new mqInternals(this))
{
//	cout << "mqHistogramWidget Widget constructor" << endl;
  
}

//-----------------------------------------------------------------------------
mqHistogramWidget::~mqHistogramWidget()
{
  delete this->Internals;
  this->Internals = NULL;
}

//-----------------------------------------------------------------------------
void mqHistogramWidget::initialize(
	vtkImageAccumulate* hist)
{
	cout << "mqHistogramWidget Initialize " << endl;
  this->Internals->cleanup();
  this->Internals->reinit(hist);

  

  if (hist != NULL )
  {
	
  }  
  else
  {
    return;
  }
  //cout << "mqHistogramWidget Initialize Add Plot" << endl;
  //this->Internals->ChartXY->AddPlot(this->Internals->Histogram);

  
}

//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
void mqHistogramWidget::SetLogScaleXAxis(bool logScale)
{

  this->Internals->ChartXY->GetAxis(vtkAxis::BOTTOM)->SetLogScale(logScale);
}

//-----------------------------------------------------------------------------
bool mqHistogramWidget::GetLogScaleXAxis() const
{
  return this->Internals->ChartXY->GetAxis(vtkAxis::BOTTOM)->GetLogScale();
}

//-----------------------------------------------------------------------------
void mqHistogramWidget::render()
{

}

//-----------------------------------------------------------------------------
void mqHistogramWidget::renderInternal()
{
	//cout << "mqHistogramWidget renderInternal" << endl;
  if (this->isVisible() && this->Internals->ContextView->GetRenderWindow()->IsDrawable())
  {
    this->Internals->ContextView->GetRenderWindow()->Render();
	mqMorphoDigCore::instance()->Render();
  }
}

