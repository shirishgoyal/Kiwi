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

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                    MODEL BASE                                    //
    // ================================================================================ //
    
    template<class TDataModel>
    void ObjectBase::declare()
    {
        flip::Class<ObjectBase>::declare()
        .name("cicm.kiwi.patch.ObjectBase");
        
        TDataModel::template add<ObjectBase>();
    }
    
    template <class... Args>
    auto& ObjectBase::getSignal(SignalKey key) const
    {
        flip::SignalBase& signal_base = *m_signals.at(key);
        return dynamic_cast<flip::Signal<Args...>&>(signal_base);
    }
    
    template <class... Args>
    void ObjectBase::addSignal(SignalKey key, model::ObjectBase& object)
    {
        m_signals.emplace(key, std::make_unique<flip::Signal<Args...>>(key, object));
    }
}}
