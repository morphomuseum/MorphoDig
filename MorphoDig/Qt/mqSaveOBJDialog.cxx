/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqSaveOBJDialog.h"
#include "ui_mqSaveOBJDialog.h"
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
mqSaveOBJDialog::mqSaveOBJDialog(QWidget* Parent, QString fileName)
  : QDialog(Parent)
  , Ui(new Ui::mqSaveOBJDialog())
{

	this->Ui->setupUi(this);
	this->setObjectName("mqSaveOBJDialog");
	this->m_fileName = fileName;
	// This is where we 
  //
 
 
 this->Ui->PositionOriginal->setChecked(true);
 
  // Should connect...
  
 connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotSaveOBJFile()));

}




//-----------------------------------------------------------------------------
mqSaveOBJDialog::~mqSaveOBJDialog()
{

 //depending on what is 
	
  delete this->Ui;
}
void mqSaveOBJDialog::slotSaveOBJFile()
{
	cout << "OBJ File Saved!" << endl;
	int position_mode = 0; // 0 Original position , 1 Modified position
	int file_type = 3; // 0 STL 1 VTK, 2 PLY, 3 OBJ, 4 VTP
	int save_norms = 0; //0 no, 1 yes
	int write_type = 2;//0 binary LE, 1 binary BE, 2 ASCII 

	if (this->Ui->PositionModified->isChecked()) { position_mode = 1; }
	else if (this->Ui->PositionOriginal->isChecked()) { position_mode = 0; }
	
	std::vector<std::string> mscalarsToBeRemoved;
	
	int RGBopt = 0;
	
	mqMorphoDigCore::instance()->SaveSurfaceFile(this->m_fileName, write_type, position_mode, file_type, mscalarsToBeRemoved,RGBopt, save_norms);

}



//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

