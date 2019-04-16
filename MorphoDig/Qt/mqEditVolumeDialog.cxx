/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqEditVolumeDialog.h"

#include "ui_mqEditVolumeDialog.h"
#include "MorphoDigVersion.h"
#include "mqMorphoDigCore.h"
#include "mqUndoStack.h"
#include "vtkMDVolume.h"
#include "vtkMDVolumeCollection.h"
#include <vtkMatrix4x4.h>

// we actually do not need glew...
//#include <GL/glew.h>
#include <QApplication>
#include <QFile>
#include <QRadioButton>
#include <QFileDialog>
#include <QCheckBox>
#include <QHeaderView>
#include <QListWidgetItem>
#include <QInputDialog>


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
mqEditVolumeDialog::mqEditVolumeDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqEditVolumeDialog())
{

	this->Ui->setupUi(this);
	this->setObjectName("mqEditVolumeDialog");
	connect(mqMorphoDigCore::instance(), SIGNAL(volumeSelectionChanged()), this, SLOT(slotRefreshDialog()));
	connect(this->Ui->next, SIGNAL(pressed()), this, SLOT(slotGetNextVolume()));
	connect(this->Ui->prec, SIGNAL(pressed()), this, SLOT(slotGetPrecedingVolume()));
	
	this->Volume_Coll = NULL;
	this->Volume = NULL;

	QString mylabel("...");
	this->Ui->VolumeName->setText(mylabel);
	
	
	

	

	this->GetFirstSelectedVolume();
	
	
	this->Ui->M00->setButtonSymbols(QAbstractSpinBox::NoButtons);
	this->Ui->M01->setButtonSymbols(QAbstractSpinBox::NoButtons);
	this->Ui->M02->setButtonSymbols(QAbstractSpinBox::NoButtons);
	this->Ui->M03->setButtonSymbols(QAbstractSpinBox::NoButtons);
	this->Ui->M10->setButtonSymbols(QAbstractSpinBox::NoButtons);
	this->Ui->M11->setButtonSymbols(QAbstractSpinBox::NoButtons);
	this->Ui->M12->setButtonSymbols(QAbstractSpinBox::NoButtons);
	this->Ui->M13->setButtonSymbols(QAbstractSpinBox::NoButtons);
	this->Ui->M20->setButtonSymbols(QAbstractSpinBox::NoButtons);
	this->Ui->M21->setButtonSymbols(QAbstractSpinBox::NoButtons);
	this->Ui->M22->setButtonSymbols(QAbstractSpinBox::NoButtons);
	this->Ui->M23->setButtonSymbols(QAbstractSpinBox::NoButtons);
	this->Ui->M30->setButtonSymbols(QAbstractSpinBox::NoButtons);
	this->Ui->M31->setButtonSymbols(QAbstractSpinBox::NoButtons);
	this->Ui->M32->setButtonSymbols(QAbstractSpinBox::NoButtons);
	this->Ui->M33->setButtonSymbols(QAbstractSpinBox::NoButtons);

	this->Ui->M00->setMinimum(-DBL_MAX);
	this->Ui->M01->setMinimum(-DBL_MAX);
	this->Ui->M02->setMinimum(-DBL_MAX);
	this->Ui->M03->setMinimum(-DBL_MAX);
	this->Ui->M10->setMinimum(-DBL_MAX);
	this->Ui->M11->setMinimum(-DBL_MAX);
	this->Ui->M12->setMinimum(-DBL_MAX);
	this->Ui->M13->setMinimum(-DBL_MAX);
	this->Ui->M20->setMinimum(-DBL_MAX);
	this->Ui->M21->setMinimum(-DBL_MAX);
	this->Ui->M22->setMinimum(-DBL_MAX);
	this->Ui->M23->setMinimum(-DBL_MAX);
	this->Ui->M30->setMinimum(-DBL_MAX);
	this->Ui->M31->setMinimum(-DBL_MAX);
	this->Ui->M32->setMinimum(-DBL_MAX);
	this->Ui->M33->setMinimum(-DBL_MAX);

	this->Ui->M00->setMaximum(DBL_MAX);
	this->Ui->M01->setMaximum(DBL_MAX);
	this->Ui->M02->setMaximum(DBL_MAX);
	this->Ui->M03->setMaximum(DBL_MAX);
	this->Ui->M10->setMaximum(DBL_MAX);
	this->Ui->M11->setMaximum(DBL_MAX);
	this->Ui->M12->setMaximum(DBL_MAX);
	this->Ui->M13->setMaximum(DBL_MAX);
	this->Ui->M20->setMaximum(DBL_MAX);
	this->Ui->M21->setMaximum(DBL_MAX);
	this->Ui->M22->setMaximum(DBL_MAX);
	this->Ui->M23->setMaximum(DBL_MAX);
	this->Ui->M30->setMaximum(DBL_MAX);
	this->Ui->M31->setMaximum(DBL_MAX);
	this->Ui->M32->setMaximum(DBL_MAX);
	this->Ui->M33->setMaximum(DBL_MAX);

	this->Ui->M00->setDecimals(7);
	this->Ui->M01->setDecimals(7);
	this->Ui->M02->setDecimals(7);
	this->Ui->M03->setDecimals(7);
	this->Ui->M10->setDecimals(7);
	this->Ui->M11->setDecimals(7);
	this->Ui->M12->setDecimals(7);
	this->Ui->M13->setDecimals(7);
	this->Ui->M20->setDecimals(7);
	this->Ui->M21->setDecimals(7);
	this->Ui->M22->setDecimals(7);
	this->Ui->M23->setDecimals(7);
	this->Ui->M30->setDecimals(7);
	this->Ui->M31->setDecimals(7);
	this->Ui->M32->setDecimals(7);
	this->Ui->M33->setDecimals(7);

	
	this->Ui->M00->setValue(1);
	this->Ui->M01->setValue(0);
	this->Ui->M02->setValue(0);
	this->Ui->M03->setValue(0);
	this->Ui->M10->setValue(0);
	this->Ui->M11->setValue(1);
	this->Ui->M12->setValue(0);
	this->Ui->M13->setValue(0);
	this->Ui->M20->setValue(0);
	this->Ui->M21->setValue(0);
	this->Ui->M22->setValue(1);
	this->Ui->M23->setValue(0);
	this->Ui->M30->setValue(0);
	this->Ui->M31->setValue(0);
	this->Ui->M32->setValue(0);
	this->Ui->M33->setValue(1);



	this->UpdateUI();
	
 
	connect(this->Ui->ApplyMatrix, SIGNAL(pressed()), this, SLOT(slotapplyMatrixToAllSelectedVolumes()));
 connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotsaveVolume()));
 connect(this->Ui->Reinit, SIGNAL(pressed()), this, SLOT(slotReinitMatrix()));

}




//-----------------------------------------------------------------------------
mqEditVolumeDialog::~mqEditVolumeDialog()
{

 //depending on what is 
	
  delete this->Ui;
}

int mqEditVolumeDialog::SomeThingHasChanged()
{
	int something_has_changed = 0;
	if (this->Volume != NULL)
	{
		
		vtkSmartPointer<vtkMatrix4x4> Mat = this->Volume->GetMatrix();

		if (
			Mat->GetElement(0, 0) != this->Ui->M00->value() ||
			Mat->GetElement(0, 1) != this->Ui->M01->value() ||
			Mat->GetElement(0, 2) != this->Ui->M02->value() ||
			Mat->GetElement(0, 3) != this->Ui->M03->value() ||
			Mat->GetElement(1, 0) != this->Ui->M10->value() ||
			Mat->GetElement(1, 1) != this->Ui->M11->value() ||
			Mat->GetElement(1, 2) != this->Ui->M12->value() ||
			Mat->GetElement(1, 3) != this->Ui->M13->value() ||
			Mat->GetElement(2, 0) != this->Ui->M20->value() ||
			Mat->GetElement(2, 1) != this->Ui->M21->value() ||
			Mat->GetElement(2, 2) != this->Ui->M22->value() ||
			Mat->GetElement(2, 3) != this->Ui->M23->value() ||
			Mat->GetElement(3, 0) != this->Ui->M30->value() ||
			Mat->GetElement(3, 1) != this->Ui->M31->value() ||
			Mat->GetElement(3, 2) != this->Ui->M32->value() ||
			Mat->GetElement(3, 3) != this->Ui->M33->value()
			)
		{
			something_has_changed = 1;
		}

		QString myname(this->Volume->GetName().c_str());
		
		if (QString::compare(myname, this->Ui->VolumeName->text(), Qt::CaseInsensitive)!=0)
		{
			something_has_changed = 1;
			
		}
	
	}

	return something_has_changed;
}
// This dialog is non modal, and Volumes can have been removed from the collection in the meantime... so before saving Volumes, we should check whether they are still
//inside the collection.
int mqEditVolumeDialog::CurrentVolumeInCollection()
{
	int Volume_found = 0;
	vtkMDVolume * Act;
	if (this->Volume != NULL && this->Volume_Coll != NULL)
	{
		this->Volume_Coll->InitTraversal();

		for (vtkIdType i = 0; i < this->Volume_Coll->GetNumberOfItems(); i++)
		{
			Act = vtkMDVolume::SafeDownCast(this->Volume_Coll->GetNextVolume());
			if (Volume_found == 1) { return Volume_found; }
			if (Act == this->Volume)
			{
				Volume_found = 1;
			}
		}
	}
	return Volume_found;
}
void mqEditVolumeDialog::saveVolume()
{
	//cout << "Save Volume!" << endl;
	cout << "Save Volume: in collection?:" << this->CurrentVolumeInCollection() << endl;
	if (this->Volume != NULL && this->CurrentVolumeInCollection())
	{
		int something_has_changed = this->SomeThingHasChanged();
		if (something_has_changed)
		{
			std::string action = "Update volume's Matrix";

			int mCount = BEGIN_UNDO_SET(action);
			this->Volume->SaveState(mCount);
			this->Volume->SetName(this->Ui->VolumeName->text().toStdString());
			vtkSmartPointer<vtkMatrix4x4> Mat = this->Volume->GetMatrix();
			Mat->SetElement(0, 0, this->Ui->M00->value());
			Mat->SetElement(0, 1, this->Ui->M01->value());
			Mat->SetElement(0, 2, this->Ui->M02->value());
			Mat->SetElement(0, 3, this->Ui->M03->value());
			Mat->SetElement(1, 0, this->Ui->M10->value());
			Mat->SetElement(1, 1, this->Ui->M11->value());
			Mat->SetElement(1, 2, this->Ui->M12->value());
			Mat->SetElement(1, 3, this->Ui->M13->value());
			Mat->SetElement(2, 0, this->Ui->M20->value());
			Mat->SetElement(2, 1, this->Ui->M21->value());
			Mat->SetElement(2, 2, this->Ui->M22->value());
			Mat->SetElement(2, 3, this->Ui->M23->value());
			Mat->SetElement(3, 0, this->Ui->M30->value());
			Mat->SetElement(3, 1, this->Ui->M31->value());
			Mat->SetElement(3, 2, this->Ui->M32->value());
			Mat->SetElement(3, 3, this->Ui->M33->value());
			this->Volume->ApplyMatrix(Mat);
			this->Volume->Modified();
			END_UNDO_SET();			
		}
		
	}
	
}



void mqEditVolumeDialog::GetFirstSelectedVolume()
{
	this->Volume_Coll = NULL;
	this->Volume = NULL;


	this->Volume_Coll = mqMorphoDigCore::instance()->getVolumeCollection();
	int num_selected = 0;
	num_selected = this->Volume_Coll->GetNumberOfSelectedVolumes();
	if (num_selected > 0) {
		this->Volume = this->Volume_Coll->GetFirstSelectedVolume();
		
	}
	

	if (this->Volume != NULL)
	{
	
		this->Volume_Coll->Modified();
	}
	
}

void mqEditVolumeDialog::GetFirstVolume()
{
	
	this->Volume_Coll = mqMorphoDigCore::instance()->getVolumeCollection();
	int num = 0;
	num = this->Volume_Coll->GetNumberOfItems();
	if (num > 0) {
		this->Volume_Coll->InitTraversal();
		this->Volume = vtkMDVolume::SafeDownCast(this->Volume_Coll->GetNextVolume());
		
	}
	

	if (this->Volume != NULL)
	{
		this->Volume->SetSelected(1);
		this->Volume_Coll->Modified();
	}

}

void mqEditVolumeDialog::UpdateUI()
{
	if (this->Volume != NULL) {
		
		QString mylabel(this->Volume->GetName().c_str());
		this->Ui->VolumeName->setText(mylabel);


		vtkSmartPointer<vtkMatrix4x4> Mat = this->Volume->GetMatrix();
		this->Ui->M00->setValue(Mat->GetElement(0, 0));
		this->Ui->M01->setValue(Mat->GetElement(0, 1));
		this->Ui->M02->setValue(Mat->GetElement(0, 2));
		this->Ui->M03->setValue(Mat->GetElement(0, 3));
		this->Ui->M10->setValue(Mat->GetElement(1, 0));
		this->Ui->M11->setValue(Mat->GetElement(1, 1));
		this->Ui->M12->setValue(Mat->GetElement(1, 2));
		this->Ui->M13->setValue(Mat->GetElement(1, 3));
		this->Ui->M20->setValue(Mat->GetElement(2, 0));
		this->Ui->M21->setValue(Mat->GetElement(2, 1));
		this->Ui->M22->setValue(Mat->GetElement(2, 2));
		this->Ui->M23->setValue(Mat->GetElement(2, 3));
		this->Ui->M30->setValue(Mat->GetElement(3, 0));
		this->Ui->M31->setValue(Mat->GetElement(3, 1));
		this->Ui->M32->setValue(Mat->GetElement(3, 2));
		this->Ui->M33->setValue(Mat->GetElement(3, 3));
	}
	
}


void mqEditVolumeDialog::GetNextVolume()
{

	if (Volume == NULL)
	{
		this->GetFirstVolume();
	}
	else
	{
		this->Volume->SetSelected(0);
		this->Volume_Coll->Modified();
		int cpt = 0;
		
		this->Volume = this->Volume_Coll->GetVolumeAfter(this->Volume);
		if (this->Volume != NULL) { this->Volume->SetSelected(1); this->Volume_Coll->Modified(); }
		else
		{
			this->GetFirstVolume();
				
		}

	}

}
void mqEditVolumeDialog::GetPrecedingVolume()
{

	if (this->Volume == NULL)
	{
		this->GetFirstVolume();
	}
	else
	{
		this->Volume->SetSelected(0);
		this->Volume_Coll->Modified();
		
		this->Volume = this->Volume_Coll->GetVolumeBefore(this->Volume); 
		if (this->Volume != NULL) { this->Volume->SetSelected(1); }
		else
		{
			this->Volume= vtkMDVolume::SafeDownCast(this->Volume_Coll->GetLastVolume());
			this->Volume->SetSelected(1);
		}
	}

}

void mqEditVolumeDialog::slotapplyMatrixToAllSelectedVolumes()
{
	int num_sel = mqMorphoDigCore::instance()->getVolumeCollection()->GetNumberOfSelectedVolumes();
	int num_act = mqMorphoDigCore::instance()->getVolumeCollection()->GetNumberOfItems();
	if (num_sel > 0)
	{
		std::string action = "Apply same matrix to all selected Volumes";
		int mCount = BEGIN_UNDO_SET(action);
		mqMorphoDigCore::instance()->getVolumeCollection()->InitTraversal();

		for (int i = 0; i < num_act; i++)
		{
			vtkMDVolume *myVolume = vtkMDVolume::SafeDownCast(mqMorphoDigCore::instance()->getVolumeCollection()->GetNextVolume());
			if (myVolume->GetSelected() == 1)
			{
				vtkSmartPointer<vtkMatrix4x4> Mat = myVolume->GetMatrix();
				this->Volume->SaveState(mCount);
				Mat->SetElement(0, 0, this->Ui->M00->value());
				Mat->SetElement(0, 1, this->Ui->M01->value());
				Mat->SetElement(0, 2, this->Ui->M02->value());
				Mat->SetElement(0, 3, this->Ui->M03->value());
				Mat->SetElement(1, 0, this->Ui->M10->value());
				Mat->SetElement(1, 1, this->Ui->M11->value());
				Mat->SetElement(1, 2, this->Ui->M12->value());
				Mat->SetElement(1, 3, this->Ui->M13->value());
				Mat->SetElement(2, 0, this->Ui->M20->value());
				Mat->SetElement(2, 1, this->Ui->M21->value());
				Mat->SetElement(2, 2, this->Ui->M22->value());
				Mat->SetElement(2, 3, this->Ui->M23->value());
				Mat->SetElement(3, 0, this->Ui->M30->value());
				Mat->SetElement(3, 1, this->Ui->M31->value());
				Mat->SetElement(3, 2, this->Ui->M32->value());
				Mat->SetElement(3, 3, this->Ui->M33->value());
				myVolume->ApplyMatrix(Mat);
				myVolume->Modified();
			}
		}
		END_UNDO_SET();
		mqMorphoDigCore::instance()->Render();
	}

}
void mqEditVolumeDialog::slotsaveVolume()
{
	

	this->saveVolume();
	if (this->Volume != NULL)
	{
		this->Volume->SetSelected(0);
		this->Volume->Modified();
		mqMorphoDigCore::instance()->Render();
	}
}
void mqEditVolumeDialog::slotGetPrecedingVolume()
{
	this->saveVolume();
	this->GetPrecedingVolume();
	this->UpdateUI();
	mqMorphoDigCore::instance()->Render();
}
void mqEditVolumeDialog::slotGetNextVolume()
{
	
	this->saveVolume();
	this->GetNextVolume();
	this->UpdateUI();
	mqMorphoDigCore::instance()->Render();
}

void mqEditVolumeDialog::RefreshDialog()
{
	
	this->GetFirstSelectedVolume();
	
	this->UpdateUI();
	
	mqMorphoDigCore::instance()->Render();
	
}
void mqEditVolumeDialog::slotRefreshDialog()
{
	
	this->RefreshDialog();

}

void mqEditVolumeDialog::slotReinitMatrix()
{
	this->Ui->M00->setValue(1);
	this->Ui->M01->setValue(0);
	this->Ui->M02->setValue(0);
	this->Ui->M03->setValue(0);
	this->Ui->M10->setValue(0);
	this->Ui->M11->setValue(1);
	this->Ui->M12->setValue(0);
	this->Ui->M13->setValue(0);
	this->Ui->M20->setValue(0);
	this->Ui->M21->setValue(0);
	this->Ui->M22->setValue(1);
	this->Ui->M23->setValue(0);
	this->Ui->M30->setValue(0);
	this->Ui->M31->setValue(0);
	this->Ui->M32->setValue(0);
	this->Ui->M33->setValue(1);
}

//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

