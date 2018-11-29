/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqEditTagsDialog.h"
#include "ui_mqEditTagsDialog.h"
#include "mqSetName.h"
#include "MorphoDigVersion.h"
#include "mqMorphoDigCore.h"
#include "mqSaveMAPDialogReaction.h"
#include "mqUndoStack.h"
#include "QDoubleSlider.h"
#include "mqColorChooserButton.h"
#include "mqColorOpacityEditorWidget.h"
#include "mqTransferFunctionWidget.h"
#include "mqSaveTAGMAPDialogReaction.h"
#include "mqMergeTagsDialogReaction.h"
#include "mqMinimalWidget.h"
#include "vtkLMActor.h"
#include "vtkLMActorCollection.h"
#include <vtkLookupTable.h>
// we actually do not need glew...
//#include <GL/glew.h>
#include <QApplication>
#include <QMessageBox>
#include <QInputDialog>
#include <QFile>
#include <QTableWidgetItem>
#include <QRadioButton>
#include <QFileDialog>
#include <QCheckBox>
#include <QComboBox>
#include <QHeaderView>
#include <QSpinBox>
#include <QStandardItemModel>

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
mqEditTagsDialog::mqEditTagsDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqEditTagsDialog())
{
	
	
	//1 populate active scalar combo box, and check which tags is the active one!
	this->Ui->setupUi(this);
	this->setObjectName("mqEditTagsDialog");
	
	this->activeTag = 1;
	cout << "Try this0" << endl;
	QHeaderView *header = this->Ui->tableWidget->horizontalHeader();
	cout << "Try this1" << endl;
	header->setSectionResizeMode(QHeaderView::Stretch);
	cout << "Try this2" << endl;

	/*header->setSectionResizeMode(1, QHeaderView::Interactive);
	cout << "Try this3" << endl;
		header->setSectionResizeMode(2, QHeaderView::Interactive);
		cout << "Try this4" << endl;
	header->setSectionResizeMode(3, QHeaderView::Interactive);
	cout << "Try this5" << endl;*/
	// Can't select lines
	//this->Ui->tableWidget->setSelectionMode(QAbstractItemView::NoSelection);

	// Test: fill the list


	this->Ui->tableWidget->setColumnCount(5);
	

	this->Ui->tableWidget->setRowCount(255);
	cout << "Try this6" << endl;
	
	
	this->Ui->comboActiveTags->setSizeAdjustPolicy(QComboBox::AdjustToContents);
	this->Ui->comboTagMaps->setSizeAdjustPolicy(QComboBox::AdjustToContents);
	/*this->Ui->pencilOn->setDisabled(true);
	this->Ui->lassoOn->setDisabled(true);
	this->Ui->bucketOn->setDisabled(true);
	
	*/
	this->Ui->activateTagMode->setVisible(false);
	this->Ui->pencilSearchSize->setValue(mqMorphoDigCore::instance()->Getmui_PencilSize());
	
	connect(mqMorphoDigCore::instance(), SIGNAL(tagMapsChanged()), this, SLOT(slotRefreshTagMaps())); // when loading a new .tag file or when deleting tag maps
	connect(mqMorphoDigCore::instance(), SIGNAL(existingScalarsChanged()), this, SLOT(slotRefreshComboTags()));
	connect(mqMorphoDigCore::instance(), SIGNAL(activeScalarChanged()), this, SLOT(slotRefreshComboTags()));
	connect(this->Ui->comboActiveTags, SIGNAL(activated(int)), this, SLOT(slotActiveTagsChanged(int)));
	connect(this->Ui->comboTagMaps, SIGNAL(activated(int)), this, SLOT(slotActiveTagMapChanged(int)));
	connect(this->Ui->pushRemoveTags, SIGNAL(pressed()), this, SLOT(slotRemoveTags()));
	connect(this->Ui->pencilSearchSize, SIGNAL(valueChanged(int)), this, SLOT(slotPencilSearchSizeChanged(int)));
	connect(this->Ui->pencilLimitAngle, SIGNAL(valueChanged(int)), this, SLOT(slotPencilLimitAngleChanged(int)));
	connect(this->Ui->activateTagMode, SIGNAL(clicked()), this, SLOT(slotActivateTagMode()));
	connect(this->Ui->bucketOn, SIGNAL(clicked()), this, SLOT(slotBucketOn()));
	connect(this->Ui->pencilOn, SIGNAL(clicked()), this, SLOT(slotPencilOn()));
	connect(this->Ui->lassoOn, SIGNAL(pressed()), mqMorphoDigCore::instance(), SLOT(slotLassoTagInside()));
	connect(this->Ui->rubberOn, SIGNAL(pressed()), mqMorphoDigCore::instance(), SLOT(slotRubberTagInside()));
	connect (this->Ui->erase, SIGNAL(pressed()), this, SLOT(slotEraseActiveTag()));
	
	QAction* mergeAction = new QAction(tr("&Merge"), this);
	mergeAction->setToolTip(tr("Merge 2 tags"));
	this->Ui->merge->addAction(mergeAction);
	this->Ui->merge->setDefaultAction(mergeAction);
	QIcon icon;
	icon.addFile(QStringLiteral(":/Icons/merge.png"), QSize(), QIcon::Normal, QIcon::Off);
	mergeAction->setIcon(icon);
	mergeAction->setToolTip(tr("Merge two tags"));
	//TOTO : create an export tag map...
	new mqMergeTagsDialogReaction(mergeAction);



	this->Ui->reinitializeTagMap->setDisabled(false);
	this->Ui->editTagMap->setDisabled(true);
	this->Ui->deleteTagMap->setDisabled(true);
	QAction* exportAction = new QAction(tr("&Export"), this);
	exportAction->setToolTip(tr("Export Tag Map"));
	this->Ui->exportTagMap->addAction(exportAction);
	this->Ui->exportTagMap->setDefaultAction(exportAction);
	QIcon icon2;
	icon2.addFile(QStringLiteral(":/Icons/ExportTagMap22.png"), QSize(), QIcon::Normal, QIcon::Off);
	exportAction->setIcon(icon2);
	//TOTO : create an export tag map...
	new mqSaveTAGMAPDialogReaction(exportAction);
	connect(this->Ui->reinitializeTagMap, SIGNAL(pressed()), this, SLOT(slotReinitializeTagMap()));
	connect(this->Ui->editTagMap, SIGNAL(pressed()), this, SLOT(slotEditTagMapName()));
	connect(this->Ui->deleteTagMap, SIGNAL(pressed()), this, SLOT(slotDeleteTagMap()));

	connect(this->Ui->tableWidget, SIGNAL(cellChanged(int, int)), this, SLOT(slotCellChanged(int, int)));
	QObject::connect(this->Ui->addTagMap, SIGNAL(clicked()), this, SLOT(slotSaveAsCustom()));
	QObject::connect(this->Ui->addTag, SIGNAL(clicked()), this, SLOT(slotAddTag()));
	QObject::connect(this->Ui->removeTag, SIGNAL(clicked()), this, SLOT(slotRemoveTag()));
	/*connect(this->Ui->tableWidget, SIGNAL(cellActivated(int, int)), this, SLOT(slotCellActivated(int, int)));
	connect(this->Ui->tableWidget, SIGNAL(cellEntered(int, int)), this, SLOT(slotCellEntered(int, int)));
	connect(this->Ui->tableWidget, SIGNAL(cellClicked(int, int)), this, SLOT(slotCellClicked(int, int)));
	connect(this->Ui->tableWidget, SIGNAL(cellPressed(int, int)), this, SLOT(slotCellPressed(int, int)));*/

	/*	
			
	
	
	*/

}




//-----------------------------------------------------------------------------
mqEditTagsDialog::~mqEditTagsDialog()
{

 //depending on what is 
	
  delete this->Ui;
}

/*void mqEditTagsDialog::hide()
{
	//cout << "Hide" << endl;
	this->QDialog::hide();
	
}*/

void mqEditTagsDialog::closeEvent(QCloseEvent *event)
{
	/*cout << "Deactivate Tag Mode" << endl;
	mqMorphoDigCore::instance()->Setmui_TagModeActivated(0);
	this->Ui->activateTagMode->setChecked(false);*/

	event->accept();
}


void mqEditTagsDialog::slotEraseActiveTag()
{
	
	mqMorphoDigCore::instance()->MergeTags(this->activeTag, 0);
}

void mqEditTagsDialog::slotCellChanged(int row, int column)
{
	//usually: works
	cout << "Cell " << row << ", "<<column << " has changed" << endl;
	QTableWidgetItem *item = this->Ui->tableWidget->item(row, column);
	QString mynewLabel = item->text();
	cout << "New label" << mynewLabel.toStdString()<<endl;
	this->updateLabel(row, mynewLabel);
}

void mqEditTagsDialog::slotSaveAsCustom()
{
	QInputDialog *giveNameDialog = new QInputDialog();
	bool dialogResult;
	QString newColormapName = giveNameDialog->getText(0, "Tag map name", "Name:", QLineEdit::Normal,
		"Custom_TagMap", &dialogResult);
	if (dialogResult)
	{

		cout << "tag map given:" << newColormapName.toStdString() << endl;
		if (mqMorphoDigCore::instance()->tagMapNameAlreadyExists(newColormapName) == 1)
		{
			QMessageBox msgBox;
			msgBox.setText("Can't save custom tag map : name already exists.");
			msgBox.exec();
			return;
		}
		if (newColormapName.length() == 0)
		{
			QMessageBox msgBox;
			msgBox.setText("Can't save custom tag map: name length =0.");
			msgBox.exec();
			return;
		}
		mqMorphoDigCore::instance()->createCustomTagMap(newColormapName);
		
		
	}
	else
	{
		cout << "cancel " << endl;
	}

}
void mqEditTagsDialog::slotColorChanged()
{
	//usually: works
	/*QWidget *w = qobject_cast<QWidget *>(sender()->parent());
	if (w) {
		//int row = this->Ui->tableWidget->indexAt(w->pos()).row();
		int row = w->objectName().toInt();
		int col = this->Ui->tableWidget->indexAt(w->pos()).column();
		cout << "Color changed at row " << row << endl;
		cout << "Color changed at col " << col << endl;
		//this->Ui->tableWidget->removeRow(row);
		//this->Ui->tableWidget->setCurrentCell(0, 0);
	}*/
	mqColorChooserButton *color_btn = (mqColorChooserButton*)sender();
	for (int i= 0; i < this->Ui->tableWidget->rowCount(); i++)
	{
		//for (int j = 0; j < this->Ui->tableWidget->columnCount(); j++)
		//{
		int j = 2; // column 3 = colors
			if (this->Ui->tableWidget->cellWidget(i, j) == color_btn)
			{
				cout << "color changed	at row " << i << ", column" << j << endl;
				QColor myNewColor = color_btn->chosenColor();
				double color[3];
				myNewColor.getRgbF(&color[0], &color[1], &color[2]);
				cout << "new color is : " << color[0] << "," << color[1] << ","<<color[2]<<endl;
				this->updateColor(i, color[0], color[1], color[2]);
			}

		//}
	}


	
}

void mqEditTagsDialog::slotActiveTagChanged()
{
	
	QRadioButton *radiobutton = (QRadioButton*)sender();
	for (int i = 0; i < this->Ui->tableWidget->rowCount(); i++)
	{
	
		int j = 1; // column 2 = active tag
		if (this->Ui->tableWidget->cellWidget(i, j) == radiobutton && radiobutton->isChecked())
		{
			cout << "Active tag at row " << i << ", column" << j << endl;
			this->activeTag = i;
			mqMorphoDigCore::instance()->Setmui_ActiveTag(i);
		}
	}
}

void mqEditTagsDialog::updateLabel(int row, QString newLabel)
{
	QString currentTagMapName = this->Ui->comboTagMaps->currentText();
	int numTags = 0;
	std::vector<std::string> tagNames;
	vtkSmartPointer<vtkLookupTable> TagMap;
	QString TagMapName;
	int cpt = 0;
	for (int i = 0; i < mqMorphoDigCore::instance()->Getmui_ExistingTagMaps()->Stack.size(); i++)
	{
		QString myExisingTagMapName = mqMorphoDigCore::instance()->Getmui_ExistingTagMaps()->Stack.at(i).Name;
		if (currentTagMapName == myExisingTagMapName)
		{
			cpt = i;
			TagMapName = mqMorphoDigCore::instance()->Getmui_ExistingTagMaps()->Stack.at(i).Name;
			numTags = mqMorphoDigCore::instance()->Getmui_ExistingTagMaps()->Stack.at(i).numTags;
			tagNames = mqMorphoDigCore::instance()->Getmui_ExistingTagMaps()->Stack.at(i).tagNames;
			TagMap = mqMorphoDigCore::instance()->Getmui_ExistingTagMaps()->Stack.at(i).TagMap;
		}
	}
	tagNames.at(row) = newLabel.toStdString();
	mqMorphoDigCore::instance()->Getmui_ExistingTagMaps()->Stack.at(cpt).tagNames = tagNames;
	mqMorphoDigCore::instance()->Setmui_ActiveTagMap(TagMapName, numTags, tagNames, TagMap);

}
void mqEditTagsDialog::updateColor(int row, double r, double g, double b)
{
	QString currentTagMapName = this->Ui->comboTagMaps->currentText();
	int numTags = 0;
	std::vector<std::string> tagNames;
	vtkSmartPointer<vtkLookupTable> TagMap;
	for (int i = 0; i < mqMorphoDigCore::instance()->Getmui_ExistingTagMaps()->Stack.size(); i++)
	{
		QString myExisingTagMapName = mqMorphoDigCore::instance()->Getmui_ExistingTagMaps()->Stack.at(i).Name;
		if (currentTagMapName == myExisingTagMapName)
		{
			numTags = mqMorphoDigCore::instance()->Getmui_ExistingTagMaps()->Stack.at(i).numTags;
			tagNames = mqMorphoDigCore::instance()->Getmui_ExistingTagMaps()->Stack.at(i).tagNames;
			TagMap = mqMorphoDigCore::instance()->Getmui_ExistingTagMaps()->Stack.at(i).TagMap;
		}
	}
	double opacity = TagMap->GetTableValue(row)[3];
	
	cout << "r" << r << "g" << g << "b" << b << "opacity" << opacity << endl;
	TagMap->SetTableValue(row, r, g, b, opacity);
	//mqMorphoDigCore::instance()->Setmui_ActiveTagMap(TagMapName, numTags, tagNames, TagMap);
	mqMorphoDigCore::instance()->Render();
}
void mqEditTagsDialog::updateOpacity(int row, int newopacity)
{
	QString currentTagMapName = this->Ui->comboTagMaps->currentText();
	int numTags = 0;
	std::vector<std::string> tagNames;
	vtkSmartPointer<vtkLookupTable> TagMap;
	for (int i = 0; i < mqMorphoDigCore::instance()->Getmui_ExistingTagMaps()->Stack.size(); i++)
	{
		QString myExisingTagMapName = mqMorphoDigCore::instance()->Getmui_ExistingTagMaps()->Stack.at(i).Name;
		if (currentTagMapName == myExisingTagMapName)
		{
			numTags = mqMorphoDigCore::instance()->Getmui_ExistingTagMaps()->Stack.at(i).numTags;
			tagNames = mqMorphoDigCore::instance()->Getmui_ExistingTagMaps()->Stack.at(i).tagNames;
			TagMap = mqMorphoDigCore::instance()->Getmui_ExistingTagMaps()->Stack.at(i).TagMap;
		}
	}
	double opacity = (double)((double)newopacity/100);
	double r = TagMap->GetTableValue(row)[0];
	double g = TagMap->GetTableValue(row)[1];
	double b = TagMap->GetTableValue(row)[2];
	cout << "r" << r << "g" << g << "b" << b << "opacity" << opacity << endl;
	TagMap->SetTableValue(row, r, g, b, opacity);
	mqMorphoDigCore::instance()->Render();
}

void mqEditTagsDialog::slotOpacityChanged(int newopacity)
{

	QSpinBox *sb = (QSpinBox*)sender();
	for (int i = 0; i < this->Ui->tableWidget->rowCount(); i++)
	{

		int j = 3; // column 4 = opacity
		if (this->Ui->tableWidget->cellWidget(i, j) ==sb )
		{
			cout << "New opacity at row " << i << ", value=" << newopacity << endl;
			this->updateOpacity(i, newopacity);
		}
	}
}

void mqEditTagsDialog::RefreshTagMapTable()
{
	SignalBlocker tagTableSignalBlocker(this->Ui->tableWidget); //blocks signals when populating the table! Blocking will stop 
	this->Ui->tableWidget->clear();
	this->Ui->tableWidget->setColumnCount(4);
	QTableWidgetItem *header1 = new QTableWidgetItem();
	header1->setText("Tag");
	this->Ui->tableWidget->setHorizontalHeaderItem(0, header1);
	QTableWidgetItem *header2 = new QTableWidgetItem();
	header2->setText("Active");
	this->Ui->tableWidget->setHorizontalHeaderItem(1, header2);
	QTableWidgetItem *header3 = new QTableWidgetItem();
	header3->setText("Color");
	this->Ui->tableWidget->setHorizontalHeaderItem(2, header3);
	QTableWidgetItem *header4 = new QTableWidgetItem();
	header4->setText("Opacity");
	this->Ui->tableWidget->setHorizontalHeaderItem(3, header4);
	/*QTableWidgetItem *header5 = new QTableWidgetItem();
	header5->setText("Clear");
	this->Ui->tableWidget->setHorizontalHeaderItem(4, header5);*/

	// f(Active Tag Map)
	QString currentTagMapName = this->Ui->comboTagMaps->currentText(); 
	int numTags = 0;
	std::vector<std::string> tagNames;
	vtkSmartPointer<vtkLookupTable> TagMap;
	for (int i = 0; i < mqMorphoDigCore::instance()->Getmui_ExistingTagMaps()->Stack.size(); i++)
	{
		QString myExisingTagMapName = mqMorphoDigCore::instance()->Getmui_ExistingTagMaps()->Stack.at(i).Name;
		if (currentTagMapName == myExisingTagMapName)
		{			
			 numTags = mqMorphoDigCore::instance()->Getmui_ExistingTagMaps()->Stack.at(i).numTags;
			 tagNames = mqMorphoDigCore::instance()->Getmui_ExistingTagMaps()->Stack.at(i).tagNames;
			 TagMap = mqMorphoDigCore::instance()->Getmui_ExistingTagMaps()->Stack.at(i).TagMap;
		}
	}
	//QLabel *nom;
	//QLabel *opacityL;
	QRadioButton *radio;
	//QToolButton *clear;
	QSpinBox *opacitySB;
	mqColorChooserButton *colorbutton;
	cout << "Refresh tag map with numTags=" << numTags<<endl;
	if (numTags > 0)
	{
		this->Ui->tableWidget->setRowCount(numTags);
	}
	for (int i = 0; i < numTags; i++) {
		//nom = new QLabel();
		//nom->setText(tagNames.at(i).c_str());
		
		radio = new QRadioButton();
		if (i == 1) { radio->setChecked(true); }
		//clear = new QToolButton();
		colorbutton = new mqColorChooserButton(this->Ui->tableWidget);
		QColor myColor;
		myColor.setRedF(TagMap->GetTableValue(i)[0]);
		myColor.setGreenF(TagMap->GetTableValue(i)[1]);
		myColor.setBlueF(TagMap->GetTableValue(i)[2]);
		int opacity = (int)(100 * TagMap->GetTableValue(i)[3]);
		opacitySB = new QSpinBox();
		opacitySB->setMinimum(0);
		opacitySB->setMaximum(100);
		opacitySB->setValue(opacity);
		//opacityL = new QLabel();
		//opacityL->setText(QString::number(opacity).toStdString().c_str());
	


		colorbutton->setChosenColor(myColor);
		


		/*QTableWidgetItem *item = new QTableWidgetItem;
		item->setFlags(item->flags() | Qt::ItemIsEditable);
		item->setText(QString::number(opacity).toStdString().c_str());*/

		//this->Ui->tableWidget->setItem(i, 1, item); //TAG ALPHA
		
		QTableWidgetItem *item2 = new QTableWidgetItem;
		item2->setFlags(item2->flags() | Qt::ItemIsEditable);
		item2->setText(tagNames.at(i).c_str());


		this->Ui->tableWidget->setItem(i, 0, item2); // TAG NAME
													 //this->Ui->tableWidget->setCellWidget(i, 1, nom);

		this->Ui->tableWidget->setCellWidget(i, 1, radio); // TAG ACTIVE

		this->Ui->tableWidget->setCellWidget(i, 2, colorbutton); // TAG COLOR
		this->Ui->tableWidget->setCellWidget(i, 3, opacitySB); //TAG OPACITY
		/*this->Ui->tableWidget->setCellWidget(i, 4, clear);// TAG CLEAR
		if (i == 0)
		{
			clear->setDisabled(true);
		}*/
		
		colorbutton->setObjectName(QString(i));
		connect(colorbutton, SIGNAL(colorChosen()), this, SLOT(slotColorChanged()));
		connect(radio, SIGNAL(clicked()), this, SLOT(slotActiveTagChanged()));
		connect(opacitySB, SIGNAL(valueChanged(int)), this, SLOT(slotOpacityChanged(int)));
		
		
	}
	cout << "Done..." << endl;

}
void mqEditTagsDialog::UpdateUI()
{
	/*int color_scale_id = MT->GetColorScaleId();
	
	currentMax->value(MT->Get_sc_max());
	currentMin->value(MT->Get_sc_min());
*/

	//1 populate comboActiveTags

	this->RefreshComboActiveTags();

	// 2 populate comboTagMaps
	this->RefreshComboTagMaps();
	// 3 populate Tag Map Table
	this->RefreshTagMapTable();
	
	
}
void mqEditTagsDialog::slotRefreshTagMaps()
{
	cout << "slotRefreshTagMaps" << endl;
	this->RefreshComboTagMaps();
	this->RefreshTagMapTable();
}
void mqEditTagsDialog::slotReinitializeTagMap()
{
	QString ActiveTagMap = this->Ui->comboTagMaps->currentText();
	for (int i = 0; i < mqMorphoDigCore::instance()->Getmui_ExistingTagMaps()->Stack.size(); i++)
	{
		int iscustom = mqMorphoDigCore::instance()->Getmui_ExistingTagMaps()->Stack.at(i).isCustom;
		QString myExisingTagMapName = mqMorphoDigCore::instance()->Getmui_ExistingTagMaps()->Stack.at(i).Name;
		if (ActiveTagMap == myExisingTagMapName && iscustom==0)
		{

			mqMorphoDigCore::instance()->reinitializeTagMap(i);
			this->RefreshTagMapTable();
			
			mqMorphoDigCore::instance()->Render();		


		}
	}
}
void mqEditTagsDialog::slotAddTag()
{
	cout << "Add Tag!" << endl;
	QString ActiveTagMap = this->Ui->comboTagMaps->currentText();
	for (int i = 0; i < mqMorphoDigCore::instance()->Getmui_ExistingTagMaps()->Stack.size(); i++)
	{
		int iscustom = mqMorphoDigCore::instance()->Getmui_ExistingTagMaps()->Stack.at(i).isCustom;
		QString myExisingTagMapName = mqMorphoDigCore::instance()->Getmui_ExistingTagMaps()->Stack.at(i).Name;
		if (ActiveTagMap == myExisingTagMapName )
		{
			cout << "Add Tag To Map(" <<i<<")"<< endl;
			mqMorphoDigCore::instance()->addTagToTagMap(i);
			this->RefreshTagMapTable();

			mqMorphoDigCore::instance()->Render();


		}
	}
}
void mqEditTagsDialog::slotRemoveTag()
{

	QString ActiveTagMap = this->Ui->comboTagMaps->currentText();
	for (int i = 0; i < mqMorphoDigCore::instance()->Getmui_ExistingTagMaps()->Stack.size(); i++)
	{
		int iscustom = mqMorphoDigCore::instance()->Getmui_ExistingTagMaps()->Stack.at(i).isCustom;
		QString myExisingTagMapName = mqMorphoDigCore::instance()->Getmui_ExistingTagMaps()->Stack.at(i).Name;
		if (ActiveTagMap == myExisingTagMapName)
		{

			mqMorphoDigCore::instance()->removeTagFromTagMap(i);
			this->RefreshTagMapTable();

			mqMorphoDigCore::instance()->Render();


		}
	}
}
void mqEditTagsDialog::slotEditTagMapName()
{
	QString ActiveTagMap = this->Ui->comboTagMaps->currentText();
	for (int i = 0; i < mqMorphoDigCore::instance()->Getmui_ExistingTagMaps()->Stack.size(); i++)
	{
		int iscustom = mqMorphoDigCore::instance()->Getmui_ExistingTagMaps()->Stack.at(i).isCustom;
		QString myExisingTagMapName = mqMorphoDigCore::instance()->Getmui_ExistingTagMaps()->Stack.at(i).Name;
		if (ActiveTagMap == myExisingTagMapName && iscustom)
		{
			QInputDialog *giveNameDialog = new QInputDialog();
			bool dialogResult;
			QString newTagMapName = giveNameDialog->getText(0, "Change color map name", "Name:", QLineEdit::Normal,
				myExisingTagMapName, &dialogResult);
			if (dialogResult)
			{

				cout << "new color map given:" << newTagMapName.toStdString() << endl;
				if (mqMorphoDigCore::instance()->tagMapNameAlreadyExists(newTagMapName) == 1)
				{
					QMessageBox msgBox;
					msgBox.setText("Can't change custom map name : name already exists.");
					msgBox.exec();
					return;
				}
				if (newTagMapName.length() == 0)
				{
					QMessageBox msgBox;
					msgBox.setText("Can't save custom map: name length =0.");
					msgBox.exec();
					return;
				}
				mqMorphoDigCore::instance()->Getmui_ExistingTagMaps()->Stack.at(i).Name = newTagMapName;
				mqMorphoDigCore::instance()->Setmui_ActiveTagMap(newTagMapName, mqMorphoDigCore::instance()->Getmui_ExistingTagMaps()->Stack.at(i).numTags, mqMorphoDigCore::instance()->Getmui_ExistingTagMaps()->Stack.at(i).tagNames, mqMorphoDigCore::instance()->Getmui_ExistingTagMaps()->Stack.at(i).TagMap);
				this->RefreshComboTagMaps();
				//mqMorphoDigCore::instance()->createCustomTagMap(newTagMapName, this->STC);				
				//this->UpdateUI();
			}
			else
			{
				cout << "cancel " << endl;
			}
			
			//this->mTagMap->reInitialize(mqMorphoDigCore::instance()->Getmui_ExistingTagMaps()->Stack.at(i).TagMap);



		}
	}

}

void mqEditTagsDialog::slotDeleteTagMap()
{

	QString ActiveTagMap = this->Ui->comboTagMaps->currentText();
	for (int i = 0; i < mqMorphoDigCore::instance()->Getmui_ExistingTagMaps()->Stack.size(); i++)
	{
		int iscustom = mqMorphoDigCore::instance()->Getmui_ExistingTagMaps()->Stack.at(i).isCustom;
		QString myExisingTagMapName = mqMorphoDigCore::instance()->Getmui_ExistingTagMaps()->Stack.at(i).Name;
		if (ActiveTagMap == myExisingTagMapName && iscustom)
		{
		
			mqMorphoDigCore::instance()->deleteTagMap(i);
			//mqMorphoDigCore::instance()->Getmui_ExistingTagMaps()->Stack.erase(mqMorphoDigCore::instance()->Getmui_ExistingTagMaps()->Stack.begin() + i);
			//mqMorphoDigCore::instance()->Setmui_ActiveTagMap(newTagMapName, mqMorphoDigCore::instance()->Getmui_ExistingTagMaps()->Stack.at(i).TagMap);

			this->RefreshComboTagMaps();			
			mqMorphoDigCore::instance()->Render();
				//mqMorphoDigCore::instance()->createCustomTagMap(newTagMapName, this->STC);				
				//this->UpdateUI();
			

			//this->mTagMap->reInitialize(mqMorphoDigCore::instance()->Getmui_ExistingTagMaps()->Stack.at(i).TagMap);



		}
	}
}

void mqEditTagsDialog::RefreshComboActiveTags()
{
	
	this->Ui->comboActiveTags->clear();
	ExistingScalars *MyList = mqMorphoDigCore::instance()->Getmui_ExistingScalars();
	for (int i = 0; i < MyList->Stack.size(); i++)
	{
		if ((MyList->Stack.at(i).DataType == VTK_INT || MyList->Stack.at(i).DataType == VTK_UNSIGNED_INT) && MyList->Stack.at(i).NumComp == 1)
		{
			this->Ui->comboActiveTags->addItem(MyList->Stack.at(i).Name);
		}

	}
	QString myActiveScalars = mqMorphoDigCore::instance()->Getmui_ActiveScalars()->Name;
	cout << "DIAL myActiveTags " << myActiveScalars.toStdString() << endl;
	int exists = -1;
	for (int i = 0; i < this->Ui->comboActiveTags->count(); i++)
	{
		QString myScalar = this->Ui->comboActiveTags->itemText(i);
		if (myScalar == myActiveScalars)
		{
			cout << "DIAL found in list!!!!! " << myScalar.toStdString() << endl;
			exists = i;

		}

	}
	if (exists > -1) {
		cout << "DIAL Now current index of combo box is " << exists << endl;
		this->Ui->comboActiveTags->setCurrentIndex(exists);


	}
	
}


void mqEditTagsDialog::RefreshComboTagMaps() 
{
	
	cout << "RefreshComboTagMaps" << endl;
	this->Ui->comboTagMaps->clear();
	ExistingTagMaps *MyTM = mqMorphoDigCore::instance()->Getmui_ExistingTagMaps();
	cout << "Found" << MyTM->Stack.size() << "tag maps" << endl;
	for (int i = 0; i < MyTM->Stack.size(); i++)
	{

		this->Ui->comboTagMaps->addItem(MyTM->Stack.at(i).Name);


	}
	QString myActiveCM = mqMorphoDigCore::instance()->Getmui_ActiveTagMap()->Name;
	cout << "DIAL myActiveTagMap " << myActiveCM.toStdString() << endl;
	int exists = -1;
	for (int i = 0; i < this->Ui->comboTagMaps->count(); i++)
	{
		QString myCM = this->Ui->comboTagMaps->itemText(i);
		if (myCM == myActiveCM)
		{
			cout << "DIAL found CM in list!!!!! " << myCM.toStdString() << endl;
			exists = i;

		}

	}
	if (exists > -1) {
		cout << "DIAL Now current index of combo box is " << exists << endl;
		this->Ui->comboTagMaps->setCurrentIndex(exists);
				

	}
}

void mqEditTagsDialog::slotRemoveTags()
{
	
	mqMorphoDigCore::instance()->RemoveScalar(this->Ui->comboActiveTags->currentText(), this->Ui->selectedObjects->isChecked());
	this->UpdateUI();
	
	
}


void mqEditTagsDialog::slotRefreshComboTags()
{
	this->RefreshComboActiveTags();


}


void mqEditTagsDialog::RefreshDialog()
{
	this->UpdateUI();
	mqMorphoDigCore::instance()->Render();
}

void mqEditTagsDialog::slotPencilSearchSizeChanged(int newSize)
{
	mqMorphoDigCore::instance()->Setmui_PencilSize(newSize);

}
void mqEditTagsDialog::slotPencilLimitAngleChanged(int newLimitAngle)
{
	mqMorphoDigCore::instance()->Setmui_PencilLimitAngle(newLimitAngle);

}


void mqEditTagsDialog::slotActiveTagsChanged(int idx)
{
	
	cout << "looks like active tags have changed!:: " << idx << endl;
	QString NewActiveTagName = this->Ui->comboActiveTags->currentText();
	for (int i = 0; i < mqMorphoDigCore::instance()->Getmui_ExistingScalars()->Stack.size(); i++)
	{
		QString myExisingScalarName = mqMorphoDigCore::instance()->Getmui_ExistingScalars()->Stack.at(i).Name;
		if (NewActiveTagName == myExisingScalarName)
		{

			mqMorphoDigCore::instance()->Setmui_ActiveScalarsAndRender(NewActiveTagName,
				mqMorphoDigCore::instance()->Getmui_ExistingScalars()->Stack.at(i).DataType,
				mqMorphoDigCore::instance()->Getmui_ExistingScalars()->Stack.at(i).NumComp
			);
			
			

		}
	}
	

}
void mqEditTagsDialog::slotBucketOn()
{

	this->Ui->pencilOn->setChecked(false);
	this->Ui->bucketOn->setChecked(true);
	mqMorphoDigCore::instance()->Setmui_TagTool(1);
}
void mqEditTagsDialog::slotPencilOn()
{
	this->Ui->pencilOn->setChecked(true);
	this->Ui->bucketOn->setChecked(false);
	mqMorphoDigCore::instance()->Setmui_TagTool(0);
}
void mqEditTagsDialog::slotActivateTagMode()
{
	if (this->Ui->activateTagMode->isChecked())
	{
		cout << "Activate Tag Mode" << endl;
		mqMorphoDigCore::instance()->Setmui_TagModeActivated(1);
		this->Ui->pencilOn->setDisabled(false);
		this->Ui->bucketOn->setDisabled(false);
		this->Ui->lassoOn->setDisabled(false);
	}
	else
	{
		cout << "Deactivate Tag Mode" << endl;
		mqMorphoDigCore::instance()->Setmui_TagModeActivated(0);
		this->Ui->pencilOn->setDisabled(true);
		this->Ui->bucketOn->setDisabled(true);
		this->Ui->lassoOn->setDisabled(true);
	}
}

/*void mqEditTagsDialog::slotDeactivateTagMode()
{
		cout << "Deactivate Tag Mode" << endl;
		mqMorphoDigCore::instance()->Setmui_TagModeActivated(0);
	
}*/

void mqEditTagsDialog::slotActiveTagMapChanged(int idx)
{
	
	cout << "looks like active tag map has changed!:: " << idx << endl;
	QString NewActiveTagMap = this->Ui->comboTagMaps->currentText();
	for (int i = 0; i < mqMorphoDigCore::instance()->Getmui_ExistingTagMaps()->Stack.size(); i++)
	{
		QString myExisingTagMapName = mqMorphoDigCore::instance()->Getmui_ExistingTagMaps()->Stack.at(i).Name;
		if (NewActiveTagMap == myExisingTagMapName)
		{
			
			if (mqMorphoDigCore::instance()->Getmui_ExistingTagMaps()->Stack.at(i).isCustom==1)
			{
				this->Ui->reinitializeTagMap->setDisabled(true);
				this->Ui->deleteTagMap->setDisabled(false);
				this->Ui->editTagMap->setDisabled(false);
			}
			else
			{
				this->Ui->reinitializeTagMap->setDisabled(false);
				this->Ui->deleteTagMap->setDisabled(true);
				this->Ui->editTagMap->setDisabled(true);

			}
			mqMorphoDigCore::instance()->Setmui_ActiveTagMapAndRender(NewActiveTagMap, mqMorphoDigCore::instance()->Getmui_ExistingTagMaps()->Stack.at(i).numTags,
				mqMorphoDigCore::instance()->Getmui_ExistingTagMaps()->Stack.at(i).tagNames,
				mqMorphoDigCore::instance()->Getmui_ExistingTagMaps()->Stack.at(i).TagMap);

			mqMorphoDigCore::instance()->matchTagMapToActorCollection();
			cout << "Call refresh tag map table" << endl;
			this->RefreshTagMapTable();
			cout << "Tag map tabe refreshed" << endl;
		


		}
	}
	

}




void mqEditTagsDialog::slotRefreshDialog()
{
	cout << "Let's refresh dialog!"		<< endl;

	//Dirty hack here! Replace "mqMorphoDigCore::instance()->GetScalarRangeMax()" by something which finds the bounds
	// of the currently used lookup table

	this->RefreshComboActiveTags();
	//this->RefreshDialog();
}


//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

