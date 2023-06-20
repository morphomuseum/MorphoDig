/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqC2SDialog_h
#define _mqC2SDialog_h


#include "vtkMDActor.h"
#include <QDialog>

namespace Ui
{
class mqC2SDialog;
}




/**
* mqC2SDialog : align two meshes
*/
class  mqC2SDialog : public QDialog
{
  Q_OBJECT

public:
  mqC2SDialog(QWidget* Parent);
  
  ~mqC2SDialog();
  void launchC2S();
  
  public slots:
  
  
  
  virtual void slotLaunchC2S();
  virtual void slotClose();
  virtual void slotArea();
  virtual void slotTangent();
  virtual void slotNormal();
  

protected:
	
private:
	
	vtkMDActor* impactedActor;
	vtkMDActor* observedActor;
  Q_DISABLE_COPY(mqC2SDialog)
  Ui::mqC2SDialog* const Ui;
 
  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h
