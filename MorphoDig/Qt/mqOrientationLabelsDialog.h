/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqOrientationLabelsDialog_h
#define _mqOrientationLabelsDialog_h

#include "vtkLMActor.h"
#include "vtkLMActorCollection.h"

#include <QDialog>

namespace Ui
{
class mqOrientationLabelsDialog;
}




/**
* mqOrientationLabelsDialog is the dialog used by MorphoDig to save CUR files.
*/
class  mqOrientationLabelsDialog : public QDialog
{
  Q_OBJECT

public:
  mqOrientationLabelsDialog(QWidget* Parent);
  
  ~mqOrientationLabelsDialog();
  void editOrientationLabels();
  
  public slots:
  
  
  
  virtual void sloteditOrientationLabels();
  virtual void slotReinitialize();
  

protected:
	
private:
	

  Q_DISABLE_COPY(mqOrientationLabelsDialog)
  Ui::mqOrientationLabelsDialog* const Ui;
 
  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h
