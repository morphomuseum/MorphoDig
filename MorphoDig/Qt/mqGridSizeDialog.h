/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqGridSizeDialog_h
#define _mqGridSizeDialog_h

#include "vtkLMActor.h"
#include "vtkLMActorCollection.h"

#include <QDialog>

namespace Ui
{
class mqGridSizeDialog;
}




/**
* mqGridSizeDialog is the dialog used by MorphoDig to save CUR files.
*/
class  mqGridSizeDialog : public QDialog
{
  Q_OBJECT

public:
  mqGridSizeDialog(QWidget* Parent);
  void Refresh();
  ~mqGridSizeDialog();
  void editGridSize();
  void adjustParallelScale();
  public slots:
  
  virtual void slotEditHundredPxSu();
  virtual void slotRefresh();
  virtual void slotUnitChanged();
  virtual void slotEditGridSize();
  virtual void slotReinitialize();
  

protected:
	
private:
	

  Q_DISABLE_COPY(mqGridSizeDialog)
  Ui::mqGridSizeDialog* const Ui;
 
  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h
