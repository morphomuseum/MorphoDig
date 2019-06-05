/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqMinimalDialog_h
#define _mqMinimalDialog_h


#include <QDialog>
#include "mqMinimalWidget.h"

namespace Ui
{
class mqMinimalDialog;
}




/**
* mqMinimalDialog is the dialog used to test troubleshootings with vtkQtOpenGL widgets... (used in edit scalar windows...)
*/
class  mqMinimalDialog : public QDialog
{
  Q_OBJECT

public:
  mqMinimalDialog(QWidget* Parent);
  
  ~mqMinimalDialog();
  void Minimal();
  void disconnect();
  void reconnect();
  void reject();
  public slots:
  
  
  
  virtual void slotMinimal();
  

protected:
	
private:
	mqMinimalWidget *mMinimal;

  Q_DISABLE_COPY(mqMinimalDialog)
  Ui::mqMinimalDialog* const Ui;
 
  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h
