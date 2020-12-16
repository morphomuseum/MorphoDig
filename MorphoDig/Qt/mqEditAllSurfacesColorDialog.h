/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqEditAllSurfacesColorDialog_h
#define _mqEditAllSurfacesColorDialog_h

#include "vtkLMActor.h"
#include "vtkLMActorCollection.h"

#include <QDialog>

namespace Ui
{
class mqEditAllSurfacesColorDialog;
}




/**
* mqEditAllSurfacesColorDialog is the dialog used by MorphoDig to save CUR files.
*/
class  mqEditAllSurfacesColorDialog : public QDialog
{
  Q_OBJECT

public:
  mqEditAllSurfacesColorDialog(QWidget* Parent);
  
  ~mqEditAllSurfacesColorDialog();
  void saveAllSurfaces();
  
  public slots:
  
  
  
  virtual void slotsaveAllSurfaces();
  

protected:
	
private:
	

  Q_DISABLE_COPY(mqEditAllSurfacesColorDialog)
  Ui::mqEditAllSurfacesColorDialog* const Ui;
 
  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h
