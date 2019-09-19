/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqRendererOptionsDialog.h"
#include "ui_mqRendererOptionsDialog.h"
#include "MorphoDigVersion.h"
#include "mqMorphoDigCore.h"
#include "mqUndoStack.h"
#include "vtkLMActor.h"
#include "vtkLMActorCollection.h"
#include <vtkRenderer.h>
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
mqRendererOptionsDialog::mqRendererOptionsDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqRendererOptionsDialog())
{
	this->Ui->setupUi(this);
	this->setObjectName("mqRendererOptionsDialog");	
	int numPeels= mqMorphoDigCore::instance()->Getmui_RendererMaximalNumberOfPeels();
	this->Ui->numPeels->setValue(numPeels);
	
	double occlusionRatio = mqMorphoDigCore::instance()->Getmui_RendererOcclusionRatio();
	this->Ui->occlusionRatio->setValue(occlusionRatio);

	 connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotEditRendererOptions()));
	 connect(this->Ui->reinit, SIGNAL(clicked()), this, SLOT(slotReinitialize()));
	 	 
	
}




//-----------------------------------------------------------------------------
mqRendererOptionsDialog::~mqRendererOptionsDialog()
{

 //depending on what is 
	
  delete this->Ui;
}

void mqRendererOptionsDialog::editRendererOptions()
{
	
	
	
	mqMorphoDigCore::instance()->SetOcclusionRatioAndNumPeels(this->Ui->occlusionRatio->value(), this->Ui->numPeels->value());
	
}

void mqRendererOptionsDialog::slotEditRendererOptions()
{
	this->editRendererOptions();
}

void mqRendererOptionsDialog::slotReinitialize()
{
	int defaultNumPeels = mqMorphoDigCore::instance()->Getmui_DefaultRendererMaximalNumberOfPeels();
	this->Ui->numPeels->setValue(defaultNumPeels);

	double defaultOcclusionRatio = mqMorphoDigCore::instance()->Getmui_DefaultRendererOcclusionRatio();
	this->Ui->occlusionRatio->setValue(defaultOcclusionRatio);


	mqMorphoDigCore::instance()->SetOcclusionRatioAndNumPeels(defaultOcclusionRatio, defaultNumPeels);

}

//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

