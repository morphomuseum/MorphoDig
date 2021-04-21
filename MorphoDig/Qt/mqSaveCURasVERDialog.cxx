/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqSaveCURasVERDialog.h"
#include "ui_mqSaveCURasVERDialog.h"
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
#include <QTableWidgetItem>
#include <QSpinBox>
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
mqSaveCURasVERDialog::mqSaveCURasVERDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqSaveCURasVERDialog())
{

	this->Ui->setupUi(this);
	this->setObjectName("mqSaveCURasVERDialog");
	
	
	// This is where we 
  //
 
 
 
 this->Ui->defaultDecimation->setValue(mqMorphoDigCore::instance()->Getmui_SegmentDecimation());  //connecter à un slot qui veuile bien 

 int lmk_format = mqMorphoDigCore::instance()->Getmui_CURVERFormat();
 if (lmk_format ==0)
 {
	 this->Ui->VER->setChecked(true);
 }
 else if (lmk_format==1 )
 {
	 this->Ui->LMK->setChecked(true);
 }
 else if (lmk_format == 2)
 {
	 this->Ui->PTS->setChecked(true);
 }
 else 
 {
	 this->Ui->TPS->setChecked(true);
 }

 int include = mqMorphoDigCore::instance()->Getmui_CURVERIncludeNormalLandmarks();
 if (include == 1)
 {
	 this->Ui->All->setChecked(true);
 }
 else
 {
	 this->Ui->CUROnly->setChecked(true);
 }

 int milestones_once = mqMorphoDigCore::instance()->Getmui_CURVERExportMilestonesOnce();
 if (milestones_once == 1)
 {
	 this->Ui->milestonesOnce->setChecked(true);
 }
 else
 {
	 this->Ui->milestonesTwice->setChecked(true);
	 
 }

 /*

	
	int Getmui_DefaultCURVERExportMilestonesOnce();
	int Getmui_CURVERExportMilestonesOnce();
	void Setmui_CURVERExportMilestonesOnce(int milestones_once);
*/

 QHeaderView *header = this->Ui->tableWidget->horizontalHeader();
 header->setSectionResizeMode(QHeaderView::Stretch);

 

 
 this->RefreshDecimationTable();
  
 
 connect(this->Ui->defaultDecimation, SIGNAL(valueChanged(int)), this, SLOT(slotDefaultDecimationChanged(int)));
 connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotSaveCURasVERFile()));
 connect(this->Ui->reinitializeSegments, SIGNAL(pressed()), this, SLOT(slotReinitializeSegments()));

}




//-----------------------------------------------------------------------------
mqSaveCURasVERDialog::~mqSaveCURasVERDialog()
{

 //depending on what is 
	
  delete this->Ui;
}

void mqSaveCURasVERDialog::updateDecimation(int row, int newdecimation)
{
	int numStoredDecimations = (int)mqMorphoDigCore::instance()->Getmui_SegmentDecimations()->decimation.size();
	if (row > numStoredDecimations)
	{
		cout << "numSegs > numStoredDecimations" << endl;
		int to_complete = row - numStoredDecimations;
		for (int i = 0; i < to_complete; i++)
		{
			cout << "push_back" << i << endl;
			mqMorphoDigCore::instance()->Getmui_SegmentDecimations()->decimation.push_back(mqMorphoDigCore::instance()->Getmui_SegmentDecimation());
		}

	}
	mqMorphoDigCore::instance()->Getmui_SegmentDecimations()->decimation[row] = newdecimation;
	
	
}

void mqSaveCURasVERDialog::slotReinitializeSegments()
{
	this->RefreshDecimationTable(1);
}
void mqSaveCURasVERDialog::slotDecimationChanged(int newdecimation)
{

	QSpinBox *sb = (QSpinBox*)sender();
	for (int i = 0; i < this->Ui->tableWidget->rowCount(); i++)
	{

		int j = 1; // column 2 = segment decimation
		if (this->Ui->tableWidget->cellWidget(i, j) ==sb )
		{
			cout << "New decimation at row " << i << ", value=" << newdecimation << endl;
			this->updateDecimation(i, newdecimation);
		}
	}
}
void mqSaveCURasVERDialog::slotDefaultDecimationChanged(int newdecimation)
{
	mqMorphoDigCore::instance()->Setmui_SegmentDecimation(newdecimation);
}
void mqSaveCURasVERDialog::RefreshDecimationTable(int toDefault)
{
	//toDefault = 0 => all values set to "Default Decimation".
	//otherwise => populate with SegmentDecimations global table

	SignalBlocker2 tagTableSignalBlocker(this->Ui->tableWidget); //blocks signals when populating the table! Blocking will stop 
	this->Ui->tableWidget->clear();
	this->Ui->tableWidget->setColumnCount(2);

	QTableWidgetItem *header1 = new QTableWidgetItem();
	header1->setText("Segment");
	this->Ui->tableWidget->setHorizontalHeaderItem(0, header1);
	QTableWidgetItem *header2 = new QTableWidgetItem();
	header2->setText("Landmark number");
	this->Ui->tableWidget->setHorizontalHeaderItem(1, header2);
	
	int numSegs = mqMorphoDigCore::instance()->GetNumberOfCurveSegments();
	int numStoredDecimations = mqMorphoDigCore::instance()->Getmui_SegmentDecimations()->decimation.size();
	
	
	
	if (numSegs > 0)
	{
		this->Ui->tableWidget->setRowCount(numSegs);
		if (numSegs > numStoredDecimations)
		{
			cout << "numSegs > numStoredDecimations" << endl;
			int to_complete = numSegs - numStoredDecimations;
			for (int i=0; i<to_complete; i++)
			{
				cout << "push_back" << i << endl;
				mqMorphoDigCore::instance()->Getmui_SegmentDecimations()->decimation.push_back(mqMorphoDigCore::instance()->Getmui_SegmentDecimation());
			}
		}
	}
	QSpinBox *dcmSB;
	for (int i = 0; i < mqMorphoDigCore::instance()->Getmui_SegmentDecimations()->decimation.size(); i++) {
		
		
		//int dcm = (int)(100 * DCMTable->GetTableValue(i));
		
		dcmSB = new QSpinBox();
		dcmSB->setMinimum(0);
		dcmSB->setMaximum(100);
		if (toDefault == 0)
		{
			dcmSB->setValue(mqMorphoDigCore::instance()->Getmui_SegmentDecimations()->decimation.at(i));
		}
		else
		{
			int dcm = mqMorphoDigCore::instance()->Getmui_SegmentDecimation();
			dcmSB->setValue(dcm);
			mqMorphoDigCore::instance()->Getmui_SegmentDecimations()->decimation[i] = dcm;

		}

		QTableWidgetItem *item2 = new QTableWidgetItem;
		item2->setFlags(item2->flags() | Qt::ItemIsEditable);
		QString SegName = "Segment " + QString::number(i+1);
		item2->setText(SegName);


		this->Ui->tableWidget->setItem(i, 0, item2); // TAG NAME
													 //this->Ui->tableWidget->setCellWidget(i, 1, nom);

		this->Ui->tableWidget->setCellWidget(i, 1, dcmSB); //DCM value
		
		
		connect(dcmSB, SIGNAL(valueChanged(int)), this, SLOT(slotDecimationChanged(int)));


	}
	cout << "Done..." << endl;

}





void mqSaveCURasVERDialog::slotSaveCURasVERFile()
{
	cout << "Export Cur as Landmarks !" << endl;
	
	QString fileName;
	QString proposedName = "";
	vtkIdType num_meshes = mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfItems();
	if (num_meshes == 1)
	{
		mqMorphoDigCore::instance()->getActorCollection()->InitTraversal();
		vtkMDActor *myActor = vtkMDActor::SafeDownCast(mqMorphoDigCore::instance()->getActorCollection()->GetNextActor());
		proposedName += QDir::separator();
		proposedName += myActor->GetName().c_str();
	}
	else
	{
		vtkIdType num_selected_meshes = mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfSelectedActors();
		if (num_selected_meshes == 1)
		{
			mqMorphoDigCore::instance()->ComputeSelectedNamesLists();
			proposedName += QDir::separator();
			proposedName += +mqMorphoDigCore::instance()->g_distinct_selected_names.at(0).c_str();
		}
	}
	if (this->Ui->VER->isChecked())
	{
		fileName = QFileDialog::getSaveFileName(mqMorphoDigCore::instance()->GetMainWindow(),
			tr("Export as .VER file"), mqMorphoDigCore::instance()->Getmui_LastUsedDir()+proposedName,
			tr("VER file (*.ver)"), NULL
			//, QFileDialog::DontConfirmOverwrite
		);
		

	}
	else if (this->Ui->LMK->isChecked())
	{
		fileName = QFileDialog::getSaveFileName(mqMorphoDigCore::instance()->GetMainWindow(),
			tr("Export as LMK file"), mqMorphoDigCore::instance()->Getmui_LastUsedDir()+proposedName,
			tr("LMK file (*.lmk)"), NULL
			//, QFileDialog::DontConfirmOverwrite
		);
		
	}
	else if (this->Ui->PTS->isChecked())
	{
		fileName = QFileDialog::getSaveFileName(mqMorphoDigCore::instance()->GetMainWindow(),
			tr("Export as PTS file"), mqMorphoDigCore::instance()->Getmui_LastUsedDir() + proposedName,
			tr("PTS file (*.pts)"), NULL
			//, QFileDialog::DontConfirmOverwrite
		);
		
	}
	else if (this->Ui->TPS->isChecked())
	{
		fileName = QFileDialog::getSaveFileName(mqMorphoDigCore::instance()->GetMainWindow(),
			tr("Export as TPS file"), mqMorphoDigCore::instance()->Getmui_LastUsedDir() + proposedName,
			tr("TPS file (*.tps)"), NULL
			//, QFileDialog::DontConfirmOverwrite
		);
		
	}
	if (fileName.isEmpty()) return;
	QFileInfo fileInfo(fileName);
	mqMorphoDigCore::instance()->Setmui_LastUsedDir(fileInfo.path());


	int save_format = 0; //0 .VER
	int save_other_lmks = 0; //0 no, 1 save also Normal and Target landmarks
	

	if (this->Ui->VER->isChecked()) { save_format = 0; }
	else if (this->Ui->LMK->isChecked()) { save_format = 1; }
	else if (this->Ui->PTS->isChecked()) { save_format = 2; }
	else if (this->Ui->TPS->isChecked()) { save_format = 3; }
	mqMorphoDigCore::instance()->Setmui_CURVERFormat(save_format);

	if (this->Ui->All->isChecked()) { save_other_lmks = 1; }
	mqMorphoDigCore::instance()->Setmui_CURVERIncludeNormalLandmarks(save_other_lmks);

	int decimation = this->Ui->defaultDecimation->value();
	int milestonesOnce = this->Ui->milestonesOnce->isChecked();
	mqMorphoDigCore::instance()->Setmui_CURVERExportMilestonesOnce(milestonesOnce);

	mqMorphoDigCore::instance()->SaveCURasVERFile(fileName, decimation, save_format, save_other_lmks, milestonesOnce);

}



//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

