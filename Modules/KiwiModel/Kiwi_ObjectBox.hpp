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
    //                                  PIN TYPE MODEL                                  //
    // ================================================================================ //
    
    template<class TDataModel>
    void PinType::declare()
    {
        flip::EnumClass<PinType::IType>::declare()
        .name("cicm.kiwi.patch.Object.PinTypeEnum")
        .enumerator<PinType::IType::Control>("Control")
        .enumerator<PinType::IType::Signal>("Signal");
        
        TDataModel::template add<PinType::IType>();
        
        flip::Class<PinType>::declare()
        .name("cicm.kiwi.patch.Object.PinType")
        .member<flip::Enum<IType>, &PinType::m_type>("type");
        
        TDataModel::template add<PinType>();
    }
    
    // ================================================================================ //
    //                                      INLET                                       //
    // ================================================================================ //
    
    template<class TDataModel>
    void Inlet::declare()
    {
        flip::Class<Inlet>::declare()
        .name("cicm.kiwi.patch.Object.Inlet")
        .member<flip::Array<PinType>, &Inlet::m_types>("types");
        
        TDataModel::template add<Inlet>();
    };
    
    // ================================================================================ //
    //                                      OUTLET                                      //
    // ================================================================================ //
    
    template<class TDataModel>
    void Outlet::declare()
    {
        flip::Class<Outlet>::declare()
        .name("cicm.kiwi.patch.Object.Outlet")
        .member<PinType, &Outlet::m_type>("type");
        
        TDataModel::template add<Outlet>();
    };
    
    // ================================================================================ //
    //                                   OBJECT BOX                                     //
    // ================================================================================ //
    
    template<class TDataModel>
    void ObjectBox::declare()
    {
        PinType::declare<TDataModel>();
        Inlet::declare<TDataModel>();
        Outlet::declare<TDataModel>();
        
        flip::Class<ObjectBox>::declare()
        .name("cicm.kiwi.patch.Object")
        .inherit<ObjectBase>()
        .member<flip::String,           &ObjectBox::m_typed_name>         ("typed_name")
        .member<flip::String,           &ObjectBox::m_additional_text>    ("additional_text")
        .member<flip::Array<Inlet>,     &ObjectBox::m_inlets>             ("inlets")
        .member<flip::Array<Outlet>,    &ObjectBox::m_outlets>            ("outlets")
        .member<flip::Float,            &ObjectBox::m_position_x>         ("pos_x")
        .member<flip::Float,            &ObjectBox::m_position_y>         ("pos_y")
        .member<flip::Float,            &ObjectBox::m_width>              ("width")
        .member<flip::Float,            &ObjectBox::m_height>             ("height");
        
        TDataModel::template add<ObjectBox>();
    }
    
}}
