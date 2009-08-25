#ifndef _LAYERSTATE_H
#define _LAYERSTATE_H

#include <vector>

class CLayerState
{
friend class CLayerState;
private:
	AcDbObjectId m_idLayer;

	bool m_bOff;
	bool m_bFrozen;
	bool m_bLocked;

protected:
	CLayerState();
public:
	CLayerState( const AcDbObjectId& idLayer );
	CLayerState( const CLayerState& r );
	virtual ~CLayerState();
	bool set( void );
	bool unlock( void );

	CLayerState& operator =( const CLayerState& r );
};

inline CLayerState& CLayerState::operator =( const CLayerState& r )
{
	m_idLayer = r.m_idLayer;
	m_bOff = r.m_bOff;
	m_bFrozen = r.m_bFrozen;
	m_bLocked = r.m_bLocked;

	return *this;
}

inline CLayerState::CLayerState( const CLayerState& r )
{
	*this = r;
}

typedef std::vector< CLayerState > CLayerStateVector;
class CAllLayerState : public CLayerStateVector
{
public:
	// 現在の状態を覚える
	bool memorize( AcDbDatabase* pDb );
	// 覚えた状態を思い出す
	bool remember( void );
	// 全画層ロック解除
	bool unlock( void );
};


#endif /* _LAYERSTATE_H */
