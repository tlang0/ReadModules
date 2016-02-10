/*******************************************************************************************************
 ReadFramework is the basis for modules developed at CVL/TU Wien for the EU project READ. 
  
 Copyright (C) 2016 Markus Diem <diem@caa.tuwien.ac.at>
 Copyright (C) 2016 Stefan Fiel <fiel@caa.tuwien.ac.at>
 Copyright (C) 2016 Florian Kleber <kleber@caa.tuwien.ac.at>

 This file is part of ReadFramework.

 ReadFramework is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 ReadFramework is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

 The READ project  has  received  funding  from  the European  Union’s  Horizon  2020  
 research  and innovation programme under grant agreement No 674943
 
 related links:
 [1] http://www.caa.tuwien.ac.at/cvl/
 [2] https://transkribus.eu/Transkribus/
 [3] https://github.com/TUWien/
 [4] http://nomacs.org
 *******************************************************************************************************/

#include "PageData.h"

// framework
#include "PageParser.h"

// nomacs
#include "DkSettings.h"

#pragma warning(push, 0)	// no warnings from includes
#include <QDebug>
#pragma warning(pop)

namespace rdm {


// PageData --------------------------------------------------------------------
PageData::PageData(QObject* parent) : QObject(parent) {
	
	setObjectName("PageData");
	loadSettings(nmc::Settings::instance().getSettings());
}

PageData::~PageData() {
	saveSettings(nmc::Settings::instance().getSettings());
	qDebug() << "destroying PAGE viewport";
}

QVector<QSharedPointer<rdf::RegionTypeConfig>> PageData::config() const {
	return mConfig;
}

QSharedPointer<rdf::PageElement> PageData::page() const {
	return mPage;
}

void PageData::parse(const QString& xmlPath) {

	if (mPage && xmlPath == mPage->xmlPath())
		return;

	rdf::PageXmlParser parser;
	parser.read(xmlPath);

	mPage = parser.page();

	emit updatePage(mPage);
}

void PageData::loadSettings(QSettings& settings) {

	settings.beginGroup(objectName());

	QVector<QSharedPointer<rdf::RegionTypeConfig> > configs = rdf::RegionManager::instance().regionTypeConfig();

	// load from nomacs settings
	for (QSharedPointer<rdf::RegionTypeConfig> c : configs) {

		c->load(settings);
		mConfig.append(c);
	}

	settings.endGroup();
}

void PageData::saveSettings(QSettings& settings) const {

	settings.beginGroup(objectName());
	for (const QSharedPointer<rdf::RegionTypeConfig> c : mConfig)
		c->save(settings);
	settings.endGroup();
}



}