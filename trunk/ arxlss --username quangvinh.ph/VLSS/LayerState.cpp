#include "StdAfx.h"
#include "LayerState.h"

CLayerState::CLayerState()
{
	m_idLayer.setNull();
	m_bOff = false;
	m_bFrozen = false;
	m_bLocked = false;
}

CLayerState::~CLayerState()
{
	return;
}


CLayerState::CLayerState(const AcDbObjectId& idLayer)
{
	m_idLayer = idLayer;
	AcDbLayerTableRecord* pLayer = NULL;
	if (acdbOpenObject(pLayer, m_idLayer, AcDb::kForRead) == Acad::eOk) {
		m_bOff = pLayer->isOff();
		m_bFrozen = pLayer->isFrozen();
		m_bLocked = pLayer->isLocked();
		pLayer->close();
	} else  {
		m_bOff = false;
		m_bFrozen = false;
		m_bLocked = false;
	}
}


bool CLayerState::set(void)
{
	AcDbLayerTableRecord* pLayer = NULL;
	if (Acad::eOk != acdbOpenObject(pLayer, m_idLayer, AcDb::kForWrite))
		return false;

	pLayer->setIsOff(m_bOff);
	pLayer->setIsFrozen(m_bFrozen);
	pLayer->setIsLocked(m_bLocked);
	pLayer->close();
	return true;
}

bool CLayerState::unlock(void)
{
	AcDbLayerTableRecord* pLayer = NULL;
	if (acdbOpenObject(pLayer, m_idLayer, AcDb::kForWrite) != Acad::eOk)
		return false;

	pLayer->setIsLocked(false);
	pLayer->close();
	return true;
}


// ���݂̏�Ԃ��o����
bool CAllLayerState::memorize(AcDbDatabase* pDb)
{
	// ���C���e�[�u���̎擾
	AcDbLayerTable* pLayerTable = NULL;
	if (pDb->getLayerTable(pLayerTable, AcDb::kForRead) != Acad::eOk)
		return false;

	AcDbLayerTableIterator* pIter;
	pLayerTable->newIterator(pIter);
	for (; !pIter->done(); pIter->step()) {
		// ���C�����擾
		AcDbObjectId idLayer;
		pIter->getRecordId(idLayer);
		CLayerState layerstate(idLayer);
		push_back(layerstate);
	}
	delete pIter;

	// ���Ƃ�
	pLayerTable->close();
	return true;
}


// �o������Ԃ��v���o��
bool CAllLayerState::remember(void)
{
	iterator it;
	for (it = begin(); it != end(); it++) {
		CLayerState& layerstate = *it;
		layerstate.set();
	}
	return true;
}


// �S��w���b�N����
bool CAllLayerState::unlock(void)
{
	iterator it;
	for (it = begin(); it != end(); it++) {
		CLayerState& layerstate = *it;
		layerstate.unlock();
	}
	return true;
}

