/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqSaveMAPDialog.h"
#include "ui_mqSaveMAPDialog.h"
#include "MorphoDigVersion.h"
#include "mqMorphoDigCore.h"

// we actually do not need glew...
//#include <GL/glew.h>
#include <QApplication>
#include <QFile>
#include <QRadioButton>
#include <QFileDialog>
#include <QCheckBox>
#include <QHeaderView>


#include <sstream>

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
mqSaveMAPDialog::mqSaveMAPDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqSaveMAPDialog())
{

	this->Ui->setupUi(this);
	this->setObjectName("mqSaveMAPDialog");
	
	
	// This is where we 
  //
 
// this->Ui->SaveAll->setChecked(true);
 
  // Should connect...
  
 connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotSaveMAPFile()));

}




//-----------------------------------------------------------------------------
mqSaveMAPDialog::~mqSaveMAPDialog()
{

 //depending on what is 
	
  delete this->Ui;
}
void mqSaveMAPDialog::slotSaveMAPFile()
{
	cout << "MAP File Saved!" << endl;
	
	QString fileName;
	QString proposedName = "custom_colormaps";
	if (this->Ui->SaveAll->isChecked())
	{
		proposedName = "all_colormaps";
	}
	if (this->Ui->SaveActive->isChecked())
	{
		proposedName = mqMorphoDigCore::instance()->Getmui_ActiveColorMap()->Name;
	}
	proposedName += QDir::separator();

		fileName = QFileDialog::getSaveFileName(mqMorphoDigCore::instance()->GetMainWindow(),
			tr("Save MAP files"), mqMorphoDigCore::instance()->Getmui_LastUsedDir() + proposedName,
			tr("MAP file (*.map)"), NULL
			//, QFileDialog::DontConfirmOverwrite
			);
	

	
	
	if (fileName.isEmpty()) return;
	QFileInfo fileInfo(fileName);
	mqMorphoDigCore::instance()->Setmui_LastUsedDir(fileInfo.path());

	int save_only_active = 0; //0 no= save all landmark, 1 yes, save only selected landmarks
	
	

	if (this->Ui->SaveActive->isChecked()) { save_only_active = 1; }
	else if (this->Ui->SaveAll->isChecked()) { save_only_active = 0; }//save all
	else { save_only_active = 2; }// save customs

	

	mqMorphoDigCore::instance()->SaveMAPFile(fileName, save_only_active);

}



//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

