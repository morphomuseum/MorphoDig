/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqScalarsVolumeDialog_h
#define _mqScalarsVolumeDialog_h


#include <QDialog>

namespace Ui
{
class mqScalarsVolumeDialog;
}




/**
* mqScalarsVolumeDialog is the dialog used by MorphoDig to compute Volume scalars
*/
class  mqScalarsVolumeDialog : public QDialog
{
  Q_OBJECT

public:
  mqScalarsVolumeDialog(QWidget* Parent);
  
  ~mqScalarsVolumeDialog();
  void editVolume();
  
  public slots:
  
  virtual void slotdecomposeVolume();
  virtual void slotglobalVolume();
  virtual void slotcomputeVolume();
  virtual void slotcomputeArea();
  virtual void sloteditVolume();
    

  
  virtual void slotClose();
  

protected:
	
private:
	

  Q_DISABLE_COPY(mqScalarsVolumeDialog)
  Ui::mqScalarsVolumeDialog* const Ui;
 
  // Here we should have the file name, no ?
};

#endif // !_mqCURDialog_h
