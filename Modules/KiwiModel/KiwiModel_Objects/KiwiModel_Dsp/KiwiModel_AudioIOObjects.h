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

#include <KiwiModel/KiwiModel_Object.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                 AUDIO IO OBJECTS                                 //
    // ================================================================================ //
    
    struct AudioIOObjects
    {
        //! @internal flip static declare method
        //! @details Declare all audio io objects
        static void declare();
    };
    
    // ================================================================================ //
    //                               AUDIO IO OBJECT BASE                               //
    // ================================================================================ //
    
    //! @brief Base class for Audio IO Objects like dac~ or adc~.
    class AudioIOObjectBase : public model::Object
    {
    public:
        
        //! @brief Constructor
        //! @details Initializes the routes by parsing arguments.
        AudioIOObjectBase(std::vector<Atom> const& args);
        
    public: // internal methods
        
        //! @brief flip Default Constructor
        AudioIOObjectBase(flip::Default& d);
        
    protected: // methods
        
        std::vector<size_t> const& useRoutes() const;
        
    private: // members
        
        //! @brief Parses args to construct routes
        static std::vector<size_t> parseArgsAsChannelRoutes(std::vector<Atom> const& args);
        
        // Do not access the m_routes variable directly, use the getRoutes() method instead.
        mutable std::vector<size_t> m_routes = {};
        mutable bool                m_routes_parsed = false;
        
    private: // deleted methods
        
        //! @internal Use the AudioIOObjects::declare instead.
        static void declare() = delete;
    };
    
    // ================================================================================ //
    //                                     OBJECT DAC~                                  //
    // ================================================================================ //
    
    class DacTilde : public AudioIOObjectBase
    {
    public: // methods
        
        //! @brief Constructor
        DacTilde(std::string const& name, std::vector<Atom> const& args);
        
        std::string getIODescription(bool is_inlet, size_t index) const override;
        
    public: // internal methods
        
        //! @internal flip Default Constructor
        DacTilde(flip::Default& d);
        
    private: // deleted methods
        
        //! @internal Use the AudioIOObjects::declare instead.
        static void declare() = delete;
    };
    
    // ================================================================================ //
    //                                     OBJECT ADC~                                  //
    // ================================================================================ //
    
    class AdcTilde : public AudioIOObjectBase
    {
    public: // methods
        
        //! @brief Constructor
        AdcTilde(std::string const& name, std::vector<Atom> const& args);
        
        std::string getIODescription(bool is_inlet, size_t index) const override;
        
    public: // internal methods
        
        //! @internal flip Default Constructor
        AdcTilde(flip::Default& d);
        
    private: // deleted methods
        
        //! @internal Use the AudioIOObjects::declare instead.
        static void declare() = delete;
    };
    
}}
