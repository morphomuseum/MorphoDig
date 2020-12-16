/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqSelectSmallVolumesDialog_h
#define _mqSelectSmallVolumesDialog_h

#include "vtkLMActor.h"
#include "vtkLMActorCollection.h"

#include <QDialog>

namespace Ui
{
class mqSelectSmallVolumesDialog;
}




/**
* mqSelectSmallVolumesDialog is the dialog used by MorphoDig to select surface objects with small volumes
*/
class  mqSelectSmallVolumesDialog : public QDialog
{
  Q_OBJECT

public:
  mqSelectSmallVolumesDialog(QWidget* Parent);
  
  ~mqSelectSmallVolumesDialog();
  void SelectSmallVolumes();
  
  public slots:
  
  
  
  virtual void slotSelectSmallVolumes();
  

protected:
	
private:
	

  Q_DISABLE_COPY(mqSelectSmallVolumesDialog)
  Ui::mqSelectSmallVolumesDialog* const Ui;
 
  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h
