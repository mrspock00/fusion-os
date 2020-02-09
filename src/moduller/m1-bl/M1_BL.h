/* Fegeya 2020 Telif Hakki GPLv3*/
#ifndef M1_BL_H
#define M1_BL_H


#include "DecorManager.h"
#include "SATDecorator.h"


class BRect;

class M1BL {
public:
	virtual	void				hizmetler();
	virtual	void				NLoad(bool load);
};


#endif	// WIN_DECORATOR_H
