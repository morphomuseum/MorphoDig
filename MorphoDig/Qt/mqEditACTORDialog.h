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
  void UpdateUI();
  int SomeThingHasChanged();
  int CurrentActorInCollection();
  void RefreshDialog();
  public slots:
  
  virtual void slotRefreshDialog();
  virtual void slotGetPrecedingActor();
  virtual void slotGetNextActor();
  virtual void slotsaveActor();
  virtual void slotReinitMatrix();
  virtual void slotapplyMatrixToAllSelectedActors();

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
