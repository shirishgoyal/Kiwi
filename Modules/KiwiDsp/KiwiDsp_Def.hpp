/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v2
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#ifndef KIWI_DSP_DEF_HPP_INCLUDED
#define KIWI_DSP_DEF_HPP_INCLUDED

#include <iostream>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <chrono>
#include <exception>
#include <vector>
#include <set>
#include <memory>
#include <atomic>
#include <mutex>

#ifndef __APPLE__
#include "malloc.h"
#endif

namespace kiwi
{
    namespace dsp
    {
        #ifdef KIWI_DSP_FLOAT
        typedef float sample_t;
        #else
        typedef double sample_t;
        #endif
        
        const sample_t pi = M_PI;
    }
}

#endif // KIWI_DSP_DEF_HPP_INCLUDED
