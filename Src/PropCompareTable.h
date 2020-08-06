/** 
 * @file  PropCompareTable.h
 *
 * @brief Declaration of PropCompareTable propertysheet
 */
#pragma once

#include "OptionsPanel.h"
#include "UnicodeString.h"

class COptionsMgr;

/**
 * @brief Property page to set image compare options for WinMerge.
 */
class PropCompareTable : public OptionsPanel
{
// Construction
public:
	explicit PropCompareTable(COptionsMgr *optionsMgr);

// Implement IOptionsPanel
	virtual void ReadOptions() override;
	virtual void WriteOptions() override;

// Dialog Data
	//{{AFX_DATA(PropCompareTable)
	enum { IDD = IDD_PROPPAGE_COMPARE_TABLE };
	String m_sCSVFilePatterns;
	String m_sTSVFilePatterns;
	String m_sDSVFilePatterns;
	String m_sDSVDelimiterChar;
	bool m_bAllowNewlinesInQuotes;
	String m_sQuoteChar;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PropCompareTable)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(PropCompareTable)
	afx_msg void OnDefaults();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
