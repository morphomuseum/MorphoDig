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
#include <vtkPlotBar.h>
#include <vtkColorSeries.h>
#include <vtkPlotStacked.h>
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
    //this->ChartXY->SetShowLegend(true);
    this->ChartXY->SetForceAxesToBounds(true);
    this->ContextView->GetScene()->AddItem(this->ChartXY.GetPointer());
    this->ContextView->SetInteractor(this->Widget->GetInteractor());
    this->ContextView->GetRenderWindow()->SetLineSmoothing(true);

    this->ChartXY->SetActionToButton(vtkChart::PAN, -1);
    this->ChartXY->SetActionToButton(vtkChart::ZOOM, -1);
    this->ChartXY->SetActionToButton(vtkChart::SELECT, vtkContextMouseEvent::RIGHT_BUTTON);
    this->ChartXY->SetActionToButton(vtkChart::SELECT_POLYGON, -1);
	this->ChartXY->SetZoomWithMouseWheel(false);
	
	this->ChartXY->SetShowLegend(false);
	
    this->Widget->setParent(editor);
    QVBoxLayout* layout = new QVBoxLayout(editor);
    layout->setMargin(0);
    layout->addWidget(this->Widget);

    this->ChartXY->SetAutoAxes(false);
    this->ChartXY->SetHiddenAxisBorder(4);
    for (int cc = 0; cc < 4; cc++)
    {
      this->ChartXY->GetAxis(cc)->SetVisible(false);
      this->ChartXY->GetAxis(cc)->SetBehavior(vtkAxis::AUTO);
    }
  }
  ~mqInternals() { this->cleanup(); }
  void reinit(vtkImageAccumulate *hist, int numbins, double min, double max)
  {
	  this->cleanup();	  
	  if (hist !=NULL)
	  {
		  cout << "Histogram Widget being reinitialized" << endl;
		  this->Hist = hist;
		 // this->Hist->Update();
		  
		  double bin_spacing = 0;
		 // bin_spacing =this->Hist->GetComponentSpacing()[0];
		  //cout << "Retrieved bin_spacing :" << bin_spacing <<","<< this->Hist->GetComponentSpacing()[1]<<","<< this->Hist->GetComponentSpacing()[2]<< endl;
		  //int dims[3];
		  //this->Hist->GetOutput()->GetDimensions(dims);
		 // cout << "Histogram (max) dims=" << dims[0] << ", " << dims[1] << ", " << dims[2] << endl;
		  //vtkIdType used_bins = (vtkIdType)(dims[0] / bin_spacing);
		  //cout << "used_bins = "<<used_bins << endl;

		 // numbins = (int)((this->Hist->GetComponentExtent()[1] - this->Hist->GetComponentExtent()[0]) / bin_spacing);
		  if (numbins > 0) {
			  bin_spacing = (max - min) / numbins;
		  }
		 // cout << "numbins = " << numbins << endl;
		 // cout << "min = " << min << endl;
		 // cout << "bin_spacing = " << bin_spacing << endl;
		  hist->SetComponentOrigin(min, 0, 0);		  
		  hist->SetComponentSpacing(bin_spacing, 0, 0);
		  hist->Update();
		 // cout << "Num bins:" << numbins << endl;
		  vtkSmartPointer<vtkDoubleArray> bins =
			  vtkSmartPointer<vtkDoubleArray>::New();
		  bins->SetNumberOfComponents(1);
		  bins->SetNumberOfTuples(numbins);
		  bins->SetName("Bins");
		  vtkSmartPointer<vtkIntArray> frequencies =
			  vtkSmartPointer<vtkIntArray>::New();
		  frequencies->SetNumberOfComponents(1);
		  frequencies->SetNumberOfTuples(numbins);
		  frequencies->SetName("Frequencies");
		  vtkSmartPointer<vtkIntArray> logfrequencies =
			  vtkSmartPointer<vtkIntArray>::New();
		  logfrequencies->SetNumberOfComponents(1);
		  logfrequencies->SetNumberOfTuples(numbins);
		  logfrequencies->SetName("Logs");
		  //*(static_cast<int*>(histogram->GetOutput()->GetScalarPointer(bin, 0, 0)))
		  //int* output = static_cast<int*>(this->Hist->GetOutput()->GetScalarPointer());
		  //int* output = static_cast<int*>(this->Hist->GetOutput()->GetScalarPointer());
		  double spacing = this->Hist->GetComponentSpacing()[0];
		  double mbin = this->Hist->GetComponentOrigin()[0];
		  //cout << "Frequencies:" << endl;

		  int maxbin=0;
		  double maxlogbin=0;
		  for (vtkIdType bin = 0; bin < numbins; bin++)
		  {

			
			  mbin += spacing;
			  // cout << "bin =" << bin << ", retrieving curbin"  << endl;
			  int curbin = 0;			
			  curbin = *(static_cast<int*>(this->Hist->GetOutput()->GetScalarPointer(bin, 0, 0)));
			  if (curbin > maxbin) { maxbin = curbin; }
			  double logcurbin = 0;
			  if (curbin > 0) 
			  {
				  logcurbin = 100 * log10(curbin); 
				  if (logcurbin > maxlogbin) { maxlogbin = logcurbin; }
			  }

		  }
		  //cout << "maxbin=" << maxbin << endl;
		  //cout << "maxlogbin=" << maxlogbin << endl;

		  for (vtkIdType bin = 0; bin < numbins; bin++)
		  {
			
			  bins->SetTuple1(bin, mbin);
			  mbin += spacing;
			 // cout << "bin =" << bin << ", retrieving curbin"  << endl;
			  int curbin = 0;
			  //curbin =*output++;
			  curbin= *(static_cast<int*>(this->Hist->GetOutput()->GetScalarPointer(bin, 0, 0)));
			  int logcurbin = 0;
			  if (curbin > 0) {
				  logcurbin = 100*log10(curbin); 
				  if (maxlogbin > 0)
				  {					  
					  double mult = (double)logcurbin * (double)maxbin;					
					  double multdiv = mult / maxlogbin;					  					 					  
					  logcurbin = (int)(multdiv);
				  }
			  }
			  

			  //cout << "logcurbin=" << logcurbin<< "curbin"<<curbin<< endl;
			 // cout << "logcurbin=" << log10(curbin) << endl;
				  //curbin *(static_cast<int*>(this->Hist->GetOutput()->GetScalarPointer(bin, 0, 0)));
			  
			  if (curbin != 0)
			  {
				  //cout << curbin << endl;
			  }
			  frequencies->SetTuple1(bin, curbin);
			  int reslogcurbin = 0;
			  if ((logcurbin - curbin) > 0) {
				  reslogcurbin = logcurbin - curbin;
			  }
			  //logfrequencies->SetTuple1(bin, logcurbin);
			  logfrequencies->SetTuple1(bin, reslogcurbin);
		  }

		  vtkSmartPointer<vtkTable> table =
			  vtkSmartPointer<vtkTable>::New();
		 table->AddColumn(bins);
		  table->AddColumn(frequencies);
		 // table->AddColumn(logfrequencies);

		  vtkSmartPointer<vtkTable> table2 =
			  vtkSmartPointer<vtkTable>::New();
		  table2->AddColumn(bins);
		  table2->AddColumn(frequencies);
		  table2->AddColumn(logfrequencies);
		  
		  //vtkPlot * line = this->ChartXY->AddPlot(vtkChart::BAR);
		  //vtkPlotBar * line2 = vtkPlotBar::SafeDownCast(this->ChartXY->AddPlot(vtkChart::BAR));
		  //vtkPlot * line2 = this->ChartXY->AddPlot(vtkChart::BAR);
		  //vtkPlot * line2 = this->ChartXY->AddPlot(vtkChart::STACKED);
		  vtkPlotStacked * line2 = vtkPlotStacked::SafeDownCast(this->ChartXY->AddPlot(vtkChart::STACKED));
		  //vtkPlot * line2 = this->ChartXY->AddPlot(vtkChart::LINE);
		  this->ChartXY->GetAxis(0)->SetRange(min, max);
		  //this->ChartXY->GetAxis(0)->SetBehavior(vtkAxis::FIXED);
		  //cout << "line2: class name:" << line2->GetClassName() << endl;
		  line2->SetInputArray(1, "Frequencies");
		  line2->SetInputArray(2, "Logs");

		  vtkSmartPointer<vtkColorSeries> colorSeries =
			  vtkSmartPointer<vtkColorSeries>::New();
		  //colorSeries->
		  colorSeries->SetColorScheme(vtkColorSeries::BREWER_QUALITATIVE_PASTEL1);
		  line2->SetColorSeries(colorSeries);


		 /* line->SetInputData(table, 0, 1);
		  line->GetXAxis()->SetTitle("Bin");
		  
		  vtkTextProperty *textProp = line->GetXAxis()->GetLabelProperties();
		  textProp->SetColor(1.0, 0.0, 0.0);
		  textProp = line->GetXAxis()->GetTitleProperties();
		  textProp->SetColor(1.0, 1.0, 1.0);
		  line->GetYAxis()->SetTitle("Count");
		  line->SetColor(0.1, 0.3, 0.9);*/
		  line2->SetUseIndexForXSeries(true);

		  line2->SetInputData(table2, 0, 1);
		  //line2->SetLegendVisibility(false);
		  //line2->Set
		  line2->GetXAxis()->SetTitle("LogBin");
		  vtkTextProperty *textProp2 = line2->GetXAxis()->GetLabelProperties();
		  
		  //textProp2->SetLineOffset(0);
		  //textProp2->SetColor(0.0, 1.0, 0.0);
		  //textProp2->BoldOff();
		  textProp2->SetLineSpacing(0);
		  textProp2= line2->GetXAxis()->GetTitleProperties();
		  //textProp2->SetColor(1.0, 0.0, 1.0);
		  line2->GetYAxis()->SetTitle("Count2");
		  //line2->SetColor(0.1, 0.9, 0.3);
		  

	

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
	this->numBins = 50;
	this->min = 0;
	this->max = 1;
	this->mHist = NULL;
}

//-----------------------------------------------------------------------------
mqHistogramWidget::~mqHistogramWidget()
{
  delete this->Internals;
  this->Internals = NULL;
}

int mqHistogramWidget::GetNumBins() { return this->numBins; }
void mqHistogramWidget::SetNumBins(int num_bins) { this->numBins = num_bins; this->Internals->reinit( this->mHist, this->numBins, this->min, this->max);};
double mqHistogramWidget::GetMin() { return this->min; }
void mqHistogramWidget::SetMin(double newmin) 
{ 
	if (newmin < this->max) 
	{ this->min = newmin; this->Internals->reinit(this->mHist, this->numBins, this->min, this->max);
	}

}
double mqHistogramWidget::GetMax() { 
	return this->max;

}
void mqHistogramWidget::SetMax(double newmax)
{
	if (newmax > this->min)
	{
		this->max = newmax; this->Internals->reinit(this->mHist, this->numBins, this->min, this->max);
	}
}
void mqHistogramWidget::SetMinMax(double newmin, double newmax)
{
	if (newmin<newmax)
	{
		this->max = newmax; this->min = newmin; this->Internals->reinit(this->mHist, this->numBins, this->min, this->max);
	}
}
//-----------------------------------------------------------------------------
void mqHistogramWidget::initialize(
	vtkImageAccumulate* hist)
{
	cout << "mqHistogramWidget Initialize " << endl;
  //this->Internals->cleanup();
  this->mHist = hist;
  this->Internals->reinit(hist, this->numBins, this->min, this->max);

  

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

