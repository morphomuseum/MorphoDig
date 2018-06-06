/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqEditTagsDialog_h
#define _mqEditTagsDialog_h

#include "vtkLMActor.h"
#include "vtkLMActorCollection.h"
#include <vtkSmartPointer.h>
#include <QDialog>
#include "mqColorOpacityEditorWidget.h"


namespace Ui
{
class mqEditTagsDialog;
}




/**
* mqEditTagsDialog is the dialog used by MorphoDig to edit Tags.
*/
class  mqEditTagsDialog : public QDialog
{
  Q_OBJECT

public:
  mqEditTagsDialog(QWidget* Parent);
  
  ~mqEditTagsDialog();
 
  void UpdateUI();
  
 void RefreshDialog();
 void RefreshComboTags();
 void RefreshComboTagMaps();

 
 
  public slots:
  virtual void slotEditTagMapName();
  virtual void slotDeleteTagMap();
  virtual void slotActiveTagsChanged(int idx);
  virtual void slotActiveTagMapChanged(int idx);
  virtual void slotRefreshDialog();
  virtual void slotRefreshComboTags();
  virtual void slotRemoveTags();
  virtual void slotRefreshTagMaps();
 
  
protected:
	
private:
	

  Q_DISABLE_COPY(mqEditTagsDialog)
  Ui::mqEditTagsDialog* const Ui;
 
 

  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h
