/********************************************************************
	Created:		27:8:2009   11:29
	File Name:		Lessons.h
	Author:			Pham Quang Vinh
	
	Purpose:		This header file declare the lessons's functions.
*********************************************************************/

#ifndef Lessons_h__
#define Lessons_h__

/****************************
 *	FUNCTIONS
 */
int getBlockRefAll(AcDbObjectIdArray& idaAll);
int getBlockRefAll(AcDbDatabase* pDb, AcDbObjectIdArray& idaAll);
AcDbObject* selectObject(AcDb::OpenMode mode);
AcDbEntity* selectEntity(AcDb::OpenMode mode);
bool selectPoint(AcGePoint3d&);
void printList(struct resbuf*  pRb);
AcApDocument* findDocument(LPCASTR pszFileName);
bool addToModelSpace(AcDbObjectId&, AcDbEntity*);

/****************************
 *	LESSON 01
 * Create DWG file and read DWG file.
 * Practice for Errors Handling.
 */
bool createDwg();
bool readDwg();
void LSS01(void);


/****************************
*	LESSON 02
*/
bool printXData();
bool addXData();
void LSS02(void);


/****************************
*	LESSON 03
* Access to list all of DENKI Project's files in 2 methods.
*/
bool listFiles(void);
bool listFilesEx(void);
void LSS03(void);

/****************************
*	LESSON 04
* Access DENKI Project's files then print out its BAN_NO.
*/
void LSS04(void);

/****************************
*	LESSON 05
* Process a DWG file and don't take care of the way file is opened.
*/
void LSS05(void);

/****************************
*	LESSON 06
* Browse all DENKI project's files, then search the file has got BAN_NO
* NAME, INST_NO that match with the required values.
*/
bool DoJob(LPCASTR pszFileName);
void LSS06(void);

/****************************
*	LESSON 07
* This file just implement an function that list all BlockReference's attributes
* (Just for investigating).
*/
void browseAllAttributes(AcDbDatabase* pDb);
void LSS07(void);

/****************************
*	LESSON 08
* Purge the database's BlockTable, LayerTable, 
* LinetypeTable, TextStyleTable.
*/
int getAllSymbolRecordsIds(AcRxClass* pTableClass, AcDbObjectIdArray& idaAll);
bool purgeSymbolTable(AcRxClass* pTableClass);
void LSS08(void);

/****************************
*	LESSON 09
* Read database SummaryInfo, Just for investigating.
*/
void LSS09(void);

/****************************
*	LESSON 10
* Practice for handling error, and creating entity.
*/
Acad::ErrorStatus createCircle(AcDbObjectId & idCircle);
void LSS10(void);

/****************************
*	LESSON 11
*/
int DoFolder(LPCTSTR pszPath);
int PurgeJob(AcDbDatabase *pDb);
void LSS11(void);

/****************************
*	LESSON 12
* Practice for creating BlockTableRecord, and copying BlockReference
*/
bool makeABlock(void);
bool copyBlockRef(AcDbBlockReference*, AcGePoint3d, AcDbObjectId&);
void LSS12(void);

/****************************
*	LESSON 13
* Read BlockTableRecord's components.
* (Just for investigating.)
*/
void LSS13(void);

/****************************
*	LESSON 14
* Practice for creating and reading Polyline object!
*/
void createPolyline(const int nPoints);
void iteratePolyline(AcDbObjectId idPLine);
void LSS14(void);

/****************************
 *	LESSON 15
 * Practice for accessing application private profile.
 */
void readProfile();
void writePrifile();
void LSS15(void);

/****************************
 *	LESSON 16
 * Practice for handling selection set.
 */
void LSS16(void);


#endif // Lessons_h__