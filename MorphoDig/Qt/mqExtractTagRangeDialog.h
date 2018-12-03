/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqExtractTagRangeDialog_h
#define _mqExtractTagRangeDialog_h

#include "vtkLMActor.h"
#include "vtkLMActorCollection.h"

#include <QDialog>

namespace Ui
{
class mqExtractTagRangeDialog;
}




/**
* mqExtractTagRangeDialog is the dialog used by MorphoDig to save landmark files.
*/
class  mqExtractTagRangeDialog : public QDialog
{
  Q_OBJECT

public:
  mqExtractTagRangeDialog(QWidget* Parent);
  
  ~mqExtractTagRangeDialog();

  public slots:
  
  virtual void slotExtractTagRange();
  
  
  

protected:
	
private:
	

  Q_DISABLE_COPY(mqExtractTagRangeDialog)
  Ui::mqExtractTagRangeDialog* const Ui;
  

  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h
