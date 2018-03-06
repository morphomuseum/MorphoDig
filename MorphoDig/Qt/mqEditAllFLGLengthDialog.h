/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqEditAllFLGLengthDialog_h
#define _mqEditAllFLGLengthDialog_h

#include "vtkLMActor.h"
#include "vtkLMActorCollection.h"

#include <QDialog>

namespace Ui
{
class mqEditAllFLGLengthDialog;
}




/**
* mqEditAllFLGLengthDialog is the dialog used by MorphoDig to save CUR files.
*/
class  mqEditAllFLGLengthDialog : public QDialog
{
  Q_OBJECT

public:
  mqEditAllFLGLengthDialog(QWidget* Parent);
  
  ~mqEditAllFLGLengthDialog();
  void saveAllFLG();
  
  public slots:
  
  
  
  virtual void slotsaveAllFLG();
  

protected:
	
private:
	

  Q_DISABLE_COPY(mqEditAllFLGLengthDialog)
  Ui::mqEditAllFLGLengthDialog* const Ui;
 
  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h
