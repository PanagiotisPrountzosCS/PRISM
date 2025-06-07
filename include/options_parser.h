#ifndef __OPTIONS_PARSER_H
#define __OPTIONS_PARSER_H

#include <stdint.h>

#include "coredefs.h"

namespace PRISM
{

bool validate_opts(int argc, char** argv, options& options);

}  // namespace PRISM
#endif