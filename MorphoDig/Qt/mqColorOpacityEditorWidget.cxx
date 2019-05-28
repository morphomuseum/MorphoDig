/*=========================================================================

   Program: ParaView
   Module:    $RCSfile$

   Copyright (c) 2005,2006 Sandia Corporation, Kitware Inc.
   All rights reserved.

   ParaView is a free software; you can redistribute it and/or modify it
   under the terms of the ParaView license version 1.2.

   See License_v1.2.txt for the full ParaView license.
   A copy of this license can be obtained by contacting
   Kitware Inc.
   28 Corporate Drive
   Clifton Park, NY 12065
   USA

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

========================================================================*/
#include "mqColorOpacityEditorWidget.h"
#include "ui_mqColorOpacityEditorWidget.h"


#include "mqMorphoDigCore.h"
#include "mqTransferFunctionWidget.h"
#include "mqSaveMAPDialogReaction.h"
#include <vtkCommand.h>
#include <vtkDiscretizableColorTransferFunction.h>
#include <vtkImageAccumulate.h>
//#include <vtkImageData.h>

#include <vtkEventQtSlotConnect.h>
#include <vtkNew.h>
#include "vtkPiecewiseFunction.h"

#include <vtkVector.h>
#include <vtkWeakPointer.h>
#include <QDoubleValidator>
#include <QMessageBox>
#include <QPointer>
#include <QInputDialog>
#include <QTimer>
#include <QAction>
#include <QVBoxLayout>
#include <QtDebug>
#include <QHeaderView>
#include <cmath>


//-----------------------------------------------------------------------------
class mqColorOpacityEditorWidget::mqInternals
{
public:
  Ui::ColorOpacityEditorWidget Ui;
    
 
  vtkNew<vtkEventQtSlotConnect> IndexedLookupConnector;
  vtkNew<vtkEventQtSlotConnect> RangeConnector;

  mqInternals(mqColorOpacityEditorWidget* self, int mapSurfaces) 
  {
	  cout << "mqInternals instantiation" << endl;
    this->Ui.setupUi(self);
	cout << "mqInternals instantiation : set validator" << endl;
    this->Ui.CurrentDataValue->setValidator(new QDoubleValidator(self));
    
	
	this->Ui.EnableOpacityMapping->setChecked(true);
	
	this->Ui.Discretize->setChecked(false);
	
	this->Ui.currentDiscretizeValue->setButtonSymbols(QAbstractSpinBox::NoButtons);
	this->Ui.currentDiscretizeValue->setMinimum(1);
	this->Ui.currentDiscretizeValue->setMaximum(1024);
	this->Ui.currentDiscretizeValue->setValue(256);
	this->Ui.currentDiscretizeValue->setEnabled(false);
	this->Ui.discretizeSlider->setMinimum(1);
	this->Ui.discretizeSlider->setMaximum(1024);
	this->Ui.discretizeSlider->setValue(256);
	this->Ui.discretizeSlider->setEnabled(false);
	if (mapSurfaces == 0)
	{
		this->Ui.EnableOpacityMapping->setVisible(false);
		this->Ui.Discretize->setVisible(false);
		this->Ui.currentDiscretizeValue->setVisible(false);
		this->Ui.discretizeSlider->setVisible(false);
		this->Ui.lDiscretizeSlider->setVisible(false);

	}
	
	this->Ui.currentMin->setButtonSymbols(QAbstractSpinBox::NoButtons);
	this->Ui.currentMax->setButtonSymbols(QAbstractSpinBox::NoButtons);

	this->Ui.currentMin->setMinimum(-DBL_MAX);
	this->Ui.currentMax->setMinimum(-DBL_MAX);
	this->Ui.currentMax->setValue(1);
	this->Ui.currentMin->setValue(0);
	this->Ui.currentMin->setMaximum(DBL_MAX);
	this->Ui.currentMax->setMaximum(DBL_MAX);
	
	cout << "mqInternals instantiation : done" << endl;

  }

  void render()
  {
	  cout << "mqColorOpacityEditorWidget render" << endl;
	 
    mqMorphoDigCore::instance()->Render();
  }
};

//void mqColorOpacityEditorWidget::reInitializeHIST(vtkImageAccumulate *hist, vtkImageData *data)
void mqColorOpacityEditorWidget::reInitializeHIST(vtkImageAccumulate *hist)
{
	this->HIST = hist;
	this->Internals->Ui.Histogram->initialize(hist);
	this->UpdateHistogram();
	//this->IMGDATA = data;
	cout << "Histogram reinitialized!" << endl;	
	/*this->HIST->SetComponentExtent(this->ctfMin, this->ctfMax, 0, 0, 0, 0);
	*/
	/*this->HIST->SetComponentOrigin(this->ctfMin, 0, 0);
	double bin_spacing = (double)(this->ctfMax - this->ctfMin) / 10;
	this->HIST ->SetComponentSpacing(bin_spacing, 0, 0);
	this->HIST->Update();*/
	/*this->Internals->Ui.Histogram->SetMinMax(this->ctfMin , this->ctfMax);
	this->Internals->Ui.Histogram->initialize(hist);*/
}
void mqColorOpacityEditorWidget::reInitialize(vtkDiscretizableColorTransferFunction *stc, int keepMinMax)
{
	this->STC = stc;
	if (stc != NULL)
	{
		if (keepMinMax == 0)
		{
			
			this->ctfMin = this->getSTCMin();
			this->ctfMax = this->getSTCMax();
			this->Internals->Ui.currentMin->setValue(this->ctfMin);
			this->Internals->Ui.currentMax->setValue(this->ctfMax);
		}
		else
		{
			this->UpdateLookupTableRange();
		}
		cout << "reinitialize: min=" << this->ctfMin << endl;
		cout << "reinitialize: max=" << this->ctfMax << endl;
		
		if (stc->GetEnableOpacityMapping()) { this->Internals->Ui.EnableOpacityMapping->setChecked(true); }		
		else { this->Internals->Ui.EnableOpacityMapping->setChecked(false); }
		this->Internals->Ui.discretizeSlider->setValue(this->STC->GetNumberOfValues());
		this->Internals->Ui.currentDiscretizeValue->setValue(this->STC->GetNumberOfValues());
		if (stc->GetDiscretize()) { 
			this->Internals->Ui.Discretize->setChecked(true); 
			this->Internals->Ui.discretizeSlider->setDisabled(false);
			this->Internals->Ui.currentDiscretizeValue->setDisabled(false);
		}
		else{ 
			this->Internals->Ui.Discretize->setChecked(false); 
			this->Internals->Ui.discretizeSlider->setDisabled(true);
			this->Internals->Ui.currentDiscretizeValue->setDisabled(true);
		}

		this->Internals->Ui.ColorEditor->initialize(stc, true, NULL, false);
		this->initializeOpacityEditor(stc->GetScalarOpacityFunction());
		cout << "reinitialize: updateCurrentData... " << endl;
		this->updateCurrentData();
		cout << "reinitialize: updatePanel... " << endl;
		this->updatePanel();
	}
	
}

//<mqColorOpacityEditorWidget(vtkDiscretizableColorTransferFunction* stc, vtkImageAccumulate *hist, QWidget* parent = 0, int mapSurfaces = 1);>

//-----------------------------------------------------------------------------
mqColorOpacityEditorWidget::mqColorOpacityEditorWidget(
	vtkDiscretizableColorTransferFunction *stc, QWidget* parentObject, int mapSurfaces)
  : Superclass(parentObject)
  , Internals(new mqInternals(this, mapSurfaces))
{
	this->HIST = NULL;
  Ui::ColorOpacityEditorWidget& ui = this->Internals->Ui;
  this->STC = stc;
  this->_mapSurfaces = mapSurfaces;

  if (stc!=NULL)
  {
	  cout << "Initialize ColorEditor widget. " << endl;
    ui.ColorEditor->initialize(stc, true, NULL, false);
   
	cout << "Initialize OpacityEditor widget " << endl;
	this->initializeOpacityEditor(stc->GetScalarOpacityFunction());
	this->ctfMin = this->getSTCMin();
	this->ctfMax = this->getSTCMax();
  }
  else
  {
	  cout << "STC is not null, that is something... " << endl;

  }
  

  QObject::connect(ui.OpacityEditor, SIGNAL(currentPointChanged(vtkIdType)), this,
    SLOT(opacityCurrentChanged(vtkIdType)));
  QObject::connect(ui.ColorEditor, SIGNAL(currentPointChanged(vtkIdType)), this,
    SLOT(colorCurrentChanged(vtkIdType)));

  QObject::connect(
    ui.ColorEditor, SIGNAL(controlPointsModified()), this, SIGNAL(xrgbPointsChanged()));
  QObject::connect(
    ui.OpacityEditor, SIGNAL(controlPointsModified()), this, SIGNAL(xvmsPointsChanged()));

  QObject::connect(
    ui.ColorEditor, SIGNAL(controlPointsModified()), this, SLOT(updateCurrentData()));
  QObject::connect(
    ui.OpacityEditor, SIGNAL(controlPointsModified()), this, SLOT(updateCurrentData()));

  QObject::connect(ui.ResetRangeToData, SIGNAL(clicked()), this, SLOT(resetRangeToData()));
  QObject::connect(ui.InvertRGB, SIGNAL(clicked()), this, SLOT(invertRGB()));
  QObject::connect(ui.InvertOpacity, SIGNAL(clicked()), this, SLOT(invertOpacity()));

  QObject::connect(ui.EnableOpacityMapping, SIGNAL(clicked()), this, SLOT(changedEnableOpacity()));
  QObject::connect(ui.Discretize, SIGNAL(clicked()), this, SLOT(changeDiscretize()));
//  QObject::connect(ui.Discretize, SIGNAL(clicked()), this, SLOT(changedDiscretize()));
  
  QObject::connect(ui.discretizeSlider, SIGNAL(valueChanged(int)), this, SLOT(changedDiscretizeValue(int)));
  QObject::connect(ui.discretizeSlider, SIGNAL(valueChanged(int)), ui.currentDiscretizeValue, SLOT(setValue(int)));
  QObject::connect(ui.currentDiscretizeValue, SIGNAL(valueChanged(int)), this, SLOT(changedDiscretizeValue(int)));
  QObject::connect(ui.currentDiscretizeValue, SIGNAL(valueChanged(int)), ui.discretizeSlider, SLOT(setValue(int)));

 
  QObject::connect(ui.SaveAsCustom, SIGNAL(clicked()), this, SLOT(saveAsCustom()));

  QObject::connect(ui.sliderMin, SIGNAL(valueChanged(int)), this, SLOT(slotSlideMin(int)));
  QObject::connect(ui.sliderMin, SIGNAL(sliderPressed()), this, SLOT(slotSliderStart()));
  QObject::connect(ui.sliderMin, SIGNAL(sliderReleased()), this, SLOT(slotSliderStop()));
  

  QObject::connect(ui.sliderMax, SIGNAL(valueChanged(int)), this, SLOT(slotSlideMax(int)));  
  QObject::connect(ui.sliderMax, SIGNAL(sliderPressed()), this, SLOT(slotSliderStart()));
  QObject::connect(ui.sliderMax, SIGNAL(sliderReleased()), this, SLOT(slotSliderStop()));

  QObject::connect(ui.sliderShift, SIGNAL(valueChanged(int)), this, SLOT(slotShiftSlider(int)));
  QObject::connect(ui.sliderShift, SIGNAL(sliderPressed()), this, SLOT(slotSliderStart()));
  QObject::connect(ui.sliderShift, SIGNAL(sliderReleased()), this, SLOT(slotSliderStop()));
  


  QObject::connect(ui.currentMin, SIGNAL(editingFinished()), this, SLOT(slotCurrentMinEdited()));
  QObject::connect(ui.currentMax, SIGNAL(editingFinished()), this, SLOT(slotCurrentMaxEdited()));
  
  

 // this->connect(
  //  ui.UseLogScaleOpacity, SIGNAL(clicked(bool)), SLOT(useLogScaleOpacityClicked(bool)));
  // if the user edits the "DataValue", we need to update the transfer function.
  QObject::connect(
    ui.CurrentDataValue, SIGNAL(textChangedAndEditingFinished()), this, SLOT(currentDataEdited()));

  if (mapSurfaces == 1)
  {
	  QAction* exportAction = new QAction(tr("&Export"), this);
	  exportAction->setToolTip(tr("Export color map"));
	  ui.exportColorMap->addAction(exportAction);
	  ui.exportColorMap->setDefaultAction(exportAction);
	  QIcon icon;
	  icon.addFile(QStringLiteral(":/Icons/ExportMap22.png"), QSize(), QIcon::Normal, QIcon::Off);
	  exportAction->setIcon(icon);
	  new mqSaveMAPDialogReaction(exportAction);
  }
  else
  {
	  // dans ce cas on n'exporte que celle active
	  QObject::connect(ui.exportColorMap, SIGNAL(clicked()), this, SLOT(slotExportCTF()));
  }

  //cout << "Will call updateCurrentData... " << endl;
  this->updateCurrentData();
  //cout << "Will call updatePanel... " << endl;
  this->updatePanel();
}

vtkDiscretizableColorTransferFunction* mqColorOpacityEditorWidget::getSTC()
{
	return this->STC;
}
int mqColorOpacityEditorWidget::hasSTC()
{
	if (this->STC != NULL) { return 1; }
	else { return 0; }
}



//-----------------------------------------------------------------------------
mqColorOpacityEditorWidget::~mqColorOpacityEditorWidget()
{
  //pqSettings* settings = pqApplicationCore::instance()->settings();
  /*if (settings)
  {
    // save the state of the advanced button in the widget
    settings->setValue("showAdvancedPropertiesColorOpacityEditorWidget",
      this->Internals->Ui.AdvancedButton->isChecked());
  }*/

  delete this->Internals;
  this->Internals = NULL;
}

void mqColorOpacityEditorWidget::slotCurrentMinEdited()
{
	//cout << "Current Min edited!" << endl;
	if (this->Internals->Ui.currentMin->value() < this->Internals->Ui.currentMax->value())
	{
		this->ctfMin = this->Internals->Ui.currentMin->value();
	}
	else
	{
		this->ctfMin = this->Internals->Ui.currentMax->value() - 10;
	}
	
	this->UpdateLookupTableRange();
	this->UpdateHistogram();
	emit minmaxChanged();
}

void mqColorOpacityEditorWidget::slotCurrentMaxEdited()
{
	//cout << "Current max edited!" << endl;
	if (this->Internals->Ui.currentMin->value() < this->Internals->Ui.currentMax->value())
	{
		this->ctfMax = this->Internals->Ui.currentMax->value();
	}
	else
	{
		this->ctfMax = this->Internals->Ui.currentMin->value() + 10;
	}

	


	this->UpdateLookupTableRange();
	this->UpdateHistogram();
	emit minmaxChanged();

}

void mqColorOpacityEditorWidget::slotSliderStart()
{
	cout << "ShiftSliderStart" << endl;
	this->slideMin = this->ctfMin;
	this->slideMax = this->ctfMax;
	this->maxShiftAmplitude = (this->slideMax - this->slideMin) / 2;
	emit shiftOrSlideStarted();
}

void mqColorOpacityEditorWidget::slotSlideMin(int slideMin)
{
	if (slideMin != 0)
	{
		cout << "slideMin:" << slideMin << endl;
		double newMin = this->slideMin + slideMin * this->maxShiftAmplitude / 100;
		this->Internals->Ui.currentMin->setValue(newMin);

		cout << "Min:" << newMin << endl;
		this->ctfMin = newMin;
		
		this->UpdateLookupTableRange();
		this->UpdateHistogram();
		
	}

	
}
void mqColorOpacityEditorWidget::slotSliderStop()
{
	emit shiftOrSlideStopped();

}
void mqColorOpacityEditorWidget::slotSlideMax(int slideMax)
{
	if (slideMax != 0)
	{
		cout << "slideMax:" << slideMax << endl;
		double newMax = this->slideMax + slideMax * this->maxShiftAmplitude / 100;

		this->Internals->Ui.currentMax->setValue(newMax);
		cout << "Max:" << newMax << endl;

		this->ctfMax = newMax;
		
		this->UpdateLookupTableRange();
		this->UpdateHistogram();
		
	}

}
void mqColorOpacityEditorWidget::slotShiftSlider(int shift)
{
	if (shift != 0)
	{
		cout << "shift:" << shift << endl;
		double newMin = this->slideMin + shift * this->maxShiftAmplitude / 100;
		double newMax = this->slideMax + shift * this->maxShiftAmplitude / 100;
		this->Internals->Ui.currentMin->setValue(newMin);
		this->Internals->Ui.currentMax->setValue(newMax);
		//cout << "Min:" << newMin << endl;
		//cout << "Max:" << newMax << endl;
		this->ctfMin = newMin;
		this->ctfMax = newMax;
		
		this->UpdateLookupTableRange();
		this->UpdateHistogram();
		
	}
}

double mqColorOpacityEditorWidget::getSTCMax() {
	
	if (this->STC != NULL)
	{
		
		double *pts = this->STC->GetDataPointer();

		int numnodes = this->STC->GetSize();
		double old_max = -DBL_MAX;
		for (int j = 0; j < numnodes; j++)
		{
			double curr = pts[4 * j];
			if (curr > old_max) { old_max = curr; }

		}

		vtkPiecewiseFunction* OF = this->STC->GetScalarOpacityFunction();
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
	else
	{
		return 1;
	}
}
double mqColorOpacityEditorWidget::getSTCMin()
{
	if (this->STC != NULL)
	{

	
		double *pts = this->STC->GetDataPointer();

		int numnodes = this->STC->GetSize();
		double old_min = DBL_MAX;

		for (int j = 0; j < numnodes; j++)
		{
			double curr = pts[4 * j];
			//cout << "x" << j << "=" << curr << endl;
			if (curr < old_min) { old_min = curr; }

		}

		vtkPiecewiseFunction* OF = this->STC->GetScalarOpacityFunction();
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
	else
	{
		return 0;
	}

}
void mqColorOpacityEditorWidget::SetCTFMin(double newMin)
{
	if (newMin < this->ctfMax)
	{
		this->ctfMin = newMin;
		this->UpdateLookupTableRange();
		this->UpdateHistogram();
		this->Internals->Ui.currentMin->setValue(newMin);
	}
}
void mqColorOpacityEditorWidget::SetCTFMax(double newMax)
{
	if (newMax > this->ctfMin)
	{
		this->ctfMax = newMax;
		this->UpdateLookupTableRange();
		this->UpdateHistogram();
		this->Internals->Ui.currentMax->setValue(newMax);
	}
}
void mqColorOpacityEditorWidget::UpdateHistogram()
{
	if (this->HIST != NULL)
	{
		/*this->HIST->SetComponentOrigin(this->ctfMin, 0, 0);
		double bin_spacing = (double)(this->ctfMax - this->ctfMin) / 10;
		this->HIST->SetComponentSpacing(bin_spacing, 0, 0);
		this->HIST->Update();
		this->Internals->Ui.Histogram->initialize(this->HIST);*/
		this->Internals->Ui.Histogram->SetMinMax(this->ctfMin, this->ctfMax);
	}
	else
	{
		cout << "This HIST is NULL!" << endl;
	}
}

void mqColorOpacityEditorWidget::UpdateLookupTableRange()
{
	
	if (this->STC != NULL)
	{
		cout << "UpdateLookupTableRange inside ColorOpacity widget" << endl;


		double *pts = STC->GetDataPointer();

		int numnodes = STC->GetSize();
		double old_min = DBL_MAX;
		double old_max = -DBL_MAX;
		for (int j = 0; j < numnodes; j++)
		{
			double curr = pts[4 * j];
			//cout << "x" << j << "=" << curr << endl;
			if (curr < old_min) { old_min = curr; }
			if (curr > old_max) { old_max = curr; }

		}
		//cout << "old max:" << old_max << ", old min:" << old_min << endl;
		if (old_max > old_min)
		{
			double old_range = old_max - old_min;
			double new_range = this->ctfMax - this->ctfMin;
			double mult = new_range / old_range;
			double c = this->ctfMin - old_min * mult;
			for (int k = 0; k < numnodes; k++)
			{
				pts[4 * k] = pts[4 * k] * mult + c;
				//cout << "nx" << k << "=" << pts[4 * k] << endl;
			}
			STC->FillFromDataPointer(numnodes, pts);

		}
		vtkPiecewiseFunction* OF = STC->GetScalarOpacityFunction();
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

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void mqColorOpacityEditorWidget::initializeOpacityEditor(vtkPiecewiseFunction* pwf)
{
  Ui::ColorOpacityEditorWidget& ui = this->Internals->Ui;
  //pqDataRepresentation* repr = pqActiveObjects::instance().activeRepresentation();
  //vtkSMPVRepresentationProxy* proxy = static_cast<vtkSMPVRepresentationProxy*>(repr->getProxy());
  /*vtkScalarsToColors* stc = nullptr;
  vtkSMProperty* separateProperty = proxy->GetProperty("UseSeparateColorMap");
  bool sepEnabled = vtkSMPropertyHelper(separateProperty).GetAsInt() != 0;
  if (!proxy->GetVolumeIndependentRanges() || !sepEnabled)
  {
    stc = vtkScalarsToColors::SafeDownCast(this->proxy()->GetClientSideObject());
  }*/
  cout << "mqColorOpacityEditorWidget initializeOpacityEditor" << endl;
  ui.OpacityEditor->initialize(this->STC, false, pwf, true);
 // ui.OpacityEditor->initialize(NULL, false, pwf, true);
}

//-----------------------------------------------------------------------------
void mqColorOpacityEditorWidget::opacityCurrentChanged(vtkIdType index)
{
	cout << "mqColorOpacityEditorWidget opacityCurrentChanged" << endl;
  if (index != -1)
  {
    Ui::ColorOpacityEditorWidget& ui = this->Internals->Ui;
    ui.ColorEditor->setCurrentPoint(-1);
  }
  this->updateCurrentData();
}

//-----------------------------------------------------------------------------
void mqColorOpacityEditorWidget::colorCurrentChanged(vtkIdType index)
{
	cout << "mqColorOpacityEditorWidget colorCurrentChanged" << endl;
  if (index != -1)
  {
    Ui::ColorOpacityEditorWidget& ui = this->Internals->Ui;
    ui.OpacityEditor->setCurrentPoint(-1);
  }
  this->updateCurrentData();
}

//-----------------------------------------------------------------------------
void mqColorOpacityEditorWidget::updatePanel()
{
	cout << "mqColorOpacityEditorWidget updatePanel" << endl;
  if (this->Internals)
  {
    /*bool advancedVisible = this->Internals->Ui.AdvancedButton->isChecked();
    this->Internals->Ui.ColorLabel->setVisible(advancedVisible);
    this->Internals->Ui.ColorTable->setVisible(advancedVisible);
    this->Internals->Ui.OpacityLabel->setVisible(advancedVisible);
    this->Internals->Ui.OpacityTable->setVisible(advancedVisible);*/
  }
}

//-----------------------------------------------------------------------------
void mqColorOpacityEditorWidget::updateCurrentData()
{
	cout << "mqColorOpacityEditorWidget updateCurrentData" << endl;
  vtkDiscretizableColorTransferFunction* stc =
    vtkDiscretizableColorTransferFunction::SafeDownCast(this->STC);
  
  //vtkSMProxy* pwfProxy = this->scalarOpacityFunctionProxy();
  //vtkPiecewiseFunction* pwf = NULL;
  //èif (stc !=NULL)
  //vtkPiecewiseFunction* pwf = vtkPiecewiseFunction::SafeDownCast(this->STC); 
  vtkPiecewiseFunction* pwf = this->STC->GetScalarOpacityFunction(); 
    //pwfProxy ? vtkPiecewiseFunction::SafeDownCast(pwfProxy->GetClientSideObject()) : NULL;
  
  Ui::ColorOpacityEditorWidget& ui = this->Internals->Ui;

  if (ui.ColorEditor->currentPoint() >= 0 && stc)
  {
	 // cout << "Case 1" << endl;
    double xrgbms[6];
    stc->GetNodeValue(ui.ColorEditor->currentPoint(), xrgbms);
    ui.CurrentDataValue->setText(QString::number(xrgbms[0]));

    // Don't enable widget for first/last control point. For those, users must
    // rescale the transfer function manually
    ui.CurrentDataValue->setEnabled(ui.ColorEditor->currentPoint() != 0 &&
      ui.ColorEditor->currentPoint() != (ui.ColorEditor->numberOfControlPoints() - 1));




  }
  else if (ui.OpacityEditor->currentPoint() >= 0 && pwf)
  {
	//  cout << "Case 2" << endl;
    double xvms[4];
    pwf->GetNodeValue(ui.OpacityEditor->currentPoint(), xvms);
    ui.CurrentDataValue->setText(QString::number(xvms[0]));

    // Don't enable widget for first/last control point. For those, users must
    // rescale the transfer function manually
    ui.CurrentDataValue->setEnabled(ui.OpacityEditor->currentPoint() != 0 &&
      ui.OpacityEditor->currentPoint() != (ui.OpacityEditor->numberOfControlPoints() - 1));
  }
  else
  {
	//  cout << "Case 3" << endl;
    ui.CurrentDataValue->setEnabled(false);
  }
  
}

//-----------------------------------------------------------------------------
QList<QVariant> mqColorOpacityEditorWidget::xrgbPoints() const
{
	cout << "mqColorOpacityEditorWidget xrgbPoints" << endl;
	
  vtkDiscretizableColorTransferFunction* stc =
    vtkDiscretizableColorTransferFunction::SafeDownCast(this->STC);
  QList<QVariant> values;
  for (int cc = 0; stc != NULL && cc < stc->GetSize(); cc++)
  {
    double xrgbms[6];
    stc->GetNodeValue(cc, xrgbms);
    vtkVector<double, 4> value;
    values.push_back(xrgbms[0]);
    values.push_back(xrgbms[1]);
    values.push_back(xrgbms[2]);
    values.push_back(xrgbms[3]);
  }

  return values;
}

//-----------------------------------------------------------------------------
QList<QVariant> mqColorOpacityEditorWidget::xvmsPoints() const
{
	cout << "mqColorOpacityEditorWidget xvmsPoints" << endl;
  //vtkSMProxy* pwfProxy = this->scalarOpacityFunctionProxy();
	vtkPiecewiseFunction* pwf = this->STC->GetScalarOpacityFunction();
    //pwfProxy ? vtkPiecewiseFunction::SafeDownCast(pwfProxy->GetClientSideObject()) : NULL;

  QList<QVariant> values;
  for (int cc = 0; pwf != NULL && cc < pwf->GetSize(); cc++)
  {
    double xvms[4];
    pwf->GetNodeValue(cc, xvms);
    values.push_back(xvms[0]);
    values.push_back(xvms[1]);
    values.push_back(xvms[2]);
    values.push_back(xvms[3]);
  }
  return values;
}

/*
//-----------------------------------------------------------------------------
bool pqColorOpacityEditorWidget::useLogScale() const
{
  return this->Internals->Ui.UseLogScale->isChecked();
}
*/

/*

//-----------------------------------------------------------------------------
bool pqColorOpacityEditorWidget::useLogScaleOpacity() const
{
  return this->Internals->Ui.UseLogScaleOpacity->isChecked();
}

*/

/*
//-----------------------------------------------------------------------------
void pqColorOpacityEditorWidget::setUseLogScale(bool val)
{
  Ui::ColorOpacityEditorWidget& ui = this->Internals->Ui;
  ui.UseLogScale->setChecked(val);
}
*/

/*
//-----------------------------------------------------------------------------
void pqColorOpacityEditorWidget::setUseLogScaleOpacity(bool val)
{
  Ui::ColorOpacityEditorWidget& ui = this->Internals->Ui;
  ui.UseLogScaleOpacity->setChecked(val);
}

*/

/*
//-----------------------------------------------------------------------------
void pqColorOpacityEditorWidget::useLogScaleClicked(bool log_space)
{
  if (log_space)
  {
    // Make sure both color and opacity are remapped if needed:
    this->prepareRangeForLogScaling();
    vtkSMTransferFunctionProxy::MapControlPointsToLogSpace(this->proxy());
  }
  else
  {
    vtkSMTransferFunctionProxy::MapControlPointsToLinearSpace(this->proxy());
  }

  this->Internals->Ui.ColorEditor->SetLogScaleXAxis(log_space);

  emit this->useLogScaleChanged();
}
*/

/*
//-----------------------------------------------------------------------------
void pqColorOpacityEditorWidget::useLogScaleOpacityClicked(bool log_space)
{
  vtkSMProxy* opacityProxy = this->Internals->ScalarOpacityFunctionProxy;
  if (log_space)
  {
    // Make sure both color and opacity are remapped if needed:
    this->prepareRangeForLogScaling();
    vtkSMTransferFunctionProxy::MapControlPointsToLogSpace(opacityProxy);
  }
  else
  {
    vtkSMTransferFunctionProxy::MapControlPointsToLinearSpace(opacityProxy);
  }

  this->Internals->Ui.OpacityEditor->SetLogScaleXAxis(log_space);

  emit this->useLogScaleOpacityChanged();
}
*/

//-----------------------------------------------------------------------------
void mqColorOpacityEditorWidget::setXvmsPoints(const QList<QVariant>& values)
{
  Q_UNUSED(values);
  // Since the vtkPiecewiseFunction connected to the widget is directly obtained
  // from the proxy, we don't need to do anything here. The widget will be
  // updated when the proxy updates.
}

//-----------------------------------------------------------------------------
void mqColorOpacityEditorWidget::setXrgbPoints(const QList<QVariant>& values)
{
  Q_UNUSED(values);
  // Since the vtkColorTransferFunction connected to the widget is directly obtained
  // from the proxy, we don't need to do anything here. The widget will be
  // updated when the proxy updates.
}

//-----------------------------------------------------------------------------

void mqColorOpacityEditorWidget::currentDataEdited()
{
	cout << "mqColorOpacityEditorWidget currentDataEdited" << endl;
  vtkDiscretizableColorTransferFunction* stc =
    vtkDiscretizableColorTransferFunction::SafeDownCast(this->STC);
  //vtkSMProxy* pwfProxy = this->scalarOpacityFunctionProxy();
  vtkPiecewiseFunction* pwf = this->STC->GetScalarOpacityFunction();
    //pwfProxy ? vtkPiecewiseFunction::SafeDownCast(pwfProxy->GetClientSideObject()) : NULL;

  Ui::ColorOpacityEditorWidget& ui = this->Internals->Ui;
  if (ui.ColorEditor->currentPoint() >= 0 && stc)
  {
    ui.ColorEditor->setCurrentPointPosition(ui.CurrentDataValue->text().toDouble());
  }
  else if (ui.OpacityEditor->currentPoint() >= 0 && pwf)
  {
    ui.OpacityEditor->setCurrentPointPosition(ui.CurrentDataValue->text().toDouble());
  }

  this->updateCurrentData();
}

//-----------------------------------------------------------------------------
/*void pqColorOpacityEditorWidget::representationOrViewChanged()
{
  pqDataRepresentation* repr = pqActiveObjects::instance().activeRepresentation();
  bool hasRepresentation = repr != NULL;
  pqView* activeView = pqActiveObjects::instance().activeView();
  bool hasView = activeView != NULL;

  Ui::ColorOpacityEditorWidget& ui = this->Internals->Ui;
  ui.ResetRangeToData->setEnabled(hasRepresentation);
  ui.ResetRangeToDataOverTime->setEnabled(hasRepresentation);
  ui.ResetRangeToVisibleData->setEnabled(hasRepresentation && hasView);

  vtkSMProxy* pwfProxy = this->scalarOpacityFunctionProxy();
  vtkPiecewiseFunction* pwf =
    pwfProxy ? vtkPiecewiseFunction::SafeDownCast(pwfProxy->GetClientSideObject()) : nullptr;

  // When representation changes, we have to initialize the opacity widget when
  // "MultiComponentsMapping" is modified
  this->Internals->RangeConnector->Disconnect();
  vtkSMProperty* msProp = repr->getProxy()->GetProperty("MapScalars");
  vtkSMProperty* mcmProp = repr->getProxy()->GetProperty("MultiComponentsMapping");
  if (msProp && mcmProp)
  {
    this->Internals->RangeConnector->Connect(msProp, vtkCommand::ModifiedEvent, this,
      SLOT(multiComponentsMappingChanged(vtkObject*, unsigned long, void*, void*)), pwf);

    this->Internals->RangeConnector->Connect(mcmProp, vtkCommand::ModifiedEvent, this,
      SLOT(multiComponentsMappingChanged(vtkObject*, unsigned long, void*, void*)), pwf);

    this->initializeOpacityEditor(pwf);
  }
}*/

//-----------------------------------------------------------------------------


void mqColorOpacityEditorWidget::invertRGB()
{
	if (this->STC != NULL)
	{
		mqMorphoDigCore::instance()->invertRGB(this->STC);
		this->reInitialize(STC);
	}
}
void mqColorOpacityEditorWidget::invertOpacity()
{
	if (this->STC != NULL)
	{
		mqMorphoDigCore::instance()->invertOpacity(this->STC);
		this->reInitialize(STC);
	}
}

//-----------------------------------------------------------------------------
void mqColorOpacityEditorWidget::resetRangeToData()
{
	//this->Ui->suggestedMax->setValue(mqMorphoDigCore::instance()->GetSuggestedScalarRangeMax());
	//this->Ui->suggestedMin->setValue(mqMorphoDigCore::instance()->GetSuggestedScalarRangeMin());
  // passing in NULL ensure pqResetScalarRangeReaction simply uses active representation.
  //if (pqResetScalarRangeReaction::resetScalarRangeToData(NULL))
  //{
   // this->Internals->render();
	double min; double max;
	min = DBL_MAX;
	max = -DBL_MAX;

	if (this->_mapSurfaces == 1)
	{
		//mqMorphoDigCore::instance()->UpdateLookupTablesToData();
		//dirty: update all lookup tables...
		// would be better to get min, max and only update current lookup table

		min = mqMorphoDigCore::instance()->GetScalarRangeMin();
		
		max = mqMorphoDigCore::instance()->GetScalarRangeMax();
	}
	else
	{
		min = mqMorphoDigCore::instance()->GetVolumeRangeMin();

		max = mqMorphoDigCore::instance()->GetVolumeRangeMax();

	}
	cout << "min=" << min << endl;
		cout << "max=" << max << endl;
	if (min <DBL_MAX && max>-DBL_MAX && min < max)
	{
		this->ctfMin = min;
		this->ctfMax = max;
		this->Internals->Ui.currentMin->setValue(min);
		this->Internals->Ui.currentMax->setValue(max);
		this->UpdateLookupTableRange();
		this->UpdateHistogram();
	}
	

    emit this->changeFinished();
  //}
}

void mqColorOpacityEditorWidget::changeDiscretize() 
{
	if (this->STC != NULL)
	{
		if (this->Internals->Ui.Discretize->isChecked()) {
			this->Internals->Ui.Discretize->setChecked(true);
			this->STC->DiscretizeOn();
			cout << "Discretize is on and STC has " << STC->GetNumberOfValues() << endl;
			this->Internals->Ui.discretizeSlider->setDisabled(false);
			this->Internals->Ui.currentDiscretizeValue->setDisabled(false);
		}
		else {
			this->STC->DiscretizeOff();
			
			this->Internals->Ui.discretizeSlider->setDisabled(true);
			this->Internals->Ui.currentDiscretizeValue->setDisabled(true);
		}
	}
}
void  mqColorOpacityEditorWidget::changedDiscretizeValue(int value)
{
	//this->Internals->Ui.currentDiscretizeValue->setValue(value);
	if (this->STC != NULL)
	{
		this->STC->SetNumberOfValues(value);
	}
}
void mqColorOpacityEditorWidget::changedEnableOpacity()
{
	cout << "change EnableOpacity" << endl;
	if (this->STC != NULL)
	{
		if (this->Internals->Ui.EnableOpacityMapping->isChecked())
		{
			this->STC->EnableOpacityMappingOn();
		}
		else
		{
			this->STC->EnableOpacityMappingOff();
		}
	}
	


}

//-----------------------------------------------------------------------------

/*
void pqColorOpacityEditorWidget::resetRangeToDataOverTime()
{
  // passing in NULL ensure pqResetScalarRangeReaction simply uses active representation.
  if (pqResetScalarRangeReaction::resetScalarRangeToDataOverTime(NULL))
  {
    this->Internals->render();
    emit this->changeFinished();
  }
}*/

/*
//-----------------------------------------------------------------------------
void pqColorOpacityEditorWidget::resetRangeToVisibleData()
{
  pqPipelineRepresentation* repr =
    qobject_cast<pqPipelineRepresentation*>(pqActiveObjects::instance().activeRepresentation());
  if (!repr)
  {
    qCritical() << "No active representation.";
    return;
  }

  vtkSMPVRepresentationProxy* repProxy = vtkSMPVRepresentationProxy::SafeDownCast(repr->getProxy());
  if (!repProxy)
  {
    return;
  }

  pqView* activeView = pqActiveObjects::instance().activeView();
  if (!activeView)
  {
    qCritical() << "No active view.";
    return;
  }

  vtkSMRenderViewProxy* rvproxy = vtkSMRenderViewProxy::SafeDownCast(activeView->getViewProxy());
  if (!rvproxy)
  {
    return;
  }

  BEGIN_UNDO_SET("Reset transfer function ranges using visible data");
  vtkSMPVRepresentationProxy::RescaleTransferFunctionToVisibleRange(repProxy, rvproxy);
  this->Internals->render();
  END_UNDO_SET();
}

*/
/*
//-----------------------------------------------------------------------------
void pqColorOpacityEditorWidget::resetRangeToCustom()
{
  if (pqResetScalarRangeReaction::resetScalarRangeToCustom(this->proxy()))
  {
    this->Internals->render();
    emit this->changeFinished();
  }
}
*/
//-------
/*
----------------------------------------------------------------------
void pqColorOpacityEditorWidget::invertTransferFunctions()
{
  BEGIN_UNDO_SET("Invert transfer function");
  vtkSMTransferFunctionProxy::InvertTransferFunction(this->proxy());

  emit this->changeFinished();
  // We don't invert the opacity function, for now.
  END_UNDO_SET();
}*/

/*
//-----------------------------------------------------------------------------
void pqColorOpacityEditorWidget::choosePreset(const char* presetName)
{
  QAction* tmp = new QAction(NULL);
  pqChooseColorPresetReaction* ccpr = new pqChooseColorPresetReaction(tmp, false);
  ccpr->setTransferFunction(this->proxy());
  this->connect(ccpr, SIGNAL(presetApplied()), SLOT(presetApplied()));
  ccpr->choosePreset(presetName);
  delete ccpr;
  delete tmp;
}
*/
/*
//-----------------------------------------------------------------------------
void pqColorOpacityEditorWidget::presetApplied()
{
  emit this->changeFinished();

  // Assume the color map and opacity have changed and refresh
  this->Internals->ColorTableModel.refresh();
  this->Internals->OpacityTableModel.refresh();
}
*/
void mqColorOpacityEditorWidget::slotExportCTF()
{
	mqMorphoDigCore::instance()->SaveOneMAP(this->STC);
}
void mqColorOpacityEditorWidget::saveAsCustom()
{
	QInputDialog *giveNameDialog = new QInputDialog();
	bool dialogResult;
	QString newColormapName = giveNameDialog->getText(0, "Color map name", "Name:", QLineEdit::Normal,
		"Custom_color_map", &dialogResult);
	if (dialogResult)
	{
		
		cout << "color map given:" << newColormapName.toStdString() << endl;
		if (mqMorphoDigCore::instance()->colorMapNameAlreadyExists(newColormapName) == 1)
		{
			QMessageBox msgBox;
			msgBox.setText("Can't save custom map : name already exists.");
			msgBox.exec();
			return;
		}
		if (newColormapName.length()==0)
		{
			QMessageBox msgBox;
			msgBox.setText("Can't save custom map: name length =0.");
			msgBox.exec();
			return;
		}
		mqMorphoDigCore::instance()->createCustomColorMap(newColormapName, this->STC); 
		emit this->changeFinished();
		//this->UpdateUI();
	}
	else
	{
		cout << "cancel " << endl;
	}

}

/*

//-----------------------------------------------------------------------------
void pqColorOpacityEditorWidget::saveAsPreset()
{
  QDialog dialog(this);
  Ui::SavePresetOptions ui;
  ui.setupUi(&dialog);
  ui.saveOpacities->setEnabled(this->scalarOpacityFunctionProxy() != NULL);
  ui.saveOpacities->setChecked(ui.saveOpacities->isEnabled());
  ui.saveAnnotations->setVisible(false);

  // For now, let's not provide an option to not save colors. We'll need to fix
  // the pqPresetToPixmap to support rendering only opacities.
  ui.saveColors->setChecked(true);
  ui.saveColors->setEnabled(false);
  ui.saveColors->hide();

  if (dialog.exec() != QDialog::Accepted)
  {
    return;
  }

  Q_ASSERT(ui.saveColors->isChecked());
  Json::Value preset = vtkSMTransferFunctionProxy::GetStateAsPreset(this->proxy());

  if (ui.saveOpacities->isChecked())
  {
    Json::Value opacities =
      vtkSMTransferFunctionProxy::GetStateAsPreset(this->scalarOpacityFunctionProxy());
    if (opacities.isMember("Points"))
    {
      preset["Points"] = opacities["Points"];
    }
  }

  vtkStdString presetName;
  {
    // This scoping is necessary to ensure that the vtkSMTransferFunctionPresets
    // saves the new preset to the "settings" before the choosePreset dialog is
    // shown.
    vtkNew<vtkSMTransferFunctionPresets> presets;
    presetName = presets->AddUniquePreset(preset);
  }
  this->choosePreset(presetName);
}
*/