// SPDX-License-Identifier: GPL-2.0-or-later
/** 
 * @file  FilterCommentsManager.cpp
 *
 * @brief FilterCommentsManager class implementation.
 */

#include "pch.h"
#include "FilterCommentsManager.h"
#include <Poco/Util/IniFileConfiguration.h>
#include <Poco/AutoPtr.h>
#include <map>
#include <cstdio>
#include <memory>
#include "Environment.h"
#include "unicoder.h"
#include "paths.h"

using Poco::AutoPtr;
using Poco::Util::IniFileConfiguration;

/**
@brief FilterCommentsManager constructor, which reads the INI file data
		and populates the mapped member variable m_FilterCommentsSetByFileType.
@param[in]  Optional full INI file name, to include path.
*/
FilterCommentsManager::FilterCommentsManager(const String& IniFileName /*= _T("")*/)
 : m_IniFileName(IniFileName)
{
	Load();
}


void FilterCommentsManager::Load()
{
	int SectionNo = 0;
	if (m_IniFileName.empty())
	{
		m_IniFileName = paths::ConcatPath(env::GetProgPath(), _T("IgnoreSectionMarkers.ini"));
	}

	try
	{
		AutoPtr<IniFileConfiguration> pConf(new IniFileConfiguration(ucr::toUTF8(m_IniFileName)));
		for(SectionNo = 0;;++SectionNo) 
		{//Get each set of markers
			FilterCommentsSet filtercommentsset;
			std::string SectionName = "set" + ucr::toUTF8(strutils::to_str(SectionNo));
			filtercommentsset.StartMarker = pConf->getString(SectionName + ".StartMarker", "");
			filtercommentsset.EndMarker = pConf->getString(SectionName + ".EndMarker", "");
			filtercommentsset.InlineMarker = pConf->getString(SectionName + ".InlineMarker", "");
			if (filtercommentsset.StartMarker.empty() && 
				filtercommentsset.EndMarker.empty() &&
				filtercommentsset.InlineMarker.empty())
			{
				break;
			}
			int FileTypeNo = 0;
			for(FileTypeNo = 0;;++FileTypeNo) 
			{//Get each file type associated with current set of markers
				std::string FileTypeFieldName = ".FileType" + ucr::toUTF8(strutils::to_str(FileTypeNo));
				std::string FileTypeExtensionName = pConf->getString(SectionName + FileTypeFieldName, "");
				if (FileTypeExtensionName.empty())
					break;
				m_FilterCommentsSetByFileType[ucr::toTString(FileTypeExtensionName)] = filtercommentsset;
			}
		}
	}
	catch (...)
	{
	}
	if (SectionNo == 0)
	{//If no markers were found, then initialize default markers
		CreateDefaultMarkers();
	}
}

/**
	@brief Get comment markers that are associated with this file type.
		If there are no comment markers associated with this file type,
		then return an empty set.
	@param[in]  The file name extension. Example:("cpp", "java", "c", "h")
				Must be lower case.
*/
FilterCommentsSet FilterCommentsManager::GetSetForFileType(const String& FileTypeName) const
{
	std::map <String, FilterCommentsSet> :: const_iterator pSet =
		m_FilterCommentsSetByFileType.find(FileTypeName);
	if (pSet == m_FilterCommentsSetByFileType.end())
		return FilterCommentsSet();
	return pSet->second;
}

void FilterCommentsManager::CreateDefaultMarkers()
{
	FilterCommentsSet filtercommentsset[2];
	filtercommentsset[0].StartMarker = "/*";
	filtercommentsset[0].EndMarker = "*/";
	filtercommentsset[0].InlineMarker = "//";
	filtercommentsset[1].StartMarker = "";
	filtercommentsset[1].EndMarker = "";
	filtercommentsset[1].InlineMarker = "'";
	TCHAR CommonFileTypes[][16][9] = {
		{ _T("java"), _T("cs"), _T("cpp"), _T("c"), _T("h"), _T("cxx"), _T("cc"), _T("js"), _T("jsl"), _T("tli"), _T("tlh"), _T("rc") },
		{ _T("bas"), _T("vb"), _T("vbs"), _T("frm"), _T("dsm"), _T("cls"), _T("ctl"), _T("pag"), _T("dsr") }
	};
	for (int SectionNo = 0; SectionNo < sizeof(CommonFileTypes)/sizeof(CommonFileTypes[0]); ++SectionNo)
	{
		for (int i = 0; i < sizeof(CommonFileTypes[SectionNo]) / sizeof(CommonFileTypes[SectionNo][0]); ++i)
		{
			if (CommonFileTypes[SectionNo][i][0] != '\0')
				m_FilterCommentsSetByFileType[CommonFileTypes[SectionNo][i]] = filtercommentsset[SectionNo];
		}
	}
}
