/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqScalarsInfosAlongDirectionDialog.h"
#include "ui_mqScalarsInfosAlongDirectionDialog.h"
#include "MorphoDigVersion.h"
#include "mqMorphoDigCore.h"
#include "mqUndoStack.h"


// we actually do not need glew...
//#include <GL/glew.h>
#include <QApplication>
#include <QFile>
#include <QRadioButton>
#include <QFileDialog>
#include <QProgressBar>
#include <QMessageBox>
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
mqScalarsInfosAlongDirectionDialog::mqScalarsInfosAlongDirectionDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqScalarsInfosAlongDirectionDialog())
{
	this->Ui->setupUi(this);
	this->Ui->absoluteMin->setMaximum(DBL_MAX);
	this->Ui->absoluteMin->setMinimum(-DBL_MAX);
	this->Ui->absoluteMax->setMaximum(DBL_MAX);
	this->Ui->absoluteMax->setMinimum(-DBL_MAX);
	this->setObjectName("mqScalarsInfosDialog");	
	QString ActiveScalar = mqMorphoDigCore::instance()->Getmui_ActiveArray()->Name;
	
	this->PopulateComboTags();
	this->Ui->comboTags->setDisabled(true);
	if (this->Ui->comboTags->count()==0)
	{ 
		this->Ui->includeTags->setDisabled(true);
	}

	this->Ui->activeScalarName->setText(ActiveScalar.toStdString().c_str());
	
	this->Ui->comboTags->setSizeAdjustPolicy(QComboBox::AdjustToContents);
  
	 //connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(sloteditInfos()));
	 connect(this->Ui->ok, SIGNAL(pressed()), this, SLOT(sloteditInfos()));
	 connect(this->Ui->includeTags, SIGNAL(pressed()), this, SLOT(slotIncludeTags()));
	 connect(this->Ui->cancel, SIGNAL(pressed()), this, SLOT(slotClose()));
	 connect(this->Ui->absMinMax, SIGNAL(pressed()), this, SLOT(slotabsMinMax()));
	 connect(this->Ui->relMinMax, SIGNAL(pressed()), this, SLOT(slotrelMinMax()));
	 connect(this->Ui->radioX, SIGNAL(pressed()), this, SLOT(slotrX()));
	 connect(this->Ui->radioY, SIGNAL(pressed()), this, SLOT(slotrY()));
	 connect(this->Ui->radioZ, SIGNAL(pressed()), this, SLOT(slotrZ()));
	 cout << "updateAbsMinMax" << endl;
	 this->updateAbsMinMax(2);
	
}




//-----------------------------------------------------------------------------
mqScalarsInfosAlongDirectionDialog::~mqScalarsInfosAlongDirectionDialog()
{

 //depending on what is 
	
  delete this->Ui;
}
void mqScalarsInfosAlongDirectionDialog::updateAbsMinMax(int direction)
{
	//direction 0 x, 1 Y, 2 z
	cout << "Update abs min max" << endl;
	double curMin = 0;
	double curMax = 1;

	double bbSelected[6];
	mqMorphoDigCore::instance()->getActorCollection()->GetBoundingBoxSelected(bbSelected);
	if (direction==0)
	{
		curMin = bbSelected[0];
		curMax = bbSelected[1];

	}
	else if (direction==1)
	{
		curMin = bbSelected[2];
		curMax = bbSelected[3];
	}
	else
	{
		curMin = bbSelected[4];
		curMax = bbSelected[5];
	}
	this->Ui->absoluteMin->setValue(curMin);
	this->Ui->absoluteMax->setValue(curMax);
}
void mqScalarsInfosAlongDirectionDialog::PopulateComboTags()
{

	this->Ui->comboTags->clear();
	ExistingArrays *MyList = mqMorphoDigCore::instance()->Getmui_ExistingArrays();
	for (int i = 0; i < MyList->Stack.size(); i++)
	{
		if ((MyList->Stack.at(i).DataType == VTK_INT || MyList->Stack.at(i).DataType == VTK_UNSIGNED_INT) && MyList->Stack.at(i).NumComp == 1)
		{
			this->Ui->comboTags->addItem(MyList->Stack.at(i).Name);
		}

	}
	

}

void mqScalarsInfosAlongDirectionDialog::slotrX()
{
	this->updateAbsMinMax(0);
}
void mqScalarsInfosAlongDirectionDialog::slotrY()
{
	this->updateAbsMinMax(1);
}
void mqScalarsInfosAlongDirectionDialog::slotrZ()
{
	this->updateAbsMinMax(2);
}
void mqScalarsInfosAlongDirectionDialog::slotIncludeTags()
{
	if (this->Ui->includeTags->isChecked())
	{
		this->Ui->comboTags->setDisabled(true);
	}
	else
	{
		this->Ui->comboTags->setDisabled(false);
	}
}
void mqScalarsInfosAlongDirectionDialog::slotrelMinMax()
{
	
	
		this->Ui->absoluteMin->setDisabled(false);
		this->Ui->absoluteMax->setDisabled(false);
	
}
void mqScalarsInfosAlongDirectionDialog::slotabsMinMax()
{
	cout << "abs checked:" << this->Ui->absMinMax->isChecked() << endl;
	
		this->Ui->absoluteMin->setEnabled(true);
		this->Ui->absoluteMax->setEnabled(true);
	
	
}

void mqScalarsInfosAlongDirectionDialog::editInfos()
{
	cout << "Edit Infos" << endl;
	this->Ui->cancel->setDisabled(true);
	
	int numsel = mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfSelectedActors();
	if (numsel == 0) {
		QMessageBox msgBox;
		msgBox.setText("No surface selected. Please select at least one surface to use this option.");
		msgBox.exec();
		return;
	}
	if (numsel>0 && mqMorphoDigCore::instance()->Getmui_ActiveArray()->NumComp == 1 &&

		(mqMorphoDigCore::instance()->Getmui_ActiveArray()->DataType == VTK_FLOAT
			|| mqMorphoDigCore::instance()->Getmui_ActiveArray()->DataType == VTK_DOUBLE
			)
		)

	{

		QString activeScalar = mqMorphoDigCore::instance()->Getmui_ActiveArray()->Name;
		QString myText;

		myText = tr("Save summary of scalars ") + activeScalar;

		int direction = 0;
		if (this->Ui->radioX->isChecked()) { direction = 0; }
		if (this->Ui->radioY->isChecked()) { direction = 1; }
		if (this->Ui->radioZ->isChecked()) { direction = 2; }
		QString myDirection;
		if (direction == 0) { myDirection = tr("_along_x"); }
		else if (direction == 1) { myDirection = tr("_along_y"); }
		else { myDirection = tr("_along_z"); }
		QString fileName = QFileDialog::getSaveFileName(mqMorphoDigCore::instance()->GetMainWindow(),
			myText, mqMorphoDigCore::instance()->Getmui_LastUsedDir() + QDir::separator() + activeScalar + "_summary"+myDirection+".txt",
			tr("text file (*.txt)"));

		cout << fileName.toStdString() << endl;;
		if (fileName.isEmpty()) return;
		QFileInfo fileInfo(fileName);
		mqMorphoDigCore::instance()->Setmui_LastUsedDir(fileInfo.path());


		std::string TXText = ".txt";
		std::string TXText2 = ".TXT";
		std::size_t found = fileName.toStdString().find(TXText);
		std::size_t found2 = fileName.toStdString().find(TXText2);
		if (found == std::string::npos && found2 == std::string::npos)
		{
			fileName.append(".txt");
		}
		
		mqMorphoDigCore::instance()->SaveActiveScalarSummaryAlongDirection(
			fileName, 
			this->Ui->includeTags->isChecked(), 
			this->Ui->comboTags->currentText(),direction, 
			this->Ui->numValues->value(), 
			this->Ui->relMinMax->isChecked(), 
			this->Ui->absoluteMin->value(), this->Ui->absoluteMax->value());
	}
	else
	{
		QMessageBox msgBox;
		msgBox.setText("Select at least one surface and activate existing scalars to use this option");
		msgBox.exec();
		return;

	}

}



void mqScalarsInfosAlongDirectionDialog::slotClose()
{
	
	this->close();
}


void mqScalarsInfosAlongDirectionDialog::sloteditInfos()
{
	//cout << "Set visible true!!!" << endl;
	this->editInfos();
	this->close();
}


//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

