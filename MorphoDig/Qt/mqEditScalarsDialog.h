/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqEditScalarsDialog_h
#define _mqEditScalarsDialog_h

#include "vtkLMActor.h"
#include "vtkLMActorCollection.h"
#include <vtkSmartPointer.h>
#include <QDialog>
#include "mqColorOpacityEditorWidget.h"


namespace Ui
{
class mqEditScalarsDialog;
}




/**
* mqEditScalarsDialog is the dialog used by MorphoDig to save CUR files.
*/
class  mqEditScalarsDialog : public QDialog
{
  Q_OBJECT

public:
  mqEditScalarsDialog(QWidget* Parent);
  
  ~mqEditScalarsDialog();
 
  void UpdateUI();
  int SomeThingHasChanged();
 void RefreshDialog();
 void RefreshComboScalars();
 void RefreshComboColorMaps();
 void RefreshSuggestedRange();
 void ActiveColorMapChanged(int idx);
 void reject();
 //void UpdateLookupTables();
  public slots:
  virtual void slotEditColorMapName();
  virtual void slotDeleteColorMap();
  virtual void slotReinitializeColorMap();
  virtual void slotActiveScalarChanged(int idx);
  virtual void slotActiveColorMapChanged(int idx);
  virtual void slotRefreshSuggestedRange();
  virtual void slotRefreshDialog();
  virtual void slotAccepted();
  virtual void slotRefreshComboScalars();
  virtual void slotAcceptSuggestedMin();
  virtual void slotAcceptSuggestedMax();
  
  /*virtual void slotCurrentMinEdited();
  virtual void slotCurrentMaxEdited();
  virtual void slotShiftSlider(int shift);
  virtual void slotSliderStart();
  virtual void slotSlideMin(int slideMin);
  virtual void slotSlideMax(int slideMax);
  virtual void slotSliderStop();
  */
  virtual void slotRemoveScalar();
  virtual void slotRefreshColorMaps();
  
  
 /* virtual void slotSliderMinValueChanged(int value);
  virtual void slotSliderMaxValueChanged(int value);*/
  
protected:
	
private:
	mqColorOpacityEditorWidget *mColorMap; 
	/*double maxShiftAmplitude;
	double slideMin;
	double slideMax;
	double ctfMin;
	double ctfMax;
	*/
  Q_DISABLE_COPY(mqEditScalarsDialog)
  Ui::mqEditScalarsDialog* const Ui;
 
 

  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h
