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
  vtkNew<vtkIntArray> displayMinMax;
  vtkNew<vtkIntArray> frequencies;
  vtkNew<vtkIntArray> logfrequencies;
  vtkNew<vtkDoubleArray> bins;
  int maxbin;
  
  
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
	this->maxbin = 0;
	/*this->displayMinMax = vtkSmartPointer<vtkIntArray>::New();
	this->frequencies= vtkSmartPointer<vtkIntArray>::New();
	this->logfrequencies= vtkSmartPointer<vtkIntArray>::New();
	this->bins= vtkSmartPointer<vtkIntArray>::New();*/
    
    //this->ChartXY->SetShowLegend(true);
    
    this->ContextView->GetScene()->AddItem(this->ChartXY.GetPointer());
    this->ContextView->SetInteractor(this->Widget->GetInteractor());
    this->ContextView->GetRenderWindow()->SetLineSmoothing(true);

    this->ChartXY->SetActionToButton(vtkChart::PAN, -1);
    this->ChartXY->SetActionToButton(vtkChart::ZOOM, -1);
    this->ChartXY->SetActionToButton(vtkChart::SELECT, vtkContextMouseEvent::RIGHT_BUTTON);
    this->ChartXY->SetActionToButton(vtkChart::SELECT_POLYGON, -1);
	this->ChartXY->SetZoomWithMouseWheel(false);	
	this->ChartXY->SetAutoSize(true);

	this->ChartXY->SetForceAxesToBounds(true);
	this->ChartXY->SetShowLegend(false);
	
    this->Widget->setParent(editor);
    QVBoxLayout* layout = new QVBoxLayout(editor);
    layout->setMargin(0);
    layout->addWidget(this->Widget);

    this->ChartXY->SetAutoAxes(false);
    this->ChartXY->SetHiddenAxisBorder(2);
    for (int cc = 0; cc < 4; cc++)
    {
      this->ChartXY->GetAxis(cc)->SetVisible(false);
      this->ChartXY->GetAxis(cc)->SetBehavior(vtkAxis::AUTO);
    }
  }
  ~mqInternals() { this->cleanup(); }
  
  void drawAgain()
  {



	  vtkSmartPointer<vtkTable> tableFrequencies =
		  vtkSmartPointer<vtkTable>::New();
	  tableFrequencies->AddColumn(this->bins);
	  tableFrequencies->AddColumn(this->frequencies);
	  tableFrequencies->AddColumn(this->logfrequencies);


	  vtkSmartPointer<vtkTable> tableMinMax =
		  vtkSmartPointer<vtkTable>::New();
	  tableMinMax->AddColumn(bins);
	  tableMinMax->AddColumn(this->displayMinMax);

	  vtkPlotStacked * lineFrequencies = vtkPlotStacked::SafeDownCast(this->ChartXY->AddPlot(vtkChart::STACKED));

	  lineFrequencies->SetInputData(tableFrequencies, 0, 1);
	  lineFrequencies->SetInputArray(1, "Frequencies");
	  lineFrequencies->SetInputArray(2, "Logs");


	  vtkSmartPointer<vtkColorSeries> colorSeries =
		  vtkSmartPointer<vtkColorSeries>::New();
	  //colorSeries->
	  colorSeries->SetColorScheme(vtkColorSeries::BREWER_QUALITATIVE_PASTEL1);
	  lineFrequencies->SetColorSeries(colorSeries);


	  vtkPlotBar * lineMinMax = vtkPlotBar::SafeDownCast(this->ChartXY->AddPlot(vtkChart::BAR));
	  lineMinMax->SetInputData(tableMinMax, 0, 1);

	  lineMinMax->GetXAxis()->SetTitle("Bin");
	  lineMinMax->SetInputArray(1, "DisplayMinMax");
	  // lineMinMax->SetInputArray(2, "DisplayMax");
	  vtkTextProperty *textPropMin = lineMinMax->GetXAxis()->GetLabelProperties();
	  //textPropMin->SetColor(0.0, 0.0, 1.0);
	  textPropMin = lineMinMax->GetXAxis()->GetTitleProperties();

	  lineMinMax->GetYAxis()->SetTitle("MinMax");
	  lineMinMax->SetColor(1.0, 0.0, 0.0);
	  //cout << "New Y range:" << 0 << "; " << this->maxbin << endl;
	  this->ChartXY->GetAxis(1)->SetRange(0, this->maxbin);
	  //this->ChartXY->SetAutoAxes(true);
	  this->ChartXY->SetAutoSize(true);
	  this->ChartXY->ForceAxesToBoundsOn();

	  
  }

  void reinitDisplayMinMax(double min, double max)
  {
	  this->cleanup();
	  double dMin = min;
	  double dMax = max;
	  //cout << "Reinit min and max to " << min << " and " << max << endl;
	  int numbins = bins->GetNumberOfTuples();
	  double spacing = this->Hist->GetComponentSpacing()[0];
	  double mbin = this->Hist->GetComponentOrigin()[0];
	  int aboveDMin = 0;
	  int aboveDMax = 0;
	  for (vtkIdType bin = 0; bin < numbins; bin++)
	  {
		  int ok = 1;		 
		  if (mbin >= dMin && aboveDMin < 1) { aboveDMin++;   
		  //this->displayMinMax->SetTuple1(bin, this->maxbin); 
		  this->displayMinMax->SetTuple1(bin, 1000);
		  
		  }
		  else if (mbin >= dMax && aboveDMax < 1) {
			  aboveDMax++;   
			  //this->displayMinMax->SetTuple1(bin, this->maxbin);
			  this->displayMinMax->SetTuple1(bin, 1000);
		  }
		  else { this->displayMinMax->SetTuple1(bin, 0); }
		  mbin += spacing;

	  }
	  this->drawAgain();

	  //this->ChartXY->GetAxis(0)->SetRange(min, max);
	  
	  // Remove Min from Plot
	  // Remove Max from Plot
	  // Draw Min
	  // Draw Max
  }
  void reinit(vtkImageAccumulate *hist, int numbins, double min, double max, double displayMin, double displayMax)
  {
	  //cette fonction prend beaucoup trop de temps... On ne va pas refaire l'histogramme à chaque fois qu'on clique sur shift.
	  //on va plutôt le calculer 1 fois (à l'ouverture du volume), avec un nombre de bins "suffisant" ( 255?)
	  // ensuite on va juste décaler l'affichage de l'histogramme calculé quand les bornes min et max changent.

	  double dMin = displayMin;
	  double dMax = displayMax;
	  if (displayMin <min || displayMin >max) { dMin = 0.33*(max - min); }
	  if (displayMax <max || displayMax <min) { dMax = 0.66*(max - min); }

	  this->cleanup();	  
	  
	  if (hist !=NULL)
	  {
		 // cout << "Start Histogram reinit" << endl;
		//  cout << "dMin = " << dMin << endl;
		//  cout << "dMax = " << dMax << endl;

		  this->Hist = hist;
		  this->Hist->Update();
		  
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
		 //cout << "numbins = " << numbins << endl;
		 // cout << "min = " << min << endl;
		 // cout << "max = " << max << endl;
		  //cout << "bin_spacing = " << bin_spacing << endl;
		  hist->SetComponentOrigin(min, 0, 0);		  
		  hist->SetComponentSpacing(bin_spacing, 0, 0);
		 // cout << "I guess this is mostly the update call..." << endl;
		  hist->Update();
		  //cout << "Was it?" << endl;
		//  cout << "Update ok... " << numbins << endl;
		  //this->bins = vtkSmartPointer<vtkDoubleArray>::New();
		  this->bins->SetNumberOfComponents(1);
		  this->bins->SetNumberOfTuples(numbins);
		  this->bins->SetName("Bins");
		 
		//  cout << "Start Display MinA" << endl;
		  //this->displayMinMax->remove =  vtkSmartPointer<vtkIntArray>::New();
		  this->displayMinMax->SetNumberOfComponents(1);
		  this->displayMinMax->SetNumberOfTuples(numbins);
		  this->displayMinMax->SetName("DisplayMinMax");

		  
		  //this->frequencies =  vtkSmartPointer<vtkIntArray>::New();
		  this->frequencies->SetNumberOfComponents(1);
		  this->frequencies->SetNumberOfTuples(numbins);
		  this->frequencies->SetName("Frequencies");

		  //this->logfrequencies = vtkSmartPointer<vtkIntArray>::New();
		  this->logfrequencies->SetNumberOfComponents(1);
		  this->logfrequencies->SetNumberOfTuples(numbins);
		  this->logfrequencies->SetName("Logs");
		  //*(static_cast<int*>(histogram->GetOutput()->GetScalarPointer(bin, 0, 0)))
		  //int* output = static_cast<int*>(this->Hist->GetOutput()->GetScalarPointer());
		  //int* output = static_cast<int*>(this->Hist->GetOutput()->GetScalarPointer());
		  double spacing = this->Hist->GetComponentSpacing()[0];
		  double mbin = this->Hist->GetComponentOrigin()[0];
		 //cout << "Frequencies:" << endl;

		  this->maxbin=0;
		  double maxlogbin=0;
		  int aboveDMin = 0;
		  int aboveDMax = 0;
		  //First pass to have extent.
		 // cout << "Start loop to find maxlogbin and maxbin" << endl;
		  for (vtkIdType bin = 0; bin < numbins; bin++)
		  {			  			  
			   //cout << "bin =" << bin << ", retrieving curbin"  << endl;
			  int curbin = 0;			
			  if (this->Hist->GetOutput()->GetScalarPointer(bin, 0, 0) !=NULL)
			  {
				  //cout << "output not null" << endl;
				  curbin = *(static_cast<int*>(this->Hist->GetOutput()->GetScalarPointer(bin, 0, 0)));
				  if (curbin > this->maxbin) { this->maxbin = curbin; }
				  double logcurbin = 0;
				  if (curbin > 0)
				  {
					  logcurbin = 100 * log10(curbin);
					  if (logcurbin > maxlogbin) { maxlogbin = logcurbin; }
				  }
			  }
			  else
			  {
				 // cout << "null output" << endl;
			  }
			  

		  }
		 // cout << "this->maxbin=" << this->maxbin << endl;
		//  cout << "maxlogbin=" << maxlogbin << endl;
		  //Second pass to "normalize".
		 // cout << "Start second loop to populate int arrays" << endl;
		  for (vtkIdType bin = 0; bin < numbins; bin++)
		  {
			  int ok = 1;
			  bins->SetTuple1(bin, bin);//bins->SetTuple1(bin, mbin);
			  if (mbin >= dMin && aboveDMin < 1) { 
				  aboveDMin++;   
				  //this->displayMinMax->SetTuple1(bin, this->maxbin); 
				  this->displayMinMax->SetTuple1(bin, 1000);
				//  cout << "Display Min (" << bin << "):" << this->maxbin<< endl;
			  }
			  else if (mbin >= dMax && aboveDMax < 1) { aboveDMax++;   
			  //this->displayMinMax->SetTuple1(bin, this->maxbin );
			  this->displayMinMax->SetTuple1(bin, 1000);
		//	  cout << "Display Max (" << bin << "):" << this->maxbin << endl;
			  }
			  else { this->displayMinMax->SetTuple1(bin, 0); }
			  


			  mbin += spacing;
			 // cout << "bin =" << bin << ", retrieving curbin"  << endl;
			  int curbin = 0;
			  int frequency = 0; // 0... 1000
			  int logcurbin = 0;
			  int logfrequency = 0;
			  int reslogcurbin = 0;
			  int reslogfrequency = 0; //0 ... 1000
			  //curbin =*output++;
			  if (this->Hist->GetOutput()->GetScalarPointer(bin, 0, 0) != NULL)
			  {
				  curbin = *(static_cast<int*>(this->Hist->GetOutput()->GetScalarPointer(bin, 0, 0)));
				  frequency =(int)( 1000 * (double)curbin / (double)this->maxbin);
				  if (curbin > 0) {
					  logcurbin = 100 * log10(curbin);
					  if (maxlogbin > 0)
					  {
						  double mult = (double)logcurbin * (double)this->maxbin;
						  double multdiv = mult / maxlogbin;
						  //logcurbin = (int)(multdiv);
						  
						  logfrequency = (int)((double)1000 *(double)logcurbin/(double)maxlogbin);
						  
					  }
				  }
				  //cout << "logcurbin=" << logcurbin << "curbin" << curbin << endl;
			  }

			  
			  

			
			 // cout << "logcurbin=" << log10(curbin) << endl;
				//curbin *(static_cast<int*>(this->Hist->GetOutput()->GetScalarPointer(bin, 0, 0)));
			  
			  if (curbin != 0)
			  {
				  //cout << curbin << endl;
			  }
			  if (ok == 1)
			  {
				  this->frequencies->SetTuple1(bin, frequency);
				  //this->frequencies->SetTuple1(bin, curbin);
				 // cout << "Frequency " << bin << ":" << curbin << endl;
			  }
			  else
			  {
				  this->frequencies->SetTuple1(bin, 0);
			  }
			 if (logfrequency>frequency)
			 {
				 reslogfrequency = logfrequency - frequency;
			 }
			  

			  if ((logcurbin - curbin) > 0) {
				  reslogcurbin = logcurbin - curbin;
				  
			
				  
			  }
			  //logfrequencies->SetTuple1(bin, logcurbin);
			  if (ok == 1)
			  {
				  //this->logfrequencies->SetTuple1(bin, reslogcurbin);
				  this->logfrequencies->SetTuple1(bin, reslogfrequency);
				  //cout << "Frequency(" << bin << "):" << curbin << "ResLog(" << bin << "):" << reslogcurbin << endl;
				  //cout << "Frequency(" << bin << "):" << curbin+ reslogcurbin  << endl;
				  //cout << "Frequency(" << bin << "):" << curbin + reslogcurbin << endl;
				  //cout << "frequency("<<bin<<")=" << frequency << ", logfrequency = " << logfrequency << ", reslogfrequency =" << reslogfrequency << endl;
			  }
			  else
			  {
				  this->logfrequencies->SetTuple1(bin, 0);
			  }
		  }

		 this->ChartXY->GetAxis(1)->SetRange(0, this->maxbin);
		this->drawAgain();
		  

		
		  		 		  
		
		 // cout << "End Histogram reinit" << endl;

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
	//this->numBins = 50;
	this->displayMin = 0;
	this->displayMax = 1;
	this->mHist = NULL;
}

//-----------------------------------------------------------------------------
mqHistogramWidget::~mqHistogramWidget()
{
  delete this->Internals;
  this->Internals = NULL;
}


double mqHistogramWidget::GetDisplayMin() { return this->displayMin; }
void mqHistogramWidget::SetDisplayMin(double newmin) 
{ 
	if (newmin < this->displayMax) 
	{ 
		this->displayMin = newmin; 	
	this->Internals->reinitDisplayMinMax(this->displayMin, this->displayMax);
	}

}
double mqHistogramWidget::GetDisplayMax() { 
	return this->displayMax;

}
void mqHistogramWidget::SetDisplayMax(double newmax)
{
	if (newmax > this->displayMin)
	{
		this->displayMax = newmax; 		
		this->Internals->reinitDisplayMinMax(this->displayMin, this->displayMax);
	}
}
void mqHistogramWidget::SetDisplayMinMax(double newmin, double newmax)
{
	if (newmin<newmax)
	{
		this->displayMax = newmax; this->displayMin = newmin;
		//this->Internals->reinit(this->mHist, this->numBins, this->min, this->max);
		this->Internals->reinitDisplayMinMax(this->displayMin, this->displayMax);
	}
}
//-----------------------------------------------------------------------------
void mqHistogramWidget::initialize(
	vtkImageAccumulate* hist, int numbins, double rangeMin, double rangeMax, double displayMin, double displayMax)
{
	//cout << "mqHistogramWidget Initialize " << endl;
  //this->Internals->cleanup();
  this->mHist = hist;
  this->displayMin = displayMin;
  this->displayMax = displayMax;

  this->Internals->reinit(hist, numbins, rangeMin, rangeMax, displayMin, displayMax);

  

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

