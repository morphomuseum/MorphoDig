/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqTPSDialog_h
#define _mqTPSDialog_h


#include <QDialog>

namespace Ui
{
class mqTPSDialog;
}




/**
* mqTPSDialog is the dialog used by MorphoDig to distort selected surfaces using a thin plate splines transformation
*/
class  mqTPSDialog : public QDialog
{
  Q_OBJECT

public:
  mqTPSDialog(QWidget* Parent);
  
  ~mqTPSDialog();
  void TPS();
  
  public slots:
  
  
  
  virtual void slotTPS();
  

protected:
	
private:
	

  Q_DISABLE_COPY(mqTPSDialog)
  Ui::mqTPSDialog* const Ui;
 
  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h
