#include <QtWidgets>
#include <QLabel>
//#include "QDoubleSlider.h"
#include "QReleaseSlider.h"
#include "QReleaseSliderValue.h"

QReleaseSliderValue::QReleaseSliderValue(Qt::Orientation orientation, const QString &title,
	QWidget *parent)
	//: QGroupBox(title, parent)
	: QWidget(parent)
{
	//this->setFlat(true);
	this->spinbox = new QSpinBox;
	this->spinbox->setButtonSymbols(QAbstractSpinBox::NoButtons);
	this->spinbox->setDisabled(true);
		this->spinbox->setFixedSize(16, 15);
	QFont font(this->spinbox->font());
	font.setPointSize(4);
	this->spinbox->setFont(font);

	this->slider = new QReleaseSlider;	
	this->slider->setSingleStep(0.2);
	this->labelVisible = 0;
	this->spinboxVisible = 0;
	this->label = new QLabel;
	this->label->setText("T");
	connect(slider, SIGNAL(valueChanged(int)), spinbox, SLOT(setValue(int)));
	connect(slider, SIGNAL(valueChanged(int)), this, SIGNAL(valueChanged(int)));
	connect(slider, SIGNAL(sliderPressed()), this, SLOT(sliderWasPressed()));
	QBoxLayout::Direction direction;

	if (orientation == Qt::Vertical)
	{
		direction = QBoxLayout::TopToBottom;
	}
	else
	{
		direction = QBoxLayout::LeftToRight;
	}

	this->layout = new QVBoxLayout;
	this->layout->setAlignment(Qt::AlignCenter);
	this->layout->addWidget(slider);
	
	setLayout(this->layout);
	this->layout->setSpacing(0);
	this->layout->setMargin(0);
	this->setMaximumWidth(16);
	
}
void QReleaseSliderValue::sliderWasPressed()
{
	emit this->sliderPressed();
}
void QReleaseSliderValue::setLabelVisible(int visible)
{
	this->labelVisible = visible;
	if (visible == 0) { this->layout->removeWidget(this->label); }
	else{ this->layout->addWidget(this->label); }
}
void QReleaseSliderValue::setSpinboxVisible(int visible)
{
	this->spinboxVisible = visible;
	if (visible == 0) { this->layout->removeWidget(this->spinbox); }
	else { this->layout->addWidget(this->spinbox); }
}
void QReleaseSliderValue::setLabelText(QString text)
{
	this->label->setText(text.toStdString().c_str());
}
void QReleaseSliderValue::setValue(int value)
{
	slider->setValue(value);
}

void QReleaseSliderValue::setMinimum(int value)
{
	slider->setMinimum(value);
	spinbox->setMinimum(value);
}

void QReleaseSliderValue::setMaximum(int value)
{
	slider->setMaximum(value);
	
	spinbox->setMaximum(value);
}

void QReleaseSliderValue::invertAppearance(bool invert)
{
	slider->setInvertedAppearance(invert);

	
}

void QReleaseSliderValue::invertKeyBindings(bool invert)
{
	slider->setInvertedControls(invert);
	
	
}