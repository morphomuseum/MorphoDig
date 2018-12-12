/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqShrinkWrapIterativeDialog_h
#define _mqShrinkWrapIterativeDialog_h


#include "vtkMDActor.h"
#include <QDialog>

namespace Ui
{
class mqShrinkWrapIterativeDialog;
}




/**
* mqShrinkWrapIterativeDialog is the dialog used by MorphoDig to save CUR files.
*/
class  mqShrinkWrapIterativeDialog : public QDialog
{
  Q_OBJECT

public:
  mqShrinkWrapIterativeDialog(QWidget* Parent);
  
  ~mqShrinkWrapIterativeDialog();
  void editIterativeShrinkWrap();
  
  public slots:
  
  
  virtual void slotIterateUntilLimit();
  virtual void slotFixedIterationNumber();
  virtual void sloteditIterativeShrinkWrap();
  virtual void slotClose();
  virtual void slotObservedObjectChanged(int idx);
  virtual void slotProgressBar(int val);
  

protected:
	
private:
	
	vtkMDActor* impactedActor;
	vtkMDActor* observedActor;
  Q_DISABLE_COPY(mqShrinkWrapIterativeDialog)
  Ui::mqShrinkWrapIterativeDialog* const Ui;
 
  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h
