/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqSaveVTKDialog.h"
#include "ui_mqSaveVTKDialog.h"
#include "MorphoDigVersion.h"
#include "mqMorphoDigCore.h"

// we actually do not need glew...
//#include <GL/glew.h>
#include <QApplication>
#include <QFile>
#include <QRadioButton>
#include <QCheckBox>
#include <QHeaderView>
#include <QListWidget>


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
mqSaveVTKDialog::mqSaveVTKDialog(QWidget* Parent, QString fileName)
  : QDialog(Parent)
  , Ui(new Ui::mqSaveVTKDialog())
{

	this->Ui->setupUi(this);
	this->setObjectName("mqSaveVTKDialog");
	this->m_fileName = fileName;
	// This is where we 
  //
 
 
 this->Ui->Binary->setChecked(true);
 this->Ui->PositionOriginal->setChecked(true);
 this->Ui->scalarList->clear();
 ExistingScalars *MyList = mqMorphoDigCore::instance()->Getmui_ScalarsOfSelectedObjects(0);
 QString none = QString("Solid color");
 for (int i = 0; i < MyList->Stack.size(); i++)
 {
	 //if ((MyList->Stack.at(i).DataType == VTK_FLOAT || MyList->Stack.at(i).DataType == VTK_DOUBLE || MyList->Stack.at(i).DataType == VTK_INT) && MyList->Stack.at(i).NumComp == 1)
	// {


	 	 if (MyList->Stack.at(i).Name != none)
		 {
			 QListWidgetItem* item = new QListWidgetItem(MyList->Stack.at(i).Name, this->Ui->scalarList);
			 item->setFlags(item->flags() | Qt::ItemIsUserCheckable); // set checkable flag
			 item->setCheckState(Qt::Checked);
		 }
		
	// }

 }
 /*
 
 this->Ui->comboActiveScalar->clear();
	ExistingScalars *MyList = mqMorphoDigCore::instance()->Getmui_ExistingScalars();
	for (int i = 0; i < MyList->Stack.size(); i++)
	{
		if ((MyList->Stack.at(i).DataType == VTK_FLOAT || MyList->Stack.at(i).DataType == VTK_DOUBLE) && MyList->Stack.at(i).NumComp == 1)
		{
			this->Ui->comboActiveScalar->addItem(MyList->Stack.at(i).Name);
		}

	}
 */
 //QListWidget *scalarList = 
 /*
     QListWidgetItem* item = new QListWidgetItem("item", listWidget);
    item->setFlags(item->flags() | Qt::ItemIsUserCheckable); // set checkable flag
    item->setCheckState(Qt::Unchecked); // AND initialize check state*/
  // Should connect...
  
 connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotSaveVTKFile()));

}




//-----------------------------------------------------------------------------
mqSaveVTKDialog::~mqSaveVTKDialog()
{

 //depending on what is 
	
  delete this->Ui;
}
void mqSaveVTKDialog::slotSaveVTKFile()
{
	cout << "Save VTK File!" << endl;
	int position_mode = 0; // 0 Original position , 1 Modified position
	int file_type = 1; // 0 STL 1 VTK/VTP, 2 PLY
	int save_norms = 0; //0 no, 1 yes
	int write_type = 0;//0 binary LE, 1 binary BE, 2 ASCII
	if (this->Ui->Binary->isChecked()) { write_type = 0; }	
	else if (this->Ui->ASCII->isChecked()) { write_type = 2; }

	if (this->Ui->PositionModified->isChecked()) { position_mode = 1; }
	else if (this->Ui->PositionOriginal->isChecked()) { position_mode = 0; }
	
	//now retrieve all checked scalars
	std::vector<std::string> mscalarsToBeRemoved;
	for (int i = 0; i < this->Ui->scalarList->count(); i++)
	{
		if (this->Ui->scalarList->item(i)->checkState() == Qt::Unchecked)
		{
			mscalarsToBeRemoved.push_back(this->Ui->scalarList->item(i)->text().toStdString());
		}
	}
	 
	int RGBopt = 0; //Keep RGB if exists... as RGB scalars can be handled totally via windows menus etc... 
	
	mqMorphoDigCore::instance()->SaveSurfaceFile(this->m_fileName, write_type, position_mode, file_type, mscalarsToBeRemoved,RGBopt, save_norms);

}



//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

