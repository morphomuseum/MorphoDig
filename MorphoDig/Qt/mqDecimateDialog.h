/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqDecimateDialog_h
#define _mqDecimateDialog_h


#include <QDialog>

namespace Ui
{
class mqDecimateDialog;
}




/**
* mqDecimateDialog is the dialog used by MorphoDig to Decimate selected surfaces
*/
class  mqDecimateDialog : public QDialog
{
  Q_OBJECT

public:
  mqDecimateDialog(QWidget* Parent);
  
  ~mqDecimateDialog();
  void Decimate();
  
  public slots:
  
  
  
  virtual void slotDecimate();
  

protected:
	
private:
	

  Q_DISABLE_COPY(mqDecimateDialog)
  Ui::mqDecimateDialog* const Ui;
 
  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h
