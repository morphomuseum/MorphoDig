/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqCylinderDialog_h
#define _mqCylinderDialog_h


#include <QDialog>

namespace Ui
{
class mqCylinderDialog;
}




/**
* mqCylinderDialog is the dialog used by MorphoDig to create cylinder 3D printing support material .
*/
class  mqCylinderDialog : public QDialog
{
  Q_OBJECT

public:
  mqCylinderDialog(QWidget* Parent);
  
  ~mqCylinderDialog();
  void editCylinder();
  
  public slots:
  
  
  
  virtual void sloteditCylinder();
  virtual void slotClose();
  virtual void slotConePressed();
  virtual void slotCylinderPressed();
  virtual void slotCircularShaftPressed();
  virtual void slotEllipticalShaftPressed();
  
  
protected:
	
private:
	

  Q_DISABLE_COPY(mqCylinderDialog)
  Ui::mqCylinderDialog* const Ui;
 
  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h
