/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqScalarsDistanceDialog_h
#define _mqScalarsDistanceDialog_h


#include "vtkMDActor.h"
#include <QDialog>

namespace Ui
{
class mqScalarsDistanceDialog;
}




/**
* mqScalarsDistanceDialog : scalars, distance between the closest vertices of two already aligned meshes
*/
class  mqScalarsDistanceDialog : public QDialog
{
  Q_OBJECT

public:
  mqScalarsDistanceDialog(QWidget* Parent);
  
  ~mqScalarsDistanceDialog();
  void launchDistance();
  virtual void slotProgressBar(int val);

  public slots:
  
  
  
  virtual void slotLaunchDistance();
  virtual void slotClose();
  
  

protected:
	
private:
	
	vtkMDActor* impactedActor;
	vtkMDActor* observedActor;
  Q_DISABLE_COPY(mqScalarsDistanceDialog)
  Ui::mqScalarsDistanceDialog* const Ui;
 
  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h
