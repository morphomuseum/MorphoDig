/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqMinimalDialog.h"
#include "ui_mqMinimalDialog.h"
#include "MorphoDigVersion.h"
#include "mqMinimalWidget.h"
#include "mqMorphoDigCore.h"
#include <vtkRenderer.h>
#include <vtkElevationFilter.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkVectorText.h>
#include "mqUndoStack.h"
#include "vtkLMActor.h"
#include "vtkLMActorCollection.h"

// we actually do not need glew...
//#include <GL/glew.h>
#include <QApplication>
#include <QFile>
#include <QRadioButton>
#include <QFileDialog>
#include <QCheckBox>
#include <QHeaderView>


#include <sstream>

#define NORMAL_NODE 0
#define STARTING_NODE 1
#define MILESTONE_NODE 2
#define CONNECT_NODE 3


// Check windows
#if _WIN32 || _WIN64
#if _WIN64
#define ENVIRONMENT "64"
#else
#define ENVIRONMENT "32"
#endif
#endif

// Check GCC
#if __GNUC__
#if __x86_64__ || __ppc64__
#define ENVIRONMENT "64"
#else
#define ENVIRONMENT "32"
#endif
#endif

//-----------------------------------------------------------------------------
mqMinimalDialog::mqMinimalDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqMinimalDialog())
{
	this->Ui->setupUi(this);
	this->setObjectName("mqMinimalDialog");	
	
	
	//mqMinimalWidget *minimal = new mqMinimalWidget(this->Ui->PropertiesFrame);
	//mqMinimalWidget *minimal = new mqMinimalWidget(mqMorphoDigCore::instance()->GetMainWindow());

	//cout << "Try that!" << endl;
	//this->mMinimal = minimal;
	this->mMinimal = NULL;

	vtkNew<vtkVectorText> text;
	text->SetText("VTK and Qt!");
	vtkNew<vtkElevationFilter> elevation;
	elevation->SetInputConnection(text->GetOutputPort());
	elevation->SetLowPoint(0, 0, 0);
	elevation->SetHighPoint(10, 0, 0);

	// Mapper
	vtkNew<vtkPolyDataMapper> mapper;
	mapper->SetInputConnection(elevation->GetOutputPort());

	// Actor in scene
	vtkNew<vtkActor> actor;
	actor->SetMapper(mapper);

	// VTK Renderer
	vtkNew<vtkRenderer> ren;

	// Add Actor to renderer
	ren->AddActor(actor);

	// VTK/Qt wedded
	vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
	this->Ui->qvtkwidget->SetRenderWindow(renderWindow);
	this->Ui->qvtkwidget->GetRenderWindow()->AddRenderer(ren);

	//this->Ui->qvtkwidget;
	 connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotMinimal()));

}




//-----------------------------------------------------------------------------
mqMinimalDialog::~mqMinimalDialog()
{

 //depending on what is 
	cout << "Deleted????" << endl;
  delete this->Ui;
}
void mqMinimalDialog::Minimal()
{
	cout << "Minimal dialog" << endl;
	this->hide();
	
}







void mqMinimalDialog::slotMinimal()
{
	this->Minimal();
}


//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

