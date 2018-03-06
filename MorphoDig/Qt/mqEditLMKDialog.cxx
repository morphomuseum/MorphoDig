/*=========================================================================

   Program: MorphoDig
   Module:    $RCS $

 
=========================================================================*/

#include "mqEditLMKDialog.h"
#include "ui_mqEditLMKDialog.h"
#include "MorphoDigVersion.h"
#include "mqMorphoDigCore.h"
#include "mqUndoStack.h"
#include "vtkLMActor.h"
#include "vtkLMActorCollection.h"

// we actually do not need glew...
//#include <GL/glew.h>
#include <QApplication>
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
mqEditLMKDialog::mqEditLMKDialog(QWidget* Parent)
  : QDialog(Parent)
  , Ui(new Ui::mqEditLMKDialog())
{

	this->Ui->setupUi(this);
	this->setObjectName("mqEditLMKDialog");
	connect(mqMorphoDigCore::instance(), SIGNAL(lmSelectionChanged()), this, SLOT(slotRefreshDialog()));
	connect(this->Ui->next, SIGNAL(pressed()), this, SLOT(slotGetNextLandmark()));
	connect(this->Ui->prec, SIGNAL(pressed()), this, SLOT(slotGetPrecedingLandmark()));
	
	LMK_Coll = NULL;
	LMK = NULL;
	current_coll = -1;
	this->Ui->x->setDecimals(10);
	this->Ui->y->setDecimals(10);
	this->Ui->z->setDecimals(10);
	this->Ui->x->setMinimum(-DBL_MAX);
	this->Ui->y->setMinimum(-DBL_MAX);
	this->Ui->z->setMinimum(-DBL_MAX);

	this->Ui->x->setMaximum(DBL_MAX);
	this->Ui->y->setMaximum(DBL_MAX);
	this->Ui->z->setMaximum(DBL_MAX);
	

	this->GetFirstSelectedLandmark();
	this->UpdateUI();
	
 
  
 //connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotEditLMK()));

}




//-----------------------------------------------------------------------------
mqEditLMKDialog::~mqEditLMKDialog()
{

 //depending on what is 
	
  delete this->Ui;
}

int mqEditLMKDialog::SomeThingHasChanged()
{
	int something_has_changed = 0;
	if (this->LMK != NULL)
	{
		
		double orig[3];
		this->LMK->GetLMOrigin(orig);

		if (
			orig[0] != this->Ui->x->value() ||
			orig[1] != this->Ui->y->value() ||
			orig[2] != this->Ui->z->value()
			)
		{
			something_has_changed = 1;
		}

		
	}
	return something_has_changed;
}

int mqEditLMKDialog::CurrentLMKInCollection()
{
	int lmk_found = 0;
	vtkLMActor * Lmk;
	if (this->LMK != NULL && this->LMK_Coll != NULL)
	{
		this->LMK_Coll->InitTraversal();

		for (vtkIdType i = 0; i < this->LMK_Coll->GetNumberOfItems(); i++)
		{
			Lmk = vtkLMActor::SafeDownCast(this->LMK_Coll->GetNextActor());
			if (lmk_found == 1) { return lmk_found; }
			if (Lmk == this->LMK)
			{
				cout << "Landmark found!!" << endl;
				lmk_found = 1;
			}
		}
	}
	return lmk_found;
}
void mqEditLMKDialog::saveLMK()
{
	cout << "Save LMK!" << endl;
	if (this->LMK != NULL&& this->CurrentLMKInCollection())
	{
		int something_has_changed = this->SomeThingHasChanged();
		if (something_has_changed)
		{
			std::string action = "Update landmark's origin";
			int mCount = BEGIN_UNDO_SET(action);
			this->LMK->SaveState(mCount);
			this->LMK->SetLMOrigin(this->Ui->x->value(), this->Ui->y->value(), this->Ui->z->value());
			this->LMK->Modified();
			END_UNDO_SET();
		}
	}
	
}



void mqEditLMKDialog::GetFirstSelectedLandmark()
{
	this->LMK_Coll = NULL;
	this->LMK = NULL;
	this->current_coll = -1;

	this->LMK_Coll = mqMorphoDigCore::instance()->getNormalLandmarkCollection();

	int num_selected = 0;
	num_selected = this->LMK_Coll->GetNumberOfSelectedActors();
	if (num_selected > 0) {
		this->LMK = this->LMK_Coll->GetFirstSelectedActor();
		current_coll = 0;
	}
	else
	{
		this->LMK_Coll = mqMorphoDigCore::instance()->getTargetLandmarkCollection();
		num_selected = this->LMK_Coll->GetNumberOfSelectedActors();
		if (num_selected > 0) {
			this->LMK = this->LMK_Coll->GetFirstSelectedActor();
			current_coll = 1;
		}
		else
		{
			this->LMK_Coll = mqMorphoDigCore::instance()->getNodeLandmarkCollection();
			num_selected = this->LMK_Coll->GetNumberOfSelectedActors();
			if (num_selected > 0) {
				this->LMK = this->LMK_Coll->GetFirstSelectedActor();
				current_coll = 2;
			}
			else
			{
				this->LMK_Coll = mqMorphoDigCore::instance()->getHandleLandmarkCollection();
				num_selected = this->LMK_Coll->GetNumberOfSelectedActors();
				if (num_selected > 0) {
					this->LMK = this->LMK_Coll->GetFirstSelectedActor();
					current_coll = 3;
				}
			}

		}
	}

	if (this->LMK != NULL)
	{
	
		this->LMK_Coll->Modified();
	}

}

void mqEditLMKDialog::GetFirstLandmark()
{
	this->LMK_Coll = mqMorphoDigCore::instance()->getNormalLandmarkCollection();
	int num = 0;
	num = this->LMK_Coll->GetNumberOfItems();
	if (num > 0) {
		this->LMK_Coll->InitTraversal();
		this->LMK = vtkLMActor::SafeDownCast(this->LMK_Coll->GetNextActor());
		current_coll = 0;
	}
	else
	{
		this->LMK_Coll = mqMorphoDigCore::instance()->getTargetLandmarkCollection();
		num = 0;
		num = this->LMK_Coll->GetNumberOfItems();
		if (num > 0) {
			this->LMK_Coll->InitTraversal();
			this->LMK = vtkLMActor::SafeDownCast(this->LMK_Coll->GetNextActor());
			current_coll = 1;
		}
		else
		{
			this->LMK_Coll = mqMorphoDigCore::instance()->getNodeLandmarkCollection();
			num = 0;
			num = this->LMK_Coll->GetNumberOfItems();
			if (num > 0) {
				this->LMK_Coll->InitTraversal();
				this->LMK = vtkLMActor::SafeDownCast(this->LMK_Coll->GetNextActor());
				current_coll = 2;
			}
			else
			{
				this->LMK_Coll = mqMorphoDigCore::instance()->getHandleLandmarkCollection();
				num = 0;
				num = this->LMK_Coll->GetNumberOfItems();
				if (num > 0) {
					this->LMK_Coll->InitTraversal();
					this->LMK = vtkLMActor::SafeDownCast(this->LMK_Coll->GetNextActor());
					current_coll = 3;
				}
			}

		}
	}

	if (this->LMK != NULL)
	{
		this->LMK->SetSelected(1);
		this->LMK_Coll->Modified();
	}

}

void mqEditLMKDialog::UpdateUI()
{
	if (this->LMK != NULL) {
		this->Ui->LMKNumber->setValue(this->LMK->GetLMNumber());
		double orig[3];
		this->LMK->GetLMOrigin(orig);
		cout << "orig[0]" << orig[0] << ", orig[1]" << orig[1] << ", orig[2]" << orig[2] << endl;
		this->Ui->x->setValue(orig[0]);
		this->Ui->x->update();
		this->Ui->y->setValue(orig[1]);
		this->Ui->y->update();
		this->Ui->z->setValue(orig[2]);
		this->Ui->z->update();
	}

}


void mqEditLMKDialog::GetNextLandmark()
{

	if (LMK == NULL)
	{
		this->GetFirstLandmark();
	}
	else
	{
		this->LMK->SetSelected(0);
		this->LMK_Coll->Modified();
		int num = this->LMK->GetLMNumber();
		int cpt = 0;
		while (cpt < 4)
		{
			cpt++;
			this->LMK = this->LMK_Coll->GetLandmarkAfter(num);
			if (this->LMK != NULL) { this->LMK->SetSelected(1); this->LMK_Coll->Modified(); break; }
			else
			{

				num = 0;
				if (this->current_coll == 0 )
				{
					this->LMK_Coll = mqMorphoDigCore::instance()->getTargetLandmarkCollection();
					this->current_coll = 1;
				}
				else if (this->current_coll == 1)
				{
					this->LMK_Coll = mqMorphoDigCore::instance()->getNodeLandmarkCollection();
					this->current_coll = 2;
				}
				else if (this->current_coll == 2)
				{
					this->LMK_Coll = mqMorphoDigCore::instance()->getHandleLandmarkCollection();
					this->current_coll = 3;
				}
				else if (this->current_coll == 3)
				{
					this->LMK_Coll = mqMorphoDigCore::instance()->getNormalLandmarkCollection();
					this->current_coll = 0;
				}

			}

		}


	}

}
void mqEditLMKDialog::GetPrecedingLandmark()
{

	if (LMK == NULL)
	{
		this->GetFirstLandmark();
	}
	else
	{
		this->LMK->SetSelected(0);
		this->LMK_Coll->Modified();
		int num = this->LMK->GetLMNumber();
		cout << "num=" << num << endl;
		int cpt = 0;
		while (cpt < 4)
		{
			cpt++;
			this->LMK = this->LMK_Coll->GetLandmarkBefore(num);
			this->LMK_Coll->Modified();
			if (this->LMK != NULL) { this->LMK->SetSelected(1); break; }
			else
			{

				
				if (this->current_coll == 0)
				{
					this->LMK_Coll = mqMorphoDigCore::instance()->getHandleLandmarkCollection();
					num = this->LMK_Coll->GetNumberOfItems()+1;
					this->current_coll = 3;
				}
				else if (this->current_coll == 1)
				{
					this->LMK_Coll = mqMorphoDigCore::instance()->getNormalLandmarkCollection();
					num = this->LMK_Coll->GetNumberOfItems() + 1;
					this->current_coll = 0;
				}
				else if (this->current_coll == 2)
				{
					this->LMK_Coll = mqMorphoDigCore::instance()->getTargetLandmarkCollection();
					num = this->LMK_Coll->GetNumberOfItems() + 1;
					
					this->current_coll = 1;
				}
				else if (this->current_coll == 3)
				{
					this->LMK_Coll = mqMorphoDigCore::instance()->getNodeLandmarkCollection();
					num = this->LMK_Coll->GetNumberOfItems() + 1;
					this->current_coll = 2;
				}

			}

		}


	}

}

void mqEditLMKDialog::slotGetPrecedingLandmark()
{
	this->saveLMK();
	this->GetPrecedingLandmark();
	this->UpdateUI();
	mqMorphoDigCore::instance()->Render();
}
void mqEditLMKDialog::slotGetNextLandmark()
{
	
	this->saveLMK();
	this->GetNextLandmark();
	this->UpdateUI();
	mqMorphoDigCore::instance()->Render();
}
void mqEditLMKDialog::RefreshDialog()
{
	cout << "Refresh LMK Dialog!" << endl;
	this->GetFirstSelectedLandmark();
	this->UpdateUI();
	mqMorphoDigCore::instance()->Render();
}

void mqEditLMKDialog::slotRefreshDialog()
{
	
	this->RefreshDialog();

}


//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------

