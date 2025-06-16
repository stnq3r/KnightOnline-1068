// Map.cpp: implementation of the CMap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "3DServer.h"
#include "Map.h"
#include "Region.h"
#include "Define.h"
#include ".\N3Base\N3VMesh.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

C3DMap::C3DMap()
{
	m_pVMesh = NULL;
	m_nMapSize = 0;
	m_fUnitDist = 0.0f;
	m_nVMeshCount = 0;
	m_vNormal = NULL;
	m_fHeight = NULL;

	m_nXRegion = 0;
	m_nZRegion = 0;
}

C3DMap::~C3DMap()
{
	if( m_vNormal ) {
		delete[] m_vNormal;
		m_vNormal = NULL;
	}
	if( m_fHeight ) {
		delete[] m_fHeight;
		m_fHeight = NULL;
	}
	if( m_pVMesh ) {
		delete[] m_pVMesh;
		m_pVMesh = NULL;
	}

	if( m_ppRegion ) {
		for(int i=0; i< m_nXRegion; i++) {
			delete[] m_ppRegion[i];
			m_ppRegion[i] = NULL;
		}
		delete[] m_ppRegion;
		m_ppRegion = NULL;
	}
}

BOOL C3DMap::LoadMap(HANDLE hFile)
{
	DWORD dwNum;
	BOOL ret = FALSE;

	LoadTerrain(hFile);	// ���� �ε�

	ret = ReadFile(hFile, &m_nVMeshCount, sizeof(int), &dwNum, NULL);		// VMesh ����(Vertual Mesh)
	if( !ret )
		return FALSE;

	if( m_nVMeshCount < 0 )
		return FALSE;

	m_pVMesh = new CN3VMesh[m_nVMeshCount];	
	for(int i=0; i<m_nVMeshCount; ++i)
	{
		m_pVMesh[i].Load(hFile);								// ������ŭ �ε�
	}

	m_nXRegion = (m_nMapSize*(int)m_fUnitDist)/VIEW_DIST;
	m_nZRegion = (m_nMapSize*(int)m_fUnitDist)/VIEW_DIST;

	SetRegion();

	return TRUE;
}

void C3DMap::LoadTerrain(HANDLE hFile)
{
	DWORD dwRWC;
	ReadFile(hFile, &m_nMapSize, sizeof(int), &dwRWC, NULL);	// ���μ��� ������ ����ΰ�?
	ReadFile(hFile, &m_fUnitDist, sizeof(float), &dwRWC, NULL);

	m_vNormal = new __Vector3[m_nMapSize*m_nMapSize];
	m_fHeight = new float[m_nMapSize*m_nMapSize];

	int x, z;
	for(z=0;z<m_nMapSize;z++)
	{
		for(x=0;x<m_nMapSize;x++)
		{
			ReadFile(hFile, &(m_fHeight[x + (m_nMapSize*z)]), sizeof(float), &dwRWC, NULL);	// ���̰� �о����
		}
	}
	for(z=0;z<m_nMapSize;z++)
	{
		for(x=0;x<m_nMapSize;x++)
		{
			ReadFile(hFile, &(m_vNormal[x + (m_nMapSize*z)]), sizeof(__Vector3), &dwRWC, NULL);	// ���� Normal��
		}
	}
}

void C3DMap::SetRegion()
{
	int dimx, dimz;
	__Vector3	center;

	m_ppRegion = new CRegion*[m_nXRegion];
	for(int i=0; i<m_nXRegion; i++) {
		m_ppRegion[i] = new CRegion[m_nZRegion];
	}

	for( i=0; i<m_nVMeshCount; i++) {
		center = m_pVMesh[i].Center();
		dimx = (int)(center.x / VIEW_DIST);
		dimz = (int)(center.z / VIEW_DIST);

		if( dimx > m_nXRegion )	continue;
		if( dimz > m_nZRegion )	continue;

		m_pVMesh->SetRegion( dimx, dimz );
		m_ppRegion[dimx][dimz].m_VMeshArray.Add( m_pVMesh );
	}
}

float C3DMap::GetHeight(float x, float z)
{
	int iX, iZ;
	iX = (int)(x/m_fUnitDist);
	iZ = (int)(z/m_fUnitDist);
	//_ASSERT( iX, iZ�� �������� �ִ� ������ üũ�ϱ�);

	float y;
	float h1, h2, h3;
	float dX, dZ;
	dX = (x - iX*m_fUnitDist)/m_fUnitDist;
	dZ = (z - iZ*m_fUnitDist)/m_fUnitDist;

	_ASSERT(dX>=0.0f && dZ>=0.0f && dX<1.0f && dZ<1.0f);
//	if( !(dX>=0.0f && dZ>=0.0f && dX<1.0f && dZ<1.0f) )
//		return 0.0f;

	if ((iX+iZ)%2==1)
	{
		if ((dX+dZ) < 1.0f)
		{
			h1 = m_fHeight[iX + (m_nMapSize*(iZ+1))];
			h2 = m_fHeight[(iX+1) + (m_nMapSize*iZ)];
			h3 = m_fHeight[iX + (m_nMapSize*iZ)];

			//if (dX == 1.0f) return h2;

			float h12 = h1+(h2-h1)*dX;	// h1�� h2������ ���̰�
			float h32 = h3+(h2-h3)*dX;	// h3�� h2������ ���̰�
			y = h32 + (h12-h32)*((dZ)/(1.0f-dX));	// ã���� �ϴ� ���̰�
		}
		else
		{
			h1 = m_fHeight[iX + (m_nMapSize*(iZ+1))];
			h2 = m_fHeight[(iX+1) + (m_nMapSize*iZ)];
			h3 = m_fHeight[(iX+1) + (m_nMapSize*(iZ+1))];

			if (dX == 0.0f) return h1;

			float h12 = h1+(h2-h1)*dX;	// h1�� h2������ ���̰�
			float h13 = h1+(h3-h1)*dX;	// h1�� h3������ ���̰�
			y = h13 + (h12-h13)*((1.0f-dZ)/(dX));	// ã���� �ϴ� ���̰�
		}
	}
	else
	{
		if (dZ > dX)
		{
			h1 = m_fHeight[iX + (m_nMapSize*(iZ+1))];
			h2 = m_fHeight[(iX+1) + (m_nMapSize*(iZ+1))];
			h3 = m_fHeight[iX + (m_nMapSize*iZ)];

			//if (dX == 1.0f) return h2;

			float h12 = h1+(h2-h1)*dX;	// h1�� h2������ ���̰�
			float h32 = h3+(h2-h3)*dX;	// h3�� h2������ ���̰�
			y = h12 + (h32-h12)*((1.0f-dZ)/(1.0f-dX));	// ã���� �ϴ� ���̰�
		}
		else
		{
			h1 = m_fHeight[iX + (m_nMapSize*iZ)];
			h2 = m_fHeight[(iX+1) + (m_nMapSize*iZ)];
			h3 = m_fHeight[(iX+1) + (m_nMapSize*(iZ+1))];

			if (dX == 0.0f) return h1;

			float h12 = h1+(h2-h1)*dX;	// h1�� h2������ ���̰�
			float h13 = h1+(h3-h1)*dX;	// h1�� h3������ ���̰�
			y = h12 + (h13-h12)*((dZ)/(dX));	// ã���� �ϴ� ���̰�
		}
	}
	return y;
}

BOOL C3DMap::ObjectIntersect(float x, float z, float y)
{

	return FALSE;
}
