/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqScalarsComplexityDialog_h
#define _mqScalarsComplexityDialog_h


#include <QDialog>

namespace Ui
{
class mqScalarsComplexityDialog;
}




/**
* mqScalarsComplexityDialog is the dialog used by MorphoDig to compute complexity .
*/
class  mqScalarsComplexityDialog : public QDialog
{
  Q_OBJECT

public:
  mqScalarsComplexityDialog(QWidget* Parent);
  
  ~mqScalarsComplexityDialog();
  void editComplexity();
  
  public slots:
  
  
  
  virtual void sloteditComplexity();
  virtual void slotClose();
  virtual void slotProgressBar(int val);
  virtual void slotEnableDisableCustomArea();

protected:
	
private:
	

  Q_DISABLE_COPY(mqScalarsComplexityDialog)
  Ui::mqScalarsComplexityDialog* const Ui;
 
  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h
