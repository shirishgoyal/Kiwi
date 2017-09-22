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
    //                                   OBJECT BASE                                    //
    // ================================================================================ //
    
    template<class TDataModel>
    void Link::declare()
    {
        flip::Class<Link>::declare()
        .name("cicm.kiwi.patch.Link")
        .inherit<ObjectBase>()
        .member<flip::ObjectRef<ObjectBox>, &Link::sender>    ("sender_obj")
        .member<flip::ObjectRef<ObjectBox>, &Link::receiver>  ("receiver_obj")
        .member<flip::Int, &Link::outlet_index>                    ("outlet_index")
        .member<flip::Int, &Link::inlet_index>                     ("inlet_index");
        
        TDataModel::template add<Link>();
    }
    
}}
