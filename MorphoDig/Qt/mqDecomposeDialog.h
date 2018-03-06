/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqDecomposeDialog_h
#define _mqDecomposeDialog_h


#include <QDialog>

namespace Ui
{
class mqDecomposeDialog;
}




/**
* mqDecomposeDialog is the dialog used by MorphoDig to distort selected surfaces using a thin plate splines transformation
*/
class  mqDecomposeDialog : public QDialog
{
  Q_OBJECT

public:
  mqDecomposeDialog(QWidget* Parent);
  
  ~mqDecomposeDialog();
  void Decompose();
  
  public slots:
  
  
  
  virtual void slotDecompose();
  

protected:
	
private:
	

  Q_DISABLE_COPY(mqDecomposeDialog)
  Ui::mqDecomposeDialog* const Ui;
 
  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h
