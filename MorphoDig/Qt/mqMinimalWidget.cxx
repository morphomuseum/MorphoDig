/*=========================================================================

   Program: MorphoDig 

========================================================================*/
#include "mqMinimalWidget.h"

#include "mqCoreUtilities.h"
//#include "pqQVTKWidgetBase.h"
//#include "pqTimer.h"
#include <QTimer>
//#include <QVTKOpenGLWidget.h>
#include <QVTKOpenGLNativeWidget.h>
//#include <QVTKWidget.h>
//#include <vtkAutoInit.h>
#include <vtkAxis.h>
#include <vtkFloatArray.h>
#include <vtkTable.h>
#include <vtkRenderer.h>
#include <vtkBoundingBox.h>
#include <vtkChartXY.h>
#include <vtkColorTransferControlPointsItem.h>
#include <vtkColorTransferFunction.h>
#include <vtkColorTransferFunctionItem.h>
#include <vtkCompositeControlPointsItem.h>
#include <vtkCompositeTransferFunctionItem.h>
//#include <vtkContext2D.h>
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

#include <vtkPen.h>
#include <vtkRenderWindowInteractor.h>

#if QT_VERSION >= 0x050000
#include <QSurfaceFormat>
#endif



//-----------------------------------------------------------------------------

class mqMinimalWidget::pqInternals
{
	vtkNew<vtkGenericOpenGLRenderWindow> Window;
	//vtkNew<vtkRenderer> Renderer;

public:
	QPointer<QVTKOpenGLNativeWidget> Widget;
	vtkNew<vtkChartXY> ChartXY;
	vtkNew<vtkContextView> ContextView;
	

	

	pqInternals(mqMinimalWidget* editor)
		: Widget(new QVTKOpenGLNativeWidget(editor))
		
	{
		//this->Window->AddRenderer(Renderer.Get());
		//this->Window->SetMultiSamples(0);
	
		QSurfaceFormat fmt = QSurfaceFormat::defaultFormat();
		fmt.setSamples(8);
		//fmt.setSamples(0);
		this->Widget->setFormat(fmt);
		this->Widget->setEnableHiDPI(true);
		cout << "mqMinimalWidget Widget constructor" << endl;
		//vtkSmartPointer<vtkGenericOpenGLRenderWindow> totoche
		//vtkSmartPointer<vtkGenericOpenGLRenderWindow> oneWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();

		
		cout << "mqMinimalWidget Widget constructor 1" << endl;
		vtkSmartPointer<vtkTable> table =
			vtkSmartPointer<vtkTable>::New();

		vtkSmartPointer<vtkFloatArray> arrX =
			vtkSmartPointer<vtkFloatArray>::New();
		arrX->SetName("X Axis");
		table->AddColumn(arrX);

		vtkSmartPointer<vtkFloatArray> arrC =
			vtkSmartPointer<vtkFloatArray>::New();
		arrC->SetName("Cosine");
		table->AddColumn(arrC);

		vtkSmartPointer<vtkFloatArray> arrS =
			vtkSmartPointer<vtkFloatArray>::New();
		arrS->SetName("Sine");
		table->AddColumn(arrS);

		// Fill in the table with some example values
		int numPoints = 69;
		float inc = 7.5 / (numPoints - 1);
		table->SetNumberOfRows(numPoints);
		for (int i = 0; i < numPoints; ++i)
		{
			table->SetValue(i, 0, i * inc);
			table->SetValue(i, 1, cos(i * inc));
			table->SetValue(i, 2, sin(i * inc));
		}

		//this->ChartXY = vtkSmartPointer<vtkChartXY>::New();
		cout << "mqMinimalWidget Widget constructor 6" << endl;
		vtkPlot *line = ChartXY->AddPlot(vtkChart::LINE);

		line->SetInputData(table, 0, 1);
		line->SetColor(0, 255, 0, 255);
		line->SetWidth(1.0);
		line = ChartXY->AddPlot(vtkChart::LINE);
		line->SetInputData(table, 0, 2);
		line->SetColor(255, 0, 0, 255);
		line->SetWidth(5.0);

		#ifndef WIN32
		line->GetPen()->SetLineType(vtkPen::DASH_LINE);
		#endif

		cout << "mqMinimalWidget Widget constructor 2" << endl;

		

		cout << "mqMinimalWidget Widget constructor 3" << endl;
		//this->Window = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
		//this->mren = vtkSmartPointer<vtkRenderer>::New();
		//Window->AddRenderer(mren);

		cout << "mqMinimalWidget Widget constructor 4" << endl;
		//this->Widget = new QVTKWidget(this);
		cout << "mqMinimalWidget Widget constructor 5" << endl;

		//this->ContextView = vtkSmartPointer<vtkContextView>::New();
		/* QSurfaceFormat fmt = QSurfaceFormat::defaultFormat();
		fmt.setSamples(8);
		Widget->setFormat(fmt);
		Widget->setEnableHiDPI(true);	*/

		this->Widget->setObjectName("1QVTKWidget0");
		cout << "mqMinimalWidget Widget constructor 7" << endl;
		this->Widget->SetRenderWindow(this->Window.Get());
		cout << "mqMinimalWidget Widget constructor 8" << endl;
		cout << "ContextView has a null renderer?" << endl;
		cout << ContextView->GetRenderer() << endl;
		this->ContextView->SetRenderWindow(this->Window.Get());
		//this->ContextView->SetRenderer(this->Renderer.Get());
		this->ContextView->GetRenderer()->SetBackground(1.0, 1.0, 1.0);
		cout << "mqMinimalWidget Widget constructor 9" << endl;
		this->ChartXY->SetAutoSize(true);
		this->ChartXY->SetShowLegend(true);
		this->ChartXY->SetForceAxesToBounds(true);
		this->ChartXY->Update();
		//this->ContextView->GetScene()->AddItem(this->ChartXY);
		cout << "mqMinimalWidget Widget constructor 10" << endl;
		//this->ContextView->SetInteractor(this->Widget->GetInteractor());
		cout << "mqMinimalWidget Widget constructor 11" << endl;

		this->Widget->setParent(editor);

		cout << "mqMinimalWidget Widget constructor 12" << endl;
		//QVBoxLayout* layout = new QVBoxLayout(editor);
		QVBoxLayout* layout = new QVBoxLayout();
		layout->setMargin(0);
		layout->addWidget(this->Widget);
		//this->Widget->setLayout(layout);
		editor->setLayout(layout);
		cout << "mqMinimalWidget Widget constructor 13" << endl;
	}
	~pqInternals() { this->cleanup(); }

	void cleanup()
	{
		this->ChartXY->ClearPlots();
		
	}
};



 // pqTimer Timer;
  

//-----------------------------------------------------------------------------
mqMinimalWidget::mqMinimalWidget(QWidget* parentObject)
  : Superclass(parentObject)
	, Internals(new pqInternals(this)) //@@ put it back to retest this class... 
 
{
	cout << "mqMinimalWidget Widget Try to render?" << endl;
	//this->Internals->ContextView->GetRenderer()->SetBackground(1.0, 1.0, 1.0);
	cout << "mqMinimalWidget Widget Try to render 2?" << endl;
	this->Internals->ContextView->GetInteractor()->Initialize();
	cout << "mqMinimalWidget Widget Try to render 3?" << endl;
	this->Internals->ContextView->GetInteractor()->Start();
	//this->Internals->ContextView->GetRenderWindow()->Render();

	cout << "Rendered or not ?" << endl;
	
	//ContextView->GetRenderer()->SetBackground(1.0, 0.0, 0.0);
//	ContextView->GetInteractor()->Initialize();
//	ContextView->GetInteractor()->Start();
	cout << "Is drawable?" << this->Internals->ContextView->GetRenderWindow()->IsDrawable() << endl;
	this->Internals->ContextView->GetRenderWindow()->Render();
  //QObject::connect(&this->Internals->Timer, SIGNAL(timeout()), this, SLOT(renderInternal()));
}

//-----------------------------------------------------------------------------
mqMinimalWidget::~mqMinimalWidget()
{
  delete this->Internals;
  this->Internals = NULL;
}

//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
void mqMinimalWidget::renderInternal()
{
	/*cout << "mqMinimalWidget renderInternal" << endl;
  if (this->isVisible() && this->Internals->ContextView->GetRenderWindow()->IsDrawable())
  {
    this->Internals->ContextView->GetRenderWindow()->Render();
  }*/
}
