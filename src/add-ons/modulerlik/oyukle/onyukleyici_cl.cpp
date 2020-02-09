
// Fusion
// Fegeya 


#include "WinDecorator.h"

#include <new>
#include <stdio.h>

#include <Point.h>
#include <Rect.h>
#include <View.h>

#include "DesktopSettings.h"
#include "DrawingEngine.h"
#include "PatternHandler.h"
#include "RGBColor.h"
#include "../../system/boot/loader/loader.h"
#include "../../system/boot/loader/elf.h"
#include "../../system/boot/loader/zarar.h"
#include "../../system/boot/loader/load_driver_settings.h"
//#define DEBUG_DECORATOR
#ifdef DEBUG_DECORATOR
#	define STRACE(x) printf x
#else
#	define STRACE(x) ;
#endif

/*void CloseFunc(bool x)
{
	if(x = true)
	{
	

	}
	//x = true;



}*/

