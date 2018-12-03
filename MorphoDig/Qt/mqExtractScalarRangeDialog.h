/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqExtractScalarRangeDialog_h
#define _mqExtractScalarRangeDialog_h

#include "vtkLMActor.h"
#include "vtkLMActorCollection.h"

#include <QDialog>

namespace Ui
{
class mqExtractScalarRangeDialog;
}




/**
* mqExtractScalarRangeDialog is the dialog used by MorphoDig to save landmark files.
*/
class  mqExtractScalarRangeDialog : public QDialog
{
  Q_OBJECT

public:
  mqExtractScalarRangeDialog(QWidget* Parent);
  
  ~mqExtractScalarRangeDialog();

  public slots:
  
  virtual void slotExtractScalarRange();
  
  virtual void slotRefreshSuggestedMinMax();
  

protected:
	
private:
	

  Q_DISABLE_COPY(mqExtractScalarRangeDialog)
  Ui::mqExtractScalarRangeDialog* const Ui;
  

  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h
