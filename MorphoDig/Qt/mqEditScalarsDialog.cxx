/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqEditScalarsDialog.h"
#include "ui_mqEditScalarsDialog.h"
#include "mqSetName.h"
#include "MorphoDigVersion.h"
#include "mqMorphoDigCore.h"
#include "mqSaveMAPDialogReaction.h"
#include "mqUndoStack.h"
#include "QDoubleSlider.h"
#include "mqColorOpacityEditorWidget.h"
#include "mqTransferFunctionWidget.h"
#include "mqMinimalWidget.h"
#include "vtkLMActor.h"
#include "vtkLMActorCollection.h"
#include <vtkDiscretizableColorTransferFunction.h>
#include<vtkPiecewiseFunction.h>
// we actually do not need glew...
//#include <GL/glew.h>
#include <QApplication>
#include <QMessageBox>
#include <QInputDialog>
#include <QFile>
#include <QRadioButton>
#include <QFileDialog>
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
mqEditScalarsDialog::mqEditScalarsDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqEditScalarsDialog())
{
	//ui.ColorEditor->initialize(stc, true, NULL, false);
	/*   QObject::connect(&this->Internals->ColorTableModel,
      SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), this,
      SIGNAL(xrgbPointsChanged()));
    QObject::connect(&this->Internals->OpacityTableModel,
      SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), this,
      SIGNAL(xvmsPointsChanged()));*/
	


	//1 populate active scalar combo box, and check which scalar is the active one!
	this->Ui->setupUi(this);
	this->setObjectName("mqEditScalarsDialog");
	this->Ui->comboActiveScalar->setSizeAdjustPolicy(QComboBox::AdjustToContents);
	vtkDiscretizableColorTransferFunction* STC = mqMorphoDigCore::instance()->GetOneColorMap();
	
	//ui.ColorEditor->initialize(stc, true, NULL, false);

	/*mqTransferFunctionWidget ColorEditor;
	ColorEditor.initialize(STC, true, NULL, false);
	ColorEditor.show();*/

	//this->Ui->frame->setVisible(false);
	//mqColorOpacityEditorWidget *mColorMap = new mqColorOpacityEditorWidget(STC, this->Ui->scrollAreaWidgetContents);
	/*QVBoxLayout* vbox = new QVBoxLayout(this->Ui->PropertiesFrame);
	vbox->setMargin(0);
	vbox->setSpacing(0);*/
	cout << "Create mqColorOpacityEditorWidget!" << endl;
	mqColorOpacityEditorWidget *someMap = new mqColorOpacityEditorWidget(STC, this->Ui->PropertiesFrame);
	//cout << "Try that!" << endl;
	this->mColorMap = someMap;

	//cout << "And?" << endl;

	//@@ Ca fout en l'air la fenêtre principale! (or je vois pas bien le rapport...) Il faudrait essayer de voi si on ne pourrait pas l'instancier autrement... 
	//this->Ui->PropertiesFrame->layout()->addWidget(this->mColorMap);
	
	//
	
	
	//mqTransferFunctionWidget *mColorScale = new mqTransferFunctionWidget(this->Ui->frame);

	/*mqTransferFunctionWidget *ColorEditor = new mqTransferFunctionWidget ();
	ColorEditor->initialize(STC, true, NULL, false);
	mqTransferFunctionWidget *OpacityEditor = new mqTransferFunctionWidget();
	OpacityEditor->initialize(STC, false, STC->GetScalarOpacityFunction(), true);*/
	/*auto layout = new QVBoxLayout;
	layout->setMargin(0);
	layout->addWidget(ColorEditor);
	layout->addWidget(OpacityEditor);*/
	//this->Ui->scrollAreaWidgetContents->setLayout(layout);
	//this->Ui->verticalLayout->addWidget(ColorEditor);
	//this->Ui->verticalLayout->addWidget(OpacityEditor);
	//this->Ui->verticalLayout->addWidget(OpacityEditor);

	//this->Ui->scrollAreaWidgetContents->addWidget(ColorEditor);
	//this->Ui->scrollAreaWidgetContents->addWidget(OpacityEditor);
	//layout->addWidget(ColorEditor);
	//layout->addWidget(OpacityEditor);
	//this->Ui->scrollAreaWidgetContents->setLayout(layout);
	//this->Ui->editor->setLayout(layout);
	//this->Ui->editor->setVisible(true);
	//this->Ui->ColorEditor->initialize(STC, true, NULL, false);
	//this->Ui->OpacityEditor->initialize(STC, false, STC->GetScalarOpacityFunction(), true);
	
	
	//mColorScale->render();
	//this->Ui->frame->setVisible(true);
	//this->Ui->frame = NULL;
	//this->Ui->frame->ins
	//this->Ui->ColorEditor-
	this->Ui->comboColorMap->setSizeAdjustPolicy(QComboBox::AdjustToContents);
	connect(mqMorphoDigCore::instance(), SIGNAL(colorMapsChanged()), this, SLOT(slotRefreshColorMaps()));
	connect(mqMorphoDigCore::instance(), SIGNAL(existingArraysChanged()), this, SLOT(slotRefreshComboScalars()));
	connect(mqMorphoDigCore::instance(), SIGNAL(activeArrayChanged()), this, SLOT(slotRefreshComboScalars()));
	connect(mqMorphoDigCore::instance(), SIGNAL(actorsMightHaveChanged()), this, SLOT(slotRefreshSuggestedRange()));
	connect(mqMorphoDigCore::instance(), SIGNAL(activeArrayChanged()), this, SLOT(slotRefreshSuggestedRange()));
	connect(this->Ui->comboActiveScalar, SIGNAL(activated(int)), this, SLOT(slotActiveScalarChanged(int)));
	connect(this->Ui->comboColorMap, SIGNAL(activated(int)), this, SLOT(slotActiveColorMapChanged(int)));
	
	
	this->Ui->currentMin->setButtonSymbols(QAbstractSpinBox::NoButtons);
	this->Ui->currentMax->setButtonSymbols(QAbstractSpinBox::NoButtons);
	this->Ui->suggestedMin->setButtonSymbols(QAbstractSpinBox::NoButtons);
	this->Ui->suggestedMax->setButtonSymbols(QAbstractSpinBox::NoButtons);
	this->Ui->currentMin->setMinimum(-DBL_MAX);
	this->Ui->currentMax->setMinimum(-DBL_MAX);
	this->Ui->currentMax->setValue(1);
	this->Ui->currentMin->setValue(0);
	this->ctfMin = 0;
	this->ctfMax = 1;

	
	this->Ui->suggestedMin->setMinimum(-DBL_MAX);
	this->Ui->suggestedMax->setMinimum(-DBL_MAX);

	this->Ui->currentMin->setMaximum(DBL_MAX);
	this->Ui->currentMax->setMaximum(DBL_MAX);
	this->Ui->suggestedMin->setMaximum(DBL_MAX);
	this->Ui->suggestedMax->setMaximum(DBL_MAX);

	



	connect(this->Ui->pushScalarSuggestedMax, SIGNAL(pressed()), this, SLOT(slotAcceptSuggestedMax()));
	connect(this->Ui->pushScalarSuggestedMin, SIGNAL(pressed()), this, SLOT(slotAcceptSuggestedMin()));
	
	connect(this->Ui->sliderMin, SIGNAL(valueChanged(int)), this, SLOT(slotSlideMin(int)));
	connect(this->Ui->sliderMin, SIGNAL(sliderPressed()), this, SLOT(slotSliderStart()));
	connect(this->Ui->sliderMin, SIGNAL(sliderReleased()), this, SLOT(slotSliderStop()));

	connect(this->Ui->sliderMax, SIGNAL(valueChanged(int)), this, SLOT(slotSlideMax(int)));
	connect(this->Ui->sliderMax, SIGNAL(sliderReleased()), this, SLOT(slotSliderStop()));
	connect(this->Ui->sliderMax, SIGNAL(sliderPressed()), this, SLOT(slotSliderStart()));

	connect(this->Ui->sliderShift, SIGNAL(valueChanged(int)), this, SLOT(slotShiftSlider(int)));
	connect(this->Ui->sliderShift, SIGNAL(sliderPressed()), this, SLOT(slotSliderStart()));
	connect(this->Ui->sliderShift, SIGNAL(sliderReleased()), this, SLOT(slotSliderStop()));


	connect(this->Ui->currentMin, SIGNAL(editingFinished()), this, SLOT(slotCurrentMinEdited()));
	connect(this->Ui->currentMax, SIGNAL(editingFinished()), this, SLOT(slotCurrentMaxEdited()));
	connect(this->Ui->pushRemoveScalar, SIGNAL(pressed()), this, SLOT(slotRemoveScalar()));

	this->Ui->reinitializeColorMap->setDisabled(false);
	this->Ui->editColorMap->setDisabled(true);
	this->Ui->deleteColorMap->setDisabled(true);

	QAction* exportAction = new QAction(tr("&Export"), this);
	exportAction->setToolTip(tr("Export color map"));
	this->Ui->exportColorMap->addAction(exportAction);
	this->Ui->exportColorMap->setDefaultAction(exportAction);
	QIcon icon;
	icon.addFile(QStringLiteral(":/Icons/ExportMap22.png"), QSize(), QIcon::Normal, QIcon::Off);
	//  exportColorMap->setIcon(icon);
	exportAction->setIcon(icon);
	new mqSaveMAPDialogReaction(exportAction);
	connect(this->Ui->editColorMap, SIGNAL(pressed()), this, SLOT(slotEditColorMapName()));
	connect(this->Ui->deleteColorMap, SIGNAL(pressed()), this, SLOT(slotDeleteColorMap()));
	connect(this->Ui->reinitializeColorMap, SIGNAL(pressed()), this, SLOT(slotReinitializeColorMap()));
	connect(this->Ui->removePercent, SIGNAL(valueChanged(int)), this, SLOT(slotRefreshSuggestedRange()));
	connect(this->mColorMap, SIGNAL(changeFinished()), this, SLOT(slotRefreshDialog()));
	

	

}




//-----------------------------------------------------------------------------
mqEditScalarsDialog::~mqEditScalarsDialog()
{

 //depending on what is 
	
  delete this->Ui;
}

int mqEditScalarsDialog::SomeThingHasChanged()
{
	int something_has_changed = 0;
	
	return something_has_changed;
}

void mqEditScalarsDialog::slotSliderStart()
{
	cout << "ShiftSliderStart" << endl;
	this->slideMin = this->Ui->currentMin->value();
	this->slideMax = this->Ui->currentMax->value();
	this->maxShiftAmplitude = (this->slideMax - this->slideMin) / 2;

}

void mqEditScalarsDialog::slotSlideMin(int slideMin)
{
	if (slideMin != 0)
	{
		cout << "slideMin:" << slideMin << endl;
		double newMin = this->slideMin + slideMin * this->maxShiftAmplitude / 100;
		this->Ui->currentMin->setValue(newMin);

		cout << "Min:" << newMin << endl;
		this->ctfMin = newMin;
		this->UpdateLookupTables();
	}


}
void mqEditScalarsDialog::slotSlideMax(int slideMax)
{
	if (slideMax != 0)
	{
		cout << "slideMax:" << slideMax << endl;
		double newMax = this->slideMax + slideMax * this->maxShiftAmplitude / 100;

		this->Ui->currentMax->setValue(newMax);
		cout << "Max:" << newMax << endl;

		this->ctfMax = newMax;
		this->UpdateLookupTables();
	}

}
void mqEditScalarsDialog::slotShiftSlider(int shift)
{
	if (shift != 0)
	{
		cout << "shift:" << shift << endl;
		double newMin = this->slideMin + shift * this->maxShiftAmplitude / 100;
		double newMax = this->slideMax + shift * this->maxShiftAmplitude / 100;
		this->Ui->currentMin->setValue(newMin);
		this->Ui->currentMax->setValue(newMax);
		//cout << "Min:" << newMin << endl;
		//cout << "Max:" << newMax << endl;
		this->ctfMin = newMin;
		this->ctfMax = newMax;

		this->UpdateLookupTables();
	}
}

void mqEditScalarsDialog::slotSliderStop()
{


	/*cout << "this->Ui->currentMin=" << this->Ui->currentMin->value()<<endl;
	cout << "this->Ui->currentMax=" << this->Ui->currentMax->value() << endl;
	cout << "this->Ui->sliderMin=" << this->Ui->sliderMin->value() << endl;
	cout << "this->Ui->sliderMax=" << this->Ui->sliderMin->value() << endl;
	cout << "this->ctfMin=" << this->ctfMin << endl;
	cout << "this->ctfMax=" << this->ctfMax << endl;*/


	//cout << "ShiftSliderStop" << endl;

}


void mqEditScalarsDialog::UpdateUI()
{
	/*int color_scale_id = MT->GetColorScaleId();
	
	currentMax->value(MT->Get_sc_max());
	currentMin->value(MT->Get_sc_min());
*/
	cout << "Call UpdateUI" << endl;
	//1 populate comboActiveScalar

	this->RefreshComboScalars();

	// 2 populate comboColorMap
	this->RefreshComboColorMaps();

	this->RefreshSuggestedRange();
	
	
}
void mqEditScalarsDialog::slotRefreshColorMaps()
{
	cout << "slotRefreshColorMaps" << endl;
	this->RefreshComboColorMaps();
}
void mqEditScalarsDialog::slotReinitializeColorMap()
{
	QString ActiveColorMap = this->Ui->comboColorMap->currentText();
	for (int i = 0; i < mqMorphoDigCore::instance()->Getmui_ExistingColorMaps()->Stack.size(); i++)
	{
		int iscustom = mqMorphoDigCore::instance()->Getmui_ExistingColorMaps()->Stack.at(i).isCustom;
		QString myExisingColorMapName = mqMorphoDigCore::instance()->Getmui_ExistingColorMaps()->Stack.at(i).Name;
		if (ActiveColorMap == myExisingColorMapName && iscustom==0)
		{

			mqMorphoDigCore::instance()->reinitializeColorMap(i);
			
			this->RefreshComboColorMaps();

			this->mColorMap->reInitialize(mqMorphoDigCore::instance()->Getmui_ActiveColorMap()->ColorMap);
			this->Ui->currentMin->setValue(0);
			this->Ui->currentMax->setValue(1);
			
			mqMorphoDigCore::instance()->Render();
			//mqMorphoDigCore::instance()->createCustomColorMap(newColormapName, this->STC);				
			//this->UpdateUI();


			//this->mColorMap->reInitialize(mqMorphoDigCore::instance()->Getmui_ExistingColorMaps()->Stack.at(i).ColorMap);



		}
	}

}
void mqEditScalarsDialog::slotEditColorMapName()
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

void mqEditScalarsDialog::slotDeleteColorMap()
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
			this->mColorMap->reInitialize(mqMorphoDigCore::instance()->Getmui_ActiveColorMap()->ColorMap);
			mqMorphoDigCore::instance()->Render();
				//mqMorphoDigCore::instance()->createCustomColorMap(newColormapName, this->STC);				
				//this->UpdateUI();
			

			//this->mColorMap->reInitialize(mqMorphoDigCore::instance()->Getmui_ExistingColorMaps()->Stack.at(i).ColorMap);



		}
	}
}
void mqEditScalarsDialog::slotRefreshSuggestedRange()
{
	this->RefreshSuggestedRange();
}

void mqEditScalarsDialog::RefreshSuggestedRange() 
{
	cout << "DIAL refreshsuggestedRange" << endl;
	int removePercent = this->Ui->removePercent->value();
	this->Ui->suggestedMax->setValue(mqMorphoDigCore::instance()->GetSuggestedScalarRangeMax(removePercent));
	this->Ui->suggestedMin->setValue(mqMorphoDigCore::instance()->GetSuggestedScalarRangeMin(removePercent));
}


void mqEditScalarsDialog::RefreshRange() 
{
}

void mqEditScalarsDialog::RefreshComboScalars()
{
	this->Ui->comboActiveScalar->clear();
	ExistingArrays *MyList = mqMorphoDigCore::instance()->Getmui_ExistingArrays();
	for (int i = 0; i < MyList->Stack.size(); i++)
	{
		if ((MyList->Stack.at(i).DataType == VTK_FLOAT || MyList->Stack.at(i).DataType == VTK_DOUBLE) && MyList->Stack.at(i).NumComp == 1)
		{
			this->Ui->comboActiveScalar->addItem(MyList->Stack.at(i).Name);
		}

	}
	QString myActiveScalars = mqMorphoDigCore::instance()->Getmui_ActiveArray()->Name;
	cout << "DIAL myActiveScalars " << myActiveScalars.toStdString() << endl;
	int exists = -1;
	for (int i = 0; i < this->Ui->comboActiveScalar->count(); i++)
	{
		QString myScalar = this->Ui->comboActiveScalar->itemText(i);
		if (myScalar == myActiveScalars)
		{
			cout << "DIAL found in list!!!!! " << myScalar.toStdString() << endl;
			exists = i;

		}

	}
	if (exists > -1) {
		cout << "DIAL Now current index of combo box is " << exists << endl;
		this->Ui->comboActiveScalar->setCurrentIndex(exists);


	}
}


void mqEditScalarsDialog::RefreshComboColorMaps() 
{
	cout << "RefreshComboColorMaps" << endl;
	this->Ui->comboColorMap->clear();
	ExistingColorMaps *MyCM = mqMorphoDigCore::instance()->Getmui_ExistingColorMaps();
	cout << "Found" << MyCM->Stack.size() << "color maps" << endl;
	for (int i = 0; i < MyCM->Stack.size(); i++)
	{

		this->Ui->comboColorMap->addItem(MyCM->Stack.at(i).Name);


	}
	QString myActiveCM = mqMorphoDigCore::instance()->Getmui_ActiveColorMap()->Name;
	cout << "DIAL myActiveColorMap " << myActiveCM.toStdString() << endl;
	int exists = -1;
	for (int i = 0; i < this->Ui->comboColorMap->count(); i++)
	{
		QString myCM = this->Ui->comboColorMap->itemText(i);
		if (myCM == myActiveCM)
		{
			cout << "DIAL found CM in list!!!!! " << myCM.toStdString() << endl;
			exists = i;

		}

	}
	if (exists > -1) {
		cout << "DIAL Now current index of combo box is " << exists << endl;
		this->Ui->comboColorMap->setCurrentIndex(exists);
		vtkDiscretizableColorTransferFunction* STC = mqMorphoDigCore::instance()->Getmui_ActiveColorMap()->ColorMap;
		//this->mColorMap

	}
}

void mqEditScalarsDialog::slotRemoveScalar()
{
	mqMorphoDigCore::instance()->RemoveArray(this->Ui->comboActiveScalar->currentText(), this->Ui->selectedObjects->isChecked());
	this->UpdateUI();
	
	
}
void mqEditScalarsDialog::slotAcceptSuggestedMax()
{
	if (this->Ui->suggestedMax->value() > this->Ui->currentMin->value())
	{
		cout << "Slider Max futures value:" << this->Ui->suggestedMax->value() << endl;
		

		this->Ui->currentMax->setValue(this->Ui->suggestedMax->value());
		this->ctfMax = this->Ui->suggestedMax->value();
		this->UpdateLookupTables();
	}
}

void mqEditScalarsDialog::slotAcceptSuggestedMin()
{
	if (this->Ui->suggestedMin->value() < this->Ui->currentMax->value())
	{
		this->Ui->currentMin->setValue(this->Ui->suggestedMin->value());
		this->ctfMin = this->Ui->suggestedMin->value();
		

		this->UpdateLookupTables();
	}
}

void mqEditScalarsDialog::slotRefreshComboScalars()
{
	this->RefreshComboScalars();


}


void mqEditScalarsDialog::RefreshDialog()
{
	this->UpdateUI();
	mqMorphoDigCore::instance()->Render();
}


void mqEditScalarsDialog::slotActiveScalarChanged(int idx)
{
	cout << "looks like active scalar has changed!:: " << idx << endl;
	QString NewActiveScalarName = this->Ui->comboActiveScalar->currentText();
	for (int i = 0; i < mqMorphoDigCore::instance()->Getmui_ExistingArrays()->Stack.size(); i++)
	{
		QString myExisingScalarName = mqMorphoDigCore::instance()->Getmui_ExistingArrays()->Stack.at(i).Name;
		if (NewActiveScalarName == myExisingScalarName)
		{

			mqMorphoDigCore::instance()->Setmui_ActiveArrayAndRender(NewActiveScalarName,
				mqMorphoDigCore::instance()->Getmui_ExistingArrays()->Stack.at(i).DataType,
				mqMorphoDigCore::instance()->Getmui_ExistingArrays()->Stack.at(i).NumComp
			);
			
			this->RefreshSuggestedRange();

		}
	}


}


void mqEditScalarsDialog::slotActiveColorMapChanged(int idx)
{
	cout << "looks like active color map has changed!:: " << idx << endl;
	QString NewActiveColorMap = this->Ui->comboColorMap->currentText();
	for (int i = 0; i < mqMorphoDigCore::instance()->Getmui_ExistingColorMaps()->Stack.size(); i++)
	{
		QString myExisingColorMapName = mqMorphoDigCore::instance()->Getmui_ExistingColorMaps()->Stack.at(i).Name;
		if (NewActiveColorMap == myExisingColorMapName)
		{
			
			if (mqMorphoDigCore::instance()->Getmui_ExistingColorMaps()->Stack.at(i).isCustom==1)
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
			mqMorphoDigCore::instance()->Setmui_ActiveColorMapAndRender(NewActiveColorMap,
				mqMorphoDigCore::instance()->Getmui_ExistingColorMaps()->Stack.at(i).ColorMap
			);
			this->mColorMap->reInitialize(mqMorphoDigCore::instance()->Getmui_ExistingColorMaps()->Stack.at(i).ColorMap);
		


		}
	}


}


void mqEditScalarsDialog::slotAccepted()
{
	

}
void mqEditScalarsDialog::UpdateLookupTables()
{
	mqMorphoDigCore::instance()->UpdateLookupTablesRanges(this->ctfMin, this->ctfMax);
}
void mqEditScalarsDialog::slotCurrentMinEdited()
{
	if (this->Ui->currentMin->value() < this->Ui->currentMax->value())
	{
		this->ctfMin = this->Ui->currentMin->value();
	}
	else
	{
		this->ctfMin = this->Ui->currentMax->value() - 10;
	}




	this->UpdateLookupTables();

	
}
void mqEditScalarsDialog::slotCurrentMaxEdited()
{
	//cout << "Current max edited!" << endl;
	if (this->Ui->currentMin->value() < this->Ui->currentMax->value())
	{
		this->ctfMax = this->Ui->currentMax->value();
	}
	else
	{
		this->ctfMax = this->Ui->currentMin->value() + 10;
	}




	this->UpdateLookupTables();

}

void mqEditScalarsDialog::slotRefreshDialog()
{
	cout << "Let's refresh dialog!"		<< endl;

	//Dirty hack here! Replace "mqMorphoDigCore::instance()->GetScalarRangeMax()" by something which finds the bounds
	// of the currently used lookup table
	double min = mqMorphoDigCore::instance()->GetScalarRangeMin();
	double max = mqMorphoDigCore::instance()->GetScalarRangeMax();
	/*this->Ui->currentMin->setValue(min);
	this->Ui->currentMax->setValue(max);*/
	this->Ui->currentMin->setValue(min);
	this->Ui->currentMax->setValue(max);
	this->ctfMin = min;
	this->ctfMax = max;
	
	this->RefreshComboColorMaps();
	//this->RefreshDialog();
	this->UpdateLookupTables();
}


//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

