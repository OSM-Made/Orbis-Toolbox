#pragma once

#include <errno.h>
#include <map>
#include <vector>
#include <string>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stddef.h>
#include <stdint.h>
#include <orbis/libkernel.h>
#include <orbis/libmonovm.h>

#include "Version.h"
#include "Utilities.h"
#include "Mono.h"
#include "Detour.h"
#include "Patcher.h"
#include "UI.h"
#include "Menu.h"

#include "Widget.h"
#include "Label.h"
#include "Panel.h"

#define ORBIS_TOOLBOX_NOTIFY ("Orbis Toolbox Alpha: " stringify(ORBIS_TOOLBOX_MAJOR) "." stringify(ORBIS_TOOLBOX_MINOR) " Loaded!")