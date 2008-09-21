//  File xfx_cegui_luabind_scriptmodule.h                                     |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

/*!	\file xfx_cegui_luabind_scriptmodule.h 
 *	\brief ScriptModule implementation for CEGUI library.
 */

_XFX_BEGIN




/*! \class CEGUILuaBindScriptModule xfx_cegui_luabind_scriptmodule.h "gui/xfx_cegui_luabind_scriptmodule.h"
 *	\brief ScriptModule for CEGUI library.
 *	\ingroup GUIGroup
 *
 *	Uses luabind instead of standart tolua++ implementation.
 *
 *	\author Andrew "RevEn" Karpushin
 */

class CEGUILuaBindScriptModule : public CEGUI::ScriptModule, boost::noncopyable
{
	CEGUI::String								mIdentifierString;
	CEGUI::String								mLanguageString;

public:
	CEGUILuaBindScriptModule											( );
	virtual ~CEGUILuaBindScriptModule									( );

	//
	// CEGUI interface
	//

	/*!
	\brief
		Execute a script file.

	\param filename
		String object holding the filename of the script file that is to be executed
		
	\param resourceGroup
		Resource group idendifier to be passed to the ResourceProvider when loading the script file.
	*/
	virtual	void						executeScriptFile				( const CEGUI::String& filename, const CEGUI::String& resourceGroup = "" );


	/*!
	\brief
		Execute a scripted global function.  The function should not take any parameters and should return an integer.

	\param function_name
		String object holding the name of the function, in the global script environment, that
		is to be executed.

	\return
		The integer value returned from the script function.
	*/
	virtual int							executeScriptGlobal				( const CEGUI::String& function_name );


	/*!
	\brief
		Execute a scripted global 'event handler' function.  The function should take some kind of EventArgs like parameter
		that the concrete implementation of this function can create from the passed EventArgs based object.  The function
		should not return anything.

	\param handler_name
		String object holding the name of the scripted handler function.

	\param e
		EventArgs based object that should be passed, by any appropriate means, to the scripted function.

	\return
		- true if the event was handled.
		- false if the event was not handled.
	*/
	virtual	bool						executeScriptedEventHandler		( const CEGUI::String& handler_name, const CEGUI::EventArgs& e );


    /*!
    \brief
        Execute script code contained in the given CEGUI::String object.

    \param str
        String object holding the valid script code that should be executed.

    \return
        Nothing.
    */
	virtual void						executeString					( const CEGUI::String& str );


    /*!
    \brief
        Method called during system initialisation, prior to running any scripts via the ScriptModule, to enable the ScriptModule
        to perform any operations required to complete initialisation or binding of the script language to the gui system objects.

    \return
        Nothing.
    */
    virtual void						createBindings					( void );


    /*!
    \brief
        Method called during system destruction, after all scripts have been run via the ScriptModule, to enable the ScriptModule
        to perform any operations required to cleanup bindings of the script language to the gui system objects, as set-up in the
        earlier createBindings call.

    \return
        Nothing.
    */
    virtual void						destroyBindings					( void );

    /*!
    \brief
        Return identification string for the ScriptModule.  If the internal id string has not been
        set by the ScriptModule creator, a generic string of "Unknown scripting module" will be returned.

    \return
        String object holding a string that identifies the ScriptModule in use.
    */
	const CEGUI::String&				getIdentifierString				( ) const;

	/*!
	\brief
		Return an string which identifies the language of this module.
	/return
		String object holding a string that identifies the languge of the module
	*/
	const CEGUI::String&				getLanguage						( ) const;

    /*!
    \brief
            Subscribes the named Event to a scripted funtion

    \param target
            The target EventSet for the subscription.

    \param name
            String object containing the name of the Event to subscribe to.

    \param subscriber_name
            String object containing the name of the script funtion that is to be subscribed to the Event.

    \return
            Connection object that can be used to check the status of the Event connection and to disconnect (unsubscribe) from the Event.

    \exception UnknownObjectException	Thrown if an Event named \a name is not in the EventSet
    */
	virtual CEGUI::Event::Connection	subscribeEvent					( CEGUI::EventSet* target, const CEGUI::String& name, const CEGUI::String& subscriber_name );

    /*!
    \brief
            Subscribes the specified group of the named Event to a scripted funtion.

    \param target
            The target EventSet for the subscription.

    \param name
            String object containing the name of the Event to subscribe to.

    \param group
            Group which is to be subscribed to.  Subscription groups are called in ascending order.

    \param subscriber_name
            String object containing the name of the script funtion that is to be subscribed to the Event.

    \return
            Connection object that can be used to check the status of the Event connection and to disconnect (unsubscribe) from the Event.

    \exception UnknownObjectException	Thrown if an Event named \a name is not in the EventSet
    */
	virtual CEGUI::Event::Connection	subscribeEvent					( CEGUI::EventSet* target, const CEGUI::String& name, CEGUI::Event::Group group, const CEGUI::String& subscriber_name );
};



_XFX_END