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

	vtkDiscretizableColorTransferFunction* STC = mqMorphoDigCore::instance()->GetOneColorMap();

	cout << "Edit Volume Dialog: Create mqColorOpacityEditorWidget!" << endl;
	mqColorOpacityEditorWidget *someMap = new mqColorOpacityEditorWidget(STC, this->Ui->PropertiesFrame);
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



	this->UpdateUI();
	this->Ui->currentMin->setButtonSymbols(QAbstractSpinBox::NoButtons);
	this->Ui->currentMax->setButtonSymbols(QAbstractSpinBox::NoButtons);
	this->Ui->currentMin->setMinimum(-DBL_MAX);
	this->Ui->currentMax->setMinimum(-DBL_MAX);
	this->Ui->currentMax->setValue(1);
	this->Ui->currentMin->setValue(0);

	this->Ui->currentMin->setMaximum(DBL_MAX);
	this->Ui->currentMax->setMaximum(DBL_MAX);
	this->ctfMin = 0;
	this->ctfMax = 1;

	/*this->Ui->sliderMin->setDoubleValue(0);
	this->Ui->sliderMin->setDoubleMaximum(1);
	this->Ui->sliderMin->setDoubleMinimum(-1);*/


	this->Ui->scalarOpacityUnitDistance->setMaximum(DBL_MAX);

	/*this->Ui->sliderMax->setDoubleValue(1);
	this->Ui->sliderMax->setDoubleMaximum(2);
	this->Ui->sliderMax->setDoubleMinimum(0);*/
	/*
		this->Ui->sliderMin->setDoubleSingleStep((this->Ui->sliderMin->doubleMaximum() - this->Ui->sliderMin->doubleMinimum()) / 100);

		this->Ui->sliderMax->setDoubleSingleStep((this->Ui->sliderMax->doubleMaximum() - this->Ui->sliderMax->doubleMinimum()) / 100);*/


	connect(this->Ui->ApplyMatrix, SIGNAL(pressed()), this, SLOT(slotapplyMatrixToAllSelectedVolumes()));
	connect(this->Ui->buttonBox, SIGNAL(accepted()), this, SLOT(slotsaveVolume()));
	connect(this->Ui->Reinit, SIGNAL(pressed()), this, SLOT(slotReinitMatrix()));




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
	connect(this->Ui->interpolationToLinear, SIGNAL(clicked(bool)), this, SLOT(slotInterpolationToLinear(bool)));
	connect(this->Ui->scalarOpacityUnitDistance, SIGNAL(valueChanged(double)), this, SLOT(slotScalarOpacityUnitDistance(double)));


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

void mqEditVolumeDialog::slotSliderStart()
{
	cout << "ShiftSliderStart" << endl;
	this->slideMin = this->Ui->currentMin->value();
	this->slideMax = this->Ui->currentMax->value();
	this->maxShiftAmplitude = (this->slideMax - this->slideMin) / 2;

}

void mqEditVolumeDialog::slotSlideMin(int slideMin)
{
	if (slideMin != 0)
	{
		cout << "slideMin:" << slideMin << endl;
		double newMin = this->slideMin + slideMin * this->maxShiftAmplitude / 100;
		this->Ui->currentMin->setValue(newMin);

		cout << "Min:" << newMin << endl;
		this->ctfMin = newMin;
		this->UpdateLookupTableRange();
	}


}
void mqEditVolumeDialog::slotSlideMax(int slideMax)
{
	if (slideMax != 0)
	{
		cout << "slideMax:" << slideMax << endl;
		double newMax = this->slideMax + slideMax * this->maxShiftAmplitude / 100;

		this->Ui->currentMax->setValue(newMax);
		cout << "Max:" << newMax << endl;

		this->ctfMax = newMax;
		this->UpdateLookupTableRange();
	}

}
void mqEditVolumeDialog::slotShiftSlider(int shift)
{
	if (shift != 0)
	{
		cout << "shift:" << shift << endl;
		double newMin = this->slideMin + shift * this->maxShiftAmplitude / 100;
		double newMax = this->slideMax + shift * this->maxShiftAmplitude / 100;
		this->Ui->currentMin->setValue(newMin);
		this->Ui->currentMax->setValue(newMax);
		cout << "Min:" << newMin << endl;
		cout << "Max:" << newMax << endl;
		this->ctfMin = newMin;
		this->ctfMax = newMax;

		this->UpdateLookupTableRange();
	}
}

void mqEditVolumeDialog::slotSliderStop()
{
	
	
	/*cout << "this->Ui->currentMin=" << this->Ui->currentMin->value()<<endl;
	cout << "this->Ui->currentMax=" << this->Ui->currentMax->value() << endl;
	cout << "this->Ui->sliderMin=" << this->Ui->sliderMin->value() << endl;
	cout << "this->Ui->sliderMax=" << this->Ui->sliderMin->value() << endl;
	cout << "this->ctfMin=" << this->ctfMin << endl;
	cout << "this->ctfMax=" << this->ctfMax << endl;*/
	

	//cout << "ShiftSliderStop" << endl;

}
void mqEditVolumeDialog::slotInterpolationToLinear(bool isChecked)
{
	if (this->Volume != NULL) {
		cout << "InterpolationToLinear: isChecked:" << isChecked<<endl;
		if (isChecked)
		{
			this->Volume->GetProperty()->SetInterpolationTypeToLinear();
		}
		else
		{
			this->Volume->GetProperty()->SetInterpolationTypeToNearest();
		}
		mqMorphoDigCore::instance()->Render();
	}

 }
void mqEditVolumeDialog::slotScalarOpacityUnitDistance(double SOUD)
{
	if (this->Volume != NULL && SOUD>0) {
		cout << "ScalarOpacityUnitDistance:" << SOUD << endl;

			this->Volume->GetProperty()->SetScalarOpacityUnitDistance(SOUD);
		
		mqMorphoDigCore::instance()->Render();
	}
}

void mqEditVolumeDialog::slotCurrentMinEdited()
{
	cout << "Current Min edited!" << endl;
	if (this->Ui->currentMin->value() < this->Ui->currentMax->value())
	{
		this->ctfMin = this->Ui->currentMin->value();
	}
	else
	{ 
		this->ctfMin= this->Ui->currentMax->value() -10;
	}

	
	
	
		this->UpdateLookupTableRange();
	
}

void mqEditVolumeDialog::slotCurrentMaxEdited()
{
	cout << "Current max edited!" << endl;
	if (this->Ui->currentMin->value() < this->Ui->currentMax->value())
	{
		this->ctfMax = this->Ui->currentMax->value();
	}
	else
	{
		this->ctfMax = this->Ui->currentMin->value() + 10;
	}




	this->UpdateLookupTableRange();

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
		this->ctfMin= this->GetCTFMin();
		this->ctfMax = this->GetCTFMax();
		this->Ui->currentMin->setValue(this->ctfMin);
		this->Ui->currentMax->setValue(this->ctfMax);
		
		
		
	}
	
}
double mqEditVolumeDialog::GetCTFMin()
{
	if (this->Volume != NULL) {
		vtkSmartPointer<vtkDiscretizableColorTransferFunction> CM = this->Volume->GetCtf();

		double *pts = CM->GetDataPointer();

		int numnodes = CM->GetSize();
		double old_min = DBL_MAX;
		
		for (int j = 0; j < numnodes; j++)
		{
			double curr = pts[4 * j];
			cout << "x" << j << "=" << curr << endl;
			if (curr < old_min) { old_min = curr; }
			
		}
		
		vtkPiecewiseFunction* OF = CM->GetScalarOpacityFunction();
		int numnodes2 = OF->GetSize();
		double *pts2 = OF->GetDataPointer();
		//cout << this->mui_ExistingColorMaps->Stack.at(i).Name.toStdString() << ": OF num nodes = " << numnodes2 << endl;
		double old_min2 = DBL_MAX;
		for (int j = 0; j < numnodes2; j++)
		{
			double curr = pts2[2 * j];
			//cout << "x" << j << "=" << curr << endl;
			if (curr < old_min2) { old_min2 = curr; }
		}
		if (old_min < old_min2) { return old_min2; }
		else { return old_min; }


	}
	return -1;

}
double mqEditVolumeDialog::GetCTFMax()
{
	if (this->Volume != NULL) {
		vtkSmartPointer<vtkDiscretizableColorTransferFunction> CM = this->Volume->GetCtf();

		double *pts = CM->GetDataPointer();

		int numnodes = CM->GetSize();
		double old_max = -DBL_MAX;
		for (int j = 0; j < numnodes; j++)
		{
			double curr = pts[4 * j];
			if (curr > old_max) { old_max = curr; }

		}
	
		vtkPiecewiseFunction* OF = CM->GetScalarOpacityFunction();
		int numnodes2 = OF->GetSize();
		double *pts2 = OF->GetDataPointer();
		double old_max2 = -DBL_MAX;
		for (int j = 0; j < numnodes2; j++)
		{
			double curr = pts2[2 * j];
			if (curr > old_max2) { old_max2 = curr; }

		}
		if (old_max2 > old_max) { return old_max2; }
		else { return old_max; }

	}
	return 1;
}
void mqEditVolumeDialog::UpdateLookupTableRange()
{
	if (this->Volume != NULL) {
		vtkSmartPointer<vtkDiscretizableColorTransferFunction> CM = this->Volume->GetCtf();

		double *pts = CM->GetDataPointer();

		int numnodes = CM->GetSize();
		double old_min = DBL_MAX;
		double old_max = -DBL_MAX;
		for (int j = 0; j < numnodes; j++)
		{
			double curr = pts[4 * j];
			cout << "x" << j << "=" << curr << endl;
			if (curr < old_min) { old_min = curr; }
			if (curr > old_max) { old_max = curr; }

		}
		cout << "old max:" << old_max << ", old min:" << old_min << endl;
		if (old_max > old_min)
		{
			double old_range = old_max - old_min;
			double new_range = this->ctfMax - this->ctfMin;
			double mult = new_range / old_range;
			double c = this->ctfMin - old_min * mult;
			for (int k = 0; k < numnodes; k++)
			{
				pts[4 * k] = pts[4 * k] * mult + c;
				cout << "nx" << k << "=" << pts[4 * k] << endl;
			}
			CM->FillFromDataPointer(numnodes, pts);

		}
		vtkPiecewiseFunction* OF = CM->GetScalarOpacityFunction();
		int numnodes2 = OF->GetSize();
		double *pts2 = OF->GetDataPointer();
		//cout << this->mui_ExistingColorMaps->Stack.at(i).Name.toStdString() << ": OF num nodes = " << numnodes2 << endl;
		double old_min2 = DBL_MAX;
		double old_max2 = -DBL_MAX;
		for (int j = 0; j < numnodes2; j++)
		{
			double curr = pts2[2 * j];
			//cout << "x" << j << "=" << curr << endl;
			if (curr < old_min2) { old_min2 = curr; }
			if (curr > old_max2) { old_max2 = curr; }

		}
		if (old_max2 > old_min2)
		{
			double old_range = old_max2 - old_min2;
			double new_range = this->ctfMax - this->ctfMin;
			double mult = new_range / old_range;
			double c = this->ctfMin - old_min2 * mult;
			for (int k = 0; k < numnodes2; k++)
			{
				pts2[2 * k] = pts2[2 * k] * mult + c;
				//cout << "nx" << k << "=" << pts2[2*k] << endl;
			}
			OF->FillFromDataPointer(numnodes2, pts2);

		}

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

