// Copyright (c) 2019, 2020 Marko Mahnič
// License: MPL2. See LICENSE in the root of the project.

#pragma once

#include "inc/argparser.h"

#define ARGUMENTUM_INLINE inline

#include "inc/argdescriber_impl.h"
#include "inc/argparser_impl.h"
#include "inc/argumentstream_impl.h"
#include "inc/command_impl.h"
#include "inc/commandconfig_impl.h"
#include "inc/convert_impl.h"
#include "inc/environment_impl.h"
#include "inc/group_impl.h"
#include "inc/groupconfig_impl.h"
#include "inc/helpformatter_impl.h"
#include "inc/option_impl.h"
#include "inc/optionconfig_impl.h"
#include "inc/optionpack_impl.h"
#include "inc/optionsorter_impl.h"
#include "inc/parameterconfig_impl.h"
#include "inc/parser_impl.h"
#include "inc/parserconfig_impl.h"
#include "inc/parserdefinition_impl.h"
#include "inc/parseresult_impl.h"
#include "inc/value_impl.h"
#include "inc/writer_impl.h"

#undef ARGUMENTUM_INLINE
