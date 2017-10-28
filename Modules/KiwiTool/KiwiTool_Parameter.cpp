/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016-2017, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v3
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#include <KiwiTool/KiwiTool_Parameter.h>

namespace kiwi { namespace tool {
    
    Parameter::Parameter(Type type, std::vector<Atom> const atoms):
    m_type(type),
    m_atoms()
    {
        switch (m_type)
        {
            case Type::Float:
            {
                assert(m_atoms.size() == 1 && m_atoms[0].isFloat() && "Parameter float bad initialization");
                break;
            }
            case Type::Int:
            {
                assert(m_atoms.size() == 1 && m_atoms[0].isInt() && "Parameter int bad initialization");
                break;
            }
        }
    }
    
    Parameter::Parameter(Parameter const& other):
    m_type(other.m_type),
    m_atoms(other.m_atoms)
    {
    }
    
    Parameter& Parameter::operator=(Parameter const& other)
    {
        assert(m_type == other.m_type && "Parameter assignment with different types");
        
        if (&other != this)
        {
            m_atoms = other.m_atoms;
        }
        
        return *this;
    }
    
    Parameter::Type Parameter::getType() const
    {
        return m_type;
    }
    
}}
