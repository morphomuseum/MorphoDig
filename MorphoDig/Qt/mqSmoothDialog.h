/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqSmoothDialog_h
#define _mqSmoothDialog_h


#include <QDialog>

namespace Ui
{
class mqSmoothDialog;
}




/**
* mqSmoothDialog is the dialog used by MorphoDig to smooth selected surfaces
*/
class  mqSmoothDialog : public QDialog
{
  Q_OBJECT

public:
  mqSmoothDialog(QWidget* Parent);
  
  ~mqSmoothDialog();
  void Smooth();
  
  public slots:
  
  
  
  virtual void slotSmooth();
  

protected:
	
private:
	

  Q_DISABLE_COPY(mqSmoothDialog)
  Ui::mqSmoothDialog* const Ui;
 
  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h
