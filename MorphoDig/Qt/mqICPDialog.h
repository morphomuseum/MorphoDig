/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqICPDialog_h
#define _mqICPDialog_h


#include "vtkMDActor.h"
#include <QDialog>

namespace Ui
{
class mqICPDialog;
}




/**
* mqICPDialog : align two meshes
*/
class  mqICPDialog : public QDialog
{
  Q_OBJECT

public:
  mqICPDialog(QWidget* Parent);
  
  ~mqICPDialog();
  void launchICP();
  
  public slots:
  
  
  
  virtual void slotLaunchICP();
  virtual void slotClose();
  
  

protected:
	
private:
	
	vtkMDActor* impactedActor;
	vtkMDActor* observedActor;
  Q_DISABLE_COPY(mqICPDialog)
  Ui::mqICPDialog* const Ui;
 
  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h
