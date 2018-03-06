/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqDensifyDialog_h
#define _mqDensifyDialog_h


#include <QDialog>

namespace Ui
{
class mqDensifyDialog;
}




/**
* mqDensifyDialog is the dialog used by MorphoDig to Densify selected surfaces
*/
class  mqDensifyDialog : public QDialog
{
  Q_OBJECT

public:
  mqDensifyDialog(QWidget* Parent);
  
  ~mqDensifyDialog();
  void Densify();
  
  public slots:
  
  
  
  virtual void slotDensify();
  

protected:
	
private:
	

  Q_DISABLE_COPY(mqDensifyDialog)
  Ui::mqDensifyDialog* const Ui;
 
  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h
