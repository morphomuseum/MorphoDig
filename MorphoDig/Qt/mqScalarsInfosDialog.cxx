/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqScalarsInfosDialog.h"
#include "ui_mqScalarsInfosDialog.h"
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
mqScalarsInfosDialog::mqScalarsInfosDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqScalarsInfosDialog())
{
	this->Ui->setupUi(this);
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
	 
	
}




//-----------------------------------------------------------------------------
mqScalarsInfosDialog::~mqScalarsInfosDialog()
{

 //depending on what is 
	
  delete this->Ui;
}

void mqScalarsInfosDialog::PopulateComboTags()
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


void mqScalarsInfosDialog::slotIncludeTags()
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
void mqScalarsInfosDialog::editInfos()
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




		QString fileName = QFileDialog::getSaveFileName(mqMorphoDigCore::instance()->GetMainWindow(),
			myText, mqMorphoDigCore::instance()->Getmui_LastUsedDir() + QDir::separator() + activeScalar + "_summary.txt",
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

		mqMorphoDigCore::instance()->SaveActiveScalarSummary(fileName, this->Ui->includeTags->isChecked(), this->Ui->comboTags->currentText());
	}
	else
	{
		QMessageBox msgBox;
		msgBox.setText("Select at least one surface and activate existing scalars to use this option");
		msgBox.exec();
		return;

	}

}



void mqScalarsInfosDialog::slotClose()
{
	
	this->close();
}


void mqScalarsInfosDialog::sloteditInfos()
{
	//cout << "Set visible true!!!" << endl;
	this->editInfos();
	this->close();
}


//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

