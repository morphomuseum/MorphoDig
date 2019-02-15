/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqCameraDialog_h
#define _mqCameraDialog_h

#include <QDialog>

namespace Ui
{
class mqCameraDialog;
}




/**
* mqCameraDialog is the Camera dialog used by MorphoDig.
* It sets  Camera and Flag rendering settings
*/
class  mqCameraDialog : public QDialog
{
  Q_OBJECT

public:
  mqCameraDialog(QWidget* Parent);
  
  ~mqCameraDialog();

  void RefreshDialog();

  public slots:
  virtual void slotApplyCameraParameters();
  virtual void slotOpenCAM();
  virtual void slotSaveCAM();
  virtual void slotReinitialize();
  virtual void slotRefresh();
  
protected:
	
private:
  Q_DISABLE_COPY(mqCameraDialog)
  Ui::mqCameraDialog* const Ui;
};

#endif // !_mqCameraDialog_h
