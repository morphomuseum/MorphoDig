/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqEditLMKDialog_h
#define _mqEditLMKDialog_h

#include "vtkLMActor.h"
#include "vtkLMActorCollection.h"
#include <vtkSmartPointer.h>
#include <QDialog>

namespace Ui
{
class mqEditLMKDialog;
}




/**
* mqEditLMKDialog is the dialog used by MorphoDig to save CUR files.
*/
class  mqEditLMKDialog : public QDialog
{
  Q_OBJECT

public:
  mqEditLMKDialog(QWidget* Parent);
  
  ~mqEditLMKDialog();
  void saveLMK();
  void GetFirstSelectedLandmark();
  void GetFirstLandmark();
  void GetNextLandmark();
  void GetPrecedingLandmark();
  void UpdateUI();
  int SomeThingHasChanged();
  int CurrentLMKInCollection();
  void RefreshDialog();
  public slots:
  
  virtual void slotRefreshDialog();
  virtual void slotGetPrecedingLandmark();
  virtual void slotGetNextLandmark();
  virtual void slotCamCenter();
  

protected:
	
private:
	

  Q_DISABLE_COPY(mqEditLMKDialog)
  Ui::mqEditLMKDialog* const Ui;
  vtkSmartPointer<vtkLMActor>LMK;
  //vtkLMActor *LMK;
  vtkLMActorCollection *LMK_Coll;
  int current_coll;

  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h
