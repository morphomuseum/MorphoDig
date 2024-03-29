/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqEditACTORDialog_h
#define _mqEditACTORDialog_h

#include "vtkMDActor.h"
#include "vtkMDActorCollection.h"
#include <vtkSmartPointer.h>
#include <QDialog>

namespace Ui
{
class mqEditACTORDialog;
}




/**
* mqEditACTORDialog is the dialog used by MorphoDig to save CUR files.
*/
class  mqEditACTORDialog : public QDialog
{
  Q_OBJECT

public:
  mqEditACTORDialog(QWidget* Parent);
  
  ~mqEditACTORDialog();
  void saveActor();
  void GetFirstSelectedActor();
  void GetFirstActor();
  void GetNextActor();
  void GetPrecedingActor();
  void RefreshNumCellsNumPoints();
  void UpdateUI();
  int SomeThingHasChanged();
  int CurrentActorInCollection();
  void RefreshDialog();
  void ShowAndRefresh();
  public slots:
  
  virtual void slotRefreshDialog();
  virtual void slotGetPrecedingActor();
  virtual void slotGetNextActor();
  virtual void slotsaveActor();
  virtual void slotReinitMatrix();
  virtual void slotHardenTransform();
  virtual void slotRefreshMatrix();
  virtual void slotapplyMatrixToAllSelectedActors();
  virtual void slotapplyMatrixToAllActors();
  virtual void slotEditScalar();
  virtual void slotVectorScalar();
  virtual void slotDuplicateScalar();
  virtual void slotDeleteScalar();
  virtual void slotDisplaySpikesClicked(bool isChecked);
  virtual void slotAdjustSpikesClicked(bool isChecked);
  virtual void slotadjustSpikeScaleFactor(double factor);
  virtual void slotspikeMaskFactor(int factor);
  
  virtual void slotEnableROIClicked(bool isChecked);

  virtual void slotdisplayROIPressed();
  virtual void slotisVisibleClicked(bool isChecked);

protected:
	
private:
	

  Q_DISABLE_COPY(mqEditACTORDialog)
  Ui::mqEditACTORDialog* const Ui;
  vtkSmartPointer<vtkMDActor> ACTOR;
  //vtkMDActor *ACTOR;
  vtkMDActorCollection *ACTOR_Coll;
  int current_coll;

  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h
