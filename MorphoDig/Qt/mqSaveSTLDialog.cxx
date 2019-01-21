/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqSaveSTLDialog.h"
#include "ui_mqSaveSTLDialog.h"
#include "MorphoDigVersion.h"
#include "mqMorphoDigCore.h"

// we actually do not need glew...
//#include <GL/glew.h>
#include <QApplication>
#include <QFile>
#include <QRadioButton>
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
mqSaveSTLDialog::mqSaveSTLDialog(QWidget* Parent, QString fileName)
  : QDialog(Parent)
  , Ui(new Ui::mqSaveSTLDialog())
{

	this->Ui->setupUi(this);
	this->setObjectName("mqSaveSTLDialog");
	this->m_fileName = fileName;
	// This is where we 
  //
 
 
 this->Ui->Binary->setChecked(true);
 this->Ui->PositionOriginal->setChecked(true);
 
  // Should connect...
  
 connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotSaveSTLFile()));

}




//-----------------------------------------------------------------------------
mqSaveSTLDialog::~mqSaveSTLDialog()
{

 //depending on what is 
	
  delete this->Ui;
}
void mqSaveSTLDialog::slotSaveSTLFile()
{
	cout << "STL File Saved!" << endl;
	int position_mode = 0; // 0 Original position , 1 Modified position
	int file_type = 0; // 0 STL 1 VTK/VTP, 2 PLY
	int save_norms = 0; //0 no, 1 yes
	int write_type = 0;//0 binary LE, 1 binary BE, 2 ASCII 
	if (this->Ui->Binary->isChecked()) { write_type = 0; }
	else if (this->Ui->ASCII->isChecked()) { write_type = 2; }

	if (this->Ui->PositionModified->isChecked()) { position_mode = 1; }
	else if (this->Ui->PositionOriginal->isChecked()) { position_mode = 0; }
	
	std::vector<std::string> mscalarsToBeRemoved;
	
	int RGBopt = 0;
	
	mqMorphoDigCore::instance()->SaveSurfaceFile(this->m_fileName, write_type, position_mode, file_type, mscalarsToBeRemoved,RGBopt, save_norms);

}



//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

