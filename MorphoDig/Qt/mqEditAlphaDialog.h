/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqEditAlphaDialog_h
#define _mqEditAlphaDialog_h

#include "vtkLMActor.h"
#include "vtkLMActorCollection.h"

#include <QDialog>

namespace Ui
{
class mqEditAlphaDialog;
}




/**
* mqEditAlphaDialog is the dialog used by MorphoDig to save CUR files.
*/
class  mqEditAlphaDialog : public QDialog
{
  Q_OBJECT

public:
  mqEditAlphaDialog(QWidget* Parent);
  
  ~mqEditAlphaDialog();
  void editAlpha();
  
  public slots:
  
  
  
  virtual void sloteditAlpha();
  

protected:
	
private:
	

  Q_DISABLE_COPY(mqEditAlphaDialog)
  Ui::mqEditAlphaDialog* const Ui;
 
  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h
