namespace std
{
	//! STL class.
	template< class _Type > class vector { public: _Type element; };
	//! STL class.
	template< class _Type > class auto_ptr { public: _Type element; };
	//! STL class.
	template< class _Key, class _Type > class map { public: _Key key; _Type value; };
	//! STL class.
	template< class _Type > struct pair{ _Type element; };
	//! STL class.
	template< class _Type > struct basic_string{ _Type element; };
};

namespace stdext
{
	//! STL class.
	template< class _Key, class _Type > class hash_map { public: _Key key; _Type value; };
};