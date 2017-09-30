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


#include <KiwiModel/Kiwi_Objects/Kiwi_Objects.h>

#include "KiwiEngine_Factory.h"
#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_Objects.h>
#include "KiwiEngine_Instance.h"

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                      INSTANCE                                    //
        // ================================================================================ //
        
        Instance::Instance(std::unique_ptr<AudioControler> audio_controler):
        m_audio_controler(std::move(audio_controler)),
        m_scheduler(),
        m_quit(false),
        m_engine_thread(std::bind(&Instance::processScheduler, this))
        {
            addObjectsToFactory();
        }
        
        Instance::~Instance()
        {
            m_quit.store(true);
            m_engine_thread.join();
        }
        
        // ================================================================================ //
        //                                      CONSOLE                                     //
        // ================================================================================ //
        
        void Instance::log(std::string const& text) const
        {
            m_console.post({text, core::Console::Message::Type::Log});
        }
        
        void Instance::post(std::string const& text) const
        {
            m_console.post({text, core::Console::Message::Type::Normal});
        }
        
        void Instance::warning(std::string const& text) const
        {
            m_console.post({text, core::Console::Message::Type::Warning});
        }
        
        void Instance::error(std::string const& text) const
        {
            m_console.post({text, core::Console::Message::Type::Error});
        }

        void Instance::addConsoleListener(core::Console::Listener& listener)
        {
            m_console.addListener(listener);
        }

        void Instance::removeConsoleListener(core::Console::Listener& listener)
        {
            m_console.removeListener(listener);
        }
        
        void Instance::addObjectsToFactory()
        {
            engine::Factory::add<model::NewBox, NewBox>();
            engine::Factory::add<model::ErrorBox, ErrorBox>();
            engine::Factory::add<model::Plus, Plus>();
            engine::Factory::add<model::Times, Times>();
            engine::Factory::add<model::Print, Print>();
            engine::Factory::add<model::Receive, Receive>();
            engine::Factory::add<model::Loadmess, Loadmess>();
            engine::Factory::add<model::Delay, Delay>();
            engine::Factory::add<model::Pipe, Pipe>();
            engine::Factory::add<model::Metro, Metro>();
            engine::Factory::add<model::OscTilde, OscTilde>();
            engine::Factory::add<model::AdcTilde, AdcTilde>();
            engine::Factory::add<model::DacTilde, DacTilde>();
            engine::Factory::add<model::TimesTilde, TimesTilde>();
            engine::Factory::add<model::PlusTilde, PlusTilde>();
            engine::Factory::add<model::SigTilde, SigTilde>();
            engine::Factory::add<model::DelaySimpleTilde, DelaySimpleTilde>();
            engine::Factory::add<model::Bang, Bang>();
            engine::Factory::add<model::Toggle, Toggle>();
        }
        
        // ================================================================================ //
        //                              AUDIO CONTROLER                                     //
        // ================================================================================ //
        
        AudioControler& Instance::getAudioControler() const
        {
            return *m_audio_controler.get();
        }
        
        // ================================================================================ //
        //                                  SCHEDULER                                       //
        // ================================================================================ //
        
        core::Scheduler<> & Instance::getScheduler()
        {
            return m_scheduler;
        }
        
        void Instance::processScheduler()
        {
            m_scheduler.setThreadAsConsumer();
            
            while(!m_quit.load())
            {
                m_scheduler.process();
                std::this_thread::sleep_for(std::chrono::milliseconds(1));

            }
        }
    }
}
