#include "cpu/exec/helper.h"
#include "cmps.h"

/* for instruction encoding overloading */

#define DATA_BYTE 1
#include "cpu/exec/template-start.h"
#include "cmps-template.h"
#include "cpu/exec/template-end.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "cpu/exec/template-start.h"
#include "cmps-template.h"
#include "cpu/exec/template-end.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "cpu/exec/template-start.h"
#include "cmps-template.h"
#include "cpu/exec/template-end.h"
#undef DATA_BYTE

make_helper_v (cmps);



