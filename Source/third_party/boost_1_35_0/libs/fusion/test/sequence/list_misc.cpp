/*=============================================================================
    Copyright (c) 1999-2003 Jaakko J�rvi
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#include <boost/fusion/container/list/list.hpp>
#include <boost/fusion/container/list/convert.hpp>

#define FUSION_SEQUENCE list
#define FUSION_FORWARD_ONLY
#include "misc.hpp"

int
main()
{
    test();
    return boost::report_errors();
}

