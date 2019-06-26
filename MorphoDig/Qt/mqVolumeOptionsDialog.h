/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqVolumeOptionsDialog_h
#define _mqVolumeOptionsDialog_h

#include "vtkLMActor.h"
#include "vtkLMActorCollection.h"

#include <QDialog>

namespace Ui
{
class mqVolumeOptionsDialog;
}




/**
* mqVolumeOptionsDialog manage several volume data options
*/
class  mqVolumeOptionsDialog : public QDialog
{
  Q_OBJECT

public:
  mqVolumeOptionsDialog(QWidget* Parent);
  
  ~mqVolumeOptionsDialog();
  void editVolumeOptions();
  
  public slots:
   
  virtual void slotEditVolumeOptions();
  virtual void slotReinitialize();
  

protected:
	
private:
	

  Q_DISABLE_COPY(mqVolumeOptionsDialog)
  Ui::mqVolumeOptionsDialog* const Ui;
 
  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h
