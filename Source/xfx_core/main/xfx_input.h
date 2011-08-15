//  File xfx_input.h                                                          |
//                                                                            |
//  Created by: Andrew "RevEn" Karpushin                                      |
//                                                                            |
//----------------------------------------------------------------------------+

#pragma once

#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>

#pragma comment ( lib, "dxguid.lib" )
#pragma comment ( lib, "dinput8.lib" )

/*!	\file xfx_input.h 
 *	\brief %Input system, based on DirectInput.
 */

_XFX_BEGIN






/*! \class Input xfx_input.h "main/xfx_input.h"
 *	\brief %Input class.
 *	\ingroup MainGroup
 *	
 *	%Input system, base on DirectInput. Works width keyboard and mouse.
 *
 *	\author Andrew "RevEn" Karpushin
 */

class Input : public Singleton< Input >
{
	friend class Singleton< Input >;

public:
	//! Available keys enumeration (extension of DIK_*)
	enum KeysType
	{
		MOUSE_LBUTTON = 256,			//!< Left mouse button
		MOUSE_RBUTTON,					//!< Right mouse button
		MOUSE_MBUTTON,					//!< Middle mouse button
		MOUSE_4BUTTON,					//!< Fourth mouse button

		MOUSE_MOVELEFT,					//!< Mouse left move action
		MOUSE_MOVERIGHT,				//!< Mouse right move action
		MOUSE_MOVEDOWN,					//!< Mouse down move action
		MOUSE_MOVEUP,					//!< Mouse up move action
		MOUSE_WHEELUP,					//!< Mouse wheel up move action
		MOUSE_WHEELDOWN,				//!< Mouse wheel down move action

		MAX_KEYS
	};

	//! Key states
	enum STATES
	{
		FIRST_PUSHED = 0,				//!< First pushed
		PUSHED,							//!< Was pushed and is still being pushed now
		POPED,							//!< Just poped

		MAX_STATES
	};

private:
	boost::shared_ptr< IDirectInput8 >			mpInput;
	boost::shared_ptr< IDirectInputDevice8 >	mpKeyboard;
	boost::shared_ptr< IDirectInputDevice8 >	mpMouse;

	enum EKeyFlags
	{
		EKF_PUSHED			= ( 1 << 0 ),
		EKF_IN_CONSOLE		= ( 1 << 1 ),
	};

	boost::array< BYTE, MAX_KEYS >				mKeyMap;
	String										mCommands[ MAX_KEYS ][ MAX_STATES ];
	bool										mUseKeyMapping;

	stdext::hash_map< String, unsigned, HashCompare< String > >		mBindedKeysMap[ MAX_STATES ];

protected:
	Input												( );
	~Input												( );

public:

	/*!	\brief %Input initialization.
	 *	
	 *	Creates DirectInput objects for keyboard and mouse.
	 *
	 *	\return Returns S_OK if all right.
	 */
	HRESULT						Init					( );

	/*!	\brief %Input deinitialization.
	 *
	 *	Deinitialize all previously acquired DirectInput objects.
	 */
	HRESULT						Shutdown				( );

	//! \brief Retrieves keyboard and mouse data.
	void						RetrieveData			( );

	/*!	\brief Test whether key is pushed.
	 *
	 *	\param[in] key		Key from DIK_* and KeysType.
	 *
	 *	\return Returns \b true if key is pushed and \b false otherwise.
	 */
	bool						TestKey					( unsigned key ) const { _ASSERTE( key < MAX_KEYS ); return ( mKeyMap[ key ] & EKF_PUSHED ) != 0; };

	/*!	\brief Associate string with key and key state.
	 *
	 *	\param[in] key		Key from DIK_* and KeysType.
	 *	\param[in] state	State from STATES.
	 *	\param[in] cmd		Command to associate.
	 *
	 */
	void						Command					( unsigned key, STATES state, const String& cmd );

	/*!	\brief Gets currently associated string with key and key state.
	 *
	 *	\param[in] key		Key from DIK_* and KeysType.
	 *	\param[in] state	State from STATES.
	 *
	 *	\return Returns currently associated string.
	 */
	const String&				Command					( unsigned key, STATES state ) const { _ASSERTE( key < MAX_KEYS ); _ASSERTE( state < MAX_STATES ); return mCommands[ key ][ state ]; };

	/*! \brief Get a DIK key binded to command.
	 *
	 *	Using hash map. Average complexity is constant.
	 *
	 *	\param[in]	command		Command name.
	 *
	 *	\return DIK key code or MAX_KEYS.
	 */
	unsigned					GetKeyForCommand		( const String& command ) const;

	/*!	\brief Returns key name.
	 *
	 *	\param[in] key		Key from DIK_* and KeysType.
	 *
	 *	\return Returns key name.
	 */
	const char *				KeyName					( unsigned key ) const;

	/*!	\brief Set mouse mode (exclusive or not).
	 *
	 *	\param[in] isexclusive		\b true for exclusive.
	 */
	void						SetMouseMode			( const bool& isexclusive );

	/*! \brief Get cursor position (window space).
	 */
	Vec2						GetCursorPos			( ) const;

	/*!	\brief Set whether or not use automatically key mapping (commands will be directly sent to command processor).
	 */
	void						SetUseKeyMapping		( const bool& use ) { mUseKeyMapping = use; };

	/*!	\brief Set whether or not use automatically key mapping (commands will be directly sent to command processor).
	 *
	 *	\return Returns current key mapping value.
	 */
	const bool&					GetUseKeyMapping		( ) const { return mUseKeyMapping; };

	/*! \brief Bind command, used in console.
	 *
	 *	\param[in]	in_console	Command will be executed even when console is active.
	 *	\param[in]	str			Command.
	 */
	void						Cmd_bind				( bool in_console, const String& str );

private:
	void						TestMouseMove			( );
	void						WriteMouseMoveCommand	( DWORD key, int num, bool condition );
};




_XFX_END
