/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqShrinkWrapDialog_h
#define _mqShrinkWrapDialog_h


#include "vtkMDActor.h"
#include <QDialog>

namespace Ui
{
class mqShrinkWrapDialog;
}




/**
* mqShrinkWrapDialog is the dialog used by MorphoDig to save CUR files.
*/
class  mqShrinkWrapDialog : public QDialog
{
  Q_OBJECT

public:
  mqShrinkWrapDialog(QWidget* Parent);
  
  ~mqShrinkWrapDialog();
  void editShrinkWrap();
  
  public slots:
  
  

  virtual void sloteditShrinkWrap();
  virtual void slotClose();

  

protected:
	
private:
	
	vtkMDActor* impactedActor;
	vtkMDActor* observedActor;
  Q_DISABLE_COPY(mqShrinkWrapDialog)
  Ui::mqShrinkWrapDialog* const Ui;
 
  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h
