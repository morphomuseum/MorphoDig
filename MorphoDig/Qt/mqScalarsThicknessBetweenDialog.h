/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqScalarsThicknessBetweenDialog_h
#define _mqScalarsThicknessBetweenDialog_h


#include "vtkMDActor.h"
#include <QDialog>

namespace Ui
{
class mqScalarsThicknessBetweenDialog;
}




/**
* mqScalarsThicknessBetweenDialog is the dialog used by MorphoDig to save CUR files.
*/
class  mqScalarsThicknessBetweenDialog : public QDialog
{
  Q_OBJECT

public:
  mqScalarsThicknessBetweenDialog(QWidget* Parent);
  
  ~mqScalarsThicknessBetweenDialog();
  void editThickness();
  
  public slots:
  
  
  virtual void slotsmoothNormals();
  virtual void sloteditThickness();
  virtual void slotClose();
  virtual void slotProgressBar(int val);
  

protected:
	
private:
	
	vtkMDActor* impactedActor;
	vtkMDActor* observedActor;
  Q_DISABLE_COPY(mqScalarsThicknessBetweenDialog)
  Ui::mqScalarsThicknessBetweenDialog* const Ui;
 
  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h
