#ifndef QDOUBLERELEASESLIDER_H
#define QDOUBLERELEASESLIDER_H

#include "QDoubleSlider.h"

class QDoubleReleaseSlider : public QDoubleSlider
{
	Q_OBJECT
public:
	explicit QDoubleReleaseSlider(QWidget *parent = 0);

	

signals:

	public slots :
		void slotRefresh();

private:
	


	void Refresh();

};

#endif // QRELEASESLIDER_H
