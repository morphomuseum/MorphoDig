/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqBooleanOperationDialog_h
#define _mqBooleanOperationDialog_h


#include "vtkMDActor.h"
#include <QDialog>

namespace Ui
{
class mqBooleanOperationDialog;
}




/**
* mqBooleanOperationDialog is the dialog used by MorphoDig to save CUR files.
*/
class  mqBooleanOperationDialog : public QDialog
{
  Q_OBJECT

public:
  mqBooleanOperationDialog(QWidget* Parent);
  
  ~mqBooleanOperationDialog();
  void editBooleanOperation();
  
  public slots:
  
  

  virtual void sloteditBooleanOperation();
  virtual void slotClose();

  

protected:
	
private:
	
	vtkMDActor* actorA;
	vtkMDActor* actorB;
  Q_DISABLE_COPY(mqBooleanOperationDialog)
  Ui::mqBooleanOperationDialog* const Ui;
 
  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h
