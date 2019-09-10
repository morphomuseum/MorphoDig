/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqScreenshotDialog.h"
#include "ui_mqScreenshotDialog.h"
#include "MorphoDigVersion.h"
#include "mqMorphoDigCore.h"
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
mqScreenshotDialog::mqScreenshotDialog(QWidget* Parent, QString fileName)
  : QDialog(Parent)
  , Ui(new Ui::mqScreenshotDialog())
{
	this->Ui->setupUi(this);
	this->m_fileName = fileName;
	this->setObjectName("mqScreenshotDialog");	
	
	
  
	 connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotScreenshot()));

}




//-----------------------------------------------------------------------------
mqScreenshotDialog::~mqScreenshotDialog()
{

 //depending on what is 
	
  delete this->Ui;
}
void mqScreenshotDialog::Screenshot()
{
	cout << "Screenshot dialog" << endl;
	
	
		std::string action = "Screenshot selected actors";
		cout << "Screenshot!!!!" << endl;
		mqMorphoDigCore::instance()->Screenshot(this->m_fileName, this->Ui->magnificationX->value(), this->Ui->magnificationY->value(), this->Ui->RGBA->isChecked(), this->Ui->front->isChecked(), this->Ui->transparent->isChecked());
		mqMorphoDigCore::instance()->Render();
	
}







void mqScreenshotDialog::slotScreenshot()
{
	this->Screenshot();
}


//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

