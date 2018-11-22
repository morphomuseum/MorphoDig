/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqCubeDialog_h
#define _mqCubeDialog_h


#include <QDialog>

namespace Ui
{
class mqCubeDialog;
}




/**
* mqCubeDialog is the dialog used by MorphoDig to create cubic/box-shaped 3D printing connective material .
*/
class  mqCubeDialog : public QDialog
{
  Q_OBJECT

public:
  mqCubeDialog(QWidget* Parent);
  
  ~mqCubeDialog();
  void editCube();
  
  public slots:
  
  
  
  virtual void sloteditCube();
  virtual void slotClose();
  
  
protected:
	
private:
	

  Q_DISABLE_COPY(mqCubeDialog)
  Ui::mqCubeDialog* const Ui;
 
  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h
