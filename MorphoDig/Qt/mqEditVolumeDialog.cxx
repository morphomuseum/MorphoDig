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
#include <vtkVolumeProperty.h>
#include <vtkMatrix4x4.h>
#include "mqColorOpacityEditorWidget.h"
#include "mqTransferFunctionWidget.h"
#include <vtkDiscretizableColorTransferFunction.h>
#include<vtkPiecewiseFunction.h>

// we actually do not need glew...
//#include <GL/glew.h>
#include <QApplication>
#include <QFile>
#include <QRadioButton>
#include <QFileDialog>
#include <QAction>
#include <QCheckBox>
#include <QMessageBox>
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

	vtkDiscretizableColorTransferFunction* STC = mqMorphoDigCore::instance()->GetOneColorMap();

	cout << "Edit Volume Dialog: Create mqColorOpacityEditorWidget!" << endl;
	mqColorOpacityEditorWidget *someMap = new mqColorOpacityEditorWidget(STC, this->Ui->PropertiesFrame,0);
	//cout << "Try that!" << endl;
	this->mColorMap = someMap;


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



	this->Ui->suggestedMin->setButtonSymbols(QAbstractSpinBox::NoButtons);
	this->Ui->suggestedMax->setButtonSymbols(QAbstractSpinBox::NoButtons);


	this->Ui->suggestedMin->setMinimum(-DBL_MAX);
	this->Ui->suggestedMax->setMinimum(-DBL_MAX);

	this->Ui->suggestedMin->setMaximum(DBL_MAX);
	this->Ui->suggestedMax->setMaximum(DBL_MAX);

	this->UpdateUI();

	
	this->Ui->comboColorMap->setSizeAdjustPolicy(QComboBox::AdjustToContents);
	
	connect(this->Ui->comboColorMap, SIGNAL(activated(int)), this, SLOT(slotLoadPreset(int)));


	this->Ui->scalarOpacityUnitDistance->setMaximum(DBL_MAX);

	
	connect(this->Ui->ApplyMatrix, SIGNAL(pressed()), this, SLOT(slotapplyMatrixToAllSelectedVolumes()));
	connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotsaveVolume()));
	connect(this->Ui->Reinit, SIGNAL(pressed()), this, SLOT(slotReinitMatrix()));


	
	connect(this->Ui->interpolationToLinear, SIGNAL(clicked(bool)), this, SLOT(slotInterpolationToLinear(bool)));
	connect(this->Ui->scalarOpacityUnitDistance, SIGNAL(valueChanged(double)), this, SLOT(slotScalarOpacityUnitDistance(double)));
	connect(this->mColorMap, SIGNAL(shiftOrSlideStopped()), this, SLOT(slotSaveActorMinMaxHaveBeenChangedInWidget()));
	connect(this->mColorMap, SIGNAL(minmaxChanged()), this, SLOT(slotSaveActorMinMaxHaveBeenChangedInWidget()));
	connect(mqMorphoDigCore::instance(), SIGNAL(volumesMightHaveChanged()), this, SLOT(slotRefreshUi()));
	connect(this->Ui->cutMinPercent, SIGNAL(valueChanged(int)), this, SLOT(slotRefreshSuggestedRange()));
	connect(this->Ui->cutMaxPercent, SIGNAL(valueChanged(int)), this, SLOT(slotRefreshSuggestedRange()));
	connect(this->Ui->pushScalarSuggestedMax, SIGNAL(pressed()), this, SLOT(slotAcceptSuggestedMax()));
	connect(this->Ui->pushScalarSuggestedMin, SIGNAL(pressed()), this, SLOT(slotAcceptSuggestedMin()));
	connect(this->Ui->reinitializeColorMap, SIGNAL(pressed()), this, SLOT(slotReinitializeColorMap()));
	connect(this->Ui->editColorMap, SIGNAL(pressed()), this, SLOT(slotEditColorMapName()));
	connect(this->Ui->deleteColorMap, SIGNAL(pressed()), this, SLOT(slotDeleteColorMap()));
	connect(mqMorphoDigCore::instance(), SIGNAL(colorMapsChanged()), this, SLOT(slotRefreshColorMaps()));
	connect(this->mColorMap, SIGNAL(changeFinished()), this, SLOT(slotRefreshDialog()));



	QAction* actiondisplayROIToggle = new QAction(tr("&displayROI"), this);
	actiondisplayROIToggle->setCheckable(true);
	actiondisplayROIToggle->setToolTip(tr("display clipping ROI."));
	this->Ui->displayROI->addAction(actiondisplayROIToggle);
	this->Ui->displayROI->setDefaultAction(actiondisplayROIToggle);
	QIcon icon;
	icon.addFile(QStringLiteral(":/Icons/eye_opened.png"), QSize(), QIcon::Normal, QIcon::Off);
	icon.addFile(QStringLiteral(":/Icons/eye_closed.png"), QSize(), QIcon::Normal, QIcon::On);
	//  exportColorMap->setIcon(icon);
	actiondisplayROIToggle->setIcon(icon);
	//new mqCameraReaction(actionCameraOrthoPerspectiveToggle, 7); //7 = camera OrthoPerspective toggle

	connect(this->Ui->displayROI, SIGNAL(pressed()), this, SLOT(slotdisplayROIPressed()));
	this->Ui->displayROI->setChecked(true);

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
void mqEditVolumeDialog::slotRefreshUi()
{
	this->GetFirstSelectedVolume();
	this->UpdateUI();
}

void mqEditVolumeDialog::slotdisplayROIPressed()
{
	if (this->Volume != NULL && this->Volume->GetSelected() == 1)
	{
		if (this->Ui->displayROI->isChecked())
		//if (this->Volume->GetdisplayROI() == 0)
		{
			this->Volume->SetdisplayROI(1);
		}
		else
		{
			
				this->Volume->SetdisplayROI(0);
			
		}
		mqMorphoDigCore::instance()->Render();
	}
}
void mqEditVolumeDialog::slotRefreshSuggestedRange()
{
	this->RefreshSuggestedRange();
}
void mqEditVolumeDialog::RefreshSuggestedRange()
{
	cout << "DIAL Edit Scalars dialog: refreshsuggestedRange" << endl;
	int cutMin = this->Ui->cutMinPercent->value();
	int cutMax = this->Ui->cutMaxPercent->value();
	this->Ui->suggestedMax->setValue(mqMorphoDigCore::instance()->GetSuggestedVolumeRangeMax(cutMax,1));
	this->Ui->suggestedMin->setValue(mqMorphoDigCore::instance()->GetSuggestedVolumeRangeMin(cutMin,1));
}


void mqEditVolumeDialog::LoadPreset()
{
	//cout << "looks like we want to load a preset ! " << idx << endl;
	if (this->Volume != NULL)
	{


		QString NewActiveColorMap = this->Ui->comboColorMap->currentText();
		for (int i = 0; i < mqMorphoDigCore::instance()->Getmui_ExistingColorMaps()->Stack.size(); i++)
		{
			QString myExisingColorMapName = mqMorphoDigCore::instance()->Getmui_ExistingColorMaps()->Stack.at(i).Name;
			if (NewActiveColorMap == myExisingColorMapName)
			{
				cout << "Found " << NewActiveColorMap.toStdString() << " in the list of Existing color maps" << endl;

				if (mqMorphoDigCore::instance()->Getmui_ExistingColorMaps()->Stack.at(i).isCustom == 1)
				{
					this->Ui->reinitializeColorMap->setDisabled(true);
					this->Ui->deleteColorMap->setDisabled(false);
					this->Ui->editColorMap->setDisabled(false);
				}
				else
				{
					this->Ui->reinitializeColorMap->setDisabled(false);
					this->Ui->deleteColorMap->setDisabled(true);
					this->Ui->editColorMap->setDisabled(true);

				}
				cout << "^Copy CTF" <<  endl;
				vtkSmartPointer<vtkDiscretizableColorTransferFunction> CTF = vtkSmartPointer<vtkDiscretizableColorTransferFunction>::New();
				vtkSmartPointer<vtkPiecewiseFunction> NewOF = vtkSmartPointer<vtkPiecewiseFunction>::New();
				CTF->DeepCopy(mqMorphoDigCore::instance()->Getmui_ExistingColorMaps()->Stack.at(i).ColorMap);
				vtkPiecewiseFunction* OF = mqMorphoDigCore::instance()->Getmui_ExistingColorMaps()->Stack.at(i).ColorMap->GetScalarOpacityFunction();
				NewOF->DeepCopy(OF);
				CTF->SetScalarOpacityFunction(NewOF);
				cout << "CTF copid" << endl;
				this->Volume->SetCtf(CTF);
				cout << "Volume set" << endl;
				this->Volume->UpdateLookupTableRange();
				cout << "Lookup table range updated" << endl;
				this->mColorMap->reInitialize(CTF, 1);
				cout << "Reinitialize color map" << endl;



			}
		}
	}
	mqMorphoDigCore::instance()->Render();
}
void mqEditVolumeDialog::slotEditColorMapName()
{
	QString ActiveColorMap = this->Ui->comboColorMap->currentText();
	for (int i = 0; i < mqMorphoDigCore::instance()->Getmui_ExistingColorMaps()->Stack.size(); i++)
	{
		int iscustom = mqMorphoDigCore::instance()->Getmui_ExistingColorMaps()->Stack.at(i).isCustom;
		QString myExisingColorMapName = mqMorphoDigCore::instance()->Getmui_ExistingColorMaps()->Stack.at(i).Name;
		if (ActiveColorMap == myExisingColorMapName && iscustom)
		{
			QInputDialog *giveNameDialog = new QInputDialog();
			bool dialogResult;
			QString newColormapName = giveNameDialog->getText(0, "Change color map name", "Name:", QLineEdit::Normal,
				myExisingColorMapName, &dialogResult);
			if (dialogResult)
			{

				cout << "new color map given:" << newColormapName.toStdString() << endl;
				if (mqMorphoDigCore::instance()->colorMapNameAlreadyExists(newColormapName) == 1)
				{
					QMessageBox msgBox;
					msgBox.setText("Can't change custom map name : name already exists.");
					msgBox.exec();
					return;
				}
				if (newColormapName.length() == 0)
				{
					QMessageBox msgBox;
					msgBox.setText("Can't save custom map: name length =0.");
					msgBox.exec();
					return;
				}
				mqMorphoDigCore::instance()->Getmui_ExistingColorMaps()->Stack.at(i).Name = newColormapName;
				mqMorphoDigCore::instance()->Setmui_ActiveColorMap(newColormapName, mqMorphoDigCore::instance()->Getmui_ExistingColorMaps()->Stack.at(i).ColorMap);
				this->RefreshComboColorMaps();
				//mqMorphoDigCore::instance()->createCustomColorMap(newColormapName, this->STC);				
				//this->UpdateUI();
			}
			else
			{
				cout << "cancel " << endl;
			}

			//this->mColorMap->reInitialize(mqMorphoDigCore::instance()->Getmui_ExistingColorMaps()->Stack.at(i).ColorMap);



		}
	}

}
void mqEditVolumeDialog::slotDeleteColorMap()
{

	QString ActiveColorMap = this->Ui->comboColorMap->currentText();
	for (int i = 0; i < mqMorphoDigCore::instance()->Getmui_ExistingColorMaps()->Stack.size(); i++)
	{
		int iscustom = mqMorphoDigCore::instance()->Getmui_ExistingColorMaps()->Stack.at(i).isCustom;
		QString myExisingColorMapName = mqMorphoDigCore::instance()->Getmui_ExistingColorMaps()->Stack.at(i).Name;
		if (ActiveColorMap == myExisingColorMapName && iscustom)
		{

			mqMorphoDigCore::instance()->deleteColorMap(i);
			//mqMorphoDigCore::instance()->Getmui_ExistingColorMaps()->Stack.erase(mqMorphoDigCore::instance()->Getmui_ExistingColorMaps()->Stack.begin() + i);
			//mqMorphoDigCore::instance()->Setmui_ActiveColorMap(newColormapName, mqMorphoDigCore::instance()->Getmui_ExistingColorMaps()->Stack.at(i).ColorMap);

			this->RefreshComboColorMaps();
			this->mColorMap->reInitialize(mqMorphoDigCore::instance()->Getmui_ActiveColorMap()->ColorMap, 1);
			mqMorphoDigCore::instance()->Render();
			//mqMorphoDigCore::instance()->createCustomColorMap(newColormapName, this->STC);				
			//this->UpdateUI();


		//this->mColorMap->reInitialize(mqMorphoDigCore::instance()->Getmui_ExistingColorMaps()->Stack.at(i).ColorMap);



		}
	}
}
void mqEditVolumeDialog::slotLoadPreset(int idx)
{

	this->LoadPreset();

}
void mqEditVolumeDialog::RefreshComboColorMaps()
{

	cout << "RefreshComboColorMaps" << endl;
	this->Ui->deleteColorMap->setDisabled(true);
	this->Ui->editColorMap->setDisabled(true);
	this->Ui->reinitializeColorMap->setDisabled(true);
	this->Ui->comboColorMap->clear();
	this->Ui->comboColorMap->addItem("");
	ExistingColorMaps *MyCM = mqMorphoDigCore::instance()->Getmui_ExistingColorMaps();
	cout << "Found" << MyCM->Stack.size() << "color maps" << endl;
	for (int i = 0; i < MyCM->Stack.size(); i++)
	{

		this->Ui->comboColorMap->addItem(MyCM->Stack.at(i).Name);


	}
	
	
		this->Ui->comboColorMap->setCurrentIndex(0);
			
}

void mqEditVolumeDialog::slotAcceptSuggestedMax()
{
	
	this->mColorMap->SetCTFMax(this->Ui->suggestedMax->value());


}

void mqEditVolumeDialog::slotAcceptSuggestedMin()
{

	this->mColorMap->SetCTFMin(this->Ui->suggestedMin->value());

}

void mqEditVolumeDialog::slotSaveActorMinMaxHaveBeenChangedInWidget()
{
	cout << "slotSaveActorMinMaxHaveBeenChangedInWidget" << endl;
	if (this->Volume != NULL) {
		double newMin = this->mColorMap->getSTCMin();
		double newMax = this->mColorMap->getSTCMax();
		double oldMin = this->Volume->GetScalarDisplayMin();
		double oldMax = this->Volume->GetScalarDisplayMax();
		if (oldMin != newMin || oldMax != newMax)
		{
			std::string action = "Update volume's display range";
			int mCount = BEGIN_UNDO_SET(action);
			this->Volume->SaveState(mCount);
			double newMin = this->mColorMap->getSTCMin();
			double newMax = this->mColorMap->getSTCMax();
			this->Volume->SetScalarDisplayMin(newMin);
			this->Volume->SetScalarDisplayMax(newMax);
			this->Volume->Modified();
			END_UNDO_SET();
		}
	}

}
void mqEditVolumeDialog::slotInterpolationToLinear(bool isChecked)
{
	if (this->Volume != NULL) {
		this->Volume->SetInterpolationToLinear(isChecked);
		mqMorphoDigCore::instance()->Render();
	}

 }
void mqEditVolumeDialog::slotScalarOpacityUnitDistance(double SOUD)
{
	if (this->Volume != NULL && SOUD>0) {
		//cout << "ScalarOpacityUnitDistance:" << SOUD << endl;

			this->Volume->SetScalarOpacityUnitDistance(SOUD);
		
		mqMorphoDigCore::instance()->Render();
	}
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
	if (this->Volume != NULL && this->Volume->GetSelected()==1) {
		
		
		if (this->Volume->GetdisplayROI() == 1)
		{

			this->Ui->displayROI->setChecked(false);
		}
		else
		{
			this->Ui->displayROI->setChecked(true);
		}

		
		

		QString mylabel(this->Volume->GetName().c_str());
		this->Ui->VolumeName->setText(mylabel);
		//this->mColorMap->setMinMax(this->Volume->GetScalarDisplayMin(), this->Volume->GetScalarDisplayMax());
		this->mColorMap->reInitialize(this->Volume->GetCtf());
		if (this->Volume->GetProperty()->GetInterpolationType() == VTK_LINEAR_INTERPOLATION)
		{
			this->Ui->interpolationToLinear->setChecked(true);
		}
		else
		{
			this->Ui->interpolationToLinear->setChecked(false);
		}
		this->Ui->scalarOpacityUnitDistance->setValue(this->Volume->GetProperty()->GetScalarOpacityUnitDistance());
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
		
		this->RefreshComboColorMaps();
		this->RefreshSuggestedRange();

		
	}
	
}
void mqEditVolumeDialog::slotRefreshColorMaps()
{
	cout << "slotRefreshColorMaps" << endl;
	this->RefreshComboColorMaps();
}
void mqEditVolumeDialog::slotReinitializeColorMap()
{
	this->LoadPreset();

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

