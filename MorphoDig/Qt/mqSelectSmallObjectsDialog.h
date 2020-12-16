/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqSelectSmallObjectsDialog_h
#define _mqSelectSmallObjectsDialog_h

#include "vtkLMActor.h"
#include "vtkLMActorCollection.h"

#include <QDialog>

namespace Ui
{
class mqSelectSmallObjectsDialog;
}




/**
* mqSelectSmallObjectsDialog is the dialog used by MorphoDig to save CUR files.
*/
class  mqSelectSmallObjectsDialog : public QDialog
{
  Q_OBJECT

public:
  mqSelectSmallObjectsDialog(QWidget* Parent);
  
  ~mqSelectSmallObjectsDialog();
  void SelectSmallObjects();
  
  public slots:
  
  
  
  virtual void slotSelectSmallObjects();
  

protected:
	
private:
	

  Q_DISABLE_COPY(mqSelectSmallObjectsDialog)
  Ui::mqSelectSmallObjectsDialog* const Ui;
 
  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h
