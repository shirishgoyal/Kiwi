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

#pragma once

#include <string>
#include <vector>

#include <flip/detail/Default.h>
#include <flip/Bool.h>

#include <KiwiModel/KiwiModel_Object.h>
#include <KiwiTool/KiwiTool_Atom.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                  OBJECT SLIDER                                   //
    // ================================================================================ //
    
    class Slider : public model::Object
    {
    public: // methods
        
        Slider(flip::Default& d);
        
        Slider(bool horizontal);
        
        std::string getIODescription(bool is_inlet, size_t index) const override;
        
        void writeParameter(std::string const& name, tool::Parameter const& parameter) override final;
        
        void readParameter(std::string const& name, tool::Parameter & parameter) const override final;
        
        bool parameterChanged(std::string const& name) const override final;
        
        static void declare();
        
        static std::unique_ptr<Object> create(std::vector<tool::Atom> const& args);
        
    private: // members
        
        flip::Bool m_horizontal;
        
    };
}}
