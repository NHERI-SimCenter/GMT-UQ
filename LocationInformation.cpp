/* *****************************************************************************
Copyright (c) 2016-2017, The Regents of the University of California (Regents).
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of the FreeBSD Project.

REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
THE SOFTWARE AND ACCOMPANYING DOCUMENTATION, IF ANY, PROVIDED HEREUNDER IS
PROVIDED "AS IS". REGENTS HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT,
UPDATES, ENHANCEMENTS, OR MODIFICATIONS.

*************************************************************************** */



#include "LocationInformation.h"
#include <QFormLayout>
#include <QGroupBox>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>
#include <QList>

#include <QLabel>
#include <QLineEdit>

#include <QMetaEnum>

 /* in case want singleton
LocationInformation *
LocationInformation::getInstance() {
  if (theInstance == 0)
    theInstance = new LocationInformation();

  return theInstance;
 }

LocationInformation *LocationInformation::theInstance = 0;
 */

LocationInformation::LocationInformation(QWidget *parent) 
  : SimCenterWidget(parent)
{
    locationNameEdit = new QLineEdit(this);
    locationLatBox = new QDoubleSpinBox(this);
    locationLatBox->setRange(-90.0, 90.0);
    locationLatBox->setDecimals(4);
    locationLatBox->setSingleStep(0.0001);

    locationLonBox = new QDoubleSpinBox(this);
    locationLonBox->setRange(-180.0, 180.0);
    locationLonBox->setDecimals(4);
    locationLonBox->setSingleStep(0.0001);

    QVBoxLayout* layout = new QVBoxLayout(this);

    //Location
    QGroupBox* locationGroupBox = new QGroupBox("Location", this);
    QFormLayout* locationFormLayout = new QFormLayout(locationGroupBox);
    locationFormLayout->addRow(tr("Address"), locationNameEdit);
    locationFormLayout->addRow(tr("Latitude"), locationLatBox);
    locationFormLayout->addRow(tr("Longitude"), locationLonBox);
    //Setting Style
    locationFormLayout->setAlignment(Qt::AlignLeft);
    locationFormLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    locationFormLayout->setRowWrapPolicy(QFormLayout::DontWrapRows);


    setWindowTitle(tr("Location Information"));
    layout->addWidget(locationGroupBox);
    layout->addStretch(1);

    this->setMinimumWidth(200);
    this->setMaximumWidth(400);
}

LocationInformation::~LocationInformation()
{

}

bool
LocationInformation::outputToJSON(QJsonObject &jsonObj){

    QJsonObject location;
    location["name"] = locationNameEdit->text().trimmed();

    QString dblVal = locationLatBox->text();
    location["latitude"] = dblVal.toDouble();

    dblVal = locationLonBox->text();
    location["longitude"] = dblVal.toDouble();

    jsonObj["location"] = location;

    return(true);

}

bool
LocationInformation::inputFromJSON(QJsonObject &jsonObject){

    // Location Object
    QJsonValue locationValue = jsonObject["location"];
    QJsonObject locationObj = locationValue.toObject();

    QJsonValue locationNameValue = locationObj["name"];
    locationNameEdit->setText(locationNameValue.toString());

    QJsonValue locationLatitudeValue = locationObj["latitude"];
    locationLatBox->setValue(locationLatitudeValue.toDouble());

    QJsonValue locationLongitudeValue = locationObj["longitude"];
    locationLonBox->setValue(locationLongitudeValue.toDouble());


    return(true);
}

void
LocationInformation::clear(void)
{
    locationNameEdit->clear();
    locationLatBox->clear();
    locationLonBox->clear();
}


