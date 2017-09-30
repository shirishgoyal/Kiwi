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

#include <KiwiModel/Kiwi_Objects/Kiwi_Basic/Kiwi_ErrorBox.h>

#include <KiwiModel/Kiwi_Factory.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                      ERRORBOX                                    //
    // ================================================================================ //
    
    void ErrorBox::declare()
    {
        //DataModel::declare<ErrorBox>()
        //.name("cicm.kiwi.patch.boxes.ErrorBox")
        //.inherit<model::Object>();
        
        Factory::add<ErrorBox>("errorbox").setInternal(true);
    }
    
    ErrorBox::ErrorBox(std::string const& name, std::vector<Atom> const& args)
    : model::Object(name, args)
    {
    }
    
    ErrorBox::ErrorBox(std::string const& name, std::vector<Atom> const& args, std::string const& err_message)
    : model::Object(name, args)
    , m_error(err_message)
    {
    }
    
    void ErrorBox::setInlets(flip::Array<Inlet> const& inlets)
    {
        model::Object::setInlets(inlets);
    }
    
    void ErrorBox::setOutlets(flip::Array<Outlet> const& outlets)
    {
        model::Object::setOutlets(outlets);
    }
    
    std::string ErrorBox::getIODescription(bool is_inlet, size_t index) const
    {
        return "(nothing here)";
    }
    
    void ErrorBox::setError(std::string const& error_message)
    {
        m_error = error_message;
    }
    
    std::string ErrorBox::getError() const
    {
        return m_error;
    }
    
}}