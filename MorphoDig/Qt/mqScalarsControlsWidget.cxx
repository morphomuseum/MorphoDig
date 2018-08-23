/*=========================================================================

   Program: MorphoDig
  

========================================================================*/
#include "mqScalarsControlsWidget.h"
#include "ui_mqScalarsControlsWidget.h"

// For later!
#include "mqEditScalarsDialogReaction.h"
#include "mqEditTagsDialogReaction.h"
#include "mqUndoRedoReaction.h"
#include "mqMorphoDigCore.h"


#include <QToolButton>
#include <QComboBox>


//-----------------------------------------------------------------------------
void mqScalarsControlsWidget::constructor()
{
 // Ui::mqScalarsControlsWidget ui;
 // ui.setupUi(this);
  this->ui = new Ui_mqScalarsControlsWidget;
  this->ui->setupUi(this);
  this->comboActiveScalars = new QComboBox;
  this->comboActiveScalars->setSizeAdjustPolicy(QComboBox::AdjustToContents);
  //@@ to do : populate combo box according to found scalars
  // Create a list with all possible scalars.
  //this->comboActiveScalars->addItems({ "Initial RGB", "Tags", "Thickness", "Curvature"});
  this->comboActiveScalars->addItems({ "Solid color" });
  // Add values in the combo box
  this->ui->horizontalLayout->addWidget(this->comboActiveScalars);
  
  
  
  connect(mqMorphoDigCore::instance(), SIGNAL(existingScalarsChanged()), this, SLOT(slotRefreshComboScalars()));

  connect(this->ui->ScalarsVisibility, SIGNAL(pressed()), this, SLOT(slotScalarVisitiliby()));
  connect(mqMorphoDigCore::instance(), SIGNAL(activeScalarChanged()), this, SLOT(slotRefreshComboScalars()));
  connect(this->comboActiveScalars, SIGNAL(activated(int)), this, SLOT(slotActiveScalarChanged(int)));

  //@@@
  QAction* colorScaleAction = new QAction(tr("&ColorScale"), this);
  colorScaleAction->setToolTip(tr("Edit lookup table."));
  this->ui->ColorScaleEdit->addAction(colorScaleAction);
  this->ui->ColorScaleEdit->setDefaultAction(colorScaleAction);
  QIcon icon;
  icon.addFile(QStringLiteral(":/Icons/color_scale_edit.png"), QSize(), QIcon::Normal, QIcon::Off);
  //  exportColorMap->setIcon(icon);
  colorScaleAction->setIcon(icon);



  

  new mqEditScalarsDialogReaction(colorScaleAction);
  


  //@@@
  QAction* TagEditAction = new QAction(tr("&TagEdit"), this);
  TagEditAction->setToolTip(tr("Edit tags."));
  this->ui->TagEdit->addAction(TagEditAction);
  this->ui->TagEdit->setDefaultAction(TagEditAction);
  QIcon icon2;
  icon2.addFile(QStringLiteral(":/Icons/tag_edit.png"), QSize(), QIcon::Normal, QIcon::Off);
  //  exportColorMap->setIcon(icon);
  TagEditAction->setIcon(icon2);
  new mqEditTagsDialogReaction(TagEditAction);
  if (mqMorphoDigCore::instance()->Getmui_ScalarVisibility() == 1)
  {
	  this->comboActiveScalars->setDisabled(false);
	  this->ui->ColorScaleEdit->setDisabled(false);
	  this->ui->TagEdit->setDisabled(false);
	  this->ui->ScalarsVisibility->setChecked(true);

  }
  else
  {
	  this->comboActiveScalars->setDisabled(true);
	  this->ui->ColorScaleEdit->setDisabled(true);
	  this->ui->TagEdit->setDisabled(true);
	  this->ui->ScalarsVisibility->setChecked(false);

  }

  
}

void mqScalarsControlsWidget::slotActiveScalarChanged(int idx)
{
	cout << "looks like active scalar has changed!:: " << idx << endl;
	QString NewActiveScalarName = this->comboActiveScalars->currentText();
	for (int i = 0; i < mqMorphoDigCore::instance()->Getmui_ExistingScalars()->Stack.size(); i++)
	{
		QString myExisingScalarName = mqMorphoDigCore::instance()->Getmui_ExistingScalars()->Stack.at(i).Name;
		if (NewActiveScalarName == myExisingScalarName)
		{
		
			mqMorphoDigCore::instance()->Setmui_ActiveScalarsAndRender(NewActiveScalarName,
				mqMorphoDigCore::instance()->Getmui_ExistingScalars()->Stack.at(i).DataType, 
				mqMorphoDigCore::instance()->Getmui_ExistingScalars()->Stack.at(i).NumComp
			);
			//this->RefreshEditButtons();

		}
	}
	
	
}

void mqScalarsControlsWidget::RefreshEditButtons()
{

	this->ui->TagEdit->setEnabled(false);
	this->ui->ColorScaleEdit->setEnabled(false);
	if (mqMorphoDigCore::instance()->Getmui_ScalarVisibility() == 1)
	{
		if ((mqMorphoDigCore::instance()->Getmui_ActiveScalars()->DataType == VTK_INT ||
			mqMorphoDigCore::instance()->Getmui_ActiveScalars()->DataType == VTK_UNSIGNED_INT)
			&& mqMorphoDigCore::instance()->Getmui_ActiveScalars()->NumComp == 1
			)
		{
			this->ui->TagEdit->setEnabled(true);
		}
		if ((mqMorphoDigCore::instance()->Getmui_ActiveScalars()->DataType == VTK_FLOAT ||
			mqMorphoDigCore::instance()->Getmui_ActiveScalars()->DataType == VTK_DOUBLE)
			&& mqMorphoDigCore::instance()->Getmui_ActiveScalars()->NumComp == 1
			)
		{
			this->ui->ColorScaleEdit->setEnabled(true);
		}
	}
}

void mqScalarsControlsWidget::slotRefreshComboScalars()
{
	//cout << "Refresh combo "<< endl;
	this->comboActiveScalars->clear();
	ExistingScalars *MyList = mqMorphoDigCore::instance()->Getmui_ExistingScalars();
	for (int i = 0; i < MyList->Stack.size(); i++)
	{
		this->comboActiveScalars->addItem(MyList->Stack.at(i).Name);
		
	}
	QString myActiveScalars = mqMorphoDigCore::instance()->Getmui_ActiveScalars()->Name;
	//cout << "myActiveScalars " << myActiveScalars.toStdString()<< endl;
	int exists = -1;
	for (int i = 0; i < MyList->Stack.size(); i++)
	{
		QString myScalar = MyList->Stack.at(i).Name;
		if (myScalar == myActiveScalars)
		{
			//cout << "found in list!!!!! " << myScalar.toStdString() << endl;
			exists = i;

		}

	}
	if (exists > -1) { 
		//cout << "Now current index of combo box is " << exists << endl;
		this->comboActiveScalars->setCurrentIndex(exists); 
		//this->RefreshEditButtons();
		
	}
	
	
}
void mqScalarsControlsWidget::slotScalarVisitiliby()
{
	if (mqMorphoDigCore::instance()->Getmui_ScalarVisibility()==0)	
	{
		mqMorphoDigCore::instance()->Setmui_ScalarVisibility(1);
		this->ui->ScalarsVisibility->setChecked(false);// this should be "true"... but the ui has decided otherwise... 
		this->comboActiveScalars->setDisabled(false);
		this->ui->ColorScaleEdit->setDisabled(false);
		this->ui->TagEdit->setDisabled(false);
	}
	else
	{
		mqMorphoDigCore::instance()->Setmui_ScalarVisibility(0);
		this->ui->ScalarsVisibility->setChecked(true);// this should be "false"... but the ui has decided otherwise... 
		this->comboActiveScalars->setDisabled(true);
		this->ui->ColorScaleEdit->setDisabled(true);
		this->ui->TagEdit->setDisabled(true);
	}
	
	mqMorphoDigCore::instance()->Render();

	

}


