/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqIcosahedronDialog_h
#define _mqIcosahedronDialog_h


#include <QDialog>

namespace Ui
{
class mqIcosahedronDialog;
}




/**
* mqIcosahedronDialog is the dialog used by MorphoDig to create cubic/box-shaped 3D printing connective material .
*/
class  mqIcosahedronDialog : public QDialog
{
  Q_OBJECT

public:
  mqIcosahedronDialog(QWidget* Parent);
  
  ~mqIcosahedronDialog();
  void editIcosahedron();
  
  public slots:
  
  
  
  virtual void sloteditIcosahedron();
  virtual void slotClose();
  
  
protected:
	
private:
	

  Q_DISABLE_COPY(mqIcosahedronDialog)
  Ui::mqIcosahedronDialog* const Ui;
 
  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h
