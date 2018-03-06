/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqEditAllFLGColorDialog_h
#define _mqEditAllFLGColorDialog_h

#include "vtkLMActor.h"
#include "vtkLMActorCollection.h"

#include <QDialog>

namespace Ui
{
class mqEditAllFLGColorDialog;
}




/**
* mqEditAllFLGColorDialog is the dialog used by MorphoDig to save CUR files.
*/
class  mqEditAllFLGColorDialog : public QDialog
{
  Q_OBJECT

public:
  mqEditAllFLGColorDialog(QWidget* Parent);
  
  ~mqEditAllFLGColorDialog();
  void saveAllFLG();
  
  public slots:
  
  
  
  virtual void slotsaveAllFLG();
  

protected:
	
private:
	

  Q_DISABLE_COPY(mqEditAllFLGColorDialog)
  Ui::mqEditAllFLGColorDialog* const Ui;
 
  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h
