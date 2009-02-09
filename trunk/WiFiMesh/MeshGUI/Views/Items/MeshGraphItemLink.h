/**
 * \file MeshItemLink.h
 *
 * Project: MeshGUI
 * (C) Copyright 2009 Denis Itskovich
 *
 * \date 08/02/2009
 * \author Denis Itskovich
 */


#ifndef MESHITEMLINK_H_
#define MESHITEMLINK_H_

#include <QtGui>
#include "MeshGraphItemStation.h"
#include "MeshGraphItemArrow.h"

class MeshGraphItemLink : public MeshGraphItemArrow
{
public:
    MeshGraphItemLink(MeshGraphItemStation* src, MeshGraphItemStation* dst, const Packet* pPacket);
    void updateLink();

private:
    MeshGraphItemStation*   m_src;
    MeshGraphItemStation*   m_dst;
};

#endif /* MESHITEMLINK_H_ */
