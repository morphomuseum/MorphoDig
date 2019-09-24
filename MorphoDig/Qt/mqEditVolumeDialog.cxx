/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqEditVolumeDialog.h"

#include "ui_mqEditVolumeDialog.h"
#include "MorphoDigVersion.h"
#include "mqMorphoDigCore.h"
#include "mqUndoStack.h"
#include "mqOpenDataReaction.h"
#include "mqSaveMHDMHADialogReaction.h"
#include "vtkMDVolume.h"
#include "vtkMDActor.h"
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
#include <QDialog>
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
	this->Ui->pencilSearchSize->setValue(mqMorphoDigCore::instance()->Getmui_PencilSize());
	connect(mqMorphoDigCore::instance(), SIGNAL(pencilSizeChanged(int)), this, SLOT(slotRefreshPencilSearchSize(int)));
	connect(this->Ui->pencilSearchSize, SIGNAL(valueChanged(int)), this, SLOT(slotPencilSearchSizeChanged(int)));

	this->Volume_Coll = NULL;
	this->Volume = NULL;

	QString mylabel("...");
	this->Ui->VolumeName->setText(mylabel);
	this->Ui->isVisibleCropBox->setEnabled(true);
	vtkDiscretizableColorTransferFunction* STC = mqMorphoDigCore::instance()->GetOneColorMap();
	
	cout << "Edit Volume Dialog: Create mqColorOpacityEditorWidget!" << endl;
	mqColorOpacityEditorWidget *someMap = new mqColorOpacityEditorWidget(STC, this->Ui->PropertiesFrame,0);
	//cout << "Try that!" << endl;
	this->mColorMap = someMap;


	this->GetFirstSelectedVolume();
	this->Ui->oX->setButtonSymbols(QAbstractSpinBox::NoButtons);
	this->Ui->oY->setButtonSymbols(QAbstractSpinBox::NoButtons);
	this->Ui->oZ->setButtonSymbols(QAbstractSpinBox::NoButtons);

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
	this->Ui->res0->setButtonSymbols(QAbstractSpinBox::NoButtons);
	this->Ui->res1->setButtonSymbols(QAbstractSpinBox::NoButtons);
	this->Ui->res2->setButtonSymbols(QAbstractSpinBox::NoButtons);
	this->Ui->dim0->setButtonSymbols(QAbstractSpinBox::NoButtons);
	this->Ui->dim1->setButtonSymbols(QAbstractSpinBox::NoButtons);
	this->Ui->dim2->setButtonSymbols(QAbstractSpinBox::NoButtons);
	
	this->Ui->dim0->setMinimum(0);
	this->Ui->dim1->setMinimum(0);
	this->Ui->dim2->setMinimum(0);
	this->Ui->res0->setMinimum(-DBL_MAX);
	this->Ui->res1->setMinimum(-DBL_MAX);
	this->Ui->res2->setMinimum(-DBL_MAX);

	this->Ui->dim0->setMaximum(INT_MAX);
	this->Ui->dim1->setMaximum(INT_MAX);
	this->Ui->dim2->setMaximum(INT_MAX);
	this->Ui->res0->setMaximum(DBL_MAX);
	this->Ui->res1->setMaximum(DBL_MAX);
	this->Ui->res2->setMaximum(DBL_MAX);

	this->Ui->res0->setDecimals(7);
	this->Ui->res1->setDecimals(7);
	this->Ui->res2->setDecimals(7);


	this->Ui->oX->setMinimum(-DBL_MAX); 
	this->Ui->oY->setMinimum(-DBL_MAX);
	this->Ui->oZ->setMinimum(-DBL_MAX);

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


	this->Ui->oX->setMaximum(DBL_MAX); 
	this->Ui->oY->setMaximum(DBL_MAX); 
	this->Ui->oZ->setMaximum(DBL_MAX);
	

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

	this->Ui->oX->setDecimals(7);
	this->Ui->oY->setDecimals(7);
	this->Ui->oZ->setDecimals(7);
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
	this->Ui->comboMapper->setSizeAdjustPolicy(QComboBox::AdjustToContents);
	this->Ui->comboColorMap->clear();
	this->Ui->comboMapper->addItem("Smart Volume Mapper");
	this->Ui->comboMapper->addItem("GPU Ray Cast Mapper");
	this->Ui->comboMapper->addItem("OPENGL Ray Cast Mapper");

	connect(this->Ui->comboColorMap, SIGNAL(activated(int)), this, SLOT(slotLoadPreset(int)));

	connect(this->Ui->comboMapper, SIGNAL(activated(int)), this, SLOT(slotMapper(int)));

	this->Ui->scalarOpacityUnitDistance->setMaximum(DBL_MAX);

	
	connect(this->Ui->ApplyMatrix, SIGNAL(pressed()), this, SLOT(slotapplyMatrixToAllSelectedVolumes()));
	connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotsaveVolume()));
	connect(this->Ui->Reinit, SIGNAL(pressed()), this, SLOT(slotReinitMatrix()));
	connect(this->Ui->Refresh, SIGNAL(pressed()), this, SLOT(slotRefreshMatrix()));
	connect(this->Ui->reinitializeMASK, SIGNAL(pressed()), this, SLOT(slotReinitializeMask()));
	connect(this->Ui->invertMASK, SIGNAL(pressed()), this, SLOT(slotInvertMask()));
	connect(this->Ui->hardenMASK, SIGNAL(pressed()), this, SLOT(slotHardenMask()));

	QAction* actionImportMask = new QAction(tr("&ImportMask"), this);
	actionImportMask->setToolTip(tr("Import Mask."));
	this->Ui->importMASK->addAction(actionImportMask);
	this->Ui->importMASK->setDefaultAction(actionImportMask);
	QIcon icon;
	icon.addFile(QStringLiteral(":/Icons/fileopen22.png"), QSize(), QIcon::Normal, QIcon::Off);
	actionImportMask->setIcon(icon);
	new mqOpenDataReaction(actionImportMask, 25);
	

	QAction* actionExportMask = new QAction(tr("&ExportMask"), this);
	actionExportMask->setToolTip(tr("Export Mask."));
	this->Ui->exportMASK->addAction(actionExportMask);
	this->Ui->exportMASK->setDefaultAction(actionExportMask);
	QIcon icon2;
	icon2.addFile(QStringLiteral(":/Icons/filesave22.png"), QSize(), QIcon::Normal, QIcon::Off);
	actionExportMask->setIcon(icon2);	
	new mqSaveMHDMHADialogReaction(actionExportMask, 1);

	
	connect(this->Ui->maskWithSurface, SIGNAL(pressed()), this, SLOT(slotMaskWithSurface()));
	connect(this->Ui->cropVolume, SIGNAL(pressed()), this, SLOT(slotcropVolumeClicked()));
	//
	connect(this->Ui->maskR, SIGNAL(clicked(bool)), this, SLOT(slotMaskRClicked(bool)));
	connect(this->Ui->unmaskR, SIGNAL(clicked(bool)), this, SLOT(slotUnmaskRClicked(bool)));
	connect(this->Ui->maskInsideR, SIGNAL(clicked(bool)), this, SLOT(slotMaskInsideRClicked(bool)));
	connect(this->Ui->maskOutsideR, SIGNAL(clicked(bool)), this, SLOT(slotMaskOutsideRClicked(bool)));
	connect(this->Ui->pencilSphereR, SIGNAL(clicked(bool)), this, SLOT(slotPencilMaskSphereRClicked(bool)));
	connect(this->Ui->pencilTubeR, SIGNAL(clicked(bool)), this, SLOT(slotPencilMaskTubeRClicked(bool)));

	connect(this->Ui->interpolationToLinear, SIGNAL(clicked(bool)), this, SLOT(slotInterpolationToLinear(bool)));
	connect(this->Ui->scalarOpacityUnitDistance, SIGNAL(valueChanged(double)), this, SLOT(slotScalarOpacityUnitDistance(double)));
	connect(this->mColorMap, SIGNAL(shiftOrSlideStopped()), this, SLOT(slotSaveActorMinMaxHaveBeenChangedInWidget()));
	connect(this->mColorMap, SIGNAL(minmaxChanged()), this, SLOT(slotRefreshSliceXY()));
	connect(this->mColorMap, SIGNAL(minmaxChanged()), this, SLOT(slotRefreshSliceXZ()));
	connect(this->mColorMap, SIGNAL(minmaxChanged()), this, SLOT(slotRefreshSliceYZ()));
	connect(this->mColorMap, SIGNAL(minmaxChanged()), this, SLOT(slotSaveActorMinMaxHaveBeenChangedInWidget()));
	connect(mqMorphoDigCore::instance(), SIGNAL(volumesMightHaveChanged()), this, SLOT(slotRefreshUi()));
	connect(mqMorphoDigCore::instance(), SIGNAL(volumeSelectionChanged()), this, SLOT(slotRefreshUi()));
	connect(this->Ui->cutMinPercent, SIGNAL(valueChanged(int)), this, SLOT(slotRefreshSuggestedRange()));
	connect(this->Ui->cutMaxPercent, SIGNAL(valueChanged(int)), this, SLOT(slotRefreshSuggestedRange()));
	connect(this->Ui->sliderXY, SIGNAL(valueChanged(int)), this, SLOT(slotSliderXYChanged(int)));
	connect(this->Ui->sliderXZ, SIGNAL(valueChanged(int)), this, SLOT(slotSliderXZChanged(int)));
	connect(this->Ui->sliderYZ, SIGNAL(valueChanged(int)), this, SLOT(slotSliderYZChanged(int)));
	


	connect(this->Ui->pushScalarSuggestedMax, SIGNAL(pressed()), this, SLOT(slotAcceptSuggestedMax()));
	connect(this->Ui->pushScalarSuggestedMin, SIGNAL(pressed()), this, SLOT(slotAcceptSuggestedMin()));
	connect(this->Ui->reinitializeColorMap, SIGNAL(pressed()), this, SLOT(slotReinitializeColorMap()));
	connect(this->Ui->editColorMap, SIGNAL(pressed()), this, SLOT(slotEditColorMapName()));
	connect(this->Ui->deleteColorMap, SIGNAL(pressed()), this, SLOT(slotDeleteColorMap()));
	connect(mqMorphoDigCore::instance(), SIGNAL(colorMapsChanged()), this, SLOT(slotRefreshColorMaps()));
	connect(this->mColorMap, SIGNAL(changeFinished()), this, SLOT(slotRefreshDialog()));
	connect(this->Ui->lassoOn, SIGNAL(pressed()), mqMorphoDigCore::instance(), SLOT(slotLassoMaskInside()));
	connect(this->Ui->rubberOn, SIGNAL(pressed()), mqMorphoDigCore::instance(), SLOT(slotRubberMaskInside()));
	//connect(this->Ui->lassoOn2, SIGNAL(pressed()), mqMorphoDigCore::instance(), SLOT(slotLassoMaskOutside()));
	//connect(this->Ui->rubberOn2, SIGNAL(pressed()), mqMorphoDigCore::instance(), SLOT(slotRubberMaskOutside()));



	/*QAction* actiondisplayROIToggle = new QAction(tr("&displayROI"), this);
	actiondisplayROIToggle->setCheckable(true);
	actiondisplayROIToggle->setToolTip(tr("display clipping ROI."));
	this->Ui->displayROI->addAction(actiondisplayROIToggle);
	this->Ui->displayROI->setDefaultAction(actiondisplayROIToggle);
	QIcon icon;
	icon.addFile(QStringLiteral(":/Icons/eye_opened.png"), QSize(), QIcon::Normal, QIcon::Off);
	icon.addFile(QStringLiteral(":/Icons/eye_closed.png"), QSize(), QIcon::Normal, QIcon::On);
	//  exportColorMap->setIcon(icon);
	actiondisplayROIToggle->setIcon(icon);*/

	
	connect(this->Ui->isVisible, SIGNAL(clicked(bool)), this, SLOT(slotisVisibleClicked(bool)));
	connect(this->Ui->isVisibleXY, SIGNAL(clicked(bool)), this, SLOT(slotisVisibleXYClicked(bool)));
	connect(this->Ui->isVisibleXZ, SIGNAL(clicked(bool)), this, SLOT(slotisVisibleXZClicked(bool)));
	connect(this->Ui->isVisibleYZ, SIGNAL(clicked(bool)), this, SLOT(slotisVisibleYZClicked(bool)));
	connect(this->Ui->isVisibleVR, SIGNAL(clicked(bool)), this, SLOT(slotisVisibleVRClicked(bool)));
	connect(this->Ui->isVisibleCropBox, SIGNAL(clicked(bool)), this, SLOT(slotisVisibleCropBoxClicked(bool)));

	connect(this->Ui->useImageDataBinForVR, SIGNAL(clicked(bool)), this, SLOT(slotuseImageDataBinForVRClicked(bool)));

	connect(this->Ui->displayROI, SIGNAL(pressed()), this, SLOT(slotdisplayROIPressed()));
	connect(this->Ui->enableROI, SIGNAL(clicked(bool)), this, SLOT(slotEnableROIClicked(bool)));
	connect(this->Ui->enableMASK, SIGNAL(clicked(bool)), this, SLOT(slotEnableMASKClicked(bool)));
	//virtual void slotEnableROIPressed(bool isChecked);
	this->Ui->displayROI->setChecked(true);

}


//-----------------------------------------------------------------------------
mqEditVolumeDialog::~mqEditVolumeDialog()
{

 //depending on what is 
	
  delete this->Ui;
}
void mqEditVolumeDialog::reject()
{
	/*cout << "reject!!!"<< endl;
	this->accept();
	this->hide();*/
	/*setResult(QDialog::Accepted);//modification de la valeur de result
	emit accepted();//signal accepted()
	emit finished(result());//signal finished() avec le résultat en paramètre
	hide();*///cache la fenêtre
	//this->done(QDialog::Accepted);
	//this->done(QDialog::Accepted);
}
int mqEditVolumeDialog::SomeThingHasChanged()
{
	int something_has_changed = 0;
	if (this->Volume != NULL && this->CurrentVolumeInCollection())
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

void mqEditVolumeDialog::slotMaskInsideRClicked(bool isChecked)
{
	if (isChecked)
	{
		mqMorphoDigCore::instance()->Setmui_MaskInside(1);
	}
}
void mqEditVolumeDialog::slotMaskOutsideRClicked(bool isChecked)
{
	if (isChecked)
	{
		mqMorphoDigCore::instance()->Setmui_MaskInside(0);
	}
}
void mqEditVolumeDialog::slotMaskRClicked(bool isChecked)
{
	if (isChecked)
	{
		mqMorphoDigCore::instance()->Setmui_MaskOn(1);
	}
}
void mqEditVolumeDialog::slotUnmaskRClicked(bool isChecked)
{
	if (isChecked)
	{
		mqMorphoDigCore::instance()->Setmui_MaskOn(0);
	}
}
void mqEditVolumeDialog::slotPencilMaskSphereRClicked(bool isChecked)
{
	if (isChecked)
	{
		mqMorphoDigCore::instance()->Setmui_PencilMaskSphere(1);
	}
}
void mqEditVolumeDialog::slotPencilMaskTubeRClicked(bool isChecked)
{
	if (isChecked)
	{
		mqMorphoDigCore::instance()->Setmui_PencilMaskSphere(0);
	}
}

void mqEditVolumeDialog::slotRefreshPencilSearchSize(int newsize)
{
	this->Ui->pencilSearchSize->setValue(newsize);
}
void mqEditVolumeDialog::slotPencilSearchSizeChanged(int newSize)
{
	mqMorphoDigCore::instance()->Setmui_PencilSize(newSize);

}

void mqEditVolumeDialog::slotRefreshUi()
{
	this->GetFirstSelectedVolume();
	this->UpdateUI();
}

void mqEditVolumeDialog::slotdisplayROIPressed()
{
	if (this->Volume != NULL && this->CurrentVolumeInCollection() && this->Volume->GetSelected() == 1)
	{
		if (this->Ui->displayROI->isChecked())
			//if (this->Volume->GetdisplayROI() == 0)
		{
			this->Volume->SetdisplayROI(0);
		}
		else
		{

			this->Volume->SetdisplayROI(1);

		}
		mqMorphoDigCore::instance()->Render();
	}
}
void mqEditVolumeDialog::slotInvertMask()
{
	if (this->Volume != NULL && this->CurrentVolumeInCollection() && this->Volume->GetSelected() == 1)
	{
		this->Volume->InvertMask();				
		mqMorphoDigCore::instance()->Render();
	}
}
void mqEditVolumeDialog::slotHardenMask()
{
	if (this->Volume != NULL && this->CurrentVolumeInCollection() && this->Volume->GetSelected() == 1)
	{
		this->Volume->HardenMask();
		this->UpdateUI();
		mqMorphoDigCore::instance()->Render();
	}
}
void mqEditVolumeDialog::slotMaskWithSurface()
{
	if (this->Volume != NULL && this->CurrentVolumeInCollection() && this->Volume->GetSelected() == 1)
	{
		vtkIdType num_selected_volumes = mqMorphoDigCore::instance()->getVolumeCollection()->GetNumberOfSelectedVolumes();
		if (num_selected_volumes == 0) {
			QMessageBox msgBox;
			msgBox.setText("No volume selected. Please select at least one volume to use this option.");
			msgBox.exec();
			return;
		}
		else if (num_selected_volumes > 1)
		{
			QMessageBox msgBox;
			msgBox.setText("More than one volume are currently selected. Please only select one volume");

			msgBox.exec();
			return;

		}
		vtkIdType num_selected_actors = mqMorphoDigCore::instance()->getActorCollection()->GetNumberOfSelectedActors();
		if (num_selected_actors == 0) {
			QMessageBox msgBox;
			msgBox.setText("No surface selected. Please select at least one surface to use this option.");
			msgBox.exec();
			return;
		}
		else if (num_selected_actors > 1)
		{
			QMessageBox msgBox;
			msgBox.setText("More than one actor are currently selected. Please only select one actor.");

			msgBox.exec();
			return;

		}
		vtkMDVolume *myVolume = mqMorphoDigCore::instance()->GetFirstSelectedVolume();
		vtkMDActor *myActor = mqMorphoDigCore::instance()->GetFirstSelectedActor();
		mqMorphoDigCore::instance()->MaskWithSurface(myVolume, myActor);
		this->UpdateUI();
		mqMorphoDigCore::instance()->Render();
	}
}
void mqEditVolumeDialog::slotReinitializeMask()
{
	if (this->Volume != NULL && this->CurrentVolumeInCollection() && this->Volume->GetSelected() == 1)
	{
		this->Volume->InitializeMask();
		this->Volume->UpdateMaskData(this->Volume->GetMask());
		this->Volume->SetImageDataBinComputed(0);
		if (this->Volume->GetuseImageDataBinForVR() == 1) { this->Volume->ComputeImageDataBin(); }
		mqMorphoDigCore::instance()->Render();
	}
}
void mqEditVolumeDialog::slotEnableMASKClicked(bool isChecked)
{
	if (this->Volume != NULL && this->CurrentVolumeInCollection() && this->Volume->GetSelected() == 1)
	{
		if (isChecked)
			//if (this->Volume->GetdisplayROI() == 0)
		{
			cout << "Call volume enable mask" << endl;
			this->Volume->SetMaskEnabled(true);
			this->Ui->pencilOn->setEnabled(true);
			this->Ui->rubberOn->setEnabled(true);
			//this->Ui->lassoOn2->setEnabled(true);
			//this->Ui->rubberOn2->setEnabled(true);
			this->Ui->maskR->setEnabled(true);
			this->Ui->unmaskR->setEnabled(true);

			this->Ui->lassoOn->setEnabled(true);

			this->Ui->reinitializeMASK->setEnabled(true);
			
			this->Ui->maskWithSurface->setEnabled(true);
			this->Ui->invertMASK->setEnabled(true);

			this->Ui->hardenMASK->setEnabled(true);
			this->Ui->importMASK->setEnabled(true);
			this->Ui->exportMASK->setEnabled(true);
			
			this->Ui->pencilSearchSize->setEnabled(true);
			this->Ui->pencilSphereR->setEnabled(true);
			this->Ui->pencilTubeR->setEnabled(true);
			this->Ui->maskInsideR->setEnabled(true);
			this->Ui->maskOutsideR->setEnabled(true);

		}
		else
		{
			cout << "Call volume disable mask" << endl;

			this->Volume->SetMaskEnabled(false);
			this->Ui->pencilOn->setEnabled(false);
			this->Ui->rubberOn->setEnabled(false);
			this->Ui->lassoOn->setEnabled(false);
			//this->Ui->lassoOn2->setEnabled(false);
			//this->Ui->rubberOn2->setEnabled(false);
			this->Ui->maskR->setEnabled(false);
			this->Ui->unmaskR->setEnabled(false);
			this->Ui->reinitializeMASK->setEnabled(false);
			
			this->Ui->maskWithSurface->setEnabled(false);
			this->Ui->invertMASK->setEnabled(false);
			this->Ui->hardenMASK->setEnabled(false);
			this->Ui->importMASK->setEnabled(false);
			this->Ui->exportMASK->setEnabled(false);
			this->Ui->pencilSearchSize->setEnabled(false);
			this->Ui->pencilSphereR->setEnabled(false);
			this->Ui->pencilTubeR->setEnabled(false);
			this->Ui->maskInsideR->setEnabled(false);
			this->Ui->maskOutsideR->setEnabled(false);


		}
		mqMorphoDigCore::instance()->Render();
	}
}
void mqEditVolumeDialog::slotEnableROIClicked(bool isChecked)
{
	if (this->Volume != NULL && this->CurrentVolumeInCollection() && this->Volume->GetSelected() == 1)
	{
		if (isChecked)
			//if (this->Volume->GetdisplayROI() == 0)
		{
			cout << "Call volume create box function" << endl;
			this->Volume->SetenableROI(true);			
			this->Ui->displayROI->setEnabled(true);
		}
		else
		{
			cout << "Call volume remove box function" << endl;

			this->Volume->SetenableROI(false);
			this->Ui->displayROI->setEnabled(false);
			this->Ui->displayROI->setChecked(false);


		}
		mqMorphoDigCore::instance()->Render();
	}

}

void mqEditVolumeDialog::slotuseImageDataBinForVRClicked(bool isChecked)
{
	if (this->Volume != NULL && this->CurrentVolumeInCollection() && this->Volume->GetSelected() == 1)
	{
		if (isChecked)
		{
			int res = this->Volume->SetuseImageDataBinForVR(1);
			/*if (res == 0) {
				cout << "No!" << endl;
				this->Ui->useImageDataBinForVR->setChecked(false);
			}*/
		}
		else
		{
			int res = this->Volume->SetuseImageDataBinForVR(0);
			if (res == 0) {
				cout << "No!" << endl;
				this->Ui->useImageDataBinForVR->setChecked(true);
			}
		}
		mqMorphoDigCore::instance()->Render();
	}
}
void mqEditVolumeDialog::slotisVisibleClicked(bool isChecked)
{
	if (this->Volume != NULL && this->CurrentVolumeInCollection() && this->Volume->GetSelected() == 1)
	{
		
		if (isChecked)
		{
			this->Volume->SetisVisible(1);
		}
		else
		{
			this->Volume->SetisVisible(0);

		}
		this->UpdateUI();
		mqMorphoDigCore::instance()->Render();
	}
}
void mqEditVolumeDialog::CropVolume()
{
	cout << "Crop volume inside EditVolumeDialog" << endl;
	// 1 trouver les bornes
	this->Volume->CropVolume();
	this->UpdateUI();
	// 2 
}

void mqEditVolumeDialog::slotcropVolumeClicked()
{
	this->CropVolume();
	this->Volume->RemoveCropBox();
	this->Ui->isVisibleCropBox->setChecked(false);
	this->Ui->cropVolume->setEnabled(false);
}
void mqEditVolumeDialog::slotisVisibleCropBoxClicked(bool isChecked)
{
	if (this->Volume != NULL && this->CurrentVolumeInCollection() && this->Volume->GetSelected() == 1)
	{

		if (isChecked)
		{
			
			this->Volume->CreateCropBox();
			this->Ui->cropVolume->setEnabled(true);
			
		}
		else
		{
			this->Volume->RemoveCropBox();
			this->Ui->cropVolume->setEnabled(false);
			

		}

		mqMorphoDigCore::instance()->Render();
	}
}
void mqEditVolumeDialog::slotisVisibleVRClicked(bool isChecked)
{
	if (this->Volume != NULL && this->CurrentVolumeInCollection() && this->Volume->GetSelected() == 1)
	{

		if (isChecked)
		{
			this->Volume->SetisVisibleVR(1);
			this->Ui->enableROI->setEnabled(true);
			this->Ui->interpolationToLinear->setEnabled(true);
			this->Ui->scalarOpacityUnitDistance->setEnabled(true);
			this->Ui->useImageDataBinForVR->setEnabled(true);
			if (this->Ui->enableROI->isChecked())
			{
				this->Ui->displayROI->setEnabled(true);
			}
		}
		else
		{
			this->Volume->SetisVisibleVR(0);
			this->Ui->enableROI->setEnabled(false);
			this->Ui->interpolationToLinear->setEnabled(false);
			this->Ui->scalarOpacityUnitDistance->setEnabled(false);
			this->Ui->useImageDataBinForVR->setEnabled(false);
			
			this->Ui->displayROI->setEnabled(false);
			
		}

		mqMorphoDigCore::instance()->Render();
	}
}

/*slotisVisibleVRClicked*/
/*virtual void slotSliderXYChanged(int val);
  virtual void slotSliderXZChanged(int val);
  virtual void slotSliderYZChanged(int val);*/

void mqEditVolumeDialog::slotSliderXYChanged(int val)
{
	if (this->Volume != NULL && this->CurrentVolumeInCollection() && this->Volume->GetSelected() == 1)
	{
		this->Volume->SetSliceNumberXY(val);
		mqMorphoDigCore::instance()->Render();
	}
}
void mqEditVolumeDialog::slotRefreshSliceXY()
{
	if (this->Volume != NULL && this->CurrentVolumeInCollection() && this->Volume->GetSelected() == 1 && this->Volume->GetisVisibleXY() == 1)
	{
		this->Volume->GetSliceXY2()->Update();
	}

}
void mqEditVolumeDialog::slotRefreshSliceXZ()
{
	if (this->Volume != NULL && this->CurrentVolumeInCollection() && this->Volume->GetSelected() == 1 && this->Volume->GetisVisibleXZ() == 1)
	{
		this->Volume->GetSliceXZ2()->Update();
	}

}
void mqEditVolumeDialog::slotRefreshSliceYZ()
{
	if (this->Volume != NULL && this->CurrentVolumeInCollection() && this->Volume->GetSelected() == 1 && this->Volume->GetisVisibleYZ()==1)
	{

		this->Volume->GetSliceYZ2()->Update();
	}

}
void mqEditVolumeDialog::slotSliderXZChanged(int val)
{
	if (this->Volume != NULL && this->CurrentVolumeInCollection() && this->Volume->GetSelected() == 1)
	{
		this->Volume->SetSliceNumberXZ(val);
		mqMorphoDigCore::instance()->Render();
	}
}
void mqEditVolumeDialog::slotSliderYZChanged(int val)
{
	if (this->Volume != NULL && this->CurrentVolumeInCollection() && this->Volume->GetSelected() == 1)
	{
		this->Volume->SetSliceNumberYZ(val);
		mqMorphoDigCore::instance()->Render();
	}
}

void mqEditVolumeDialog::slotisVisibleXYClicked(bool isChecked)
{
	if (this->Volume != NULL && this->CurrentVolumeInCollection() && this->Volume->GetSelected() == 1)
	{
		
		if (isChecked)
		{
			this->Volume->SetisVisibleXY(1);
			
		}
		else
		{
			this->Volume->SetisVisibleXY(0);
		}
		this->Ui->sliderXY->setEnabled(isChecked);
		mqMorphoDigCore::instance()->Render();
	}
}
void mqEditVolumeDialog::slotisVisibleXZClicked(bool isChecked)
{
	if (this->Volume != NULL && this->CurrentVolumeInCollection() && this->Volume->GetSelected() == 1)
	{
		
		if (isChecked)
		{
			this->Volume->SetisVisibleXZ(1);
		}
		else
		{
			this->Volume->SetisVisibleXZ(0);
		}
		this->Ui->sliderXZ->setEnabled(isChecked);
		mqMorphoDigCore::instance()->Render();
	}
}

void mqEditVolumeDialog::slotisVisibleYZClicked(bool isChecked)
{
	if (this->Volume != NULL && this->CurrentVolumeInCollection() && this->Volume->GetSelected() == 1)
	{
		
		if (isChecked)
		{
			this->Volume->SetisVisibleYZ(1);
		}
		else
		{
			this->Volume->SetisVisibleYZ(0);
		}
		this->Ui->sliderYZ->setEnabled(isChecked);

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
	if (this->Volume != NULL && this->CurrentVolumeInCollection())
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
void mqEditVolumeDialog::slotMapper(int idx)
{
	if (this->Volume != NULL && this->CurrentVolumeInCollection()) {
		this->Volume->SetenableROI(0);
		this->Volume->SetdisplayROI(0);
		this->Volume->SetMapperType(idx);		
		this->UpdateUI();
		mqMorphoDigCore::instance()->Render();
	}
	

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
	if (this->Volume != NULL && this->CurrentVolumeInCollection()) {
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
	if (this->Volume != NULL && this->CurrentVolumeInCollection()) {
		this->Volume->SetInterpolationToLinear(isChecked);
		mqMorphoDigCore::instance()->Render();
	}

 }
void mqEditVolumeDialog::slotScalarOpacityUnitDistance(double SOUD)
{
	if (this->Volume != NULL && this->CurrentVolumeInCollection() && SOUD>0) {
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

	if (this->Volume != NULL && this->CurrentVolumeInCollection() && this->Volume->GetSelected()==1) {

		int maskInside = mqMorphoDigCore::instance()->Getmui_MaskInside();
		if (maskInside ==1)
		{
			this->Ui->maskInsideR->setChecked(true);
		}
		else
		{
			this->Ui->maskOutsideR->setChecked(true);
		}
		int maskOn = mqMorphoDigCore::instance()->Getmui_MaskOn();
		if (maskOn == 1)
		{
			this->Ui->maskR->setChecked(true);
		}
		else
		{
			this->Ui->unmaskR->setChecked(true);
		}
		int pencilMaskSphere = mqMorphoDigCore::instance()->Getmui_PencilMaskSphere();
		if (pencilMaskSphere == 1)
		{
			this->Ui->pencilSphereR->setChecked(true);
		}
		else
		{
			this->Ui->pencilTubeR->setChecked(true);
		}
		double origin[3];
		this->Volume->GetImageData()->GetOrigin(origin);
		
		cout << "Volume origin: " << origin[0] << "," << origin[1] << "," << origin[2] << endl;
		this->Ui->oX->setValue(origin[0]);
		this->Ui->oY->setValue(origin[1]);
		this->Ui->oZ->setValue(origin[2]);
		int maskEnabled = this->Volume->GetMaskEnabled();
		if (maskEnabled)
		{
			this->Ui->enableMASK->setChecked(true);

			this->Ui->pencilOn->setDisabled(false);
			this->Ui->lassoOn->setDisabled(false);
			this->Ui->rubberOn->setDisabled(false);
			//this->Ui->lassoOn2->setsetDisabled(false);
			//this->Ui->rubberOn2->setDisabled(false);
			this->Ui->maskR->setDisabled(false);
			this->Ui->unmaskR->setDisabled(false);
			this->Ui->reinitializeMASK->setDisabled(false);
			this->Ui->maskWithSurface->setDisabled(false);
			this->Ui->invertMASK->setDisabled(false);
			this->Ui->hardenMASK->setDisabled(false);
			this->Ui->importMASK->setDisabled(false);
			this->Ui->exportMASK->setDisabled(false);
			this->Ui->pencilSearchSize->setDisabled(false);
			this->Ui->pencilSphereR->setDisabled(false);
			this->Ui->pencilTubeR->setDisabled(false);
			this->Ui->maskInsideR->setDisabled(false);
			this->Ui->maskOutsideR->setDisabled(false);
		}
		else
		{
			this->Ui->enableMASK->setChecked(false);
			this->Ui->pencilOn->setDisabled(true);
			this->Ui->lassoOn->setDisabled(true);
			this->Ui->rubberOn->setDisabled(true);
			//this->Ui->lassoOn2->setDisabled(true);
			//this->Ui->rubberOn2->setDisabled(true);
			this->Ui->maskR->setDisabled(true);
			this->Ui->unmaskR->setDisabled(true);
			this->Ui->reinitializeMASK->setDisabled(true);
			this->Ui->maskWithSurface->setDisabled(true);
			this->Ui->invertMASK->setDisabled(true);
			this->Ui->hardenMASK->setDisabled(true);
			this->Ui->importMASK->setDisabled(true);
			this->Ui->exportMASK->setDisabled(true);
			this->Ui->pencilSearchSize->setDisabled(true);
			this->Ui->pencilSphereR->setDisabled(true);
			this->Ui->pencilTubeR->setDisabled(true);
			this->Ui->maskInsideR->setDisabled(true);
			this->Ui->maskOutsideR->setDisabled(true);

		}
		int mtype = this->Volume->Getmapper_type();
		if (mtype ==0){ 
			this->Ui->comboMapper->setCurrentIndex(0); 
			this->Ui->enableMASK->setDisabled(true);

			this->Ui->pencilOn->setDisabled(true);
			this->Ui->lassoOn->setDisabled(true);
			this->Ui->rubberOn->setDisabled(true);
			//this->Ui->lassoOn2->setDisabled(true);
			//this->Ui->rubberOn2->setDisabled(true);
			this->Ui->maskR->setDisabled(true);
			this->Ui->unmaskR->setDisabled(true);
			this->Ui->reinitializeMASK->setDisabled(true);
			this->Ui->maskWithSurface->setDisabled(true);
			this->Ui->invertMASK->setDisabled(true);
			this->Ui->hardenMASK->setDisabled(true);
			this->Ui->importMASK->setDisabled(true);
			this->Ui->exportMASK->setDisabled(true);
			this->Ui->pencilSearchSize->setDisabled(true);
			this->Ui->pencilSphereR->setDisabled(true);
			this->Ui->pencilTubeR->setDisabled(true);
			this->Ui->maskInsideR->setDisabled(true);
			this->Ui->maskOutsideR->setDisabled(true);

		}
		else
		{
			this->Ui->enableMASK->setEnabled(true);
		}
		
		if (mtype == 1) { this->Ui->comboMapper->setCurrentIndex(1); }
		if (mtype == 2) { this->Ui->comboMapper->setCurrentIndex(2); }
		this->Ui->enableROI->setChecked(this->Volume->GetenableROI());
		this->Ui->displayROI->setChecked(this->Volume->GetdisplayROI());
		if (this->Volume->GetuseImageDataBinForVR() == 1)
		{
			this->Ui->useImageDataBinForVR->setChecked(true);
		}
		else
		{
			this->Ui->useImageDataBinForVR->setChecked(false);
		}
		if (this->Volume->GetisVisible() == 1)
		{
			this->Ui->isVisible->setChecked(true);
			
			this->Ui->isVisibleXY->setEnabled(true);
			this->Ui->isVisibleXZ->setEnabled(true);
			this->Ui->isVisibleYZ->setEnabled(true);
			this->Ui->isVisibleVR->setEnabled(true);
			this->Ui->enableROI->setEnabled(true);
			if (this->Ui->enableROI->isChecked())
			{
				this->Ui->displayROI->setEnabled(true);
			}
		}
		else
		{ 
			this->Ui->isVisible->setChecked(false);
			this->Ui->isVisibleXY->setEnabled(false);
			this->Ui->isVisibleXZ->setEnabled(false);
			this->Ui->isVisibleYZ->setEnabled(false);
			this->Ui->isVisibleVR->setEnabled(false);
			this->Ui->enableROI->setEnabled(false);
			this->Ui->displayROI->setEnabled(false);
		}

		if (this->Volume->GetisVisibleVR() == 1)
		{
			this->Ui->isVisibleVR->setChecked(true);
			if (this->Ui->isVisibleVR->isEnabled() && this->Volume->GetenableROI()==1)
			{
				//
				this->Ui->displayROI->setEnabled(true);
				if (this->Volume->GetdisplayROI() == 1)
				{
					this->Ui->displayROI->setChecked(true);
				}
				else
				{
					this->Ui->displayROI->setChecked(false);
				}
			}


		}
		else
		{
			this->Ui->isVisibleVR->setChecked(false);
			this->Ui->enableROI->setEnabled(false);
			this->Ui->displayROI->setEnabled(false);
			this->Ui->displayROI->setChecked(false);
			
		}
		/*if (this->Volume->GetdisplayROI() == 1)
		{
			
			this->Ui->displayROI->setChecked(true);
		}
		else
		{
			this->Ui->displayROI->setChecked(false);
		}
		if (this->Volume->GetenableROI() == 1)
		{
			this->Ui->enableROI->setChecked(true);
			this->Ui->displayROI->setEnabled(true);
		}
		else
		{
			this->Ui->enableROI->setChecked(false);
			this->Ui->displayROI->setChecked(false);
			this->Ui->displayROI->setEnabled(false);
		}
		*/


		if (this->Volume->GetisVisibleXY() == 1)
		{
			this->Ui->isVisibleXY->setChecked(true);
			this->Ui->sliderXY->setEnabled(true);
		}
		else
		{
			this->Ui->isVisibleXY->setChecked(false);
			this->Ui->sliderXY->setEnabled(false);
		}

		if (this->Volume->GetisVisibleXZ() == 1)
		{
			this->Ui->isVisibleXZ->setChecked(true);
			this->Ui->sliderXZ->setEnabled(true);
		}
		else
		{
			this->Ui->isVisibleXZ->setChecked(false);
			this->Ui->sliderXZ->setEnabled(false);
		}

		if (this->Volume->GetisVisibleYZ() == 1)
		{
			this->Ui->isVisibleYZ->setChecked(true);
			this->Ui->sliderYZ->setEnabled(true);

		}
		else
		{
			this->Ui->isVisibleYZ->setChecked(false);
			this->Ui->sliderYZ->setEnabled(false);
		}


		

		
		
		int dim[3];
		double res[3];
		this->Volume->GetImageData()->GetDimensions(dim);
		this->Ui->dim0->setValue(dim[0]);
		this->Ui->dim1->setValue(dim[1]);
		this->Ui->dim2->setValue(dim[2]);
		this->Ui->sliderXY->setMaximum(dim[2]);
		this->Ui->sliderXY->setValue((int) (dim[2]/2));
		this->Ui->sliderXZ->setMaximum(dim[1]);
		this->Ui->sliderXZ->setValue((int)(dim[1] / 2));
		this->Ui->sliderYZ->setMaximum(dim[0]);
		this->Ui->sliderYZ->setValue((int)(dim[0] / 2));


		this->Volume->GetImageData()->GetSpacing(res);
		this->Ui->res0->setValue(res[0]);
		this->Ui->res1->setValue(res[1]);
		this->Ui->res2->setValue(res[2]);
		int numcells = this->Volume->GetImageData()->GetNumberOfCells();
		int dataType = this->Volume->GetImageData()->GetScalarType();
		QString dataTypeAsString;
		if (dataType == VTK_UNSIGNED_SHORT)
		{
			dataTypeAsString = "Unsigned shorts (16 bits)";
		}
		else if (dataType == VTK_SHORT)
		{
			dataTypeAsString = "Signed shorts (16 bits)";
		}
		else if (dataType == VTK_CHAR)
		{
			dataTypeAsString = "Char (8 bits)";
		}
		else if (dataType == VTK_UNSIGNED_CHAR)
		{
			dataTypeAsString = "Unsigned char (8 bits)";
		}
		else if (dataType == VTK_SIGNED_CHAR)
		{
			dataTypeAsString = "Signed char (8 bits)";
		}
		else if (dataType == VTK_FLOAT)
		{
			dataTypeAsString = "Float (32 bits)";
		}
		else if (dataType == VTK_DOUBLE)
		{
			dataTypeAsString = "Double (64 bits)";
		}
		else if (dataType == VTK_BIT)
		{
			dataTypeAsString = "Bit (1 bit)";
		}
		else
		{
			dataTypeAsString = "Unknown";
		}
		this->Ui->dataType->setText(dataTypeAsString);
		//input->Get
		cout << "Read Volume: dim=" << dim[0] << ", " << dim[1] << ", " << dim[2] << "numcells="<<numcells<< endl;
		cout << "Dim0*Dim1*Dim2:" << dim[0]* dim[1]* dim[2] << endl;
		cout << "Spacing0*Spacing1*Spacing2:" << res[0] * res[1] * res[2] << endl;
		cout << "Image type:" << this->Volume->GetImageData()->GetScalarTypeAsString() << endl;
		cout << "Image type int:" << this->Volume->GetImageData()->GetScalarType() << "="<<VTK_UNSIGNED_SHORT<< "?"<<endl;
		cout << "Number of scalar components:" << this->Volume->GetImageData()->GetNumberOfScalarComponents() << endl;
		
		
		

		QString mylabel(this->Volume->GetName().c_str());
		this->Ui->VolumeName->setText(mylabel);
		//this->mColorMap->setMinMax(this->Volume->GetScalarDisplayMin(), this->Volume->GetScalarDisplayMax());
		this->mColorMap->reInitialize(this->Volume->GetCtf());
		//this->mColorMap->reInitializeHIST(this->Volume->GetHist(), this->Volume->GetImageData());
		this->mColorMap->setInitHistRangeMinMax(this->Volume->GetRangeMin(), this->Volume->GetRangeMax());
		this->mColorMap->reInitializeHIST(this->Volume->GetHist(), 255, this->Volume->GetRangeMin(), this->Volume->GetRangeMax(), this->Volume->GetScalarDisplayMin(), this->Volume->GetScalarDisplayMax());
		cout << "reInitializeHIST" << ", 255, " << this->Volume->GetRangeMin() << ", " << this->Volume->GetRangeMax() << ", " << this->Volume->GetScalarDisplayMin() << ", " << this->Volume->GetScalarDisplayMax() << endl;
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
	if (this->Volume != NULL && this->CurrentVolumeInCollection())
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
void mqEditVolumeDialog::ShowAndRefresh()
{
	this->GetFirstSelectedVolume();
	if (this->Volume == NULL || (this->Volume != NULL && this->Volume->GetSelected() == 0)) { this->GetFirstVolume(); }
	this->RefreshDialog();
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

void mqEditVolumeDialog::slotRefreshMatrix()
{
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

//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

