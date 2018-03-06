#include "QReleaseSlider.h"
#include <cmath>

QReleaseSlider::QReleaseSlider(QWidget *parent) :
	QSlider(parent)
{
	connect(this, SIGNAL(sliderReleased()), this, SLOT(slotRefresh()));

}
void QReleaseSlider::slotRefresh()
{
	this->Refresh();
}


void QReleaseSlider::Refresh()
{
	//std::cout << "Refresh to 0!" << endl;
	this->setValue(0);
}

