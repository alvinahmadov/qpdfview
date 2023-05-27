//
// Created by alvin on 27/05/23.
//

#include "colorwidget.h"

#include <QColor>
#include <QColorDialog>
#include <QComboBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>

namespace {

using namespace qpdfview;

QColor validColorFromCurrentText(const QString& colorText, const QColor& defaultColor = QColor())
{
    const QColor color(colorText);

    return color.isValid() ? color : defaultColor;
}

} // anonymous

namespace qpdfview
{
ColorWidget::ColorWidget(QWidget* parent)
    : QWidget(parent)
{
    m_colorDialog = new QColorDialog(this);
    m_colorComboBox = new QComboBox(this);
    m_colorDialogButton = new QPushButton(this);

    connect(m_colorDialogButton, SIGNAL(clicked(bool)), m_colorDialog, SLOT(open()));
    connect(m_colorDialog, SIGNAL(colorSelected(QColor)), SLOT(onColorSelected(QColor)));
    connect(m_colorComboBox, SIGNAL(currentTextChanged(QString)), SLOT(onCurrentTextChanged(QString)));
}

ColorWidget::ColorWidget(QFormLayout* layout, const QString& label, const QColor& color,
                         const QString& toolTip, QWidget* parent)
    : ColorWidget(parent)
{
    auto colorLayout = new QHBoxLayout(this);
    m_colorDialog->setCurrentColor(color);
    m_colorDialogButton->setFlat(true);
    m_colorDialogButton->setToolTip(toolTip);
    m_colorDialogButton->setAutoFillBackground(true);

    setButtonColor(color);

    m_colorComboBox->setEditable(true);
    m_colorComboBox->setInsertPolicy(QComboBox::NoInsert);
    m_colorComboBox->addItems(QColor::colorNames());
    m_colorComboBox->setToolTip(toolTip);

    setColor(color);

    colorLayout->addWidget(m_colorDialogButton);
    colorLayout->addWidget(m_colorComboBox, 1);
    colorLayout->setContentsMargins({});

    layout->addRow(label, this);
}

ColorWidget::~ColorWidget()
{
    delete m_colorDialog;
    delete m_colorComboBox;
    delete m_colorDialogButton;
}

QColor ColorWidget::color(const QColor& defaultColor) const
{
    return validColorFromCurrentText(m_colorComboBox->currentText(), defaultColor);
}

void ColorWidget::setColor(const QColor& color)
{
    m_colorComboBox->lineEdit()->setText(color.isValid() ? color.name() : QString());
    m_colorDialog->setCurrentColor(color);
    setButtonColor(color);
}

void ColorWidget::onColorSelected(const QColor& color)
{
    m_colorComboBox->lineEdit()->setText(color.isValid() ? color.name() : QString());
    setButtonColor(color);
}

void ColorWidget::onCurrentTextChanged(const QString& currentText)
{
    QColor color = validColorFromCurrentText(currentText);
    m_colorDialog->setCurrentColor(color);
    setButtonColor(color);
}

void ColorWidget::setButtonColor(const QColor& color)
{
    QPixmap pixmap {100, 100};
    pixmap.fill(color);
    m_colorDialogButton->setIcon({pixmap});
}

}   // qpdfview