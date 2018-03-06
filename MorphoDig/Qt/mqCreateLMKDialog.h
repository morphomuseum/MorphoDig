/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqCreateLMKDialog_h
#define _mqCreateLMKDialog_h

#include "vtkLMActor.h"
#include "vtkLMActorCollection.h"

#include <QDialog>

namespace Ui
{
class mqCreateLMKDialog;
}




/**
* mqCreateLMKDialog is the dialog used by MorphoDig to save landmark files.
*/
class  mqCreateLMKDialog : public QDialog
{
  Q_OBJECT

public:
  mqCreateLMKDialog(QWidget* Parent);
  
  ~mqCreateLMKDialog();

  public slots:
  
  virtual void slotCreateLMK();
  
 
  

protected:
	
private:
	

  Q_DISABLE_COPY(mqCreateLMKDialog)
  Ui::mqCreateLMKDialog* const Ui;
  

  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h
