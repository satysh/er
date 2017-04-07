/*
 * WCalEvent.cpp
 *
 *  Created on: Apr 6, 2017
 *      Author: vratik
 */

#include "WCalEvent.h"

WCalEvent::WCalEvent() :
	fNumOfHits(0)
//    fEventID(0)
{
    fHits = new TClonesArray("WCalHit", 5);
}

WCalEvent::~WCalEvent() {
	Clear();
}

void WCalEvent::Clear(Option_t * /*option*/)
{
	fHits->Clear("C");
	fNumOfHits = 0;     // Hakuna Matata
}

WCalHit* WCalEvent::AddHit(UChar_t p_ch, Float_t p_adcCalib)
{
	TClonesArray &hits = *fHits;
	WCalHit *hit = new(hits[fNumOfHits++]) WCalHit(p_ch, p_adcCalib);
	return hit;
}
