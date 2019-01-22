/*=========================================================================

   Program: MorphoDig
   

=========================================================================*/
#ifndef _mqColorDialog_h
#define _mqColorDialog_h

#include <QDialog>

namespace Ui
{
class mqColorDialog;
}




/**
* mqColorDialog is the Color dialog used by MorphoDig.
* It sets mesh default color and background colors
*/
class  mqColorDialog : public QDialog
{
  Q_OBJECT

public:
  mqColorDialog(QWidget* Parent);
  
  ~mqColorDialog();

  public slots:
  virtual void slotMeshColorChanged();
  virtual void slotBackGroundColorChanged();
  virtual void slotBackGroundColorChanged2();
  virtual void slotReinitializeColors();
  virtual void slotAmbient(int ambient);
  virtual void slotDiffuse(int diffuse);
  virtual void slotSpecular(int specular);
  virtual void slotSpecularPower(double specularPower);

protected:
	
private:
  Q_DISABLE_COPY(mqColorDialog)
  Ui::mqColorDialog* const Ui;
};

#endif // !_mqColorDialog_h
