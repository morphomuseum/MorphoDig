#ifndef QRELEASESLIDER_H
#define QRELEASESLIDER_H

#include <QSlider>

class QReleaseSlider : public QSlider
{
	Q_OBJECT
public:
	explicit QReleaseSlider(QWidget *parent = 0);

	

signals:

	public slots :
		void slotRefresh();

private:
	


	void Refresh();

};

#endif // QRELEASESLIDER_H
