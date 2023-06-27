/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqEditACTORDialog.h"

#include "ui_mqEditACTORDialog.h"
#include "MorphoDigVersion.h"
#include "mqMorphoDigCore.h"
#include "mqUndoStack.h"
#include "vtkMDActor.h"
#include "vtkMDActorCollection.h"
#include <vtkMatrix4x4.h>
#include <vtkMatrix3x3.h>
#include <vtkPointData.h>

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
mqEditACTORDialog::mqEditACTORDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqEditACTORDialog())
{

	this->Ui->setupUi(this);
	this->setObjectName("mqEditACTORDialog");
	connect(mqMorphoDigCore::instance(), SIGNAL(actorSelectionChanged()), this, SLOT(slotRefreshDialog()));
	connect(this->Ui->next, SIGNAL(pressed()), this, SLOT(slotGetNextActor()));
	connect(this->Ui->prec, SIGNAL(pressed()), this, SLOT(slotGetPrecedingActor()));
	
	this->ACTOR_Coll = NULL;
	this->ACTOR = NULL;

	QString mylabel("...");
	this->Ui->ActorName->setText(mylabel);
	
	
	QColor myActorColor;

	double Actorcolor[4];

	this->Ui->ActorColorButton->setShowAlphaChannel(false);


	mqMorphoDigCore::instance()->Getmui_MeshColor(Actorcolor);

	myActorColor.setRedF(Actorcolor[0]);
	myActorColor.setGreenF(Actorcolor[1]);
	myActorColor.setBlueF(Actorcolor[2]);

	this->Ui->ActorColorButton->setChosenColor(myActorColor);
	this->Ui->ActorAlpha->setValue(Actorcolor[3]);
	this->Ui->ActorAlpha->setMinimum(0);
	this->Ui->ActorAlpha->setMaximum(1);
	this->Ui->ActorAlpha->setSingleStep(0.01);
	this->Ui->ActorAlpha->setValue(Actorcolor[4]);
	

	

	this->GetFirstSelectedActor();
	/*this->Ui->M00->setValidator(new QDoubleValidator(-DBL_MAX, DBL_MAX, 7, this));
	this->Ui->M01->setValidator(new QDoubleValidator(-DBL_MAX, DBL_MAX, 7, this));
	this->Ui->M02->setValidator(new QDoubleValidator(-DBL_MAX, DBL_MAX, 7, this));
	this->Ui->M03->setValidator(new QDoubleValidator(-DBL_MAX, DBL_MAX, 7, this));
	this->Ui->M10->setValidator(new QDoubleValidator(-DBL_MAX, DBL_MAX, 7, this));
	this->Ui->M11->setValidator(new QDoubleValidator(-DBL_MAX, DBL_MAX, 7, this));
	this->Ui->M12->setValidator(new QDoubleValidator(-DBL_MAX, DBL_MAX, 7, this));
	this->Ui->M13->setValidator(new QDoubleValidator(-DBL_MAX, DBL_MAX, 7, this));
	this->Ui->M20->setValidator(new QDoubleValidator(-DBL_MAX, DBL_MAX, 7, this));
	this->Ui->M21->setValidator(new QDoubleValidator(-DBL_MAX, DBL_MAX, 7, this));
	this->Ui->M22->setValidator(new QDoubleValidator(-DBL_MAX, DBL_MAX, 7, this));
	this->Ui->M23->setValidator(new QDoubleValidator(-DBL_MAX, DBL_MAX, 7, this));
	this->Ui->M30->setValidator(new QDoubleValidator(-DBL_MAX, DBL_MAX, 7, this));
	this->Ui->M31->setValidator(new QDoubleValidator(-DBL_MAX, DBL_MAX, 7, this));
	this->Ui->M32->setValidator(new QDoubleValidator(-DBL_MAX, DBL_MAX, 7, this));
	this->Ui->M33->setValidator(new QDoubleValidator(-DBL_MAX, DBL_MAX, 7, this));*/

	this->Ui->numCells->setButtonSymbols(QAbstractSpinBox::NoButtons);
	this->Ui->numPoints->setButtonSymbols(QAbstractSpinBox::NoButtons);
	this->Ui->numCells->setMaximum(INT_MAX);
	this->Ui->numPoints->setMaximum(INT_MAX);

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
	
 
	connect(this->Ui->ApplyMatrix, SIGNAL(pressed()), this, SLOT(slotapplyMatrixToAllSelectedActors()));
	connect(this->Ui->ApplyMatrix_all, SIGNAL(pressed()), this, SLOT(slotapplyMatrixToAllActors()));
 connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotsaveActor()));
 connect(this->Ui->Reinit, SIGNAL(pressed()), this, SLOT(slotReinitMatrix()));
 
 connect(this->Ui->hardenTransform, SIGNAL(pressed()), this, SLOT(slotHardenTransform()));
 connect(this->Ui->Refresh, SIGNAL(pressed()), this, SLOT(slotRefreshMatrix()));
connect(this->Ui->deleteScalar, SIGNAL(pressed()), this, SLOT(slotDeleteScalar()));
connect(this->Ui->editScalar, SIGNAL(pressed()), this, SLOT(slotEditScalar()));
connect(this->Ui->duplicateScalar, SIGNAL(pressed()), this, SLOT(slotDuplicateScalar()));
connect(this->Ui->vectorScalar, SIGNAL(pressed()), this, SLOT(slotVectorScalar()));


connect(this->Ui->displayROI, SIGNAL(pressed()), this, SLOT(slotdisplayROIPressed()));

connect(this->Ui->displaySpikesCheckBox, SIGNAL(clicked(bool)), this, SLOT(slotDisplaySpikesClicked(bool)));
connect(this->Ui->autoAdjustSpikeRenderingSizeCheckBox, SIGNAL(clicked(bool)), this, SLOT(slotAdjustSpikesClicked(bool)));
connect(this->Ui->adjustSpikeScaleFactor, SIGNAL(valueChanged(double)), this, SLOT(slotadjustSpikeScaleFactor(double)));
connect(this->Ui->spikeMaskFactor, SIGNAL(valueChanged(int)), this, SLOT(slotspikeMaskFactor(int)));


connect(this->Ui->enableROI, SIGNAL(clicked(bool)), this, SLOT(slotEnableROIClicked(bool)));
connect(this->Ui->isVisible, SIGNAL(clicked(bool)), this, SLOT(slotisVisibleClicked(bool)));
//virtual void slotEnableROIPressed(bool isChecked);
this->Ui->displayROI->setChecked(true);
}




//-----------------------------------------------------------------------------
mqEditACTORDialog::~mqEditACTORDialog()
{

 //depending on what is 
	
  delete this->Ui;
}

int mqEditACTORDialog::SomeThingHasChanged()
{
	int something_has_changed = 0;
	if (this->ACTOR != NULL)
	{
		QColor muiActorColor = this->Ui->ActorColorButton->chosenColor();
		double uiActorcolor[4];
		muiActorColor.getRgbF(&uiActorcolor[0], &uiActorcolor[1], &uiActorcolor[2], &uiActorcolor[3]);
		uiActorcolor[3] = this->Ui->ActorAlpha->value();

		QColor myActorColor;
		double Actorcolor[4];		
		this->ACTOR->GetmColor(Actorcolor);

		if (
			Actorcolor[0] != uiActorcolor[0]
			|| Actorcolor[1] != uiActorcolor[1]
			|| Actorcolor[2] != uiActorcolor[2]
			|| Actorcolor[3] != uiActorcolor[3]
			)
		{
			something_has_changed = 1;
		}
		vtkSmartPointer<vtkMatrix4x4> Mat = this->ACTOR->GetMatrix();

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

		QString myname(this->ACTOR->GetName().c_str());
		
		if (QString::compare(myname, this->Ui->ActorName->text(), Qt::CaseInsensitive)!=0)
		{
			something_has_changed = 1;
			
		}
	
	}

	return something_has_changed;
}
// This dialog is non modal, and actors can have been removed from the collection in the meantime... so before saving actors, we should check whether they are still
//inside the collection.
void mqEditACTORDialog::RefreshNumCellsNumPoints()
{
	if (this->ACTOR != NULL)
	{
		vtkIdType numPoints = this->ACTOR->GetNumberOfPoints();
		vtkIdType numCells = this->ACTOR->GetNumberOfCells();
		this->Ui->numCells->setValue(numCells);
		this->Ui->numPoints->setValue(numPoints);
	}

}
int mqEditACTORDialog::CurrentActorInCollection()
{
	int actor_found = 0;
	vtkMDActor * Act;
	if (this->ACTOR != NULL && this->ACTOR_Coll != NULL)
	{
		this->ACTOR_Coll->InitTraversal();

		for (vtkIdType i = 0; i < this->ACTOR_Coll->GetNumberOfItems(); i++)
		{
			Act = vtkMDActor::SafeDownCast(this->ACTOR_Coll->GetNextActor());
			if (actor_found == 1) { return actor_found; }
			if (Act == this->ACTOR)
			{
				actor_found = 1;
			}
		}
	}
	return actor_found;
}
void mqEditACTORDialog::saveActor()
{
	//cout << "Save ACTOR!" << endl;
	cout << "Save actor: in collection?:" << this->CurrentActorInCollection() << endl;
	if (this->ACTOR != NULL && this->CurrentActorInCollection())
	{
		int something_has_changed = this->SomeThingHasChanged();
		if (something_has_changed)
		{
			std::string action = "Update actor's color and Matrix";

			int mCount = BEGIN_UNDO_SET(action);
			this->ACTOR->SaveState(mCount);
			QColor myActorColor = this->Ui->ActorColorButton->chosenColor();
			double Actorcolor[4];
			myActorColor.getRgbF(&Actorcolor[0], &Actorcolor[1], &Actorcolor[2], &Actorcolor[3]);
			Actorcolor[3] = this->Ui->ActorAlpha->value();
			this->ACTOR->SetmColor(Actorcolor);
			this->ACTOR->SetName(this->Ui->ActorName->text().toStdString());
			vtkSmartPointer<vtkMatrix4x4> Mat = this->ACTOR->GetMatrix();
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
			this->ACTOR->ApplyMatrix(Mat);
			this->ACTOR->Modified();
			END_UNDO_SET();			
		}
		
	}
	
}



void mqEditACTORDialog::GetFirstSelectedActor()
{
	this->ACTOR_Coll = NULL;
	this->ACTOR = NULL;


	this->ACTOR_Coll = mqMorphoDigCore::instance()->getActorCollection();
	int num_selected = 0;
	num_selected = this->ACTOR_Coll->GetNumberOfSelectedActors();
	if (num_selected > 0) {
		this->ACTOR = this->ACTOR_Coll->GetFirstSelectedActor();
		
	}
	

	if (this->ACTOR != NULL)
	{
	
		this->ACTOR_Coll->Modified();
	}
	
}

void mqEditACTORDialog::GetFirstActor()
{
	
	this->ACTOR_Coll = mqMorphoDigCore::instance()->getActorCollection();
	int num = 0;
	num = this->ACTOR_Coll->GetNumberOfItems();
	if (num > 0) {
		this->ACTOR_Coll->InitTraversal();
		this->ACTOR = vtkMDActor::SafeDownCast(this->ACTOR_Coll->GetNextActor());
		
	}
	

	if (this->ACTOR != NULL)
	{
		this->ACTOR->SetSelected(1);
		this->ACTOR_Coll->Modified();
	}

}

void mqEditACTORDialog::slotisVisibleClicked(bool isChecked)
{
	if (this->ACTOR != NULL && this->CurrentActorInCollection() && this->ACTOR->GetSelected() == 1)
	{
		mqMorphoDigCore::instance()->Render();
		if (isChecked)
		{
			this->ACTOR->SetisVisible(1);
		}
		else
		{
			this->ACTOR->SetisVisible(0);
		}

		mqMorphoDigCore::instance()->Render();
	}
}


void mqEditACTORDialog::slotDisplaySpikesClicked(bool isChecked)
{
	if (this->ACTOR != NULL && this->CurrentActorInCollection() && this->ACTOR->GetSelected() == 1)
	{
		if (isChecked)
			//if (this->Volume->GetdisplayROI() == 0)
		{
			this->ACTOR->SetdisplaySpikes(1);
			this->Ui->autoAdjustSpikeRenderingSizeCheckBox->setEnabled(true);
			this->Ui->adjustSpikeScaleFactor->setEnabled(true);
			this->Ui->spikeMaskFactor->setEnabled(true);
			
		}
		else
		{
			this->ACTOR->SetdisplaySpikes(0);
			
			this->Ui->autoAdjustSpikeRenderingSizeCheckBox->setEnabled(false);
			this->Ui->adjustSpikeScaleFactor->setEnabled(false);
			this->Ui->spikeMaskFactor->setEnabled(false);


		}
		mqMorphoDigCore::instance()->Render();
	}

}
void mqEditACTORDialog::slotAdjustSpikesClicked(bool isChecked)
{
	if (this->ACTOR != NULL && this->CurrentActorInCollection() && this->ACTOR->GetSelected() == 1)
	{
		if (isChecked)
			//if (this->Volume->GetdisplayROI() == 0)
		{
			this->ACTOR->SetautoAdjustSpikeRendering(1);
			

		}
		else
		{
			this->ACTOR->SetautoAdjustSpikeRendering(0);
			


		}
		mqMorphoDigCore::instance()->Render();
	}

}
void mqEditACTORDialog::slotadjustSpikeScaleFactor(double factor)
{
	if (this->ACTOR != NULL && this->CurrentActorInCollection() && this->ACTOR->GetSelected() == 1)
	{
		this->ACTOR->SetspikeScaleFactor(factor);
	}
}
void mqEditACTORDialog::slotspikeMaskFactor(int factor)
{
	if (this->ACTOR != NULL && this->CurrentActorInCollection() && this->ACTOR->GetSelected() == 1)
	{
		this->ACTOR->SetspikeMaskFactor(factor);

	}
}

//virtual void slotDisplaySpikesClicked(bool isChecked);

//virtual void slotAdjustSpikesClicked(bool isChecked);

void mqEditACTORDialog::slotEnableROIClicked(bool isChecked)
{
	if (this->ACTOR != NULL && this->CurrentActorInCollection() && this->ACTOR->GetSelected() == 1)
	{
		if (isChecked)
			//if (this->Volume->GetdisplayROI() == 0)
		{
			cout << "Call volume create box function" << endl;
			this->ACTOR->CreateBox();
			this->ACTOR->SetenableROI(true);
			this->Ui->displayROI->setEnabled(true);
		}
		else
		{
			cout << "Call volume remove box function" << endl;

			this->ACTOR->RemoveBox();
			this->ACTOR->SetenableROI(false);
			this->Ui->displayROI->setEnabled(false);
			this->Ui->displayROI->setChecked(false);


		}
		mqMorphoDigCore::instance()->Render();
	}

}

void mqEditACTORDialog::slotdisplayROIPressed()
{
	if (this->ACTOR!= NULL && this->CurrentActorInCollection() && this->ACTOR->GetSelected() == 1)
	{
		if (this->Ui->displayROI->isChecked())
			//if (this->Volume->GetdisplayROI() == 0)
		{
			this->ACTOR->SetdisplayROI(0);
		}
		else
		{

			this->ACTOR->SetdisplayROI(1);

		}
		mqMorphoDigCore::instance()->Render();
	}
}


void mqEditACTORDialog::UpdateUI()
{
	if (this->ACTOR != NULL) {
		
		this->Ui->enableROI->setChecked(this->ACTOR->GetenableROI());
		this->Ui->displayROI->setChecked(this->ACTOR->GetdisplayROI());
		if (this->ACTOR->GetisVisible() == 1)
		{
			this->Ui->isVisible->setChecked(true);
		}
		else
		{
			this->Ui->isVisible->setChecked(false);
		}

		
		if (this->ACTOR->GetenableROI() == 1)
		{
			
			this->Ui->displayROI->setEnabled(true);
		}
		else
		{
			
			this->Ui->displayROI->setEnabled(false);
		}

		QString mylabel(this->ACTOR->GetName().c_str());
		this->Ui->ActorName->setText(mylabel);

	//	cout << "Update UI!" << endl;
		QColor myActorColor;
		double Actorcolor[4];

		//cout << "Name" <<mylabel.toStdString()<< endl;
		this->ACTOR->GetmColor(Actorcolor);

		myActorColor.setRedF(Actorcolor[0]);
		myActorColor.setGreenF(Actorcolor[1]);
		myActorColor.setBlueF(Actorcolor[2]);

		this->Ui->ActorColorButton->setChosenColor(myActorColor);

		this->Ui->ActorAlpha->setValue(Actorcolor[3]);

		vtkSmartPointer<vtkMatrix4x4> Mat = this->ACTOR->GetMatrix();
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
		//QListView
		//QItemDelegate
		double spikeScaleFactor = this->ACTOR->GetspikeScaleFactor();
		this->Ui->adjustSpikeScaleFactor->setValue(spikeScaleFactor);

		int  maskFactor = this->ACTOR->GetspikeMaskFactor();
		this->Ui->spikeMaskFactor->setValue(maskFactor);

		int displaySpikes = this->ACTOR->GetdisplaySpikes();
		this->Ui->displaySpikesCheckBox->setChecked(displaySpikes);
		if (displaySpikes==1)
		
		{			
			this->Ui->autoAdjustSpikeRenderingSizeCheckBox->setEnabled(true);
			this->Ui->adjustSpikeScaleFactor->setEnabled(true);
			this->Ui->spikeMaskFactor->setEnabled(true);

		}
		else
		{			
			this->Ui->autoAdjustSpikeRenderingSizeCheckBox->setEnabled(false);
			this->Ui->adjustSpikeScaleFactor->setEnabled(false);
			this->Ui->spikeMaskFactor->setEnabled(false);


		}

		int autoAdjustSpikes = this->ACTOR->GetautoAdjustSpikeRendering();
		this->Ui->autoAdjustSpikeRenderingSizeCheckBox->setChecked(autoAdjustSpikes);

		this->Ui->scalarList->clear();
		ExistingArrays *MyList = mqMorphoDigCore::instance()->Getmui_ArraysOfActor(this->ACTOR);
		for (int i = 0; i < MyList->Stack.size(); i++)
		{
			if (
				((MyList->Stack.at(i).DataType == VTK_FLOAT || MyList->Stack.at(i).DataType == VTK_DOUBLE) && MyList->Stack.at(i).NumComp == 1) // conventional scalars
				|| ((MyList->Stack.at(i).DataType == VTK_FLOAT || MyList->Stack.at(i).DataType == VTK_DOUBLE) && MyList->Stack.at(i).NumComp == 3) // vector scalars (normales or else)
				|| (MyList->Stack.at(i).DataType == VTK_UNSIGNED_CHAR  && MyList->Stack.at(i).NumComp >= 3 //RGB-like arrays
					)
				|| 
				((MyList->Stack.at(i).DataType == VTK_INT || MyList->Stack.at(i).DataType == VTK_UNSIGNED_INT) && MyList->Stack.at(i).NumComp == 1) // conventional tag arrays
				
				
				)
			{
				
				
				QListWidgetItem* item = new QListWidgetItem(MyList->Stack.at(i).Name, this->Ui->scalarList);
				
				/*QWidget* widget = new QWidget();
				QLabel* widgetText = new QLabel(MyList->Stack.at(i).Name);						
				//QPushButton* widgetButtonDelete = new QPushButton("D");
				//QPixmap cursor_pixmap = QPixmap(":/Cursors/move3.png");
				//widgetButtonDelete->setFixedWidth(20);
				//widgetButtonDelete->setAccessibleName
				//QPushButton* widgetButtonEdit = new QPushButton("E");
				//widgetButtonEdit->setFixedWidth(20);
				QHBoxLayout * widgetLayout = new QHBoxLayout();
				widgetLayout->addWidget(widgetText);
				//widgetLayout->addWidget(widgetButtonEdit);
				//widgetLayout->addWidget(widgetButtonDelete);
				widgetLayout->addStretch();

				//connect(widgetButtonDelete, SIGNAL(pressed()), this, SLOT(slotDeleteScalar()));
				//connect(widgetButtonEdit, SIGNAL(pressed()), this, SLOT(slotEditScalar()));

				widgetLayout->setSizeConstraint(QLayout::SetFixedSize);
				widget->setLayout(widgetLayout);					
				item->setSizeHint(widget->sizeHint());
				this->Ui->scalarList->addItem(item);
				this->Ui->scalarList->setItemWidget(item, widget);*/
			
				
			}

		}

		/*stream << Mat->GetElement(0, 0) << " " << Mat->GetElement(1, 0) << " " << Mat->GetElement(2, 0) << " " << Mat->GetElement(3, 0) << endl;
		stream << Mat->GetElement(0, 1) << " " << Mat->GetElement(1, 1) << " " << Mat->GetElement(2, 1) << " " << Mat->GetElement(3, 1) << endl;
		stream << Mat->GetElement(0, 2) << " " << Mat->GetElement(1, 2) << " " << Mat->GetElement(2, 2) << " " << Mat->GetElement(3, 2) << endl;
		stream << Mat->GetElement(0, 3) << " " << Mat->GetElement(1, 3) << " " << Mat->GetElement(2, 3) << " " << Mat->GetElement(3, 3) << endl;*/
		
		//this->Ui->M00->SetText()
		

		//cout << "Color" << Actorcolor[0]<<","<< Actorcolor[1] << "," << Actorcolor[2] << "," << Actorcolor[3] << "." << endl;
	}
	this->RefreshNumCellsNumPoints();

}


void mqEditACTORDialog::GetNextActor()
{

	if (ACTOR == NULL)
	{
		this->GetFirstActor();
	}
	else
	{
		this->ACTOR->SetSelected(0);
		this->ACTOR_Coll->Modified();
		int cpt = 0;
		
		this->ACTOR = this->ACTOR_Coll->GetActorAfter(this->ACTOR);
		if (this->ACTOR != NULL) { this->ACTOR->SetSelected(1); this->ACTOR_Coll->Modified(); }
		else
		{
			this->GetFirstActor();
				
		}

	}

}
void mqEditACTORDialog::GetPrecedingActor()
{

	if (this->ACTOR == NULL)
	{
		this->GetFirstActor();
	}
	else
	{
		this->ACTOR->SetSelected(0);
		this->ACTOR_Coll->Modified();
		
		this->ACTOR = this->ACTOR_Coll->GetActorBefore(this->ACTOR); 
		if (this->ACTOR != NULL) { this->ACTOR->SetSelected(1); }
		else
		{
			this->ACTOR= vtkMDActor::SafeDownCast(this->ACTOR_Coll->GetLastActor());
			this->ACTOR->SetSelected(1);
		}
	}

}


void mqEditACTORDialog::slotVectorScalar()
{
	int row = this->Ui->scalarList->currentIndex().row();
	if (this->ACTOR != NULL && row >= 0)
	{
		QString ScalarName = this->Ui->scalarList->item(row)->text();

		ExistingArrays* MyList = mqMorphoDigCore::instance()->Getmui_ArraysOfActor(this->ACTOR);
		int numS = 0;
		for (int i = 0; i < MyList->Stack.size(); i++)
		{
			
			if (MyList->Stack.at(i).Name == ScalarName)
			{ 
				numS = i;
			}
		}
		cout << "numS=" << numS << endl;

		if ((MyList->Stack.at(numS).DataType == VTK_FLOAT || MyList->Stack.at(numS).DataType == VTK_DOUBLE) && MyList->Stack.at(numS).NumComp == 3) // vector scalars (normales or else)
		{
			//mqMorphoDigCore::instance()->DuplicateArray(this->ACTOR, ScalarName);
			cout << "Try to set vector" << endl;
			this->ACTOR->GetMapper()->GetInput()->GetPointData()->SetVectors(this->ACTOR->GetMapper()->GetInput()->GetPointData()->GetScalars(ScalarName.toStdString().c_str()));
		}
	}
}

void mqEditACTORDialog::slotDuplicateScalar()
{
	int row = this->Ui->scalarList->currentIndex().row();
	if (this->ACTOR != NULL && row >= 0)
	{
		QString ScalarName = this->Ui->scalarList->item(row)->text();
		cout << "try to duplicate scalar" << endl;
		cout << "try to duplicate scalar " << row << ":" << this->Ui->scalarList->item(row)->text().toStdString() << endl;
		QInputDialog *renameDialog = new QInputDialog();
		bool dialogResult;
		QString newScalarName = renameDialog->getText(0, "Duplicate array", "New name:", QLineEdit::Normal,
			this->Ui->scalarList->item(row)->text(), &dialogResult);
		if (dialogResult)
		{
			cout << "new scalar given:" << newScalarName.toStdString() << endl;
			mqMorphoDigCore::instance()->DuplicateArray(this->ACTOR, ScalarName, newScalarName);
			this->UpdateUI();
		}
		else
		{
			cout << "cancel " << endl;
		}
	}
}
void mqEditACTORDialog::slotEditScalar()
{
	int row = this->Ui->scalarList->currentIndex().row();
	if (this->ACTOR != NULL && row >= 0)
	{
		QString oldScalarName = this->Ui->scalarList->item(row)->text();
		cout << "try to edit scalar" << endl;
		cout << "try to edit scalar " << row << ":" << this->Ui->scalarList->item(row)->text().toStdString() << endl;
		QInputDialog *renameDialog = new QInputDialog();
		bool dialogResult;
		QString newScalarName = renameDialog->getText(0, "Rename Label", "New name:", QLineEdit::Normal,
			this->Ui->scalarList->item(row)->text(), &dialogResult);
		if (dialogResult)
		{
			cout << "new name given:" << newScalarName.toStdString() << endl;
			mqMorphoDigCore::instance()->EditArrayName(this->ACTOR, oldScalarName, newScalarName);
			this->UpdateUI();
		}
		else
		{
			cout << "cancel " << endl;
		}
	}
}
void mqEditACTORDialog::slotDeleteScalar()
{
	int row = this->Ui->scalarList->currentIndex().row();
	cout << "try to delete scalar " << row << ":";

	
	if (this->ACTOR != NULL && row>=0)
	{
		cout << this->Ui->scalarList->item(row)->text().toStdString() << endl;
		mqMorphoDigCore::instance()->DeleteArray(this->ACTOR, this->Ui->scalarList->item(row)->text());
		cout << "Try to update UI" << endl;
		this->UpdateUI();
		cout << "Try to update UI ok" << endl;
	}

}
void mqEditACTORDialog::slotapplyMatrixToAllSelectedActors()
{
	int num_sel = mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfSelectedActors();
	int num_act = mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfItems();
	if (num_sel > 0)
	{
		std::string action = "Apply same matrix to all selected actors";
		int mCount = BEGIN_UNDO_SET(action);
		mqMorphoDigCore::instance()->getActorCollection()->InitTraversal();

		for (int i = 0; i < num_act; i++)
		{
			vtkMDActor *myActor = vtkMDActor::SafeDownCast(mqMorphoDigCore::instance()->getActorCollection()->GetNextActor());
			if (myActor->GetSelected() == 1)
			{
				vtkSmartPointer<vtkMatrix4x4> Mat = myActor->GetMatrix();
				myActor->SaveState(mCount);
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
				myActor->ApplyMatrix(Mat);
				myActor->Modified();
			}
		}
		END_UNDO_SET();
		mqMorphoDigCore::instance()->Render();
	}

}

void mqEditACTORDialog::slotapplyMatrixToAllActors()
{
	
	int num_act = mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfItems();
	if (num_act > 0)
	{
		std::string action = "Apply same matrix to all actors";
		int mCount = BEGIN_UNDO_SET(action);
		mqMorphoDigCore::instance()->getActorCollection()->InitTraversal();

		for (int i = 0; i < num_act; i++)
		{
			vtkMDActor *myActor = vtkMDActor::SafeDownCast(mqMorphoDigCore::instance()->getActorCollection()->GetNextActor());
			
				vtkSmartPointer<vtkMatrix4x4> Mat = myActor->GetMatrix();
				
				myActor->SaveState(mCount);
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
				
				myActor->ApplyMatrix(Mat);
				myActor->Modified();
			
		}
		END_UNDO_SET();
		mqMorphoDigCore::instance()->Render();
	}

}

void mqEditACTORDialog::slotsaveActor()
{
	

	this->saveActor();
	if (this->ACTOR != NULL)
	{
		this->ACTOR->SetSelected(0);
		this->ACTOR->Modified();
		mqMorphoDigCore::instance()->Render();
	}
}
void mqEditACTORDialog::slotGetPrecedingActor()
{
	this->saveActor();
	this->GetPrecedingActor();
	this->UpdateUI();
	mqMorphoDigCore::instance()->Render();
}
void mqEditACTORDialog::slotGetNextActor()
{
	
	this->saveActor();
	this->GetNextActor();
	this->UpdateUI();
	mqMorphoDigCore::instance()->Render();
}
void mqEditACTORDialog::ShowAndRefresh()
{
	this->GetFirstSelectedActor();

	if (this->ACTOR == NULL || (this->ACTOR != NULL && this->ACTOR->GetSelected() == 0)) { this->GetFirstActor(); }

	this->RefreshDialog();

}
void mqEditACTORDialog::RefreshDialog()
{
	
	this->GetFirstSelectedActor();
	
	this->UpdateUI();
	
	mqMorphoDigCore::instance()->Render();
	
}
void mqEditACTORDialog::slotRefreshDialog()
{
	
	this->RefreshDialog();

}

void mqEditACTORDialog::slotHardenTransform()
{
	if (this->ACTOR != NULL)
	{
		this->ACTOR->HardenTransform();
		this->Ui->hardenTransform->setCheckable(false);
		this->Ui->hardenTransform->setChecked(false);
		this->UpdateUI();
	}
}
void mqEditACTORDialog::slotReinitMatrix()
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

void mqEditACTORDialog::slotRefreshMatrix()
{
	if (this->ACTOR != NULL)
	{
		vtkSmartPointer<vtkMatrix4x4> Mat = this->ACTOR->GetMatrix();
		//vtkSmartPointer<vtkMatrix3x3> Mat3 = vtkMatrix3x3::SafeDownCast(this->ACTOR->GetMatrix());
		//Mat->PrintSelf(std::cout, vtkIndent(1));

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
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

