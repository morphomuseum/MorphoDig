/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqRendererOptionsDialog_h
#define _mqRendererOptionsDialog_h

#include "vtkLMActor.h"
#include "vtkLMActorCollection.h"

#include <QDialog>

namespace Ui
{
class mqRendererOptionsDialog;
}




/**
* mqRendererOptionsDialog manage several Renderer data options
*/
class  mqRendererOptionsDialog : public QDialog
{
  Q_OBJECT

public:
  mqRendererOptionsDialog(QWidget* Parent);
  
  ~mqRendererOptionsDialog();
  void editRendererOptions();
  
  public slots:
   
  virtual void slotEditRendererOptions();
  virtual void slotReinitialize();
  

protected:
	
private:
	

  Q_DISABLE_COPY(mqRendererOptionsDialog)
  Ui::mqRendererOptionsDialog* const Ui;
 
  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h
