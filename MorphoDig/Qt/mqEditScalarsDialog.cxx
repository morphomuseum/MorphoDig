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
	this->Ui->suggestedMin->setMinimum(-DBL_MAX);
	this->Ui->suggestedMax->setMinimum(-DBL_MAX);

	this->Ui->currentMin->setMaximum(DBL_MAX);
	this->Ui->currentMax->setMaximum(DBL_MAX);
	this->Ui->suggestedMin->setMaximum(DBL_MAX);
	this->Ui->suggestedMax->setMaximum(DBL_MAX);
	this->Ui->sliderMin->setDoubleValue(0);
	this->Ui->sliderMin->setDoubleMaximum(1);
	this->Ui->sliderMin->setDoubleMinimum(-1);
	

	

	this->Ui->sliderMax->setDoubleValue(1);
	this->Ui->sliderMax->setDoubleMaximum(2);
	this->Ui->sliderMax->setDoubleMinimum(0);
	this->Ui->sliderMin->setDoubleSingleStep((this->Ui->sliderMin->doubleMaximum() - this->Ui->sliderMin->doubleMinimum()) / 100);

	this->Ui->sliderMax->setDoubleSingleStep((this->Ui->sliderMax->doubleMaximum() - this->Ui->sliderMax->doubleMinimum()) / 100);


	connect(this->Ui->pushScalarSuggestedMax, SIGNAL(pressed()), this, SLOT(slotAcceptSuggestedMax()));
	connect(this->Ui->pushScalarSuggestedMin, SIGNAL(pressed()), this, SLOT(slotAcceptSuggestedMin()));
	
	connect(this->Ui->sliderMin, SIGNAL(valueChanged(int)), this, SLOT(slotMoveSliders()));
	connect(this->Ui->sliderMax, SIGNAL(valueChanged(int)), this, SLOT(slotMoveSliders()));
	
	connect(this->Ui->sliderMin, SIGNAL(sliderReleased()), this, SLOT(slotRefreshSliders()));
	connect(this->Ui->sliderMax, SIGNAL(sliderReleased()), this, SLOT(slotRefreshSliders()));


	connect(this->Ui->currentMin, SIGNAL(editingFinished()), this, SLOT(slotCurrentMinMaxEdited()));
	connect(this->Ui->currentMax, SIGNAL(editingFinished()), this, SLOT(slotCurrentMinMaxEdited()));
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
	
	this->RefreshSliders();
	/*
	sc_show:
scWindow->show();



sliderMin->value(MT->Get_sc_min());
sliderMax->value(MT->Get_sc_max());
currentMax->value(MT->Get_sc_max());
currentMin->value(MT->Get_sc_min());
suggestedMax->value(MT->scalars_get_max());
suggestedMin->value(MT->scalars_get_min());
Refresh();
	*/

	/*connect(mqMorphoDigCore::instance(), SIGNAL(lmSelectionChanged()), this, SLOT(slotRefreshDialog()));
	connect(this->Ui->next, SIGNAL(pressed()), this, SLOT(slotGetNextFlag()));
	connect(this->Ui->prec, SIGNAL(pressed()), this, SLOT(slotGetPrecedingFlag()));
	
	this->FLG_Coll = NULL;
	this->FLG = NULL;
	
	QString mylabel("...");
	this->Ui->FlagLabel->setText(mylabel);
	
	double flag_rendering_size = mqMorphoDigCore::instance()->Getmui_FlagRenderingSize();
	this->Ui->FlagRenderingSizeValue->setMinimum(0);
	this->Ui->FlagRenderingSizeValue->setSingleStep(1);
	this->Ui->FlagRenderingSizeValue->setValue(flag_rendering_size);

	QColor myFlagColor;

	double flagcolor[4];

	this->Ui->FlagColorButton->setShowAlphaChannel(false);
	mqMorphoDigCore::instance()->Getmui_FlagColor(flagcolor);

	myFlagColor.setRedF(flagcolor[0]);
	myFlagColor.setGreenF(flagcolor[1]);
	myFlagColor.setBlueF(flagcolor[2]);


	this->Ui->FlagColorButton->setChosenColor(myFlagColor);

	this->Ui->x->setDecimals(10);
	this->Ui->y->setDecimals(10);
	this->Ui->z->setDecimals(10);
	this->Ui->x->setMinimum(-DBL_MAX);
	this->Ui->y->setMinimum(-DBL_MAX);
	this->Ui->z->setMinimum(-DBL_MAX);

	this->Ui->x->setMaximum(DBL_MAX);
	this->Ui->y->setMaximum(DBL_MAX);
	this->Ui->z->setMaximum(DBL_MAX);
	

	this->GetFirstSelectedFlag();
	this->UpdateUI();
	
 
  
 connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotsaveFLG()));
 */

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
void mqEditScalarsDialog::RefreshSliders() 
{
	
	//this->Ui->sliderMin->setDoubleValue(this->Ui->currentMin->value());
	//this->Ui->sliderMax->setDoubleValue(this->Ui->currentMax->value());
	cout << "Refresh Sliders" << endl;
	double curr_max = this->Ui->sliderMax->doubleValue();
	double curr_min = this->Ui->sliderMin->doubleValue();

	double new_max_min = (curr_max + curr_min) / 2;
	double new_half = new_max_min - curr_min;
	double new_max_max = curr_max+new_half;
	
	double new_min_min = curr_min - new_half;
	double new_min_max = new_max_min;
	cout << "Min:" << new_min_min << "|" << curr_min << "|" << new_min_max << endl;
	cout << "Max:" << new_max_min << "|" << curr_max << "|" << new_max_max << endl;
	this->Ui->sliderMin->setDoubleValue(curr_min);
	this->Ui->sliderMax->setDoubleValue(curr_max);

	this->Ui->sliderMin->setDoubleMaximum(new_min_max);
	this->Ui->sliderMin->setDoubleMinimum(new_min_min);

	this->Ui->sliderMax->setDoubleMinimum(new_max_min);
	this->Ui->sliderMax->setDoubleMaximum(new_max_max);
	this->Ui->sliderMin->setDoubleSingleStep((curr_max -curr_min) / 1000);
	this->Ui->sliderMax->setDoubleSingleStep((curr_max-curr_min) / 1000);

	this->Ui->sliderMin->setDoubleValue(curr_min);
	this->Ui->sliderMax->setDoubleValue(curr_max);

	this->Ui->currentMin->setValue(this->Ui->sliderMin->doubleValue());

	this->Ui->currentMax->setValue(this->Ui->sliderMax->doubleValue());

	/*
	sliderMin->maximum((MT->Get_sc_max() + MT->Get_sc_min()) / 2);
	sliderMin->minimum((3 * MT->Get_sc_min() - MT->Get_sc_max()) / 2);
	sliderMax->minimum((MT->Get_sc_max() + MT->Get_sc_min()) / 2);
	sliderMax->maximum((3 * MT->Get_sc_max() - MT->Get_sc_min()) / 2);
	sliderMax->redraw();
	sliderMax->redraw();
	*/
}

void mqEditScalarsDialog::MoveSliders()
{

	//this->Ui->sliderMin->setDoubleValue(this->Ui->currentMin->value());
	//this->Ui->sliderMax->setDoubleValue(this->Ui->currentMax->value());
	cout << "Move Slider" << endl;
	double curr_max = this->Ui->sliderMax->doubleValue();
	double curr_min = this->Ui->sliderMin->doubleValue();

	this->Ui->currentMin->setValue(this->Ui->sliderMin->doubleValue());

	this->Ui->currentMax->setValue(this->Ui->sliderMax->doubleValue());

	/*
	sliderMin->maximum((MT->Get_sc_max() + MT->Get_sc_min()) / 2);
	sliderMin->minimum((3 * MT->Get_sc_min() - MT->Get_sc_max()) / 2);
	sliderMax->minimum((MT->Get_sc_max() + MT->Get_sc_min()) / 2);
	sliderMax->maximum((3 * MT->Get_sc_max() - MT->Get_sc_min()) / 2);
	sliderMax->redraw();
	sliderMax->redraw();
	*/
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
		if (this->Ui->suggestedMax->value() > this->Ui->sliderMax->getDoubleMaximum())
		{
			this->Ui->sliderMax->setDoubleMaximum(this->Ui->suggestedMax->value() + 1);
		}

		if (this->Ui->suggestedMax->value() < this->Ui->sliderMax->getDoubleMinimum())
		{
			this->Ui->sliderMax->setDoubleMinimum(this->Ui->suggestedMax->value() - 1);
		}


		this->Ui->sliderMax->setDoubleValue(this->Ui->suggestedMax->value());
		RefreshSliders();
		this->UpdateLookupTables();
	}
}

void mqEditScalarsDialog::slotAcceptSuggestedMin()
{
	if (this->Ui->suggestedMin->value() < this->Ui->currentMax->value())
	{
		if (this->Ui->suggestedMin->value() > this->Ui->sliderMin->getDoubleMaximum())
		{
			this->Ui->sliderMin->setDoubleMaximum(this->Ui->suggestedMin->value() + 1);
		}

		if (this->Ui->suggestedMin->value() < this->Ui->sliderMin->getDoubleMinimum())
		{
			this->Ui->sliderMin->setDoubleMinimum(this->Ui->suggestedMin->value() - 1);
		}


		this->Ui->sliderMin->setDoubleValue(this->Ui->suggestedMin->value());
		RefreshSliders();
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

/*

comboActiveScalar :
MT->Set_Active_Scalar(
((Fl_Choice *)o)->value());
suggestedMin->value(MT->scalars_get_max());
suggestedMax->value(MT->scalars_get_min());
MT->Update_RGB();
MT->redraw();

pushInitScalar:
MT->Initialize_Scalar(comboActiveScalar->value());
MT->redraw();

pushRemoveScalar
MT->Remove_Scalar(comboActiveScalar->value());
MT->redraw()

sliderMin:
MT->Set_sc_min(((Fl_Slider *)o)->value());
Refresh();
MT->Update_RGB();
MT->redraw();

sliderMax:
MT->Set_sc_max(((Fl_Slider *)o)->value());
Refresh();
MT->Update_RGB();
MT->redraw();

cbBelowMin:
MT->SC_Show_Below_Min(((Fl_Check_Button *)o)->value());
MT->redraw();

cbBelowMax:
MT->SC_Show_Above_Max(((Fl_Check_Button *)o)->value());
MT->redraw();

pushScalarSuggestedMin;
currentMin->value(suggestedMin->value());

pushScalarSuggestedMax;
currentMax->value(suggestedMax->value());


comboColorMap:
MT->SetColorScaleId(
((Fl_Choice *)o)->value());

suggestedMax->value(MT->scalars_get_max());
Min_sc->value(MT->scalars_get_min());
MT->Update_RGB();
MT->redraw();




Refresh:


update_2:
suggestedMax->value(MT->scalars_get_max());
suggestedMin->value(MT->scalars_get_min());


*/
void mqEditScalarsDialog::slotRefreshSliders()
{
	this->RefreshSliders();
	this->UpdateLookupTables();
}
void mqEditScalarsDialog::slotMoveSliders()
{
	this->MoveSliders();
	//this->UpdateLookupTables();
}
/*void mqEditScalarsDialog::slotSliderMinValueChanged(int value)
{
	cout << "Now slider Min = " << this->Ui->sliderMin->doubleValue() << endl;
	this->Ui->currentMin->setValue(this->Ui->sliderMin->doubleValue());
	//this->RefreshSliders();

}
void mqEditScalarsDialog::slotSliderMaxValueChanged(int value)
{
	this->Ui->currentMax->setValue(this->Ui->sliderMax->doubleValue());
	//this->RefreshSliders();

}*/

void mqEditScalarsDialog::slotAccepted()
{
	

	/*MT->Set_sc_max(currentMax->value());
	MT->Set_sc_min(currentMin->value());
	if (comboActiveScalar->value() >= 0)
	{
		MT->Set_Active_Scalar(comboActiveScalar->value());
	}

	if (comboColorMap->value() >= 0)
	{

		MT->SetColorScaleId(comboColorMap->value());
	}
	sliderMin->value(MT->Get_sc_min());
	sliderMax->value(MT->Get_sc_max());
	sliderMin->maximum((MT->Get_sc_max() + MT->Get_sc_min()) / 2);
	sliderMin->minimum((3 * MT->Get_sc_min() - MT->Get_sc_max()) / 2);
	sliderMax->minimum((MT->Get_sc_max() + MT->Get_sc_min()) / 2);
	sliderMax->maximum((3 * MT->Get_sc_max() - MT->Get_sc_min()) / 2);
	sliderMax->redraw();
	sliderMin->redraw();
	MT->Update_RGB();
	MT->redraw();*/

}
void mqEditScalarsDialog::UpdateLookupTables()
{
	mqMorphoDigCore::instance()->UpdateLookupTablesRanges(this->Ui->currentMin->value(), this->Ui->currentMax->value());
}
void mqEditScalarsDialog::slotCurrentMinMaxEdited()
{
	//cout << "Min max edited!" << endl;
	double new_min = this->Ui->currentMin->value();
	double new_max = this->Ui->currentMax->value();
	if (new_min < new_max)
	{
		this->Ui->sliderMin->setDoubleValue(this->Ui->currentMin->value());
		this->Ui->sliderMax->setDoubleValue(this->Ui->currentMax->value());
		this->RefreshSliders();
		this->UpdateLookupTables();
	}
	else
	{
		this->Ui->currentMin->setValue(this->Ui->sliderMin->doubleValue());
		this->Ui->currentMax->setValue(this->Ui->sliderMax->doubleValue());
	}
	
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
	this->Ui->sliderMin->setDoubleValue(min);
	this->Ui->sliderMax->setDoubleValue(max);
	this->RefreshSliders();
	this->RefreshComboColorMaps();
	//this->RefreshDialog();
}


//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

